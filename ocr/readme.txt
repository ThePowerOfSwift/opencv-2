1、如何安装和使用CLion (linux下的gcc IDE， 和windows下的VS类似)
http://blog.csdn.net/win_lin/article/details/39579799
跳过步骤<1>、<2>
<1> 安装cmake (如果安装了opencv 那么cmake肯定已经安装了，不需要再安装)
未安装cmake 可以从http://www.cmake.org/files/v2.8/cmake-2.8.12.2.tar.gz
下载
<2> 安装gdb (系统一般默认会安装了gdb)

<3> 安装Clion
下载clion tar包  https://download.jetbrains.com/cpp/CLion-2016.2.1.tar.gz
找到tar包位置，拷贝到需要安装的目录进行解压 
tar -zxvf CLion-2016.2.1.tar.gz
进入clion bin目录 
root@yq-pc:~# cd /home/cloud/CLion/clion-2016.2.1/bin/
执行clion启动脚本即可
root@yq-pc:/home/cloud/CLion/clion-2016.2.1/bin# ./clion.sh 
Clion 是收费的IDE，只能用30天的试用期，不过网上有别人的key
http://blog.sina.com.cn/s/blog_6cee149d0102wk40.html
在Clion IDE下打开help->register选择activation code， 将网站中的key复制到下面的框里点击OK 即可
由于Clion 是基于cmake的， 所以写的编译脚本不是Makefile，而是cmake，但其实和Makefile类似，比较简单，如何编写cmake参看下面介绍

2、如何创建一个cmake文件
opencv cmake demo
http://blog.csdn.net/u012816621/article/details/51732932

3、如何编译demo 程序
<1> 在主目录下执行cmake . 生成对应的makefile
<2> 执行make 生成可执行档

4、opencv
<1> 图像操作基础结构体及之间关系IplImage, CvMat, Mat 的关系
http://www.cnblogs.com/summerRQ/articles/2406109.html
<2> opencv3.0 mat api
http://docs.opencv.org/3.0-beta/modules/core/doc/basic_structures.html#mat-convertto
<3> 图像二值化算法
http://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html#threshold
http://stackoverflow.com/questions/17141535/how-to-use-the-otsu-threshold-in-opencv
http://blog.csdn.net/hitwengqi/article/details/6856768
http://blog.csdn.net/ningyaliuhebei/article/details/40736377






