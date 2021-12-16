#include <iostream>
#include "OpenNI.h"
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace std;

int main()
{
	//定义oni文件中视频的总帧数以及得到的图片的保存目录
	int total = 0;
	const char* imagefile = "D:\\User\\Downloads\\scenenn\\225\\image";
	const char* depthfile = "D:\\User\\Downloads\\scenenn\\225\\depth";
	const char* timeFilename = "D:\\User\\Downloads\\scenenn\\225\\timestamp.txt";

	//初始化OpenNI环境
	openni::OpenNI::initialize();

	//声明设备并打开oni文件
	openni::Device fromonifile;
	fromonifile.open("D:\\User\\Downloads\\scenenn\\225.oni");

	//声明控制对象，这对视频流的控制起到了关键作用
	openni::PlaybackControl* pController = fromonifile.getPlaybackControl();

	//声明视频流对象以及帧对象
	openni::VideoStream streamColor;
	openni::VideoFrameRef frameColor;
	openni::VideoStream streamDepth;
	openni::VideoFrameRef frameDepth;

	//验证是否有彩色传感器（是否有彩色视频）和建立与设备想关联的视频流
	if (fromonifile.hasSensor(openni::SENSOR_COLOR))
	{
		if (streamColor.create(fromonifile, openni::SENSOR_COLOR) == openni::STATUS_OK)
		{
			cout << "建立视频流成功" << endl;
		}
		else
		{
			cerr << "ERROR: 建立视频流没有成功" << endl;
			system("pause");
			return -1;
		}
	}
	else
	{
		cerr << "ERROR: 该设备没有彩色传感器" << endl;
		system("pause");
		return -1;
	}

	//验证是否有深度传感器和建立与设备想关联的视频流
	if (fromonifile.hasSensor(openni::SENSOR_DEPTH))
	{
		if (streamDepth.create(fromonifile, openni::SENSOR_DEPTH) == openni::STATUS_OK)
		{
			cout << "建立视频流成功" << endl;
		}
		else
		{
			cerr << "ERROR: 建立视频流没有成功" << endl;
			system("pause");
			return -1;
		}
	}
	else
	{
		cerr << "ERROR: 该设备没有深度传感器" << endl;
		system("pause");
		return -1;
	}

	//建立显示窗口
	cv::namedWindow("Image");
	cv::namedWindow("Depth");

	//获取总的视频帧数并将该设备的速度设为-1以便能留出足够的时间对每一帧进行处理、显示和保存
	total = pController->getNumberOfFrames(streamColor);
	pController->setSpeed(-1);

	//开启视频流
	streamColor.start();
	streamDepth.start();
	FILE* file_timestamp = fopen(timeFilename, "w");
	for (int i = 1; i <= total; ++i)
	{
		//读取视频流的当前帧
		streamColor.readFrame(&frameColor);
		streamDepth.readFrame(&frameDepth);

		cout << "当前正在读的帧数是：" << frameColor.getFrameIndex() << endl;
		cout << "当前的循环次数是：  " << i << endl;

		//将帧保存到Mat中并且将其转换到BGR模式，因为在OpenCV中图片的模式是BGR
		cv::Mat rgbImg(frameColor.getHeight(), frameColor.getWidth(), CV_8UC3, (void*)frameColor.getData());
		cv::Mat bgrImg;
		cvtColor(rgbImg, bgrImg, CV_RGB2BGR);
		
		cv::Mat depImg(frameDepth.getHeight(), frameDepth.getWidth(), CV_16UC1, (void*)frameDepth.getData());
		cv::Mat depMat;
		depImg.convertTo(depMat, CV_8UC1);

		//将每一帧按顺序帧保存到图片目录下
		char imagefullname[255];
		char imagenum[50];
		sprintf_s(imagenum, "\\image%05d.png", i);
		strcpy_s(imagefullname, imagefile);
		strcat_s(imagefullname, imagenum);
		cv::imwrite(imagefullname, bgrImg);

		char depthfullname[255];
		char depthnum[50];
		sprintf_s(depthnum, "\\depth%05d.png", i);
		strcpy_s(depthfullname, depthfile);
		strcat_s(depthfullname, depthnum);
		cv::imwrite(depthfullname, depImg);
		
		fprintf(file_timestamp, "%d %lld %lld\n", i, frameColor.getTimestamp(), frameDepth.getTimestamp());

		//显示当前帧
		cv::imshow("Image", bgrImg);
		//cv::imshow("Depth", depImg);
		if (cv::waitKey(30) == 27)
		{
			break;
		}
	}
	fclose(file_timestamp);
	//销毁显示窗口
	cv::destroyWindow("Image");
	cv::destroyWindow("Depth");

	//关闭视频流
	streamColor.destroy();
	streamDepth.destroy();

	//关闭设备
	fromonifile.close();

	//关闭OpenNI
	openni::OpenNI::shutdown();

	return 0;
}