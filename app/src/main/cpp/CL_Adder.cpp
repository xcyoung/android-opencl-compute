//
// Created by xcyoung on 6/4/22.
//

#include "CL_Adder.h"
#include <iostream>
#include <cstdlib>
#include "log.h"

#define CL_USE_DEPRECATED_OPENCL_1_1_APIS

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

static const int arrayLength = 1 << 24;
static const int bufferSize = arrayLength * sizeof(float);

static const char* addClSl = R"clsl(
__kernel void add(__global const float *a, __global const float *b, __global float *c)
{
    int gid = get_global_id(0);
    c[gid] = a[gid] + b[gid];
}
)clsl";

void clAdd() {
    float* data = (float *)malloc(bufferSize);
    float* data2 = (float *)malloc(bufferSize);
    float* result = (float *)malloc(bufferSize);

    for (int i = 0; i < arrayLength; i++) {
        data[i] = (float)random()/(float)(RAND_MAX);
        data2[i] = (float)random()/(float)(RAND_MAX);
    }

    clock_t startTimer, stopTimer;

    cl_int err = CL_SUCCESS;

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if (platforms.empty()) {
        LOGCATE("Platform size 0");
        return;
    }

    cl_context_properties properties[] =
            {CL_CONTEXT_PLATFORM, (cl_context_properties) (platforms[0])(), 0};
    cl::Context context(CL_DEVICE_TYPE_GPU, properties);

    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    cl::CommandQueue queue(context, devices[0], 0, &err);

    std::string kernelSource = addClSl;

    cl::Program::Sources source(1, std::make_pair(kernelSource.c_str(),
                                                  kernelSource.length() + 1));
    cl::Program program(context, source);
    const char *options = "-cl-fast-relaxed-math";
    program.build(devices, options);

    cl::Kernel kernel(program, "add", &err);

    startTimer = clock();
    cl::Buffer aBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                    bufferSize, (void *) &data[0], &err);
    cl::Buffer bBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                    bufferSize, (void *) &data2[0], &err);
    cl::Buffer cBuffer = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                    bufferSize, (void *) &result[0], &err);
    stopTimer = clock();
    LOGCATD("write buffer took %g ms", 1000.0 * (double) (stopTimer - startTimer) / (double) CLOCKS_PER_SEC);

    kernel.setArg(0, aBuffer);
    kernel.setArg(1, bBuffer);
    kernel.setArg(2, cBuffer);

    size_t workgroupSize = 0;
    err = kernel.getWorkGroupInfo(devices[0], CL_KERNEL_WORK_GROUP_SIZE, &workgroupSize);

    startTimer = clock();
    err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(arrayLength), cl::NDRange(workgroupSize));
    queue.finish();
    stopTimer = clock();
    LOGCATD("OpenCL code on the GPU took %g ms", 1000.0 * (double) (stopTimer - startTimer) / (double) CLOCKS_PER_SEC);

    startTimer = clock();
    queue.enqueueReadBuffer(cBuffer, CL_TRUE, 0, bufferSize, result);
    stopTimer = clock();
    LOGCATD("read buffer took %g ms", 1000.0 * (double) (stopTimer - startTimer) / (double) CLOCKS_PER_SEC);

    for (int i = 0; i < arrayLength; i++) {
        if (data[i] + data2[i] != result[i]) {
            LOGCATE("data[%d] + data2[%d] != result[%d], %f + %f != %f", i, i, i, data[i], data2[i], result[i]);
        }
    }

    free(data);
    free(data2);
    free(result);
}

//  https://github.com/alibaba/MNN/blob/master/source/backend/opencl/core/runtime/OpenCLRuntime.cpp
void clInfo() {
    std::vector<cl::Platform> platforms;
    cl_int res = cl::Platform::get(&platforms);

    if (platforms.empty() || res != CL_SUCCESS) {
        LOGCATE("Platform size 0");
        return;
    }

    const std::string platformName = platforms[0].getInfo<CL_PLATFORM_NAME>();
    LOGCATD("cl_platform_name:%s", platformName.c_str());

    std::vector<cl::Device> gpuDevices;
    res = platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &gpuDevices);

    if (gpuDevices.empty() || res != CL_SUCCESS) {
        LOGCATE("Devices size 0");
        return;
    }

    const std::string deviceName = gpuDevices[0].getInfo<CL_DEVICE_NAME>();
    LOGCATD("cl_device_name:%s", deviceName.c_str());
    const std::string deviceVersion = gpuDevices[0].getInfo<CL_DEVICE_VERSION>();
    LOGCATD("cl_device_version:%s", deviceVersion.c_str());
    const std::string deviceVendor = gpuDevices[0].getInfo<CL_DEVICE_VENDOR>();
    LOGCATD("cl_device_vendor:%s", deviceVendor.c_str());
}