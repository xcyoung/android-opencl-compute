//
// Created by xcyoung on 6/4/22.
//

#ifndef ANDROID_GPU_COMPUTE_LOG_H
#define ANDROID_GPU_COMPUTE_LOG_H
#include<android/log.h>

#define  LOG_TAG "gpu-compute"

#define  LOGCATE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGCATV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define  LOGCATD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGCATI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#endif //ANDROID_GPU_COMPUTE_LOG_H
