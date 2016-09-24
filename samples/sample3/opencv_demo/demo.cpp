#include <stdio.h>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

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
	Mat roiImg;
	Mat bImg, cImg;
	int x_s,y_s,x_e,y_e;
	int cnt=0;
	double rate;
	while(itc!= circles.end()){  
		x_s = (int)(*itc)[0] - (int)(*itc)[2];
		x_e = (int)(*itc)[0] + (int)(*itc)[2];
		y_s = (int)(*itc)[1] - (int)(*itc)[2];
		y_e = (int)(*itc)[1] + (int)(*itc)[2];
		if((x_s < 0) || (x_e > width) \
            || (y_s < height/2) || (y_e > height))
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
		rate = double(cnt)/double(roiImg.rows*roiImg.cols);
		//printf("total = %d, cnt = %d, rate= %lf\n", bImg.rows*bImg.cols, cnt, rate);
		if(rate > 0.1)
		{
			printf("%d, %d, %d, %d\n", x_s, x_e, y_s, y_e);
			printf("[Circle1] total = %d, cnt = %d, rate= %lf\n", roiImg.rows*roiImg.cols, cnt, rate);
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
			rate = double(cnt)/double(cImg.rows*cImg.cols);
			printf("[Circle2] total = %d, cnt = %d, rate= %lf\n", cImg.rows*cImg.cols, cnt, rate);
			if(rate > 0.55)
			//if(rate > 0.1)
			{  
				circle(orgImg,  
					Point((* itc)[0],(*itc)[1]),  //circle centre  
					(*itc)[2],  //circle radius  
					Scalar(0,255,0), //color  
					5); //thickness*/
			}
		}
		cnt=0;
		++itc;  
	}  
	//imshow("HoughCircleDetect",orgImg);
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "%s_detect.jpg", desimg);
    imwrite(cmd, orgImg);
}

int main(const int argc, const char* argv[])
{
    DIR *dirptr = NULL;
    struct dirent *entry;
    int count = 0;
    char desdir[128] = {0};
    char srcdir[128] = {0};

    printf("dir: %s\n", argv[1]);
    if((dirptr = opendir(argv[1])) == NULL) {
        printf("open dir error!\n");
        return -1;
    }

    char *p1, *p2;
    char index[10] = {0};
    while(entry = readdir(dirptr)) {
        if((strstr(entry->d_name, "jpg") != NULL) || (strstr(entry->d_name, "JPG") != NULL))
        {
            memset(srcdir, 0, sizeof(srcdir));
            sprintf(srcdir, "./%s/%s", argv[1], entry->d_name);
            memset(desdir, 0, sizeof(desdir));
            sprintf(desdir, "./%s/%d", argv[2], count);
            printf("src:%s\n", srcdir);
            printf("des:%s\n", desdir);
            circle_detect(srcdir, desdir);
            count++;
        }    
    }
    closedir(dirptr);
    return 0;
}
