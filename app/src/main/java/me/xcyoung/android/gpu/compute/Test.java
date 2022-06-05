package me.xcyoung.android.gpu.compute;

public class Test {
    static {
        System.loadLibrary("gpu_compute");
    }

    native void nativeCLAdd();
    native void nativeCLInfo();
}
