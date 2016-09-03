#include <string>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "ocr_read.h"
#include "ocr_tesseract.h"
#include "convert.h"
#include <dirent.h>

using namespace cv;
using namespace std;

#define DEFAULT_THRESHOLD 210
#define DEFAULT_SIZE 3
#define DEFAULT_DELTA 3

#define MAX_SIZE 256
char path[6][MAX_SIZE];

#if 1
int ocrs_detect(const char* dir, const char* result, int cnt) {
    DIR *dirptr = NULL;
    struct dirent *entry;
    int count = 0;

    //printf("dir: %s\n", dir);
    if((dirptr = opendir(dir)) == NULL) {
        printf("open dir error!\n");
        return -1;
    }

    char *p1, *p2;
    char index[10] = {0};
    while(entry = readdir(dirptr)) {
        if(strncmp(entry->d_name, "temp_", 5) == 0) {
            p1 = strstr(entry->d_name, "_");
            p2 = strstr(entry->d_name, ".");
            strncpy(index, p1+1, p2-p1-1);
            count = atoi(index);
            strcpy(path[count], dir);
            strcat(path[count], "/");
            strcat(path[count], entry->d_name);
            //printf("%s\n", entry->d_name);
        }
    }
    closedir(dirptr);

    memset(index, 0, sizeof(index));
    for(int i=0; i<cnt; i++) {
        p1 = ocr_tesseract(path[i], NULL, "eng");
        strncat(index, p1, sizeof(*p1));
        //remove(path[i]);
    }
    strcat(index, "\n");
    FILE* pf;
    pf = fopen(result, "a+");
    if(pf == NULL) {
        fprintf(stderr, "open %s error\n", result);
        return -1;
    }

    fprintf(pf, "%s", index);
    printf("ocr result is %s\n", index);
    fclose(pf);
    return 0;
}
int main(int argc, const char* argv[]) {
    Mat mImg;
    int count;
/*    if(argc <= 2) {
        printf("usage: ./ocr_read xx.png\n");
    }*/

    mImg = ocr_preprocess(argv[1], argv[2]);
    //convert(argv[1], (char*)argv[3]);
    //ocr_tesseract(argv[2], argv[4], "eng");

    count = ocr_cut(mImg, argv[4], 3);
    ocrs_detect(argv[4], argv[5], count);
    //mImg =imread(argv[1]);
    //ocr_rgb_histogram(mImg);
    //ocr_hsv_histogram(mImg);
    waitKey(0);
    return 0;
}
#endif
#if 0
Mat src,dst;
 int spatialRad=10,colorRad=10,maxPryLevel=1;
 //const Scalar& colorDiff=Scalar::all(1);

 void meanshift_seg(int,void *)
 {
     //调用meanshift图像金字塔进行分割
     pyrMeanShiftFiltering(src,dst,spatialRad,colorRad,maxPryLevel);
     RNG rng=theRNG();
     Mat mask(dst.rows+2,dst.cols+2,CV_8UC1,Scalar::all(0));
     for(int i=0;i<dst.rows;i++)    //opencv图像等矩阵也是基于0索引的
         for(int j=0;j<dst.cols;j++)
             if(mask.at<uchar>(i+1,j+1)==0)
             {
                 Scalar newcolor(rng(256),rng(256),rng(256));
                 floodFill(dst,mask,Point(i,j),newcolor,0,Scalar::all(1),Scalar::all(1));
         //        floodFill(dst,mask,Point(i,j),newcolor,0,colorDiff,colorDiff);
             }
     imshow("dst",dst);
 }


 int main(int argc, const char* argv[]) {

     namedWindow("src", WINDOW_AUTOSIZE);
     namedWindow("dst", WINDOW_AUTOSIZE);

     src = imread(argv[1]);
     CV_Assert(!src.empty());

     spatialRad = 10;
     colorRad = 50;
     maxPryLevel = 3;

     //虽然createTrackbar函数的参数onChange函数要求其2个参数形式为onChange(int,void*)
     //但是这里是系统响应函数，在使用createTrackbar函数时，其调用的函数可以不用写参数，甚至
     //括号都不用写，但是其调用函数的实现过程中还是需要满足(int,void*)2个参数类型
     while(true)
     {   createTrackbar("spatialRad", "dst", &spatialRad, 80, meanshift_seg);
     createTrackbar("colorRad", "dst", &colorRad, 60, meanshift_seg);
     createTrackbar("maxPryLevel", "dst", &maxPryLevel, 5, meanshift_seg);

     //    meanshift_seg(0,0);

     imshow("src", src);
     /*char c=(char)waitKey();
     if(27==c)
         return 0;*/
     imshow("dst", src);
         char key = (char) waitKey(300);
         if(key == 27)
             break;
     }
 //    while(1);//这里不能用while(1)的原因是需要等待用户的交互，而while(1)没有该功能。虽然2者都有无限等待的作用。
     return 0;
}
#endif

