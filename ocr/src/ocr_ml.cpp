//
// Created by root on 16-10-3.
//

#include "ocr_ml.h"
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include <dirent.h>
#include <string>
#include <opencv2/ml.hpp>

using namespace cv;
using namespace std;
using namespace ml;

static const int sample_num =20;
static const int sample_type_num = 54; //8
static const int sample_width = 20;
static const int sample_height = 20;
static const char sample_name[54] = {
        '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd',
        'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'm', 'n', 'p', 'q', 'r',
        's', 't', 'u', 'v', 'w', 'x',
        'y', 'z', 'A', 'B', 'C', 'D',
        'E', 'F', 'G', 'H', 'K', 'M',
        'N', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z'
};

/*static const char sample_name[8] = {
        '2', '3', '4', '5',
        '6', '7', '8', '9'
};*/

#define BUF_LEN 128

static Ptr<TrainData> ocr_training_convert(Mat &training_data_mat, Mat &lable_mat)
{
    Ptr<TrainData> train_data = TrainData::create(training_data_mat, ROW_SAMPLE, lable_mat);
    return train_data;
}

int ocr_traning(const char* samplePath, const char* bpPath) {
    float trainingData[sample_num*sample_type_num][sample_width*sample_height] = {{0}};
    float labels[sample_num*sample_type_num][sample_type_num] = {{0}};
    Mat mImg, mResizeImg;
    char sample_dir[BUF_LEN]={0};
    char sample_img[BUF_LEN]={0};
    int count = 0;
    DIR *dirptr = NULL;
    struct dirent *entry;

    //init traning data
    for(int i=0; i<sample_type_num; ++i) {
        memset(sample_dir, 0, BUF_LEN);
        sprintf(sample_dir, "%s/%c", samplePath, sample_name[i]);
        count = 0;
        //printf("dir: %s\n", dir);
        if((dirptr = opendir(sample_dir)) == NULL) {
            printf("open dir error!\n");
            return -1;
        }
        while(entry = readdir(dirptr)) {
            if((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
                memset(sample_img, 0, BUF_LEN);
                sprintf(sample_img, "%s/%s", sample_dir, entry->d_name);
                printf("sample img: %s\n", sample_img);
                mImg = imread(sample_img, IMREAD_UNCHANGED);
                resize(mImg, mResizeImg, Size(20, 20), 0, 0, CV_INTER_LINEAR);
                for(int k=0; k<400; ++k) {
                    trainingData[i*sample_num + count][k] = (float)mResizeImg.data[k];
                }
                ++count;
                if(count == 20)
                    break;
            }
        }

    }

    // Set up training data Mat
    Mat trainingDataMat(sample_num*sample_type_num, sample_height*sample_width, CV_32FC1, trainingData);
    cout<<"trainingDataMat——OK！"<<endl;

    //init label
    for(int i=0; i<sample_type_num; ++i) {
        for(int j=0; j<sample_num; ++j) {
            for(int k = 0; k<sample_type_num; ++k) {
                if(k==i)
                    labels[i*sample_num + j][k] = 1;
                else
                    labels[i*sample_num + j][k] = 0;
            }
        }
    }
    Mat labelsMat(sample_num*sample_type_num, sample_type_num, CV_32FC1,labels);
    cout<<"labelsMat:"<<endl;
    cout<<labelsMat<<endl;
    cout<<"labelsMat——OK！"<<endl;

    //Setup the BPNetwork
    Mat layerSizes=(Mat_<int>(1,5) << sample_width*sample_height,128,128,128, sample_type_num);
    //Mat layerSizes = (Mat_<int>(1,3) << sample_width*sample_height, 128, sample_type_num);
    //start training...
    cout<<"training start...."<<endl;
    Ptr<ANN_MLP> ann = ANN_MLP::create();
    ann->setLayerSizes(layerSizes);
    ann->setActivationFunction(ANN_MLP::SIGMOID_SYM, 1, 1);
    ann->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS, 5000, FLT_EPSILON));
    ann->setTrainMethod(ANN_MLP::BACKPROP, 0.001);

    //Ptr<TrainData> tData = TrainData::create(&trainingDataMat, ROW_SAMPLE, &labels);
    //tData->create(trainingData, ROW_SAMPLE, labels);
    Ptr<TrainData> tData = ocr_training_convert(trainingDataMat, labelsMat);
    ann->train(tData);
    ann->save(bpPath);

    cout<<"training finish...bpModel1.xml saved "<<endl;
    return 0;
}

int ocr_predict(const char * testPath, const char * bpPath) {
    Mat mImg = imread(testPath, IMREAD_UNCHANGED);
    Mat mResizeImg;
    float testData[1][sample_width*sample_height] = {{0}};

    resize(mImg, mResizeImg, Size(20, 20), 0, 0, CV_INTER_LINEAR);
    for(int k=0; k<400; ++k) {
        testData[0][k] = (float)mResizeImg.data[k];
    }
    Mat testDataMat(1, sample_height*sample_width, CV_32FC1, testData);
    Ptr<ANN_MLP> ann = ANN_MLP::load(bpPath);
    float response = ann->predict(testDataMat);
    printf("result is = %c\n", sample_name[(int)response]);
    return sample_name[(int)response];
}
