# scenenn_rosbag
This repository is modified by  ethz-asl/scenenn_to_rosbag  and part of it is from websites  

## Usage
ReadONI.cpp可以读取.oni文件，得到image和depth图片，以及timestamp.txt  
需要OpenNI 2和OpenCV库（本人是在Windows vs2019上使用，因此未提供cmakelist等文件）  

scenenn_to_rosbag.py可以将图片转换为rosbag包  
转换所需要的图片的存储方式请看[ethz-asl/scenenn_to_rosbag相关部分](https://github.com/ethz-asl/scenenn_to_rosbag)  
image和depth图片以及timestamp.txt由ReadONI.cpp获得，其他可从[SceneNN](http://scenenn.net/)下的服务器(可以从[ethz-asl/scenenn_to_rosbag/issues部分](https://github.com/hkust-vgd/scenenn/issues/16)找到服务器网址)，其下有contrib/ dev/ downsample/ home/ images/ main/ shrec17/ shrec18/ video/ check.txt 。  
mask图片可在shrec17/labels/下压缩包找到对应场景mask图片（图片的名字不需要修改为mask00001.png的形式）  
trajectory.log可在main/对应场景文件夹下找到  
asus.ini和kinect2.ini可在main/intrinsic/下找到  
场景的oni文件在main/oni下找到  
  
使用方法如下：  
python scenenn_to_rosbag.py --scenenn-path PATH/TO/scenenn_data --scene-id ID --output-bag NAME  
正常ROS下可以直接运行  
