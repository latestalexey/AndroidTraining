#include <jni.h>
#include <stdio.h>
#include <android/bitmap.h>
#include <cstring>

extern "C"
{
JNIEXPORT jobject JNICALL Java_com_example_hellojni_HelloJni_nativeTransform(JNIEnv *env,
                                                                             jobject obj,
                                                                             jobject bitmap);
}

JNIEXPORT jobject JNICALL Java_com_example_hellojni_HelloJni_nativeTransform(JNIEnv *env,
                                                                             jobject obj,
                                                                             jobject bitmap) {
    //
    //getting bitmap info:
    //
    AndroidBitmapInfo info;
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) {
        return NULL;
    }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        return NULL;
    }
    //
    //read pixels of bitmap into native memory :
    //
    void *bitmapPixels;
    if (AndroidBitmap_lockPixels(env, bitmap, &bitmapPixels) < 0) {
        return NULL;
    }
    uint32_t *src = (uint32_t *) bitmapPixels;
    uint32_t *tempPixels = new uint32_t[info.height * info.width];
    int pixelsCount = info.height * info.width;
    memcpy(tempPixels, src, sizeof(uint32_t) * pixelsCount);
    AndroidBitmap_unlockPixels(env, bitmap);
    //
    //recycle bitmap - using bitmap.recycle()
    //
    jclass bitmapCls = env->GetObjectClass(bitmap);
    jmethodID recycleFunction = env->GetMethodID(bitmapCls, "recycle", "()V");
    if (recycleFunction == 0) {
        return NULL;
    }
    env->CallVoidMethod(bitmap, recycleFunction);
    //
    //creating a new bitmap to put the pixels into it - using Bitmap Bitmap.createBitmap (int width, int height, Bitmap.Config config) :
    //
    jmethodID createBitmapFunction = env->GetStaticMethodID(bitmapCls, "createBitmap",
                                                            "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jstring configName = env->NewStringUTF("ARGB_8888");
    jclass bitmapConfigClass = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID valueOfBitmapConfigFunction = env->GetStaticMethodID(bitmapConfigClass, "valueOf",
                                                                   "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");
    jobject bitmapConfig = env->CallStaticObjectMethod(bitmapConfigClass,
                                                       valueOfBitmapConfigFunction, configName);
    jobject newBitmap = env->CallStaticObjectMethod(bitmapCls, createBitmapFunction, info.height,
                                                    info.width, bitmapConfig);
    //
    // putting the pixels into the new bitmap:
    //
    if (AndroidBitmap_lockPixels(env, newBitmap, &bitmapPixels) < 0) {
        return NULL;
    }
    uint32_t *newBitmapPixels = (uint32_t *) bitmapPixels;
    int whereToPut = 0;
    for (int x = info.width - 1; x >= 0; --x) {
        for (int y = 0; y < info.height; ++y) {
            uint32_t pixel = tempPixels[info.width * y + x];
            newBitmapPixels[whereToPut++] = pixel;
        }
    }
    AndroidBitmap_unlockPixels(env, newBitmap);
    //
    // freeing the native memory used to store the pixels
    //
    delete[] tempPixels;
    return newBitmap;
}