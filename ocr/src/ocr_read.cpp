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

Mat ocr_read(const char* ocr_name) {
    return imread(ocr_name, IMREAD_UNCHANGED);
}

int ocr_write(Mat mSrcImg, const char* ocr_name) {
    imwrite(ocr_name, mSrcImg);
    return 0;
}

Mat ocr_read_grey(const char* ocr_name) {
    return imread(ocr_name, IMREAD_GRAYSCALE);
}

Mat ocr_binary(Mat mSrcImg, double thresh, int method, int size, double delta) {
    //convert to binary
    Mat mDesImg;
    switch(method) {
        case E_NORMAL: //normal threshold
        default:
            threshold(mSrcImg, mDesImg, thresh, 255, THRESH_BINARY_INV);
            break;
        case E_GAUSSIAN: //adaptive gaussian threshold
            adaptiveThreshold(mSrcImg, mDesImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, size, delta);
            break;
        case E_MEAN: //adaptive mean threshold
            adaptiveThreshold(mSrcImg, mDesImg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, size, delta);
            break;
        case E_OTSU: //adaptive otsu threshold
            threshold(mSrcImg, mDesImg, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
    }
    return mDesImg;
}

Mat ocr_rgb2hsv(Mat mSrcImg) {
    Mat mHsv;
    //hsv
    cvtColor(mSrcImg, mHsv, COLOR_RGB2HSV);
    return mHsv;
}

Mat ocr_gethsv(Mat mSrcImg, int hsv_channel) {
    Mat mHsv, mHue, mSaturation, mValue, mError;
    vector<Mat> mChannels;

    cvtColor(mSrcImg, mHsv, COLOR_RGB2HSV);
    split(mHsv, mChannels);
    switch(hsv_channel) {
        case E_HUE:
            mHue = mChannels.at(0);
            return mHue;
        case E_SATURATION:
            mSaturation = mChannels.at(1);
            return  mSaturation;
        case E_VALUE:
            mValue = mChannels.at(2);
            return  mValue;
        default:
            return mError;
    }
}

Mat ocr_smooth(Mat mSrcImg, int smooth_type) {
    Mat mDes;
    switch(smooth_type) {
        case CV_MEDIAN:
            medianBlur(mSrcImg, mDes, 3);
            break;
        default:
            break;
    }
    return mDes;
}

int ocr_preprocess(const char* srcImg, const char* desImg) {

    Mat mGrey, mBinary, mSmooth;
    mGrey = ocr_read_grey(srcImg);
    mBinary = ocr_binary(mGrey, DEFAULT_THRESHOLD, E_OTSU, DEFAULT_SIZE, DEFAULT_DELTA);

    mSmooth = ocr_smooth(mBinary, CV_MEDIAN);
    ocr_write(mSmooth, desImg);
    return 0;
}


