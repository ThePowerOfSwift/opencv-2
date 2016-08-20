#! /bin/bash

export TESSDATA_PREFIX=/home/cloud/tesseract/tessdata
echo $TESSDATA_PREFIX

java -Djava.library.path=. com.ocr_java test/3.jpg test/1.tif test/2.txt

