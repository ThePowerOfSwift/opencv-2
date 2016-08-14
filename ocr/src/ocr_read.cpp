#include <string>
#include <iostream>
#include <opencv/cv.hpp>

using namespace cv;
using namespace std;

#define OCR_DBG 1

int ocr_dbg(Mat img) {
    //display ocr png
    namedWindow("ocr_dbg", WINDOW_AUTOSIZE);
    imshow("ocr_dbg", img);
    waitKey(0);
}

Mat ocr_read(char* ocr_name, Mat img) {
	img = imread(ocr_name, 1);
	if (img.empty()) {
		cout<<"Could not open or find the image!"<<endl;
		exit(0);
    }

    return img;
}

Mat ocr_grey(Mat img, Mat grey) {
    //covert to grey
    cvtColor(img, grey, COLOR_BGR2GRAY);
    return grey;
}

int main(int argc, const char* argv[]) {
	if(argc <= 1) {
		printf("usage: ./ocr_read xx.png\n");
		return -1;
	}

    Mat img, grey;
	img = ocr_read((char*)argv[1], img);
    ocr_dbg(img);
    grey = ocr_grey(img, grey);
    ocr_dbg(grey);

	return 0;
}
