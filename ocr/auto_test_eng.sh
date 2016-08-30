#! /bin/bash

export TESSDATA_PREFIX=/home/cloud/tesseract/tessdata

if [ -f ./test/result.txt ]; then
	rm -rf ./test/result.txt
fi

#for file in ./png/code1/*.png
#	do ./ocr_detect ${file} ./test/temp.jpg ./test/temp.tif ./test/result.txt
#done
./ocr_detect ./png/code1/1468531458890.png ./test/temp.jpg ./test/temp.tif ./test/result.txt


