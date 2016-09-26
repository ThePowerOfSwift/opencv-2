#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "ocr_read.h"
#include<iostream>

using namespace cv;
using namespace std;

//threshold define
#define OCR_DBG 0
#define DEFAULT_THRESHOLD 200
#define DEFAULT_SIZE 3
#define DEFAULT_DELTA 3

void ocr_dbg(Mat mSrcImg, const char* name) {
    //display ocr png
#if OCR_DBG
    namedWindow(name,CV_WINDOW_AUTOSIZE);
    imshow(name, mSrcImg);
#endif
}

static int ocr_max(int a, int b) {
    return a>=b?a:b;
}

static int ocr_min(int a, int b) {
    return a<=b?a:b;
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

Mat ocr_binary(Mat mSrcImg, double thresh, int method, int size, double div) {
    //convert to binary
    Mat mDesImg;
    switch(method) {
        case E_NORMAL: //normal threshold
        default:
            threshold(mSrcImg, mDesImg, thresh, 255, THRESH_BINARY_INV);
            break;
        case E_GAUSSIAN: //adaptive gaussian threshold
            adaptiveThreshold(mSrcImg, mDesImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, size, div);
            break;
        case E_MEAN: //adaptive mean threshold
            adaptiveThreshold(mSrcImg, mDesImg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, size, div);
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

Mat ocr_mergeMatCol(Mat mMat1, Mat mMat2) {
    CV_Assert(mMat2.rows == mMat1.rows);//行数不相等，出现错误中断
    Mat mMerge, mTemp1, mTemp2;
    mMerge.create(mMat1.rows, mMat1.cols+mMat2.cols, mMat1.type());
    mTemp1 = mMerge.colRange(0, mMat1.cols);
    mMat1.copyTo(mTemp1);
    mTemp2 = mMerge.colRange(mMat1.cols, mMat1.cols + mMat2.cols);
    mMat2.copyTo(mTemp2);
    return  mMerge;
}

Mat ocr_mergeMatRow(Mat mMat1, Mat mMat2) {
    CV_Assert(mMat2.rows == mMat1.rows);//行数不相等，出现错误中断
    Mat mMerge, mTemp1, mTemp2;
    mMerge.create(mMat1.rows + mMat2.rows, mMat1.cols, mMat1.type());
    mTemp1 = mMerge.rowRange(0, mMat1.rows);
    mMat1.copyTo(mTemp1);
    mTemp2 = mMerge.rowRange(mMat1.rows, mMat1.rows + mMat2.rows);
    mMat2.copyTo(mTemp2);
    return  mMerge;
}

/*fill image as follows:
 *     ***img***      */
Mat ocr_xfill(Mat mMat1, int col) {
    Mat mFill;
    Mat mTemp = Mat::zeros(mMat1.rows, col, mMat1.type());
    //mTemp.create(mMat1.rows, col, mMat1.type());
    //Mat mTemp(mMat1.rows, col, mMat1.type(), Sca);
    mFill = ocr_mergeMatCol(mTemp, mMat1);
    mFill = ocr_mergeMatCol(mFill, mTemp);
    return mFill;
}

static Mat ocr_sub_cut(Mat mSrcImg, const char* srcImgPath, int offset, int width, const char* cutImgPath) {
    Mat mImg = imread(srcImgPath, IMREAD_UNCHANGED);
    Mat mSubImg = mImg(Range(0, mImg.rows), Range(offset, offset+width));
    ocr_write(mSubImg, cutImgPath);

    int x1 = 0;
    int x2 = 0;
    uchar* ptr_m = mSrcImg.ptr<uchar>(mSrcImg.rows>>1);
    for(int j=0; j<mSrcImg.cols; j++) {
        if((ptr_m[j] == 255) && (ptr_m[j+1] == 255) && (ptr_m[j+2] == 255) && (x1 == 0))
            x1 = j+2;
        if((ptr_m[mSrcImg.cols-j] == 255) && (ptr_m[mSrcImg.cols-j-1] == 255) && (ptr_m[mSrcImg.cols-j-2] == 255) && (x2 == 0))
            x2 = mSrcImg.cols-j-2;
    }

    vector<Mat> mChannels;
    split(mSubImg, mChannels);
    Mat mRed = mChannels.at(2);
    Mat mGreen = mChannels.at(1);
    Mat mBlue = mChannels.at(0);
    uchar gv1,bv1,rv1, gv2, bv2, rv2;
    uchar* ptr_mR = mRed.ptr<uchar>(mSrcImg.rows>>1);
    uchar* ptr_mB = mBlue.ptr<uchar>(mSrcImg.rows>>1);
    uchar* ptr_mG = mGreen.ptr<uchar>(mSrcImg.rows>>1);
    gv1 = ptr_mG[x1];
    gv2 = ptr_mG[x2];
    bv1 = ptr_mB[x1];
    bv2 = ptr_mB[x2];
    rv1 = ptr_mR[x1];
    rv2 = ptr_mR[x2];

    //printf("x1=%d, x2=%d, gv1=%d, gv2=%d, bv1=%d, bv2=%d, rv1=%d, rv2=%d\n", x1, x2, gv1, gv2, bv1, bv2, rv1, rv2);
    //printf("%d, %d, %d, %d\n", mBlue.rows, mBlue.cols, mSrcImg.rows, mSrcImg.cols);
    int x1_max=0;
    int x2_max=mSrcImg.cols;
    for(int i=0; i<mSrcImg.rows; i++) {
        ptr_mR = mRed.ptr<uchar>(i);
        ptr_mB = mBlue.ptr<uchar>(i);
        ptr_mG = mGreen.ptr<uchar>(i);
        ptr_m = mSrcImg.ptr<uchar>(i);
        for(int j=0; j<mSrcImg.cols; j++) {
            if((ptr_mR[j] > rv1-10) && (ptr_mR[j] < rv1+10) \
                 && (ptr_mB[j] > bv1-10) && (ptr_mB[j] < bv1+10) \
                 && (ptr_mG[j] > gv1-10) && (ptr_mG[j] < gv1+10)) {
                //ptr_m[j] = 255;
                if(x1_max>x2_max)
                    x1_max=0;
                if((j>x1_max) && (ptr_m[j-1] == 255) && (x1_max < x2_max))
                    x1_max=j;
            }
            else if((ptr_mR[j] > rv2-10) && (ptr_mR[j] < rv2+10) \
                 && (ptr_mB[j] > bv2-10) && (ptr_mB[j] < bv2+10) \
                 && (ptr_mG[j] > gv2-10) && (ptr_mG[j] < gv2+10)) {
                if(x2_max<x1_max)
                    x2_max=mSrcImg.cols;
                if((j<x2_max) && (x2_max > x1_max))
                    x2_max=j;
                //ptr_m[j] = 0;
            }
        }
    }
    printf("x1_max=%d, x2_max=%d\n", x1_max, x2_max);
    printf("%s\n", cutImgPath);
    Mat roiImg1 = mSrcImg(Range(0, mSrcImg.rows), Range(0,x1_max+3));
    roiImg1 = ocr_xfill(roiImg1, 5);
    Mat roiImg2 = mSrcImg(Range(0, mSrcImg.rows), Range(x2_max-3, mSrcImg.cols));
    roiImg2 = ocr_xfill(roiImg2, 5);
    char cmd[128];
    strcpy(cmd, cutImgPath);
    char *p = strstr(cmd, ".png");
    p--;
    *p -= 1;
    //printf("cmd = %s\n", cmd);
    ocr_write(roiImg1, cmd);
    ocr_write(roiImg2, cutImgPath);
    return mSubImg;
}

/* div value is between 1~100 , must not be set to zero */
int ocr_cut(Mat mSrcImg, const char* srcImgPath, const char* desImgDir, int div, int count) {

    Mat mDilateImg, mErodeImg, mCannyImg;
    int idx0, idx1, valid_num;
    int m_offset = 0;
    int m_width = 0;

    //dilate img
    mDilateImg = ocr_dilate(mSrcImg, 3);
    //imshow("dialte", mDilateImg);
    //erode img
    mErodeImg = ocr_erode(mDilateImg, 3);
    //imshow("erode", mErodeImg);

    //the pram. for findContours,
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using canny
    //Canny(mDilateImg, mCannyImg, 80, 255, 3);
    Canny(mErodeImg, mCannyImg, 80, 255, 3);
    /// Find contours
    findContours(mCannyImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    valid_num = contours.size();
    //printf("valid_num = %d\n", valid_num);
    idx1 = 0;
    Rect* pRect = new Rect[valid_num];
    //remove small area ,  width*height < 50
    for (idx0 = 0; idx0 < contours.size(); idx0++) {
        Rect aRect =boundingRect(contours[idx0]);
        if (aRect.width * aRect.height < 30) {
            //printf("remove small size = %d\n", idx0);
            valid_num--;
            //删除面积小于设定值的轮廓
            //contours.erase(contours.begin() + idx0);
            continue;
        }
        //printf("x: %d, y: %d, width: %d, height: %d\n", aRect.x, aRect.y, \
          aRect.width, aRect.height);
        pRect[idx1++] = boundingRect(contours[idx0]);
    }

    Rect stRectTemp;
    for(idx0 = 0;  idx0 < valid_num; idx0++) {
        for (idx1 = 0; idx1 < valid_num - idx0 - 1; idx1++) {
            if ((pRect + idx1)->x > (pRect + 1 + idx1)->x) {
                stRectTemp = *(pRect + idx1);
                *(pRect + idx1) = *(pRect + 1 + idx1);
                *(pRect + 1 + idx1) = stRectTemp;
            }
        }
    }

    int p_start, p_end;
    //check reuse area
    for(idx0 = 0;  idx0 < valid_num; idx0++) {
        Rect aRect = pRect[idx0];
        for (idx1 = idx0 + 1; idx1 < valid_num; idx1++) {

            Rect bRect = pRect[idx1];

            if ((pRect[idx0].x + pRect[idx0].width < pRect[idx1].x + div) \
              || (pRect[idx0].x > pRect[idx1].x + pRect[idx1].width - div)) {
                continue;
            } else {
                p_start = ocr_min(pRect[idx0].x, pRect[idx1].x);
                p_end = ocr_max(pRect[idx0].x + pRect[idx0].width, pRect[idx1].x + pRect[idx1].width);
                //x & width use max
                pRect[idx0].x = p_start;
                pRect[idx1].x = p_start;
                pRect[idx0].width = p_end - p_start;
                pRect[idx1].width = pRect[idx0].width;

                 //height default use image max height, so no use to compare
                /*
                //y & height use max
                p_start = ocr_min(pRect[idx0].y, pRect[idx1].y);
                p_end = ocr_max(pRect[idx0].y + pRect[idx0].height, pRect[idx1].y + pRect[idx1].height);
                //y & width use max
                pRect[idx0].y = p_start;
                pRect[idx1].y = p_start;
                pRect[idx0].height = p_end - p_start;
                pRect[idx1].height = pRect[idx0].height;*/
            }
        }
    }

    /*for(idx0 = 0; idx0 < valid_num; idx0++) {
        printf("x: %d, y: %d, width: %d, height: %d\n", pRect[idx0].x, pRect[idx0].y, \
                  pRect[idx0].width, pRect[idx0].height);
    }
    printf("valid_num = %d\n", valid_num);*/

    //only for show contours debug
    Mat roiImg;
    char cutPath[128] = {0};
    idx1 = 0;
    /// Draw contours,彩色轮廓
    Mat mDrawImg = Mat::zeros(mCannyImg.size(), CV_8UC3);
    for(idx0 = 0; idx0 < valid_num; idx0++) {

        if(idx0 != 0) {
            if (((pRect[idx0 -1].x + pRect[idx0 -1].width < pRect[idx0].x + div) \
               || (pRect[idx0 -1].x > pRect[idx0 -1].x + pRect[idx0].width - div)) && (pRect[idx0].width > 8)) {
                rectangle(mDrawImg, pRect[idx0], Scalar(0, 0, 255), 3, 8, 0);//用矩形画矩形窗
                if(pRect[idx0].width > 40) {
                    //printf("x: %d, y: %d, width: %d, height: %d\n", pRect[idx0].x, pRect[idx0].y, \
                       pRect[idx0].width, pRect[idx0].height);
                    m_offset = pRect[idx0].x;
                    m_width = pRect[idx0].width;
                    idx1++;
                }
                // y direction not cut , just use origin img height
                roiImg = mSrcImg(Range(0, mSrcImg.rows), \
                    Range(pRect[idx0].x, pRect[idx0].x + pRect[idx0].width));
                if(m_width == 0)
                    roiImg = ocr_xfill(roiImg, 5);
                memset(cutPath, 0, sizeof(cutPath));
                snprintf(cutPath, sizeof(cutPath), "./%s/%d_%d.png", desImgDir, count, idx1);
                ocr_write(roiImg, cutPath);
                idx1 ++;
                //imshow("roi", roiImg);

            } else {
                continue;
            }
        } else if(pRect[idx0].width > 8) {
            rectangle(mDrawImg, pRect[idx0], Scalar(0, 0, 255), 3, 8, 0);//用矩形画矩形窗
            if(pRect[idx0].width > 40) {
                //printf("x: %d, y: %d, width: %d, height: %d\n", pRect[idx0].x, pRect[idx0].y, \
                   pRect[idx0].width, pRect[idx0].height);
                idx1++;
                m_offset = pRect[idx0].x;
                m_width = pRect[idx0].width;
            }
            // y direction not cut , just use origin img height
            roiImg = mSrcImg(Range(0, mSrcImg.rows), \
                    Range(pRect[idx0].x, pRect[idx0].x + pRect[idx0].width));
            if(m_width == 0)
                roiImg = ocr_xfill(roiImg, 5);
            memset(cutPath, 0, sizeof(cutPath));
            snprintf(cutPath, sizeof(cutPath), "./%s/%d_%d.png", desImgDir, count, idx1);
            ocr_write(roiImg, cutPath);
            idx1 ++;
        }

        if(m_width != 0){
           // printf("m_width = %d, m_offset = %d\n", m_width, m_offset);
            ocr_sub_cut(roiImg, srcImgPath, m_offset, m_width, cutPath);
            m_width = 0;
        }
    }
    //imshow("draw_contours", mDrawImg);

    //printf("single ocr num: %d\n", idx1);
    return idx1;
}

int ocr_rgb_histogram (Mat mSrcImg) {
    Mat mChannelsRGB[3];//分离的BGR通道
    MatND outputRGB[3];//输出直方图分量
    int hisSize[1],hisWidth,hisHeight;//直方图的大小,宽度和高度
    float range[2];//直方图取值范围
    const float *ranges;

    hisSize[0] = 256;
    hisWidth = 400;
    hisHeight = 400;
    range[0] = 0.0;
    range[1] = 255.0;
    ranges = &range[0];

    split(mSrcImg, mChannelsRGB);
    calcHist(&mChannelsRGB[0],1,0,Mat(),outputRGB[0],1,hisSize,&ranges);
    calcHist(&mChannelsRGB[1],1,0,Mat(),outputRGB[1], 1, hisSize, &ranges);
    calcHist(&mChannelsRGB[2],1,0,Mat(),outputRGB[2], 1, hisSize, &ranges);

    //输出各个bin的值
    for (int i = 0; i < hisSize[0]; ++i) {
        cout << i << "   B:" << outputRGB[0].at<float>(i);
        cout <<"   G:" << outputRGB[1].at<float>(i);
        cout << "   R:" << outputRGB[2].at<float>(i) << endl;
    }

    //显示直方图
    Mat rgbHist[3];
    for (int i = 0; i < 3; i++) {
        rgbHist[i] = Mat(hisWidth, hisHeight, CV_8UC3, Scalar::all(0));
    }
    normalize(outputRGB[0], outputRGB[0], 0, hisWidth - 20, NORM_MINMAX);
    normalize(outputRGB[1], outputRGB[1], 0, hisWidth - 20, NORM_MINMAX);
    normalize(outputRGB[2], outputRGB[2], 0, hisWidth - 20, NORM_MINMAX);
    for (int i = 0; i < hisSize[0]; i++) {
        int val = saturate_cast<int>(outputRGB[0].at<float>(i));
        rectangle(rgbHist[0], Point(i * 2 + 10, rgbHist[0].rows), Point((i + 1) * 2 + 10, rgbHist[0].rows - val), \
            Scalar(0, 0, 255), 1, 8);
        val = saturate_cast<int>(outputRGB[1].at<float>(i));
        rectangle(rgbHist[1], Point(i * 2 + 10, rgbHist[1].rows), Point((i + 1) * 2 + 10, rgbHist[1].rows - val), \
            Scalar(0, 255, 0), 1, 8);
        val = saturate_cast<int>(outputRGB[2].at<float>(i));
        rectangle(rgbHist[2], Point(i * 2 + 10, rgbHist[2].rows), Point((i + 1) * 2 + 10, rgbHist[2].rows - val), \
            Scalar(255, 0, 0), 1, 8);
    }

    imshow("R", rgbHist[0]);
    imshow("G", rgbHist[1]);
    imshow("B", rgbHist[2]);
    imshow("image", mSrcImg);

    Mat mHsv;
    mHsv = mSrcImg;

    int iLowH = 130;
    int iHighH = 137;

    int iLowS = 1;
    int iHighS = 254;

    int iLowV = 1;
    int iHighV = 254;
    Mat imgThresholded;

    inRange(mHsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
    imshow("Thresholded Image", imgThresholded); //show the thresholded image
    imshow("mhsv", mHsv);

    return 0;
}

int ocr_hsv_histogram(Mat mSrcImg) {
    Mat mHue;
    MatND mHueHist;
    int hisSize[1];
    hisSize[0] = 256;
    Mat mHsv;

    int iLowH = 104;
    int iHighH = 107;

    int iLowS = 1;
    int iHighS = 254;

    int iLowV = 1;
    int iHighV = 254;

    cvtColor(mSrcImg, mHsv, COLOR_RGB2HSV);
    mHue =ocr_gethsv(mSrcImg, E_HUE);

    float hranges[] = { 0, 256};
    const float *ranges;
    ranges = &hranges[0];
    calcHist(&mHue, 1, 0, Mat(), mHueHist, 1, hisSize, &ranges);

    //输出各个bin的值
    for (int i = 0; i < hisSize[0]; ++i) {
        printf("i = %d : ", i);
        cout << "   B:" << mHueHist.at<float>(i);
        printf("\n");
    }

    //显示直方图
    Mat hueHist;
    hueHist = Mat(400, 400, CV_8UC3, Scalar::all(0));
    normalize(mHueHist, mHueHist, 0, 400 - 20, NORM_MINMAX);

    for (int i = 0; i < hisSize[0]; i++) {
        int val = saturate_cast<int>(mHueHist.at<float>(i));
        rectangle(hueHist, Point(i * 2 + 10, hueHist.rows), Point((i + 1) * 2 + 10, hueHist.rows - val), \
            Scalar(0, 0, 255), 1, 8);
    }

    imshow("R", hueHist);

    Mat imgThresholded;

    inRange(mHsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
    imshow("Thresholded Image", imgThresholded); //show the thresholded image
    imshow("mhsv", mHsv);

#if 0
    Mat src, hsv;
    src = mSrcImg;

    cvtColor(src, hsv, CV_BGR2HSV);
    vector<Mat> hsv_plane;
    split(hsv, hsv_plane);
    Mat inputs[]={hsv_plane[1], hsv_plane[2], hsv_plane[0]};

    vector<Mat> mixmat_plane;
    mixmat_plane.push_back(hsv_plane[2]);
    mixmat_plane.push_back(hsv_plane[0]);
    Mat mixmat;
    merge(mixmat_plane, mixmat);
    Mat mixed[]={mixmat,hsv_plane[1]};

    int vbins = 128, sbins = 128, hbins = 128;
    int histSize[] = {sbins, vbins, hbins};
    float sranges[] = { 0, 256};
    float vranges[] = { 0, 256};
    float hranges[] = { 0, 256};
    const float*ranges[] = {sranges, vranges, hranges};
    MatND hist;

    //#define SINGLE_MAT
#define MIX_MAT

#ifdef SINGLE_MAT
    /*
use one multi-channel mat, channels param gives the channels used;
使用多通道的图像计算多维直方图，可以计算1,2,3维的;
*/  int channels[] = {1, 2};
    calcHist(&hsv, 1, channels, Mat(),hist, 2, histSize, ranges,true, false );
#elif defined MIX_MAT
/*
use mix mat array, the first elem is a single channel mat, second is a two channel mat;
使用混合通道图像数组，第1个图像是2通道的，第2个是单通道的;
channels指定每一维对应的通道;
*/
    int channels[] = {1, 2, 0};

// #define DIM_2
#ifdef DIM_2
    //统计二维直方图;
    calcHist(mixed, 2, channels, Mat(),hist, 2, histSize, ranges,true, false);
#else
//统计三维直方图;
    calcHist(mixed, 2, channels, Mat(),hist, 3, histSize, ranges,true, false);
#endif

#else
    /*
use multi-mat arrays, channels param gives the array mat and its channels used;
使用都是单通道图像数组计算2维直方图--也可以计算3维的;
*/
    int channels[] = {2, 1};
    hbins = 1;
    calcHist(inputs, 3, channels, Mat(),hist, 2, histSize, ranges,true, false );
#endif

#ifndef MIX_MAT
    double maxVal=0;
    minMaxLoc(hist, 0, 0, 0, 0);//only can process mat that dims<=2--minMaxLoc只能处理2维以下的;
#endif

    int scale = 4;
    Mat histImg = Mat::zeros(vbins*scale, sbins*scale, CV_8UC3);
    float *hist_sta = new float[sbins];
    float *hist_val = new float[vbins];
    float *hist_hue = new float[hbins];
    memset(hist_val, 0, vbins*sizeof(float));
    memset(hist_sta, 0, sbins*sizeof(float));
    memset(hist_hue, 0, hbins*sizeof(float));

    for( int s = 0; s < sbins; s++ )
    {
        for( int v = 0; v < vbins; v++ )
        {
            for(int h=0; h<hbins; h++)
            {
#ifdef MIX_MAT
//-----------------------------------------------------------//
#ifdef DIM_2
                float binVal = hist.at<float>(s, v);
#else
                float binVal = hist.at<float>(s, v, h);
                hist_hue[h] += binVal;
#endif
//-----------------------------------------------------------//
#else
                float binVal = hist.at<float>(s, v);
                int intensity = cvRound(binVal*255/maxVal);
                rectangle( histImg, Point(s*scale, v*scale),Point((s+1)*scale-1, (v+1)*scale-1), Scalar::all(intensity), CV_FILLED);
#endif
                hist_val[v] += binVal;
                hist_sta[s] += binVal;

            }
        }
    }
    //find max bin value;
    double max_sta=.0, max_val=.0,max_hue=.0;
    for(int i=0; i<sbins; ++i)
    {
        if(hist_sta[i]>max_sta)
            max_sta = hist_sta[i];
    }
    for(int i=0; i<vbins; ++i)
    {
        if(hist_val[i]>max_val)
            max_val = hist_val[i];
    }
    for(int i=0; i<hbins; ++i)
    {
        if(hist_hue[i]>max_hue)
            max_hue = hist_hue[i];
    }

    Mat sta_img = Mat::zeros(310, sbins*scale+20, CV_8UC3);
    Mat val_img = Mat::zeros(310, vbins*scale+20, CV_8UC3);
    Mat hue_img = Mat::zeros(310, hbins*scale+20, CV_8UC3);

    for(int i=0; i<sbins; ++i)
    {
        int intensity = cvRound(hist_sta[i]*(sta_img.rows-10)/max_sta);
        rectangle(sta_img, Point(i*scale+10, sta_img.rows-intensity),Point((i+1)*scale-1+10, sta_img.rows-1), Scalar(0,255,0), 1);
    }
    for(int i=0; i<vbins; ++i)
    {
        int intensity = cvRound(hist_val[i]*(val_img.rows-10)/max_val);
        rectangle(val_img, Point(i*scale+10, val_img.rows-intensity),Point((i+1)*scale-1+10, val_img.rows-1), Scalar(0,0,255), 1);
    }
    for(int i=0; i<hbins; ++i)
    {
        int intensity = cvRound(hist_hue[i]*(hue_img.rows-10)/max_hue);
        rectangle(hue_img, Point(i*scale+10, hue_img.rows-intensity),Point((i+1)*scale-1+10, hue_img.rows-1), Scalar(255,0,0), 1);
    }

    namedWindow( "Source");
    imshow( "Source", src );
    namedWindow( "Histogram");
    imshow( "Histogram", histImg );
    namedWindow("dim1");
    imshow("dim1", sta_img);
    namedWindow("dim2");
    imshow("dim2", val_img);
    namedWindow("dim3");
    imshow("dim3", hue_img);
#endif

    return 0;
}

Mat ocr_preprocess(const char* srcImg, const char* desImg) {

    Mat mGrey, mBinary, mSmooth;
    mGrey = ocr_read_grey(srcImg);
    mGrey = ocr_blur(mGrey, 2);
    //mBinary = ocr_binary(mGrey, DEFAULT_THRESHOLD, E_OTSU, DEFAULT_SIZE, DEFAULT_DELTA);
    mBinary = ocr_binary(mGrey, DEFAULT_THRESHOLD, E_NORMAL, DEFAULT_SIZE, DEFAULT_DELTA);
    ocr_dbg(mBinary, "Bianry");
    mSmooth = ocr_smooth(mBinary, CV_MEDIAN);
    ocr_dbg(mSmooth, "Smooth");
    ocr_write(mSmooth, desImg);
    return mSmooth;
}

Mat ocr_filter(const char* srcImg, const char* desImg) {
    Mat mImg, mGray;
    int pos_delta = 10;
    uchar color_detla = 5;
    mImg = imread(srcImg, IMREAD_UNCHANGED);
    mGray = imread(srcImg, IMREAD_GRAYSCALE);
    Mat mTop = mGray(Range(0, pos_delta), \
                    Range(0, mGray.cols));
    Mat mBottom =  mGray(Range(mGray.rows - pos_delta, mGray.rows), \
                    Range(0, mGray.cols));

    vector<Mat> mChannels;
    split(mImg, mChannels);
    Mat mRed = mChannels.at(2);
    Mat mGreen = mChannels.at(1);
    Mat mBlue = mChannels.at(0);
    uchar gv,bv,rv;
    int x,y;
    x=0;
    y=0;
    for(int i=0; i<pos_delta; i++) {
        uchar* pTop = mTop.ptr<uchar>(i);
        uchar* pBottom = mBottom.ptr<uchar>(i);

        for(int j=0; j<mGray.cols; j++) {
            if ((pTop[j] < 200)) {
                x = i;
                y = j;
                goto exit;
            } else if(pBottom[j] < 195) {
                x = mGray.rows - pos_delta + i;
                y = j;
                goto exit;

            }
        }
    }
    exit:
    if(y!=0) {
        uchar *ptr_Red = mRed.ptr<uchar>(x);
        uchar *ptr_Green = mGreen.ptr<uchar>(x);
        uchar *ptr_Blue = mBlue.ptr<uchar>(x);
        bv = ptr_Blue[y];
        rv = ptr_Red[y];
        gv = ptr_Green[y];
        //printf("x=%d, y=%d, bv=%d, rv=%d, gv=%d\n", x, y, bv, rv, gv);
        for(int i=0; i<mGray.rows; i++) {
            ptr_Red = mRed.ptr<uchar>(i);
            ptr_Green = mGreen.ptr<uchar>(i);
            ptr_Blue = mBlue.ptr<uchar>(i);
            uchar* ptr_Gray = mGray.ptr<uchar>(i);
            for(int j=0; j<mGray.cols; j++) {
                if((ptr_Red[j] <= rv+color_detla) && (ptr_Red[j] >= rv-color_detla) \
                    && (ptr_Blue[j] <= bv+color_detla) && (ptr_Blue[j] >= bv-color_detla) \
                    && (ptr_Green[j] <= gv+color_detla) && (ptr_Green[j] >= gv-color_detla)) {
                    ptr_Gray[j] = 255;
                }

            }
        }
    }
    Mat mBinary = ocr_binary(mGray, DEFAULT_THRESHOLD, E_NORMAL, DEFAULT_SIZE, DEFAULT_DELTA);
    mBinary = ocr_smooth(mBinary, CV_MEDIAN);
    mBinary = ocr_blur(mBinary, 2);
    mBinary = ocr_dilate(mBinary, 2);
    ocr_write(mBinary, desImg);
    return mBinary;
}


