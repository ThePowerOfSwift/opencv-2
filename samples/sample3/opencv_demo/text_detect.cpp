#include <stdio.h>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "text_detect.h"

using namespace cv;
using namespace std;

void text_detect(const char* srcimg, const char* desimg)
{
	char cmd[128] = {0};

	Mat colorImg = imread(srcimg, IMREAD_UNCHANGED);
	Mat grayImg = imread(srcimg, IMREAD_GRAYSCALE);
	Mat mCannyImg;
	//text_getbackgroud(srcimg, desimg);
    int width = grayImg.cols;  
    int height = grayImg.rows;

   	sprintf(cmd, "%s_gray.jpg", desimg);
    imwrite(cmd, grayImg);
	Canny(grayImg, mCannyImg, 80, 255, 3);
   	sprintf(cmd, "%s_canny.jpg", desimg);
    imwrite(cmd, mCannyImg);

	for (int i=0;i<height;i++)
	{
        uchar *pmid=mCannyImg.ptr<uchar>(i);
	    for (int j=0;j<width;j++)
		{
			int delta = width/60;
			if(pmid[j] == 255)
			{
				int count = 0;
				for(int k=0; k<5; k++) {
					if((j>5) && (j < width-5))
					{
						if((pmid[j-k] == 255) || (pmid[j+k] == 255))
							count++;
					}
				}
				if(count >= 2) {
					for(int k=0; k<delta; k++) {
							pmid[j-k] = 255;
					}
				}

			}
		}
	}
   	sprintf(cmd, "%s_canny1.jpg", desimg);
    imwrite(cmd, mCannyImg);

    //the pram. for findContours,
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
	int valid_num;

   /// Find contours
    findContours(mCannyImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    valid_num = contours.size();
    printf("valid_num = %d\n", valid_num);

    //remove small area ,  width*height < 50
    for (int idx0 = 0; idx0 < contours.size(); idx0++) {
        Rect aRect =boundingRect(contours[idx0]);
		if ((aRect.height < height/100) \
			|| (aRect.width < height/100)
			|| (aRect.height > height*4/5) \
			|| (aRect.width > width*4/5))
		{
            //printf("remove small size = %d\n", idx0);
            valid_num--;
            //删除面积小于设定值的轮廓
            //contours.erase(contours.begin() + idx0);
            continue;
        }
		else rectangle(colorImg, aRect, Scalar(0, 0, 255), 3, 8, 0);//用矩形画矩形窗
    }
   	sprintf(cmd, "%s_color.jpg", desimg);
    imwrite(cmd, colorImg);
}

#if 0
#define TEXT_FILTER_RATE 2
#define TEXT_GRAY_DELTA 200
#define TEXT_GRAY_DELTA1 50

Mat textColorFilter(const char * img)  
{  
	int i, j;
    int x_s,y_s,x_e,y_e;

	Mat srcImg = imread(img, IMREAD_UNCHANGED);
    Mat grayImg = imread(img, IMREAD_GRAYSCALE);
    int width = srcImg.cols;  
    int height = srcImg.rows;
    char cmd[128];
    
    Mat mRed, mGreen, mBlue;
    vector<Mat> mChannels;
    split(srcImg, mChannels);
	mRed = mChannels.at(2);
	mGreen = mChannels.at(1);
    mBlue = mChannels.at(0);
	float rate1,rate2,rate3;
	for (int i=0;i<height;i++)
	{
        uchar *pR=mRed.ptr<uchar>(i);
		uchar *pG=mGreen.ptr<uchar>(i);
  		uchar *pB=mBlue.ptr<uchar>(i);
        uchar *pGray=grayImg.ptr<uchar>(i);    
	    for (int j=0;j<width;j++)
		{
			rate1 = (float)pR[j]/(float)pB[j];
			rate2 = (float)pR[j]/(float)pG[j];
			rate3 = (float)pB[j]/(float)pG[j];
			if((rate1 > TEXT_FILTER_RATE) || (rate1 < 1/TEXT_FILTER_RATE) \
				|| (rate2 > TEXT_FILTER_RATE) || (rate2 < 1/TEXT_FILTER_RATE) \
				|| (rate3 > TEXT_FILTER_RATE) || (rate3 < 1/TEXT_FILTER_RATE) \
				|| (pR[j] > 200) || (pG[j] > 200) || (pB[j] > 200))
            {
                pGray[j] = 255;
            }
		}
    }
	return grayImg;
}

#define PI 3.14159265358979

void text_getbackgroud(const char* srcimg, const char* desimg)
{
	char cmd[128] = {0};

	Mat colorImg = imread(srcimg, IMREAD_UNCHANGED);
	Mat grayImg = imread(srcimg, IMREAD_GRAYSCALE);
	int width = grayImg.cols;  
    int height = grayImg.rows;
	
	uchar value[25]={0};

	for (int i=2;i<height-2;i++)
	{
        uchar *pmid=grayImg.ptr<uchar>(i);
		uchar *ptop=grayImg.ptr<uchar>(i-1);
		uchar *pbottom=grayImg.ptr<uchar>(i+1);
		uchar *ptop1=grayImg.ptr<uchar>(i-2);
		uchar *pbottom1=grayImg.ptr<uchar>(i+2);
	    for (int j=2;j<width-2;j++)
		{
			for(int k=0; k<5; k++)
			{
				value[k] = ptop1[k+j-2];
				value[k+5] = ptop[k+j-2];
				value[k+10] = pmid[k+j-2];
				value[k+15] = pbottom[k+j-2];
				value[k+20] = pbottom1[k+j-2];
			}

			for(int l=0; l<25; l++)
				for(int m=0; m<24-l; m++)
				{
					if(value[m] < value[m+1])
					{
						int temp;
						temp = value[m];
						value[m] = value[m+1];
						value[m+1] = temp;
					}
				}
			int means;
			for(int l=0; l<5; l++)
				means += value[l+1];
			means /= 5;
			pmid[j] = means;
		}
	}

   	sprintf(cmd, "%s_back.jpg", desimg);
    imwrite(cmd, grayImg);
}
#endif
