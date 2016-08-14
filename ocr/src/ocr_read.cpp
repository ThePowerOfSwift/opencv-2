#include <string>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "ocr_read.hpp.h"

using namespace cv;
using namespace std;

#define OCR_DBG 1

//threshold define
#define DEFAULT_THRESHOLD 210
#define DEFAULT_SIZE 3
#define DEFAULT_DELTA 3

IplImage *g_img = NULL;
IplImage *g_grey = NULL;
IplImage *g_binary = NULL;
IplImage *g_hsv = NULL;
IplImage *g_value = NULL;

int ocr_dbg(IplImage* img, char* name) {
    //display ocr png
    cvNamedWindow(name, CV_WINDOW_AUTOSIZE);
    cvShowImage(name, img);
    //waitKey(0);
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

IplImage* ocr_rgb2hsv(IplImage* srcImg) {
    IplImage *hsv, *hue, *saturation, *value;

    //hsv
    hsv = cvCreateImage(cvGetSize(srcImg), 8, 3);
    cvCvtColor(srcImg, hsv, CV_BGR2HSV);
    return hsv;
}

IplImage* ocr_gethsv(IplImage* srcImg, int hsv_channel) {
    IplImage *hue, *saturation, *value;
    switch(hsv_channel)
    {
        case E_HUE:
            hue = cvCreateImage(cvGetSize(srcImg), 8, 1);
            cvSplit(srcImg, hue, 0, 0, 0);
            return hue;
        case E_SATURATION:
            saturation = cvCreateImage(cvGetSize(srcImg), 8, 1);
            cvSplit(srcImg, 0, saturation, 0, 0);
            return  saturation;
        case E_VALUE:
            value = cvCreateImage(cvGetSize(srcImg), 8, 1);
            cvSplit(srcImg, 0, 0, value, 0);
            return  value;
        default:
            return 0;
    }
}

IplImage* ocr_smooth(IplImage* srcImg, int smooth_type)
{
    IplImage* desImg;
    cvSmooth(srcImg, desImg, smooth_type, 3, 3);
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
    g_binary = ocr_binary(g_grey, DEFAULT_THRESHOLD, E_OTSU, DEFAULT_SIZE, DEFAULT_DELTA);
    //g_binary = ocr_binary(g_binary, DEFAULT_THRESHOLD, E_GAUSSIAN, DEFAULT_SIZE, DEFAULT_DELTA);
    ocr_dbg(g_binary, "grey");
    g_binary = ocr_smooth(g_binary, CV_MEDIAN);
    g_binary = ocr_binary(g_binary, DEFAULT_THRESHOLD, E_GAUSSIAN, DEFAULT_SIZE, DEFAULT_DELTA);
    ocr_dbg(g_binary, "binary_grey");
	//g_hsv = ocr_rgb2hsv(g_img);
    //g_value = ocr_gethsv(g_hsv, E_VALUE);

    waitKey(0);
    return 0;
}
