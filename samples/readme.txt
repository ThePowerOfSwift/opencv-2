step 1 设置动态库路径
root@ubuntu:~# vim /etc/ld.so.conf
/home/cloud/opencv/opencv/lib  (调整为自己的路径)
/home/cloud/opencv/opencv/opencv_lib/lib (调整为自己的路径)
root@ubuntu:~# ldconfig

step 2 设置opencv.pc里的路径
root@ubuntu:/home/cloud/opencv_demo# vim opencv_libib/pkgcon/lfig/opencv.pc
prefix=/home/cloud/opencv_demo/opencv_lib (调整为自己的路径)
step 3 设置pkg config路径，每次进入命令行时都需要设置，可以加入.bashrc里自动保存
root@ubuntu:~# export PKG_CONFIG_PATH=/home/cloud/opencv/opencv/opencv_lib/lib/pkgconfig (调整为自己的路径)

step 4 make 








