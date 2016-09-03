#! /bin/bash

export TESSDATA_PREFIX=/home/cloud/tesseract/tessdata

if [ -d ./test ]; then
	rm -rf ./test/*
fi

for file in ./png/code1/*.png
	do ./ocr_detect ${file} ./test/temp.jpg ./test/temp.tif test ./test/result.txt
done
#./ocr_detect ./png/code1/1468498462821.png ./test/temp.jpg ./test/temp.tif test ./test/result.txt
#./ocr_detect ./temp_1.png ./test/temp.jpg ./test/temp.tif test ./test/result.txt

