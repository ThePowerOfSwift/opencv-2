//
// Created by root on 16-8-14.
//

#ifndef OCR_OCR_READ_HPP_H
#define OCR_OCR_READ_HPP_H

typedef enum {
    E_NORMAL = 1,
    E_GAUSSIAN,
    E_MEAN,
    E_OTSU,
    E_MAX,
} eThresholdType;

typedef enum {
    E_HUE = 1,
    E_SATURATION,
    E_VALUE
} eHsvChannel;

void ocr_dbg(IplImage* img, char* name);
IplImage* ocr_read(const char* ocr_name);
int ocr_write(cv::Mat srcImg, const char* ocr_name);
IplImage* ocr_grey(IplImage* srcImg);
IplImage* ocr_binary(IplImage* srcImg, int threshold, int method, int size, double delta);
IplImage* ocr_rgb2hsv(IplImage* srcImg);
IplImage* ocr_gethsv(IplImage* srcImg, int hsv_channel);
cv::Mat ocr_smooth(IplImage* srcImg, int smooth_type);
int ocr_preprocess(const char* srcImg, const char* desImg);


#endif //OCR_OCR_READ_HPP_H
