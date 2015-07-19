#---------------useful url----------------#
http://stackoverflow.com/questions/25504964/opencv-python-valueerror-too-many-values-to-unpack

http://blog.csdn.net/sunny2038/article/details/9170013

http://stackoverflow.com/questions/26132081/is-cv2-cv-missing-in-opencv-3-0

http://www.cnblogs.com/pmars/p/4143158.html

http://answers.opencv.org/question/32183/cv2boxpoints-used-in-tutorials-does-not-exist-in-opencv-248/

#---------------bug fix-------------------#
根据博客中给出的python for opencv code检测图片条形码，发现遇到几个编译不过的问题，原因是其给出的opencv版本是2.X， 我使用的是3.X，解决方法如下：

1、cv2.cv.XX  -----> cv2.XX

2、(cnts, _) = cv2.findContours(closed.copy(), cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
	----------->
   (_, cnts, _) = cv2.findContours(closed.copy(), cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)

3、cv2.cv.BoxPoints(rect) ------> cv2.boxPoints(rect)
	
修改后运行python detect_barcode.py --image test.jpg 成功
3幅图片运行截图见snapshotX.png
