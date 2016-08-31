#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "ocr_read.h"
#include<iostream>

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

/* div value is between 1~100 , must not be set to zero */
int ocr_cut(Mat mSrcImg, const char* desImg, int div) {

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
        printf("x: %d, y: %d, width: %d, height: %d\n", aRect.x, aRect.y, \
          aRect.width, aRect.height);
    }

    Rect* pRect = new Rect[num];
    int valid_num = num;
    //check reuse area
    for(count = 0;  count < num; count++) {
        Rect aRect = boundingRect(contours[count]);
        pRect[count] = aRect;
        for (int tmp = count + 1; tmp < contours.size(); tmp++) {

            Rect bRect = boundingRect(contours[tmp]);

            if ((aRect.x + aRect.width < bRect.x + aRect.width/div) \
              || (aRect.x > bRect.x + bRect.width - aRect.width/div)) {
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
    Mat roiImg;
    char cutPath[128] = {0};
    /// Draw contours,彩色轮廓
    Mat mDrawImg = Mat::zeros(mCannyImg.size(), CV_8UC3);
    for(idx0 = 0; idx0 < num; idx0++) {

        if(idx0 != 0) {
            if ((pRect[idx0 -1].x + pRect[idx0 -1].width < pRect[idx0].x + pRect[idx0].width/div) \
               || (pRect[idx0 -1].x > pRect[idx0 -1].x + pRect[idx0].width - pRect[idx0 - 1].width/div)) {
                rectangle(mDrawImg, pRect[idx0], Scalar(0, 0, 255), 3, 8, 0);//用矩形画矩形窗
                printf("x: %d, y: %d, width: %d, height: %d\n", pRect[idx0].x, pRect[idx0].y, \
                  pRect[idx0].width, pRect[idx0].height);
                // y direction not cut , just use origin img height
                roiImg = mSrcImg(Range(0, mSrcImg.rows), \
                    Range(pRect[idx0].x, pRect[idx0].x + pRect[idx0].width));
                memset(cutPath, 0, sizeof(cutPath));
                snprintf(cutPath, sizeof(cutPath), "%s_%d", desImg, idx0);
                ocr_write(roiImg, cutPath);
                //imshow("roi", roiImg);

            } else {
                continue;
            }
        } else {
            rectangle(mDrawImg, pRect[idx0], Scalar(0, 0, 255), 3, 8, 0);//用矩形画矩形窗
            printf("x: %d, y: %d, width: %d, height: %d\n", pRect[idx0].x, pRect[idx0].y, \
               pRect[idx0].width, pRect[idx0].height);
            // y direction not cut , just use origin img height
            roiImg = mSrcImg(Range(0, mSrcImg.rows), \
                    Range(pRect[idx0].x, pRect[idx0].x + pRect[idx0].width));
            memset(cutPath, 0, sizeof(cutPath));
            snprintf(cutPath, sizeof(cutPath), "%s_%d", desImg, idx0);
            ocr_write(roiImg, cutPath);
        }
    }
    //imshow("draw_contours", mDrawImg);

    return valid_num;
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
    mBinary = ocr_binary(mGrey, DEFAULT_THRESHOLD, E_OTSU, DEFAULT_SIZE, DEFAULT_DELTA);
    ocr_dbg(mBinary, "Bianry");
    mSmooth = ocr_smooth(mBinary, CV_MEDIAN);
    ocr_dbg(mSmooth, "Smooth");
    ocr_write(mSmooth, desImg);
    return mSmooth;
}


