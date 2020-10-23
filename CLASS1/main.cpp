#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// 课堂三
//-------练习1 利用HSV模型实现目标像素的提取--------------------------------------------------------//
void test3_1()
{
	//VideoCapture cap(0);
	double scale = 0.5;

	double i_minH = 0;
	double i_maxH = 20;

	double i_minS = 43;
	double i_maxS = 255;

	double i_minV = 55;
	double i_maxV = 255;
	while (1)
	{
		Mat frame;
		Mat hsvMat;
		Mat detectMat;
		VideoCapture cap(0);
		cap >> frame;
		Size ResImgSiz = Size(frame.cols * scale, frame.rows * scale);
		Mat rFrame = Mat(ResImgSiz, frame.type());
		resize(frame, rFrame, ResImgSiz, INTER_LINEAR);
		cvtColor(rFrame, hsvMat, COLOR_BGR2HSV);

		rFrame.copyTo(detectMat);
		cv::inRange(hsvMat, Scalar(i_minH, i_minS, i_minV), Scalar(i_maxH, i_maxS, i_maxV), detectMat);
		imshow("while: in the range", detectMat);
		imshow("frame", rFrame);
		waitKey(3);
	}
}
//-------练习2 二值化的两种方法--------------------------------------------------------//
void test3_2()//二值化的两种方法
{
	cv::Mat srcMat = imread("D://image/timg1.jpg", 0);
	cv::Mat dstMat;
	//threshold(srcMat, dstMat, 100, 255, THRESH_BINARY);
	//-----------------100阈值--255预设满足条件的最大值--使用的二值化算法
	adaptiveThreshold(srcMat, dstMat, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 15, 8);
	//--------255预设满足条件的最大值------自适应阈值算法-------二进制或反二进制阈值--局部区域尺寸一般3/5/7等--10与算法有关
	namedWindow("dstMat", WINDOW_NORMAL);
	imshow("dstMat", dstMat);
	waitKey(0);
}
//------练习3-回调函数------------------------------------------------//
string window_name = "binaryMat";
void threshold_Mat(int th, void* data)
{
	Mat src = *(Mat*)(data);
	Mat dst;
	//二值化
	threshold(src, dst, th, 255, 0);
	//------th阈值--255预设满足条件的最大值--使用的二值化算法
	imshow(window_name, dst);

}
int test3_3()
{
	//Mat srcMat = imread("D://image/timg1.jpg");
	Mat gryMat = imread("D://image/timg1.jpg", 0);
	int lowTH = 30;
	int maxTH = 255;
	if (!gryMat.data)
	{
		cout << "图像载入失败" << endl;
		return 0;
	}
	//cvtColor(srcMat, gryMat, CV_BGR2GRAY);
	threshold_Mat(lowTH, &gryMat);
	//namedWindow("gryMat", WINDOW_NORMAL);
	//imshow(window_name, gryMat);
	createTrackbar(
		"threshold",//轨迹条的名字
		window_name,//轨迹条依附的窗口名字
		&lowTH,		//指向整型的指针，表示滑块的位置，在创建时，滑块的初始位置就是该变量的初始值
		maxTH,		//滑块可以到达的最大位置的值
		threshold_Mat,//指向回调函数的指针
		&gryMat		//用户传给回调函数的数据
	);
	waitKey(0);
	return 0;
}
int main()
{
	test3_3();
	return 0;
}/**/