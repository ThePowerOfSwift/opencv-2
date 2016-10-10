//
// Created by root on 16-8-17.
//
#include <jni.h>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "com_ocr_java.h"
#include "base64.h"
#include <iostream>


using namespace cv;
using namespace std;

JNIEXPORT jstring JNICALL Java_com_ocr_1java_getOcrArray
  (JNIEnv * env, jobject obj, jstring srcImg) {
	const char* path = env->GetStringUTFChars(srcImg, 0);
	//printf("path = %s\n", path);

	Mat mImg = imread(path, IMREAD_UNCHANGED);
    
	vector<uchar> buf;
	imencode(".jpg", mImg, buf);
	//imencode(".png", mImg, buf);
	uchar *enc_msg = new uchar[buf.size()];
	for(int i=0; i < buf.size(); i++) enc_msg[i] = buf[i];
	string encoded = base64_encode(enc_msg, buf.size());
	const char* cstr = encoded.c_str();
	//printf("%s\n", cstr);
	env->ReleaseStringUTFChars(srcImg, path);
	return env->NewStringUTF(cstr);
}

