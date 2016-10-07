#include <stdio.h>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "circle_detect.h"
#include "text_detect.h"

using namespace cv;
using namespace std;



int main(const int argc, const char* argv[])
{
    DIR *dirptr = NULL;
    struct dirent *entry;
    int count = 0;
    char desdir[128] = {0};
    char srcdir[128] = {0};

	if(argc < 4) {
		printf("usage: ./demo_opencv srcimg desimg type\n");
		return -1;
	}

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
			if(strcmp(argv[3], "circle") == 0)
            	circle_detect(srcdir, desdir);
			else if(strcmp(argv[3], "text") == 0)
				text_detect(srcdir, desdir);
			else
			{
				printf("unknown type, error return!\n");
				break;
			}
            count++;
        }    
    }
    closedir(dirptr);
    return 0;
}
