//
// Created by root on 16-8-17.
//
#include <jni.h>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "com_ocr_java.h"

using namespace cv;
using namespace std;

JNIEXPORT jcharArray JNICALL Java_com_ocr_1java_getOcrArray
  (JNIEnv * env, jobject obj, jstring srcImg) {
	//for test
#if 0
	int size = 100;
	jcharArray array = env->NewCharArray(size);
	jchar* pArray = new jchar[size];
	for(int i=0; i<size; i++)
	  pArray[i] = 'c';
#endif
	const char* path = env->GetStringUTFChars(srcImg, 0);
	printf("path = %s\n", path);

	Mat mImg = imread(path, IMREAD_UNCHANGED);
	
	int size = mImg.cols * mImg.rows;

	jcharArray array = env->NewCharArray(size);
	jchar* pArray = new jchar[size];

	for(int i=0; i<mImg.rows; i++)
	{
		uchar *pImg = mImg.ptr<uchar>(i);
		for(int j=0; j<mImg.cols; j++)
		{
			pArray[i*mImg.cols+j] = pImg[j];
		}
	}
	env->ReleaseCharArrayElements(array, pArray, JNI_COMMIT);
	return array;
}

JNIEXPORT void JNICALL Java_com_ocr_1java_setOcrArray
  (JNIEnv * env, jobject obj, jcharArray array) {
	jchar* pArray = env->GetCharArrayElements(array, 0);
	int size = env->GetArrayLength(array);
	printf("size = %d\n", size);	
	for(int i=0; i<size; i++)
		printf("pArray[%d] = %d\n", i, pArray[i]);
}
