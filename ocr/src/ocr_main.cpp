#include <string>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "ocr_read.h"
//#include "ocr_tesseract.h"
#include "convert.h"
#include <dirent.h>
#include "ocr_ml.h"

using namespace cv;
using namespace std;

#define DEFAULT_THRESHOLD 210
#define DEFAULT_SIZE 3
#define DEFAULT_DELTA 3

#define MAX_SIZE 256
char path[6][MAX_SIZE];
char cutResult[6];

int ocrs_detect(const char* dir, const char* result, int cnt, const char* bpPath) {
    DIR *dirptr = NULL;
    struct dirent *entry;
    int count = 0;

    //printf("dir: %s\n", dir);
    if((dirptr = opendir(dir)) == NULL) {
        printf("open dir error!\n");
        return -1;
    }

    for(count = 0; count <6; count ++) {
        sprintf(path[count], "%s/%d_%d.png", dir, cnt, count);
        cutResult[count] = (char)ocr_predict(path[count], bpPath);
        //printf("Cut path: %s\n", path[count]);
    }

    printf("%d:result: %s\n", cnt, cutResult);
    FILE* pf;
    pf = fopen(result, "a+");
    if(pf == NULL) {
        fprintf(stderr, "open %s error\n", result);
        return -1;
    }
    fprintf(pf, "%d: ", cnt);
    fprintf(pf, "%s", cutResult);
    fprintf(pf, "\n");
    fclose(pf);
    return 0;
}

int main(int argc, const char* argv[]) {

    if(strcmp(argv[1], "train") == 0) {
        //train cmd: ./ocr_detect train sampledir bp.xml
        ocr_traning(argv[2], argv[3]);
    } else {
        //detect cmd: ./ocr_detect srcImg tempImg cutImgDir result detect_count bp.xml
        //ocr_predict(argv[3], argv[2]);
        Mat mImg;
        int cut_count;
        int cnt = atoi(argv[5]);
        mImg = ocr_preprocess(argv[1], argv[2]);
        cut_count = ocr_cut(mImg, argv[1], argv[3], 3, cnt, NULL);
        if(cut_count == 6)
            ocrs_detect(argv[3], argv[4], cnt, argv[6]);
    }
    waitKey(0);
    return 0;
}


