//
// Created by root on 16-8-17.
//
#include <jni.h>
#include <opencv/cv.hpp>
#include "../com_ocr_java.h"
#include "ocr_read.h"
#include "ocr_tesseract.h"


JNIEXPORT jint JNICALL Java_com_ocr_1java_OcrPreprocess
        (JNIEnv * env, jobject obj, jstring srcImg, jstring desImg) {
    const char* input = env->GetStringUTFChars(srcImg, 0);
    const char* output = env->GetStringUTFChars(desImg, 0);

    printf("hello\n");
    printf("input: %s\n", input);
    printf("output: %s\n", output);

    //ocr_preprocess(input, output);
    ocr_preprocess("test/3.jpg","test/1.jpg");
    env->ReleaseStringUTFChars(srcImg, input);
    env->ReleaseStringUTFChars(desImg, output);
    return 0;
}

JNIEXPORT jint JNICALL Java_com_ocr_1java_OcrTesseract
        (JNIEnv * env, jobject obj, jstring srcImg, jstring desImg, jstring ocr_type) {
    const char* input = env->GetStringUTFChars(srcImg, 0);
    const char* output = env->GetStringUTFChars(desImg, 0);
    const char* type = env->GetStringUTFChars(ocr_type, 0);

    printf("input: %s\n", input);
    printf("output: %s\n", output);
    printf("type: %s\n", type);

    ocr_tesseract(input, output, type);

    env->ReleaseStringUTFChars(srcImg, input);
    env->ReleaseStringUTFChars(desImg, output);
    env->ReleaseStringUTFChars(ocr_type, type);
    return 1;
}
