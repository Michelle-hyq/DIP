#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
void test_1()//利用不同的物体在HSV色彩空间上的不同色域，实现目标像素的提取。
{
	VideoCapture cap(0);
	double scale = 0.5;

	//0-180肤色
	double i_minH = 0;
	double i_maxH = 20;
	//0-255
	double i_minS = 43;
	double i_maxS = 255;
	//0-255
	double i_minV = 55;
	double i_maxV = 255;
	while (1)
	{
		Mat frame;
		Mat hsvMat;
		Mat detectMat;

		cap >> frame;
		Size ResImgSiz = Size(frame.cols * scale, frame.rows* scale);
		Mat rFrame = Mat(ResImgSiz, frame.type());
		resize(frame, rFrame,ResImgSiz, INTER_LINEAR);//双线性插值法
		cvtColor(rFrame, hsvMat, COLOR_BGR2HSV);
		rFrame.copyTo(detectMat);
		cv::inRange(hsvMat, Scalar(i_minH, i_minS, i_minV), Scalar(i_maxH, i_maxV, i_maxV), detectMat);
		imshow("while: in the range", detectMat);
		imshow("frame", rFrame);
		waitKey(30);
	}
}
void test_2()//两种二值化函数调用
{
	cv::Mat srcMat = imread("D://image/timg1.jpg",0);
	Mat dstMst;
	//threshold(srcMat, dstMst, 100, 255, THRESH_BINARY);//阈值、预设满足条件的最大值
	adaptiveThreshold(srcMat, dstMst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 121 , 5
	);
	//-------------------------------自适应阈值算法--二进制阈值或反二进制阈值--局部区域尺寸3/5/7---10参数与算法有关
	namedWindow("dstMst", WINDOW_NORMAL);
	imshow("dstMst", dstMst);
	waitKey(0);
}

string window_name = "binaryMat";
void threshod_Mat(int th, void* data)
{
	Mat src = *(Mat*)(data);
	Mat dst;
	//二值化
	threshold(src, dst, th, 255, 0);
	imshow(window_name, dst);
}
int test_3()//回调函数
{
	Mat srcMat = imread("D://image/timg1.jpg");
	Mat gryMat;
	int LowTh = 30;
	int maxTh = 255;
	if (!srcMat.data)
	{
		cout << "图像载入失败" << endl;
		return 0;
	}
	cvtColor(srcMat, gryMat, CV_BGR2GRAY);//转成灰度图
	imshow(window_name, gryMat);
	createTrackbar( //创建滑动条+回调函数
		"threshold",//滑动条名称：阈值
		window_name,//窗口名称
		&LowTh,		//默认初始值 指向整型的指针 表示滑块位置 初始位置为当前值
		maxTh,		//滑块可到达的最大位置的值 最小默认为0
		threshod_Mat,//回调函数名，滑块位置改变时会再次调用此函数
		&gryMat);	//传入函数的数据

	//namedWindow("dstMat", WINDOW_NORMAL);
	waitKey(0);
	return 0;
}
int main()
{
	test_3();
	return 0;
}/**/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
Mat coin, res;
Mat labels, stats(55, 55, CV_8UC3), centroids(22, 22, CV_8UC3);
//回调函数
/*
void on_threshold(int, void*)
{
	threshold(coin, res, t, 255, THRESH_BINARY);
	imshow("coin", res);

}
*/
int main()
{
	int shu;
	int t = 88;
	coin = imread("C:/Users/25686/Desktop/yan.jpg", 0);
	//	namedWindow("coin",1);
	//	createTrackbar("阈值", "coin",&t,255,on_threshold);            //    滑动条创建
	threshold(coin, res, t, 255, THRESH_BINARY);
	imshow("coin", res);
	shu = connectedComponentsWithStats(res, labels, stats, centroids, 8, CV_32S);
	cout << "洞洞数量：" << shu - 1 << endl;
	waitKey(0);
	return 0;
}
