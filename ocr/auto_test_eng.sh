#! /bin/bash

export TESSDATA_PREFIX=/home/cloud/tesseract/tessdata

if [ -d ./test ]; then
	rm -rf ./test/*
fi

if [ -d ./cut ]; then
	rm -rf ./cut/*
fi
i=1

#for file in ./png/nianlian/*.png
for file in ./png/code8/*.png
	do 
		./ocr_detect ${file} ./test/$i.png cut ./test/result.txt $i bp.xml
#		exit
		i=`expr $i + 1`
done

