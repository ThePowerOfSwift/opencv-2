#include <string>
#include <opencv/cv.hpp>
#include "ocr_read.h"
#include "ocr_tesseract.h"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {
    if(argc <= 2) {
        printf("usage: ./ocr_read xx.png\n");
        return -1;
    }

    ocr_preprocess(argv[1], argv[2]);
    ocr_tesseract(argv[2], argv[3], "eng");

  /*  CvSeq* contours = NULL;
    CvMemStorage* storage = cvCreateMemStorage(0);
    IplImage* img = ocr_read(argv[2]);
    int count = cvFindContours(img, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL);
    cvReleaseMemStorage(&storage);
    printf("count : %d\n", count);
    int idx = 0;
    char szName[56] = {0};
    int tempCount = 0;
    for(CvSeq* c = contours; c != NULL; c=c->h_next) {
        CvRect rc = cvBoundingRect(c, 0);
        if((rc.height <= 5) || (rc.width <= 5)) {
            continue;
        }
        cvDrawRect(img, cvPoint(rc.x, rc.y), cvPoint(rc.x + rc.width, rc.y + rc.height), CV_RGB(255,0,0));
        IplImage* imgNo =cvCreateImage(cvSize(rc.width, rc.height), IPL_DEPTH_8U, 1);
        cvSetImageROI(img, rc);
        printf("rc.height = %d\n", rc.height);
        printf("rc.width = %d\n", rc.width);
        printf("rc.x = %d\n", rc.x);
        printf("rc.y = %d\n", rc.y);

        printf("g_binary->width = %d\n", img->width);
        printf("g_binary->height = %d\n", img->height);
        printf("imgNo->width = %d\n", imgNo->width);
        printf("imgNo->height = %d\n", imgNo->height);
        *cvCopy(g_binary, imgNo, NULL);
        cvResetImageROI(g_binary);
        sprintf(szName, "wnd_%d", idx++);
        cvReleaseImage(&imgNo);
        cvNamedWindow(szName);
        cvShowImage(szName, imgNo);*/
   /* }
   ocr_dbg(g_binary, "result");
    //g_hsv = ocr_rgb2hsv(g_img);
    //g_value = ocr_gethsv(g_hsv, E_VALUE);
*/

    return 0;
}