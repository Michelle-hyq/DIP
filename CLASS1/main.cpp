#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// 课堂十二
//--------------------------------------------------------------------------------------------//
//练习1 背景差分
cv::Mat frame, bgMat, subMat, bny_subMat;
VideoCapture cap;
cv::Mat frame_out;
void test12_1()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "不能打开视频文件" << std::endl;
	}
	int cnt = 0;

	while(1){
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (cnt == 0) {
			frame.copyTo(bgMat);
		}
		else {
			//第二帧开始背景差分
			//相减
			absdiff(frame, bgMat, subMat);
			//absdiff(bgMat, frame, subMat);
			//差分结果二值化
			threshold(subMat, bny_subMat, 50, 255, CV_THRESH_BINARY);
			imshow("b_subMat", bny_subMat);
			imshow("subMat", subMat);
			waitKey(30);
		}
		cnt = 1;
	}
}
//--------------------------------------------------------------------------------------------//
//高斯建模及背景差分
void test12_2()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "不能打开视频文件" << std::endl;
	}
	int cnt = 0;

	while (1) {
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (cnt == 0) {
			frame.copyTo(bgMat);
		}
		else {
			//第二帧开始背景差分
			//相减
			absdiff(frame, bgMat, subMat);
			//absdiff(bgMat, frame, subMat);
			//差分结果二值化
			threshold(subMat, bny_subMat, 45, 255, CV_THRESH_BINARY);
			imshow("b_subMat", bny_subMat);
			imshow("subMat", subMat);
			waitKey(30);
		}
		cnt = 1;
	}
}

int median_size = 5;
void med_fil_sizeChange(int, void*);
void med_proc() {
	bool rSucess = cap.read(frame);
	if (!rSucess) {
		std::cout << "不能从视频文件读取帧" << std::endl;
	}
	namedWindow("中值原图", WINDOW_NORMAL);
	imshow("中值原图", frame);
	medianBlur(frame, frame_out, median_size * 2 + 1);//一直出错，不能为偶数
	namedWindow("中值滤波", WINDOW_NORMAL);
	imshow("中值滤波", frame_out);
	waitKey(30);
}
void med_fil_sizeChange(int, void*)
{
	med_proc();
}
void test5_1()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "不能打开视频文件" << std::endl;
	}

	med_proc();
	createTrackbar("中值尺寸", "中值滤波", &median_size, 10, med_fil_sizeChange);
	while (1)
	{
		med_proc();
		createTrackbar("中值尺寸", "中值滤波", &median_size, 10, med_fil_sizeChange);
		//cvSetTrackbarPos("滤波尺寸", "中值滤波", median_size);	
	}
	/*
	createTrackbar(
		"threshold",//轨迹条的名字
		window_name,//轨迹条依附的窗口名字
		&lowTH,		//指向整型的指针，表示滑块的位置，在创建时，滑块的初始位置就是该变量的初始值
		maxTH,		//滑块可以到达的最大位置的值
		threshold_Mat,//指向回调函数的指针
		&gryMat		//用户传给回调函数的数据
	);/**/
}
//--------------------------------------------------------------------------------------------//
//练习2 均值滤波 读取摄像头图像，并对摄像头图像进行均值滤波
int avr_size = 3;
void avr_fil_sizeChange(int, void*);
void avr_proc() {
	bool rSucess = cap.read(frame);
	if (!rSucess) {
		std::cout << "不能从视频文件读取帧" << std::endl;
	}
	namedWindow("均值原图", WINDOW_NORMAL);
	imshow("均值原图", frame);
	if (avr_size == 0) avr_size = 1;//尺寸不能为0
	blur(frame, frame_out, Size(avr_size, avr_size));
	namedWindow("均值滤波", WINDOW_NORMAL);
	imshow("均值滤波", frame_out);
	waitKey(30);
}
void avr_fil_sizeChange(int, void*)
{
	avr_proc();
}
void test5_2()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "不能打开视频文件" << std::endl;
	}
	avr_proc();
	createTrackbar("均值尺寸", "均值滤波", &avr_size, 10, avr_fil_sizeChange);
	while (1)
	{
		avr_proc();
		createTrackbar("均值尺寸", "均值滤波", &avr_size, 10, avr_fil_sizeChange);
	}
}
//--------------------------------------------------------------------------------------------//
//练习3 均值滤波-高斯滤波 读取摄像头图像，并对摄像头图像进行高斯滤波
int Gauss_size = 5;
void Gauss_fil_sizeChange(int, void*);
void Gauss_proc() {
	bool rSucess = cap.read(frame);
	if (!rSucess) {
		std::cout << "不能从视频文件读取帧" << std::endl;
	}
	namedWindow("高斯原图", WINDOW_NORMAL);
	imshow("高斯原图", frame);
	if (Gauss_size == 0) Gauss_size = 1;//尺寸不能为0
	blur(frame, frame_out, Size(Gauss_size, Gauss_size));
	namedWindow("高斯滤波", WINDOW_NORMAL);
	imshow("高斯滤波", frame_out);
	waitKey(30);
}
void Gauss_fil_sizeChange(int, void*)
{
	Gauss_proc();
}
void test5_3()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "不能打开视频文件" << std::endl;
	}
	Gauss_proc();
	createTrackbar("高斯尺寸", "高斯滤波", &Gauss_size, 10, Gauss_fil_sizeChange);
	while (1)
	{
		Gauss_proc();
		createTrackbar("高斯尺寸", "高斯滤波", &Gauss_size, 10, Gauss_fil_sizeChange);
	}
}
//--------------------------------------------------------------------------------------------//
//练习4 边缘提取 读取摄像头图像，并对摄像头图像进行边缘提取分别提取x，y方向上的边缘，观察结果有何区别。
int Sobel_size = 3;
cv::Mat frame_outx, frame_outy;
cv::Mat abs_grad_outx, abs_grad_outy;
void Sobel_fil_sizeChange(int, void*);
void Sobel_proc() {
	bool rSucess = cap.read(frame);
	if (!rSucess) {
		std::cout << "不能从视频文件读取帧" << std::endl;
	}
	namedWindow("边缘原图", WINDOW_NORMAL);
	imshow("边缘原图", frame);
	cvtColor(frame, frame, CV_BGR2GRAY);
	//threshold(frame, frame, 100, 255, THRESH_BINARY);
	Sobel(frame, frame_outx, CV_16S, 1, 0, Sobel_size * 2 + 1);//输出图像的深度、x差分阶数、y差分阶数，sobel核大小：奇数
	//convertScaleAbs(frame_outx, abs_grad_outx);
	namedWindow("x边缘提取", WINDOW_NORMAL);
	imshow("x边缘提取", frame_outx);
	Sobel(frame, frame_outy, CV_16S, 0, 1, Sobel_size * 2 + 1);//输出图像的深度、x差分阶数、y差分阶数，sobel核大小：奇数
	//convertScaleAbs(frame_outy, abs_grad_outy);
	namedWindow("y边缘提取", WINDOW_NORMAL);
	imshow("y边缘提取", frame_outy);

	waitKey(30);
}
void Sobel_fil_sizeChange(int, void*)
{
	Sobel_proc();
}
void test5_4()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "不能打开视频文件" << std::endl;
	}
	Sobel_proc();
	createTrackbar("边缘内核", "边缘原图", &Sobel_size, 10, Sobel_fil_sizeChange);
	while (1)
	{
		Sobel_proc();
		createTrackbar("边缘内核", "边缘原图", &Sobel_size, 10, Sobel_fil_sizeChange);
	}
}
int main()
{
	test12_1();
	return 0;
}