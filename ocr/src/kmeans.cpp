//
// Created by root on 16-12-11.
//

#include "ocr_kmeans.h"
#include "kmeans.h"
#include <stdio.h>
#include <math.h>
#include<iostream>
#include <string.h>
#include <iterator>

using namespace std;

#ifndef KDEBUG_ENABLE
#define KDEBUG_ENABLE 0
#endif
#if KDEBUG_ENABLE
#define KDEBUG(x) \
	do{ \
		x; \
	} while(0);
#else
#define KDEBUG(x)
#endif

static double _kmeans_calDict(std::vector<double> dict1, std::vector<double> dict2) {
    double sum = 0;
    double temp = 0;

    if ((dict1.size() < 2) || (dict2.size() < 2) || (dict1.size() != dict2.size())) {
        printf("Kmeans cal para error!\n");
        return -1;
    }

    for(int i=0; i< dict1.size()-1; i++) {
        temp = dict1[i] - dict2[i];
        sum += temp * temp;
    }

    sum = sqrt(sum);
    //KDEBUG(printf("sum = %f\n", sum));
    return sum;
}

static double kmeans_calDict(KMEAN_STRUCT point1, KMEAN_STRUCT point2, \
  void* fn(KMEAN_STRUCT&, KMEAN_STRUCT&, std::vector<double>&, std::vector<double>&)) {
    int ret = 0;
    std::vector<double> tmp1;
    std::vector<double> tmp2;
    fn(point1, point2, tmp1, tmp2);
    return _kmeans_calDict(tmp1, tmp2);
}

static int kmeans_sort(KMEAN_STRUCT &point, std::vector<KMEAN_STRUCT> &means, \
  void* fn(KMEAN_STRUCT&, KMEAN_STRUCT&, std::vector<double>&, std::vector<double>&)) {
    double dict = 0;
    double sum = 0;
    int label = 0;
    if(means.size() < 2) {
        printf("Kmeans sort para error, means size < 2!\n");
        return -1;
    }
    dict = kmeans_calDict(point, means[0], fn);
    for(int i=1; i < means.size(); i++) {
        sum = kmeans_calDict(point, means[i], fn);
        if(dict > sum)	{
            dict = sum;
            label = i;
        }
    }
    //KDEBUG(printf("Point label is %d\n", label));
    return label;
}

//Init means value
int kmeans_init(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT> &cluster, int knum, \
  void* fn(std::vector<KMEAN_STRUCT>&, std::vector<KMEAN_STRUCT>&, int)) {
    int cnt = 0;
    int ret = 0;
    if((cluster.size() < 2) || (knum < 2)) {
        printf("Kmeans init para error, means size < 2!\n");
        return -1;
    }
    fn(means, cluster, knum);
    return ret;
}

int kmeans_sort_cluster(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT> &cluster, std::vector<int> &label, std::vector<int> &cnt, \
  void* fn(KMEAN_STRUCT&, KMEAN_STRUCT&, std::vector<double>&, std::vector<double>&)) {
    if ((cnt.size() != means.size()) || (label.size() < 2) || (means.size() < 2) || (label.size() != cluster.size())) {
        printf("Kmeans sort cluster para error: cnt_size = %d, means_size = %d, label_size = %d, cluster_size = %d!\n", \
          (int)cnt.size(), (int)means.size(), (int)label.size(), (int)cluster.size());
        return -1;
    }
    cnt.assign(cnt.size(), 0);
    label.assign(label.size(), 0);
    for(int i=0; i < cluster.size(); i++) {
        label[i] = kmeans_sort(cluster[i], means, fn);
        cnt[label[i]] ++;
    }

    for(int i=0; i<cnt.size(); i++) {
        KDEBUG(printf("Cluster[%d] cnt is %d\n", i, cnt[i]));
    }
    return 0;
}

double kmeans_cal_deltaSum(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT> &cluster, std::vector<int> &label, \
  void* fn(KMEAN_STRUCT&, KMEAN_STRUCT&, std::vector<double>&, std::vector<double>&)) {
    double sum = 0;
    for(int i = 0; i < cluster.size(); i++) {
        sum += kmeans_calDict(cluster[i], means[label[i]], fn);
    }
    KDEBUG(printf("cluster dict means sum: %f\n", sum));
    sum /= cluster.size();
    //KDEBUG(printf("cluster dict means sum: %f\n", sum));
    return sum;
}

int kmeans_update(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT> cluster, std::vector<int> &label, std::vector<int> &cnt, \
  void* fn(std::vector<KMEAN_STRUCT>&, std::vector<KMEAN_STRUCT>&, std::vector<int>&, std::vector<int>&)) {
    int ret = 0;
    fn(means, cluster, label, cnt);
    return ret;
}