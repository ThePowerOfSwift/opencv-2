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

void ocr_dbg(cv::Mat mSrcImg, const char* name);
cv::Mat ocr_read(const char* ocr_name);
int ocr_write(cv::Mat srcImg, const char* ocr_name);
cv::Mat ocr_read_grey(const char* ocr_name);
cv::Mat ocr_binary(cv::Mat mSrcImg, double thresh, int method, int size, double delta);
cv::Mat ocr_rgb2hsv(cv::Mat mSrcImg);
cv::Mat ocr_gethsv(cv::Mat mSrcImg, int hsv_channel);
cv::Mat ocr_smooth(cv::Mat mSrcImg, int smooth_type);
int ocr_preprocess(const char* srcImg, const char* desImg);


#endif //OCR_OCR_READ_HPP_H
