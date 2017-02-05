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

//Idea : auto adjust every para weight, use neron network or other supervised algorithms to traning weights for kmeans
//To adjust weight for x para (X_WEIGHT is theta1 in under line,  no use for others)
//eg.  sum = theta1 * dx^2 + theta2 * rb^2 + theta3 * rg^2 + theta4 * gb^2
#define X_WEIGHT 1.2

using namespace cv;
using namespace std;

void* ocr_kmeans_regularziation(std::vector<KMEAN_STRUCT> &cluster) {

    cluster[0].dx = cluster[0].x;
    cluster[0].rb = (double)cluster[0].rv/(double)cluster[0].bv;
    cluster[0].rg = (double)cluster[0].rv/(double)cluster[0].gv;
    cluster[0].gb = (double)cluster[0].gv/(double)cluster[0].bv;

    KMEAN_STRUCT kmax = cluster[0], kmean = cluster[0], kmin = cluster[0];
    kmean.dx = kmax.dx = kmin.dx = kmean.x;

    for(int i=1; i<cluster.size(); i++) {
        cluster[i].rb = (double) cluster[i].rv / (double) cluster[i].bv;
        cluster[i].rg = (double) cluster[i].rv / (double) cluster[i].gv;
        cluster[i].gb = (double) cluster[i].gv / (double) cluster[i].bv;

        kmean.dx += cluster[i].x;
        kmean.rb += cluster[i].rb;
        kmean.gb += cluster[i].gb;
        kmean.rg += cluster[i].rg;

        if (kmax.rb < cluster[i].rb)
            kmax.rb = cluster[i].rb;
        if (kmax.gb < cluster[i].gb)
            kmax.gb = cluster[i].gb;
        if (kmax.rg < cluster[i].rg)
            kmax.rg = cluster[i].rg;
        if (kmax.dx < cluster[i].x)
            kmax.dx = cluster[i].x;

        if (kmin.rb > cluster[i].rb)
            kmin.rb = cluster[i].rb;
        if (kmin.gb > cluster[i].gb)
            kmin.gb = cluster[i].gb;
        if (kmin.rg > cluster[i].rg)
            kmin.rg = cluster[i].rg;
        if (kmin.dx > cluster[i].x)
            kmin.dx = cluster[i].x;
    }

    kmean.dx /= cluster.size();
    kmean.rb /= cluster.size();
    kmean.gb /= cluster.size();
    kmean.rg /= cluster.size();
    //printf("kmean.dx = %f, kmean.rb = %f, kmean.gb = %f, kmean.rg = %f\n", kmean.dx, kmean.rb, kmean.gb, kmean.rg);
    //printf("kmax.dx = %f, kmax.rb = %f, kmax.gb = %f, kmax.rg = %f\n", kmax.dx, kmax.rb, kmax.gb, kmax.rg);
    //printf("kmin.dx = %f, kmin.rb = %f, kmin.gb = %f, kmin.rg = %f\n", kmin.dx, kmin.rb, kmin.gb, kmin.rg);
    KMEAN_STRUCT kscope;
    kscope.dx = kmax.dx -kmin.dx;
    kscope.rb = kmax.rb - kmin.rb;
    kscope.gb = kmax.gb - kmin.gb;
    kscope.rg = kmax.rg - kmin.rg;
    //printf("kscope.dx = %f, kscope.rb = %f, kscope.gb = %f, kscope.rg = %f\n", kscope.dx, kscope.rb, kscope.gb, kscope.rg);
    for(int i=0; i<cluster.size(); i++) {
        cluster[i].dx = ((double)(cluster[i].x - kmean.dx))/kscope.dx * X_WEIGHT;
        cluster[i].rb = (cluster[i].rb - kmean.rb)/(kmax.rb - kmin.rb);
        cluster[i].gb = (cluster[i].gb - kmean.gb)/(kmax.gb - kmin.gb);
        cluster[i].rg = (cluster[i].rg - kmean.rg)/(kmax.rg - kmin.rg);
    }
}

void* ocr_kmeans_init(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT> &cluster, int knum) {
    int cnt = 0;
    std::vector<KMEAN_STRUCT> xcluster;

    kmeans_regularization(cluster, ocr_kmeans_regularziation);
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
        if(xcluster[cnt].x != cluster[i].x) {
            xcluster.push_back(cluster[i]);
            cnt ++;
        } else {
            continue;
        }
    }

    for(int i=0; i<knum; i++) {
        means.push_back(xcluster[i*((cnt-1)/(knum-1))]);
        //printf("means[%d].x = %d, .y = %d, .dx = %f, .rb = %f, .rg = %f, .gb = %f\n", i, means[i].x, means[i].y, means[i].dx, means[i].rb, means[i].rg, means[i].gb);
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
    point.dx = 0;

    means.assign(means.size(), point);
    for(int i=0; i<cluster.size(); i++) {
        means[label[i]].x += cluster[i].x;
        means[label[i]].rb += cluster[i].rb;
        means[label[i]].rg += cluster[i].rg;
        means[label[i]].gb += cluster[i].gb;
        means[label[i]].dx += cluster[i].dx;
    }
    for(int i=0; i<means.size(); i++) {
        means[i].x /= cnt[i];
        means[i].rg /= cnt[i];
        means[i].rb /= cnt[i];
        means[i].gb /= cnt[i];
        means[i].dx /= cnt[i];
        //printf("Update %d, kmean.dx = %f, kmean.rb = %f, kmean.gb = %f, kmean.rg = %f\n", i, means[i].dx, means[i].rb, means[i].gb, means[i].rg);
    }
}

void* ocr_kmeans_load(KMEAN_STRUCT& point1, KMEAN_STRUCT& point2, std::vector<double> &v1, std::vector<double> &v2) {

    v1.push_back(point1.rb);
    v1.push_back(point1.rg);
    v1.push_back(point1.gb);
    v1.push_back(point1.dx);
    v2.push_back(point2.rb);
    v2.push_back(point2.rg);
    v2.push_back(point2.gb);
    v2.push_back(point2.dx);
}









