//
// Created by root on 16-12-11.
//
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include<iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string.h>
#include <iterator>
#include "kmeans.h"
#include "ocr_kmeans.h"

using namespace cv;
using namespace std;

void* ocr_kmeans_init(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT> &cluster, int knum) {
    int cnt = 0;
    std::vector<KMEAN_STRUCT> xcluster;
    cluster[0].rb = (double)cluster[0].rv/(double)cluster[0].bv;
    cluster[0].rg = (double)cluster[0].rv/(double)cluster[0].gv;
    cluster[0].gb = (double)cluster[0].gv/(double)cluster[0].bv;
    xcluster.push_back(cluster[0]);

    //按x值 冒泡排序
    KMEAN_STRUCT temp;
    for(int i=0; i<cluster.size(); i++) {
        for(int j=0; j<cluster.size()-i-1; j++) {
            if(cluster[j].x > cluster[j+1].x) {
                temp = cluster[j];
                cluster[j] = cluster[j+1];
                cluster[j+1] = temp;
            }
        }
    }

    for(int i=0; i<cluster.size();i++) {
        cluster[i].rb = (double)cluster[i].rv/(double)cluster[i].bv;
        cluster[i].rg = (double)cluster[i].rv/(double)cluster[i].gv;
        cluster[i].gb = (double)cluster[i].gv/(double)cluster[i].bv;
        if(xcluster[cnt].x != cluster[i].x) {
            xcluster.push_back(cluster[i]);
            cnt ++;
        } else {
            continue;
        }
    }

    for(int i=0; i<knum; i++) {
        means.push_back(xcluster[i*((cnt-1)/(knum-1))]);
        printf("means[%d].x = %d, .y = %d, .rb = %f, .rg = %f, .gb = %f\n", i, means[i].x, means[i].y, means[i].rb, means[i].rg, means[i].gb);
    }
}

void* ocr_kmeans_update(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT>& cluster, std::vector<int> &label, std::vector<int> &cnt) {
    KMEAN_STRUCT point;
    point.x = 0;
    point.y = 0;
    point.gv = 0;
    point.bv = 0;
    point.rv = 0;
    point.rb = 0;
    point.rg = 0;
    point.gb = 0;

    means.assign(means.size(), point);
    for(int i=0; i<cluster.size(); i++) {
        means[label[i]].x += cluster[i].x;
        means[label[i]].rb += cluster[i].rb;
        means[label[i]].rg += cluster[i].rg;
        means[label[i]].gb += cluster[i].gb;
    }

    for(int i=0; i<means.size(); i++) {
        means[i].x /= cnt[i];
        means[i].rg /= cnt[i];
        means[i].rb /= cnt[i];
        means[i].gb /= cnt[i];
    }
}

void* ocr_kmeans_load(KMEAN_STRUCT& point1, KMEAN_STRUCT& point2, std::vector<double> &v1, std::vector<double> &v2) {

    v1.push_back(point1.rb);
    v1.push_back(point1.rg);
    v1.push_back(point1.gb);
    v2.push_back(point2.rb);
    v2.push_back(point2.rg);
    v2.push_back(point2.gb);
}









