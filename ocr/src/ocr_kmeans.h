//
// Created by root on 16-12-11.
//

#ifndef OCR_OCR_KMEANS_H
#define OCR_OCR_KMEANS_H
#include <vector>

struct KPoint {
    int x;
    int y;
    int rv; //red value
    int bv;
    int gv;
    double rb; //red value/ blue value
    double rg;
    double gb;
    double dx;
};

#define KMEAN_STRUCT KPoint

void* ocr_kmeans_init(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT> &cluster, int knum);
void* ocr_kmeans_update(std::vector<KMEAN_STRUCT> &means, std::vector<KMEAN_STRUCT> &cluster, std::vector<int> &label, std::vector<int> &cnt);
void* ocr_kmeans_load(KMEAN_STRUCT& point1, KMEAN_STRUCT& point2, std::vector<double> &v1, std::vector<double> &v2);
#endif //OCR_OCR_KMEANS_H

