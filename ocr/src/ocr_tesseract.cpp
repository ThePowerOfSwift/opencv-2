//
// Created by root on 16-8-15.
//

#include "ocr_tesseract.h"
#include <stdio.h>
#include <stdlib.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

char* ocr_tesseract(const char* input, const char* output, const char* ocr_type) {

    int ret = 0;
    static char result[128];

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    //Initialize tesseract-ocr with English
    if(api->Init(NULL, ocr_type)) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return NULL;
    }

    //Open input image with leptonica lib
    Pix* image = pixRead(input);
    api->SetImage(image);

    //Get ocr result
    char* outText;
    outText = api->GetUTF8Text();
    printf("OCR output: %s.\n", outText);
    strcpy(result, outText);

    FILE* pf;
    pf = fopen(output, "a+");
    if(pf == NULL)
    {
        fprintf(stderr, "open %s error\n", output);
        ret = -1;
        goto ocr_finish;
    }
    fprintf(pf, "%s", outText);
    fclose(pf);

    ocr_finish:
        //Destroy used obj and release memory
        api->End();
        delete[] outText;
        pixDestroy(&image);
    return result;
}