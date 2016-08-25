#include <string>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "ocr_read.h"
#include "ocr_tesseract.h"
#include "convert.h"

using namespace cv;
using namespace std;

#define DEFAULT_THRESHOLD 210
#define DEFAULT_SIZE 3
#define DEFAULT_DELTA 3

int main(int argc, const char* argv[]) {
    Mat mImg;

/*    if(argc <= 2) {
        printf("usage: ./ocr_read xx.png\n");
    }*/

    mImg = ocr_preprocess(argv[1], argv[2]);
    //convert(argv[1], (char*)argv[3]);
    //ocr_tesseract(argv[2], argv[4], "eng");

    ocr_cut(mImg, argv[3], 20);
    waitKey(0);
    return 0;
}