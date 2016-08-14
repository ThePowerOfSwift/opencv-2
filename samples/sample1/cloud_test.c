#include "cv.h"
#include "highgui.h"

int main(void)
{
	    IplImage* img;
		img = cvLoadImage("cloud.jpg",1);
		cvNamedWindow("ShowImage",1);
		cvShowImage("ShowImage",img);
		cvWaitKey(0);
		return 0;
}
