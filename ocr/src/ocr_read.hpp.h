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


#endif //OCR_OCR_READ_HPP_H
