#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "ocr_read.h"

using namespace cv;
using namespace std;

//threshold define
#define OCR_DBG 0
#define DEFAULT_THRESHOLD 210
#define DEFAULT_SIZE 3
#define DEFAULT_DELTA 3

void ocr_dbg(IplImage* img, char* name) {
    //display ocr png
#if OCR_DBG
    cvNamedWindow(name, CV_WINDOW_AUTOSIZE);
    cvShowImage(name, img);
    waitKey(0);
#endif
}

IplImage* ocr_read(const char* ocr_name) {
    return cvLoadImage(ocr_name, CV_LOAD_IMAGE_UNCHANGED);
}

int ocr_write(IplImage* srcImg, const char* ocr_name) {
    cvSaveImage(ocr_name, srcImg);
    return 0;
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
            cvThreshold(srcImg, desImg, threshold, 255, CV_THRESH_BINARY_INV);
            break;
        case E_GAUSSIAN: //adaptive gaussian threshold
            cvAdaptiveThreshold(srcImg, desImg, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, size, delta);
            break;
        case E_MEAN: //adaptive mean threshold
            cvAdaptiveThreshold(srcImg, desImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, size, delta);
            break;
        case E_OTSU: //adaptive otsu threshold
            cvThreshold(srcImg, desImg, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
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

int ocr_preprocess(const char* srcImg, const char* desImg) {

    IplImage *org_img, *grey_img, *binary_img;
    org_img = ocr_read(srcImg);
    grey_img = ocr_grey(org_img);
    binary_img = ocr_binary(grey_img, DEFAULT_THRESHOLD, E_OTSU, DEFAULT_SIZE, DEFAULT_DELTA);
    binary_img = ocr_smooth(binary_img, CV_MEDIAN);
    ocr_write(binary_img, desImg);
}


