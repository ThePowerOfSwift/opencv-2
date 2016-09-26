#! /bin/bash

export TESSDATA_PREFIX=/home/cloud/tesseract/tessdata

if [ -d ./test ]; then
	rm -rf ./test/*
fi

if [-d ./cut ]; then
	rm -rf ./cut/*
fi
i=1

for file in ./png/code5/*.png
	do 
		./ocr_detect ${file} ./test/$i.jpg ./test/temp.tif cut ./test/result.txt $i
		i=`expr $i + 1`
done
#./ocr_detect ./png/code1/1468500000502.png ./test/temp.jpg ./test/temp.tif test ./test/result.txt
#./ocr_detect ./temp_1.png ./test/temp.jpg ./test/temp.tif test ./test/result.txt

