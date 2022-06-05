//
// Created by xcyoung on 6/4/22.
//

#include <jni.h>
#include <android/log.h>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include "CL_Adder.h"

#define JNI_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
  Java_me_xcyoung_android_gpu_compute_Test_##method_name

extern "C" {
    JNI_METHOD(void, nativeCLAdd)(JNIEnv *env, jobject instance) {
        clAdd();
    }

    JNI_METHOD(void, nativeCLInfo)(JNIEnv *env, jobject instance) {
        clInfo();
    }
}