#include <stdio.h>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "circle_detect.h"

using namespace cv;
using namespace std;

#define RED_RATE 1.2
#define RED_MIN 60
#define GREEN_RATE 1.8
#define SUB_DELTA 1
Mat colorFilter(const char * img)  
{  
	int i, j;
    int x_s,y_s,x_e,y_e;

	Mat srcImg = imread(img, IMREAD_UNCHANGED);
    Mat grayImg = imread(img, IMREAD_GRAYSCALE);
    int width = srcImg.cols;  
    int height = srcImg.rows;
    int width_sub = width/SUB_DELTA;
    int height_sub = height/SUB_DELTA;
    char cmd[128];
    
    x_s = width - width/SUB_DELTA;
    y_s = height - height/SUB_DELTA;
    x_e = width;
    y_e = height;
    Mat srcImg_sub = srcImg(Range(y_s, y_e), \
							Range(x_s, x_e));
    Mat grayImg_sub = grayImg(Range(y_s, y_e), \
							Range(x_s, x_e));

    Mat mRed, mGreen, mBlue;
    vector<Mat> mChannels;
    split(srcImg_sub, mChannels);
	mRed = mChannels.at(2);
	mGreen = mChannels.at(1);
    mBlue = mChannels.at(0);

	for (int i=0;i<height_sub;i++)
	{
        uchar *pR=mRed.ptr<uchar>(i);
		uchar *pG=mGreen.ptr<uchar>(i);
  		uchar *pB=mBlue.ptr<uchar>(i);
        uchar *pGray=grayImg_sub.ptr<uchar>(i);    
	    for (int j=0;j<width_sub;j++)
		{
            //if((((double)pR[j]/(double)pG[j] >= RED_RATE) || ((double)pR[j]/(double)pB[j] >= RED_RATE)) \
                && ((double)pG[j]/(double)pB[j] < RED_RATE) && ((double)pB[j]/(double)pG[j] < RED_RATE))
	    if(((double)pR[j]/(double)pG[j] >= RED_RATE) && (pR[j] >= RED_MIN) && ((double)pG[j]/(double)pB[j] < GREEN_RATE))
            {
                pGray[j] = 255;
            }
            else
            {
                pGray[j] = 0;
            }
		}
    }
	return grayImg_sub;
}

