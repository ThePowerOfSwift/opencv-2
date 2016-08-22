#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "ocr_read.h"

using namespace cv;
using namespace std;

//threshold define
#define OCR_DBG 1
#define DEFAULT_THRESHOLD 210
#define DEFAULT_SIZE 3
#define DEFAULT_DELTA 3

void ocr_dbg(Mat mSrcImg, const char* name) {
    //display ocr png
#if OCR_DBG
    namedWindow(name,CV_WINDOW_AUTOSIZE);
    imshow(name, mSrcImg);
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

Mat ocr_blur(Mat mSrcImg, int index) {
    Mat mDesImg;
    blur(mSrcImg, mDesImg, Size(index, index));
    return  mDesImg;
}

Mat ocr_dilate(Mat mSrcImg, int index) {
    Mat mDesImg;
    Mat element(index, index, CV_8U, Scalar(255));
    dilate(mSrcImg, mDesImg, element);
    return mDesImg;
}

Mat ocr_erode(Mat mSrcImg, int index) {
    Mat mDesImg;
    Mat element(index, index, CV_8U, Scalar(255));
    erode(mSrcImg, mDesImg, element);
    return mDesImg;
}

Mat ocr_cut(Mat mSrcImg, const char* desImg) {

    Mat mDilateImg, mCannyImg;

    //dilate img
    mDilateImg = ocr_dilate(mSrcImg, 3);

    //the pram. for findContours,
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using canny
    Canny(mDilateImg, mCannyImg, 80, 255, 3);
    /// Find contours
    findContours(mCannyImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    int count = 0;
    int num = contours.size();
    //remove small area ,  width*height < 50
    for (count = 0; count < contours.size(); count++) {
        Rect aRect =boundingRect(contours[count]);
        if (aRect.width * aRect.height < 30) {
            printf("remove small size = %d\n", count);
            num--;
            //删除面积小于设定值的轮廓
            contours.erase(contours.begin() + count);
            continue;
        }
    }

    Rect* pRect = new Rect[num];
    int valid_num = num;
    //check reuse area
    for(count = 0;  count < num; count++) {
        Rect aRect = boundingRect(contours[count]);
        pRect[count] = aRect;
        for (int tmp = count + 1; tmp < contours.size(); tmp++) {

            Rect bRect = boundingRect(contours[tmp]);

            if ((aRect.x + aRect.width < bRect.x + aRect.width/10) \
              || (aRect.x > bRect.x + bRect.width - aRect.width/10)) {
                continue;
            } else {
                //x & width use max
                if(aRect.x > bRect.x)
                    pRect[count].x = bRect.x;
                if(aRect.x + aRect.width > bRect.x + bRect.width) {
                    pRect[count].width = aRect.x + aRect.width - pRect[count].x;
                } else {
                    pRect[count].width = bRect.x + bRect.width - pRect[count].x;
                }

                //y & height use max
                if(aRect.y > bRect.y)
                    pRect[count].y = bRect.y;
                if(aRect.y + aRect.height > bRect.y + bRect.height) {
                    pRect[count].height = aRect.y + aRect.height - pRect[count].y;
                } else {
                    pRect[count].height = bRect.y + bRect.height - pRect[count].y;
                }
                valid_num --;
                printf("%d and %d is reuse area\n", count, tmp);
            }
        }
    }

    printf("area total: %d, area valid: %d\n", num, valid_num);
    Rect stRectTemp;
    int idx0, idx1;
    for(idx0 = 0;  idx0 < num; idx0++) {
        for (idx1 = 0; idx1 < num - idx0 - 1; idx1++) {
            if ((pRect + idx1)->x > (pRect + 1 + idx1)->x) {
                stRectTemp = *(pRect + idx1);
                *(pRect + idx1) = *(pRect + 1 + idx1);
                *(pRect + 1 + idx1) = stRectTemp;
            }
        }
    }

    //only for show contours debug
#if 1
    /// Draw contours,彩色轮廓
    Mat mDrawImg = Mat::zeros(mCannyImg.size(), CV_8UC3);
    for(idx0 = 0; idx0 < num; idx0++) {
        printf("x: %d, y: %d, width: %d, height: %d\n", pRect[idx0].x, pRect[idx0].y, \
          pRect[idx0].width, pRect[idx0].height);
        if((pRect[idx0-1].x == pRect[idx0].x) && (idx0 != 0)) {
            printf("idx0-cont: %d\n", idx0);
            continue;
        }
            printf("idx0 = %d\n", idx0);
            rectangle(mDrawImg, pRect[idx0], Scalar(0,0,255), 3, 8, 0);//用矩形画矩形窗
    }
    imshow("draw_contours", mDrawImg);
#endif

    return mSrcImg;
}

Mat ocr_preprocess(const char* srcImg, const char* desImg) {

    Mat mGrey, mBinary, mSmooth;
    mGrey = ocr_read_grey(srcImg);
    mGrey = ocr_blur(mGrey, 2);
    mBinary = ocr_binary(mGrey, DEFAULT_THRESHOLD, E_OTSU, DEFAULT_SIZE, DEFAULT_DELTA);
    ocr_dbg(mBinary, "Bianry");
    mSmooth = ocr_smooth(mBinary, CV_MEDIAN);
    ocr_dbg(mSmooth, "Smooth");
    ocr_write(mSmooth, desImg);
    return mSmooth;
}


