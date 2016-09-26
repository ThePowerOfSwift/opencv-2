//
// Created by root on 16-10-3.
//

#ifndef OCR_OCR_ML_H
#define OCR_OCR_ML_H

int ocr_traning(const char* samplePath, const char* bpPath);
int ocr_predict(const char * testPath, const char * bpPath);

#endif //OCR_OCR_ML_H