void circle_detect(const char* srcimg, const char* desimg)
{
	int width, height;
	int circle_min, circle_max;
    char cmd[128] = {0};

	Mat circleImg = colorFilter(srcimg);
    sprintf(cmd, "%s_color.jpg", desimg);
    imwrite(cmd, circleImg);
    Mat orgImg = imread(srcimg, IMREAD_UNCHANGED);
	Mat orgImg1 = imread(srcimg, IMREAD_UNCHANGED);
    Mat element1(2, 2, CV_8U, Scalar(255));
    erode(circleImg, circleImg, element1);   
	Mat element(3, 3, CV_8U, Scalar(255));
	dilate(circleImg, circleImg, element);
    //Mat element1(2, 2, CV_8U, Scalar(255));
    //erode(circleImg, circleImg, element1);
    GaussianBlur(circleImg, circleImg, Size(3,3), 1.5);
	//medianBlur(circleImg, circleImg, 3);
    //imshow("GaussianBlur", circleImg);
    
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "%s_binary.jpg", desimg);
    imwrite(cmd, circleImg);

	width = circleImg.cols;
	height = circleImg.rows;
	if(width > height)
	{
		circle_min = width/28;
		circle_max = width/10;
	}
	else
	{
		circle_min = height/28;
		circle_max = height/10;
	}
	printf("circleImg.height = %d\n", height);
	printf("circleImg.width = %d\n", width);
	printf("circle_max = %d\n", circle_max);
	printf("circle_min = %d\n", circle_min);
	vector<Vec3f> circles;
	HoughCircles(circleImg,circles,CV_HOUGH_GRADIENT,
		2, //accumulator resolution (size of the image/2)  
		circle_max, //minimum distance between two circles  
		200, //canny high threshold  
		30, //minimum number of votes  
		circle_min,circle_max);//min and max radius
	vector<Vec3f>::
		const_iterator itc= circles.begin();

    //the pram. for findContours,
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    /// Find contours
	//Mat mCannyImg;
	//Canny(circleImg, mCannyImg, 80, 255, 3);
    findContours(circleImg, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	vector<RotatedRect> minEllipse(contours.size());
	int elipse_num = 0;
	int elipse_max_num = 0xffff;
	double elipse_area = 0;
    double elipse_area1 = 0;
    int elipse_max_num1 = 0xffff;
    for (int i = 0; i < contours.size(); i++)  
    {
		Rect aRect = boundingRect(contours[i]);
		if((aRect.y < height/2) && ((aRect.x < width/4) || (aRect.x > width/2)))
			continue;
		printf("size = %lf\n",  contourArea(contours[i]));
        if ((contours[i].size() > 5) && (contourArea(contours[i]) >= circle_min * circle_min * 2) \
			&& (contourArea(contours[i]) < circle_max * circle_max * 4))
			//&& ())  
        {  
            minEllipse[elipse_num] = fitEllipse(Mat(contours[i]));
			printf("elipse_num = %d\n", elipse_num);
			//printf("size = %lf\n",  contourArea(contours[i]));
	    	if((contourArea(contours[i]) > elipse_area) && (aRect.y > height/2))
			{
				elipse_area = contourArea(contours[i]);
				elipse_max_num = elipse_num; 
				printf("elipse_max_num = %d\n", elipse_max_num);
			}
            else if((contourArea(contours[i]) > elipse_area) && (aRect.y < height/4))
            {
				elipse_area1 = contourArea(contours[i]);
				elipse_max_num1 = elipse_num; 
				printf("elipse_max_num1 = %d\n", elipse_max_num1);
            }
			elipse_num ++;
        }  
    }

 	Mat roiImg;
	Mat bImg, cImg;
	int x_s,y_s,x_e,y_e;
	int cnt=0;
	double rate1, rate2;
	while(itc!= circles.end()){  
		x_s = (int)(*itc)[0] - (int)(*itc)[2];
		x_e = (int)(*itc)[0] + (int)(*itc)[2];
		y_s = (int)(*itc)[1] - (int)(*itc)[2];
		y_e = (int)(*itc)[1] + (int)(*itc)[2];
		//if((x_s < 0) || (x_e > width) \
            || (y_s < height/2) || (y_e > height))
        if((x_s < 0) || (x_e > width) \
            || (y_s < 0) || (y_e > height))
		{
			++itc;
			continue;
		}
		roiImg = circleImg(Range(y_s, y_e), \
							Range(x_s, x_e));
		cnt=0;
		for (int i=0;i<roiImg.rows;i++)
		{
			uchar *p=roiImg.ptr<uchar>(i);    //获取行地址
			for (int j=0;j<roiImg.cols;j++)
			{
				if(p[j] != 0)
				  cnt++;
			}
		}
		rate1 = double(cnt)/double(roiImg.rows*roiImg.cols);
		//printf("total = %d, cnt = %d, rate= %lf\n", bImg.rows*bImg.cols, cnt, rate);
		if(rate1 > 0.1)
		{
			printf("%d, %d, %d, %d\n", x_s, x_e, y_s, y_e);
			printf("[Circle1] total = %d, cnt = %d, rate1= %lf\n", roiImg.rows*roiImg.cols, cnt, rate1);
			cImg = roiImg(Range(roiImg.rows/2-roiImg.rows/12, roiImg.rows/2+roiImg.rows/12), \
					Range(roiImg.cols/2-roiImg.cols/12, roiImg.cols/2+roiImg.cols/12));
			cnt=0;
		    for (int i=0;i<cImg.rows;i++)
			{
				uchar *p=cImg.ptr<uchar>(i);    //获取行地址
				for (int j=0;j<cImg.cols;j++)
				{										
					if(p[j] != 0)
					  cnt++;
				}
			}
			rate2 = double(cnt)/double(cImg.rows*cImg.cols);
			printf("[Circle2] total = %d, cnt = %d, rate2= %lf\n", cImg.rows*cImg.cols, cnt, rate2);
			if((rate2 > 0.55) && (y_s > height/2))
			//if(rate > 0.1)
			{  
				circle(orgImg,  
					Point((* itc)[0],(*itc)[1]),  //circle centre  
					(*itc)[2],  //circle radius  
					Scalar(0,255,0), //color  
					5); //thickness*/
			
			}
            else if((y_s < height/4) && (rate2 > 0.05) && (rate1 > 0.3) &&\
				((int)((*itc)[0]) > width/2 - width/25) && ((int)((*itc)[0]) < width/2 + width/25))
            {
				circle(orgImg,  
					Point((* itc)[0],(*itc)[1]),  //circle centre  
					(*itc)[2],  //circle radius  
					Scalar(0,255,0), //color  
					5); //thickness*/
            }
			if(elipse_max_num != 0xffff)
			{
				ellipse(orgImg1,minEllipse[elipse_max_num],Scalar(255, 255, 0), 5);
			}
			if(elipse_max_num1 != 0xffff)
			{
            	ellipse(orgImg1,minEllipse[elipse_max_num1],Scalar(255, 255, 0), 5);
			}
		}
		cnt=0;
		++itc;  
	}  
	//imshow("HoughCircleDetect",orgImg);
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "%s_circle_detect.jpg", desimg);
    imwrite(cmd, orgImg);

    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "%s_elipse_detect.jpg", desimg);
    imwrite(cmd, orgImg1);
}
