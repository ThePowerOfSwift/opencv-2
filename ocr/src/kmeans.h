//
// Created by root on 16-12-11.
//

#ifndef OCR_KMEANS_H
#define OCR_KMEANS_H
#include <vector>
#include "ocr_kmeans.h"

#ifndef KMEAN_STRUCT
#define KMEAN_STRUCT double
#endif

int kmeans_init(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT> &cluster, int knum, \
  void* fn(std::vector<KMEAN_STRUCT>&, std::vector<KMEAN_STRUCT>&, int));
int kmeans_sort_cluster(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT> &cluster, std::vector<int> &label, std::vector<int> &cnt, \
  void* fn(KMEAN_STRUCT&, KMEAN_STRUCT&, std::vector<double>&, std::vector<double>&));
double kmeans_cal_deltaSum(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT> &cluster, std::vector<int> &label, \
  void* fn(KMEAN_STRUCT&, KMEAN_STRUCT&, std::vector<double>&, std::vector<double>&));
int kmeans_update(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT> cluster, std::vector<int> &label, std::vector<int> &cnt, \
  void* fn(std::vector<KMEAN_STRUCT>&, std::vector<KMEAN_STRUCT>&, std::vector<int>&, std::vector<int>&));
void kmeans_regularization(std::vector<KMEAN_STRUCT> &cluster, \
  void* fn(std::vector<KMEAN_STRUCT>&));

#endif //OCR_KMEANS_H
