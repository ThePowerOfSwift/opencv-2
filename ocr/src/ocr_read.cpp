#include <string>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

#define OCR_DBG 1
#define DEFAULT_THRESHOLD 210
IplImage *g_img = NULL;
IplImage *g_grey = NULL;
IplImage *g_binary = NULL;

int ocr_dbg(IplImage* img) {
    //display ocr png
    cvNamedWindow("ocr_dbg", CV_WINDOW_AUTOSIZE);
    cvShowImage("ocr_dbg", img);
    waitKey(0);
}

int ocr_read(char* ocr_name) {
    g_img = cvLoadImage(ocr_name, CV_LOAD_IMAGE_UNCHANGED);
    return 0;
}

int ocr_grey(void) {
    //convert to grey
    g_grey = cvCreateImage(cvGetSize(g_img), IPL_DEPTH_8U, 1);
    cvCvtColor(g_img, g_grey, CV_BGR2GRAY);
    return 0;
}

int ocr_binary(int threshold, int method) {
    //convert to binary
    g_binary = cvCreateImage(cvGetSize(g_grey), IPL_DEPTH_8U, 1);
    switch(method) {
        case 0: //normal threshold
        default:
            cvThreshold(g_grey, g_binary, threshold, 255, CV_THRESH_BINARY);
            break;
        case 1: //adaptive gaussian threshold
            cvAdaptiveThreshold(g_grey, g_binary, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 37, 25);
            break;
        case 2: //adaptive mean threshold
            cvAdaptiveThreshold(g_grey, g_binary, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 5, 5);
            break;
        case 3: //adaptive otsu threshold
            cvThreshold(g_grey, g_binary, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    }
    return 0;
}

int main(int argc, const char* argv[]) {
	if(argc <= 1) {
		printf("usage: ./ocr_read xx.png\n");
		return -1;
	}

	ocr_read((char*)argv[1]);
    //ocr_dbg(g_img);
    ocr_grey();
    //ocr_dbg(g_grey);
    ocr_binary(DEFAULT_THRESHOLD, 3);
   // cvAdaptiveThreshold(g_binary, g_binary, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 3, 5);
    ocr_dbg(g_binary);
	return 0;
}
