//
// Created by root on 16-8-17.
//
#include <jni.h>
#include <opencv/cv.hpp>
#include "../com_ocr_java.h"
#include "ocr_read.h"
#include "ocr_tesseract.h"
#include "convert.h"


JNIEXPORT jint JNICALL Java_com_ocr_1java_OcrPreprocess
        (JNIEnv * env, jobject obj, jstring srcImg, jstring desImg) {
    const char* input = env->GetStringUTFChars(srcImg, 0);
    const char* output = env->GetStringUTFChars(desImg, 0);

    printf("hello\n");
    printf("input: %s\n", input);
    printf("output: %s\n", output);


    ocr_preprocess(input, output);
    convert(input, (char*)output);
    env->ReleaseStringUTFChars(srcImg, input);
    env->ReleaseStringUTFChars(desImg, output);
    return 0;
}

JNIEXPORT jstring JNICALL Java_com_ocr_1java_OcrTesseract
        (JNIEnv * env, jobject obj, jstring srcImg, jstring desImg, jstring ocr_type) {
    const char* input = env->GetStringUTFChars(srcImg, 0);
    const char* output = env->GetStringUTFChars(desImg, 0);
    const char* type = env->GetStringUTFChars(ocr_type, 0);
    char* buf;
    printf("input: %s\n", input);
    printf("output: %s\n", output);
    printf("type: %s\n", type);

    buf = ocr_tesseract(input, output, type);

    env->ReleaseStringUTFChars(srcImg, input);
    env->ReleaseStringUTFChars(desImg, output);
    env->ReleaseStringUTFChars(ocr_type, type);
    return env->NewStringUTF(buf);
}
