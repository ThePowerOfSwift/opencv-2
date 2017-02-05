#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "ocr_read.h"
#include "kmeans.h"
#include<iostream>

using namespace cv;
using namespace std;

//threshold define
#define OCR_DBG 0
#define DEFAULT_THRESHOLD 200
#define DEFAULT_SIZE 3
#define DEFAULT_DELTA 3
#define KMEAN_DELTA 0.00001

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

static int _search_label(vector<KMEAN_STRUCT> means, int value, int num) {
    int i = 0;
    for(;i<num; i++) {
        if(means[i].x == value)
            break;
    }
    return i;
}

static int ocr_kmeans_cut(Mat mSrc, const char* srcImgPath, int offset, int width, const char* cutImgPath) {
    int kmeans_cnt = width/40 + 1;
    if((kmeans_cnt < 2) || (kmeans_cnt > 6)) {
        printf("kmeans cut count error : %d\n", kmeans_cnt);
        return -1;
    }
    char cmd[128];
    std::vector<int> knum(kmeans_cnt, 0);
    std::vector<Mat> mChannels;
    std::vector<KMEAN_STRUCT> cluster;
    std::vector<KMEAN_STRUCT> means;
    std::vector<int> label;
    KMEAN_STRUCT point;
    Mat mImg = imread(srcImgPath, IMREAD_UNCHANGED);
    Mat mSubImg = mImg(Range(0, mImg.rows), Range(offset, offset+width));
    split(mSubImg, mChannels);
    Mat mR = mChannels.at(2);
    Mat mG = mChannels.at(1);
    Mat mB = mChannels.at(0);

    //init cluster
    for(int i=0; i<mSrc.rows; i++) {
        uchar* ptr_m = mSrc.ptr<uchar>(i);
        uchar* ptr_mR = mR.ptr<uchar>(i);
        uchar* ptr_mB = mB.ptr<uchar>(i);
        uchar* ptr_mG = mG.ptr<uchar>(i);
        for(int j=0; j<mSrc.cols; j++) {
            if(ptr_m[j] != 0) {
                point.x = j;
                point.y = i;
                point.gv = ptr_mG[j];
                point.bv = ptr_mB[j];
                point.rv = ptr_mR[j];
                cluster.push_back(point);
                label.push_back(0);
            }
        }
    }

    //init means
    kmeans_init(means, cluster, kmeans_cnt, ocr_kmeans_init);
    //sort cluster & init label
    kmeans_sort_cluster(means, cluster, label, knum, ocr_kmeans_load);
    //cal sum
    double kmeans_sum = 0;
    double kmeans_sum_old = 0;
    kmeans_sum = kmeans_cal_deltaSum(means, cluster, label, ocr_kmeans_load);
    //printf("src_sum is %f, src_sum_old is %f\n", kmeans_sum, kmeans_sum_old);
    while(abs(kmeans_sum - kmeans_sum_old) > KMEAN_DELTA) {
        kmeans_sum_old = kmeans_sum;
        kmeans_update(means, cluster, label, knum, ocr_kmeans_update);
        kmeans_sort_cluster(means, cluster, label, knum, ocr_kmeans_load);
        kmeans_sum = kmeans_cal_deltaSum(means, cluster, label, ocr_kmeans_load);
        //printf("src_sum is %f, src_sum_old is %f\n", kmeans_sum, kmeans_sum_old);
    }

    kmeans_update(means, cluster, label, knum, ocr_kmeans_update);
    vector<int> xpos;
    for(int k = 0; k<means.size(); k++) {
        xpos.push_back(means[k].x);
    }

    int tmp = 0;
    for(int i=0; i<means.size(); i++) {
        for(int j=0; j<means.size()-i-1; j++) {
            if(xpos[j] > xpos[j+1]) {
                tmp = xpos[j];
                xpos[j] = xpos[j+1];
                xpos[j+1] = tmp;
            }
        }
    }

    Mat roiImg;
    int idx = 0;
    for(int k = 0; k<means.size(); k++) {
        mSrc.copyTo(roiImg);
        idx = _search_label(means, xpos[k], means.size());
        for(int i=0; i<cluster.size(); i++) {
            uchar* ptr_m = roiImg.ptr<uchar>(cluster[i].y);
            if(idx != label[i])
                ptr_m[cluster[i].x] = 0;
        }
        //printf("cluster[%d] center: %d\n", k, xpos[k]);
        int x_start, x_end;
        if(xpos[k] >= 20) {
            x_start = xpos[k] - 20;
        } else {
            x_start = 0;
        }
        if(width >= (xpos[k] + 20)) {
            x_end = xpos[k] + 20;
        } else {
            x_end = width;
        }
        // kmeans debug start
        // To save kmeans cut png to debug
#if 0
        memset(cmd, 0, sizeof(cmd));
        strcpy(cmd, cutImgPath);
        char *q = strstr(cmd, ".png");
        q--;
        *q -= (means.size()-k-1+10);
        ocr_write(roiImg, cmd);
#endif
        // kmeans debug end
        Mat mCutImg = roiImg(Range(0, roiImg.rows), Range(x_start, x_end));
        mCutImg = ocr_dilate(mCutImg, 3);
        mCutImg = ocr_erode(mCutImg, 3);
        mCutImg = ocr_smooth(mCutImg, CV_MEDIAN);
        memset(cmd, 0, sizeof(cmd));
        strcpy(cmd, cutImgPath);
        char *p = strstr(cmd, ".png");
        p--;
        *p -= (means.size()-k-1);
        ocr_write(mCutImg, cmd);
        ocr_cut(mCutImg, srcImgPath, NULL, 3, 0, cmd);
    }
}

/* div value is between 1~100 , must not be set to zero */
int ocr_cut(Mat mSrcImg, const char* srcImgPath, const char* desImgDir, int div, int count, const char* singleImgDir) {

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
                    if(pRect[idx0].width > 80) {
                        idx1++;
                    }
                    idx1++;
                }
                // y direction not cut , just use origin img height
                roiImg = mSrcImg(Range(0, mSrcImg.rows), \
                    Range(pRect[idx0].x, pRect[idx0].x + pRect[idx0].width));
                if(m_width == 0)
                    roiImg = ocr_xfill(roiImg, 5);
                memset(cutPath, 0, sizeof(cutPath));
                if(desImgDir == NULL) {
                    snprintf(cutPath, sizeof(cutPath), "%s", singleImgDir);
                } else {
                    snprintf(cutPath, sizeof(cutPath), "./%s/%d_%d.png", desImgDir, count, idx1);
                }
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
            if(desImgDir == NULL) {
                snprintf(cutPath, sizeof(cutPath), "%s", singleImgDir);
            } else {
                snprintf(cutPath, sizeof(cutPath), "./%s/%d_%d.png", desImgDir, count, idx1);
            }
            ocr_write(roiImg, cutPath);
            idx1 ++;
        }

        if(m_width != 0){
            // kmeans debug start
#if 0
            memset(cutPath, 0, sizeof(cutPath));
            snprintf(cutPath, sizeof(cutPath), "./%s/kmeans_merge.png", desImgDir);
            ocr_write(roiImg, cutPath);
#endif
            printf("m_width = %d, m_offset = %d\n", m_width, m_offset);
            // kmeans debug end
            ocr_kmeans_cut(roiImg, srcImgPath, m_offset, m_width, cutPath);
            m_width = 0;
        }
    }
    //imshow("draw_contours", mDrawImg);

    //printf("single ocr num: %d\n", idx1);
    return idx1;
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
