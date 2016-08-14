#include <string>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "ocr_read.hpp.h"

using namespace cv;
using namespace std;

#define OCR_DBG 1

//threshold define
#define DEFAULT_THRESHOLD 210
#define DEFAULT_SIZE 5
#define DEFAULT_DELTA 5

IplImage *g_img = NULL;
IplImage *g_grey = NULL;
IplImage *g_binary = NULL;

int ocr_dbg(IplImage* img) {
    //display ocr png
    cvNamedWindow("ocr_dbg", CV_WINDOW_AUTOSIZE);
    cvShowImage("ocr_dbg", img);
    waitKey(0);
}

IplImage* ocr_read(char* ocr_name) {
    return cvLoadImage(ocr_name, CV_LOAD_IMAGE_UNCHANGED);
}

IplImage* ocr_grey(IplImage* srcImg) {
    //convert to grey
    IplImage* desImg;
    desImg = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_8U, 1);
    cvCvtColor(srcImg, desImg, CV_BGR2GRAY);
    return desImg;
}

IplImage* ocr_binary(IplImage* srcImg, int threshold, int method, int size, double delta) {
    //convert to binary
    IplImage* desImg;
    desImg = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_8U, 1);
    switch(method) {
        case E_NORMAL: //normal threshold
        default:
            cvThreshold(srcImg, desImg, threshold, 255, CV_THRESH_BINARY);
            break;
        case E_GAUSSIAN: //adaptive gaussian threshold
            cvAdaptiveThreshold(srcImg, desImg, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, size, delta);
            break;
        case E_MEAN: //adaptive mean threshold
            cvAdaptiveThreshold(srcImg, desImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, size, delta);
            break;
        case E_OTSU: //adaptive otsu threshold
            cvThreshold(srcImg, desImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    }
    return desImg;
}

int main(int argc, const char* argv[]) {
	if(argc <= 1) {
		printf("usage: ./ocr_read xx.png\n");
		return -1;
	}

	g_img = ocr_read((char*)argv[1]);
    //ocr_dbg(g_img);
    g_grey = ocr_grey(g_img);
    //ocr_dbg(g_grey);
    g_binary = ocr_binary(g_grey, DEFAULT_THRESHOLD, E_OTSU, DEFAULT_SIZE, DEFAULT_DELTA);
    g_binary = ocr_binary(g_binary, DEFAULT_THRESHOLD, E_GAUSSIAN, DEFAULT_SIZE, DEFAULT_DELTA);
    ocr_dbg(g_binary);
	return 0;
}
