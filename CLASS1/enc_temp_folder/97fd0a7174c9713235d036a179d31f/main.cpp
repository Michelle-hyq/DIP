#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// 课堂八
//轮廓 特征判断
cv::Mat frame;
cv::Mat frame_out;
VideoCapture cap;
//--------------------------------------------------------------------------------------------//
//练习1：寻找轮廓
cv::Mat srcMat, binMat, gryMat,minRectMat, canny_img;
int canny_d= 150;
void canny_change(int, void*)
{
	Canny(srcMat, canny_img, canny_d, 110, 3);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(canny_img, canny_img, MORPH_CLOSE, kernel);
	imshow("边缘滤波", canny_img);
}
void test8_1()
{
	int i;
	//vector<vector<Point>> contours;//m个<n个坐标点>
	//第0行（第一个轮廓的n个坐标点）：[x1,y1],[x2,y2]... 向量型的点 --n个点
	//第1行...
	//vector<Vec4i> hierarchy;//Vec4i:n个<4个整型变量>   的向量
	//输出轮廓的继承关系 int型[a1,b1,c1,d1],[a2,b2,c2,d2]...

	//vector<Rect>像素width*height from 位置(x,y)
	//[width x height from(x,y)]

	//vector<RotatedRect>
	//angle:0 center:[0,0] size:[0 x 0]
	srcMat = imread("D://image/timg9.jpg");
	namedWindow("原图", WINDOW_AUTOSIZE);
	imshow("原图", srcMat);
	gryMat = imread("D://image/timg9.jpg",0);//灰度图
	//二值化
	Mat binary_image;
	threshold(gryMat, binary_image, 100, 255, THRESH_BINARY);
	imshow("binary", binary_image);
	//查找所有轮廓
	vector<vector<Point>> contours;//m个<n个坐标点>
	vector<Vec4i> hireachy;//Vec4i:n个<4个整型变量>   的向量
	//写法一
	//findContours(binMat, contours,hireachy, RETR_TREE, CHAIN_APPROX_NONE);
	//for (int i = 0; i < contours.size(); i++) {
	//    drawContours(dst, contours, i, Scalar(0, 0, 255), 1, 8, hierarchy);
	//}
	//写法二
	findContours(binary_image, contours, RETR_TREE, CHAIN_APPROX_NONE);
	//定义结果图 复制
	Mat dstMat = Mat(srcMat.rows, srcMat.cols, CV_8UC3, Scalar(0, 0, 0));
	//颜色表，用不同颜色画出找到的不同轮廓
	Scalar color[] = { Scalar(0,0,255), Scalar(0,255,0), Scalar(255,0,0), Scalar(255,255,0) ,Scalar(255,0,255) };
	//初始化周长、面积、圆形度、周径比
	double len = 0, area = 0, roundness = 0, lenratio = 0;
	cout << contours.size() << endl;
	//循环找出所有符合条件的轮廓
	for (int t = 0; t < contours.size(); t++)
	{
		//条件：过滤掉小的干扰轮廓
		Rect rect = boundingRect(contours[t]);//矩形轮廓
		if (rect.width < 10)
			continue;
		//画出找到的轮廓，第t个
		drawContours(dstMat, contours, static_cast<int>(t), Scalar(255, 255, 255), 1, 8, hireachy);

		//绘制该轮廓的最小外接矩形
		RotatedRect minrect = minAreaRect(contours[t]);
		Point2f P[4];
		minrect.points(P);
		for (int i = 0; i < 4; i++) cv::line(dstMat, P[i], P[i < 3 ? i + 1 : 0], color[t%5], 1, CV_AA);
		//cout <<P<< endl;//外接矩形尺寸

		//计算面积、周长、圆形度、周径比
		area = contourArea(contours[t]);//面积
		len = arcLength(contours[t], true);//周长
		roundness = (4 * CV_PI * area) / (len * len);//圆形度
		if(roundness>0.55&& area>100) //圆形度大于0.6 则画出轮廓
			drawContours(srcMat, contours, static_cast<int>(t), Scalar(0, 255, 0), CV_FILLED);
		cout << "圆形度：" << area << endl;
		
	}
	//显示结果
	namedWindow("轮廓图", WINDOW_AUTOSIZE);
	imshow("轮廓图", dstMat);
	namedWindow("标记原图", WINDOW_AUTOSIZE);
	imshow("标记原图", srcMat);
	//drawContours(dstMat, contours, -1, Scalar(0, 255, 0), 1);
	//findContours(binMat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//CV_RETR_EXTERNAL:只检测外围轮廓 CV_RETR_LIST 检测所有轮廓包括内围外围
	//CV_CHAIN_APPROX_NONE保存边界上所有连续的轮廓点到contours向量内
	//CV_CHAIN_APPROX_SIMPLE仅保存拐点
	//			二值图·保存轮廓的向量·寻找模式·近似方法
	//drawContours(dstMat, contours, -1, Scalar(0, 255, 255), 1, 8);//轮廓;
	
	waitKey(0);
}

void test8_2()
{
	int i;
	//vector<vector<Point>> contours;//m个<n个坐标点>
	//第0行（第一个轮廓的n个坐标点）：[x1,y1],[x2,y2]... 向量型的点 --n个点
	//第1行...
	//vector<Vec4i> hierarchy;//Vec4i:n个<4个整型变量>   的向量
	//输出轮廓的继承关系 int型[a1,b1,c1,d1],[a2,b2,c2,d2]...

	//vector<Rect>像素width*height from 位置(x,y)
	//[width x height from(x,y)]

	//vector<RotatedRect>
	//angle:0 center:[0,0] size:[0 x 0]
	srcMat = imread("D://image/timg8.jpg");
	namedWindow("原图", WINDOW_AUTOSIZE);
	imshow("原图", srcMat);
	gryMat = imread("D://image/timg8.jpg", 0);//灰度图
	//二值化
	Mat binary_image;
	threshold(gryMat, binary_image, 100, 255, THRESH_BINARY);
	imshow("binary", binary_image);
	//查找所有轮廓
	vector<vector<Point>> contours;//m个<n个坐标点>
	vector<Vec4i> hireachy;//Vec4i:n个<4个整型变量>   的向量
	//写法一
	//findContours(binMat, contours,hireachy, RETR_TREE, CHAIN_APPROX_NONE);
	//for (int i = 0; i < contours.size(); i++) {
	//    drawContours(dst, contours, i, Scalar(0, 0, 255), 1, 8, hierarchy);
	//}
	//写法二
	findContours(binary_image, contours, RETR_TREE, CHAIN_APPROX_NONE);
	//定义结果图 复制
	Mat dstMat = Mat(srcMat.rows, srcMat.cols, CV_8UC3, Scalar(0, 0, 0));
	//颜色表，用不同颜色画出找到的不同轮廓
	Scalar color[] = { Scalar(0,0,255), Scalar(0,255,0), Scalar(255,0,0), Scalar(255,255,0) ,Scalar(255,0,255) };
	//初始化周长、面积、圆形度、周径比
	double len = 0, area = 0, roundness = 0, lenratio = 0;
	cout << contours.size() << endl;
	//循环找出所有符合条件的轮廓
	for (int t = 0; t < contours.size(); t++)
	{
		//条件：过滤掉小的干扰轮廓
		Rect rect = boundingRect(contours[t]);//矩形轮廓
		if (rect.width < 10)
			continue;
		//画出找到的轮廓，第t个
		drawContours(dstMat, contours, static_cast<int>(t), Scalar(255, 255, 255), 1, 8, hireachy);

		//绘制该轮廓的最小外接矩形
		RotatedRect minrect = minAreaRect(contours[t]);
		Point2f P[4];
		minrect.points(P);
		for (int i = 0; i < 4; i++) cv::line(dstMat, P[i], P[i < 3 ? i + 1 : 0], color[t % 5], 1, CV_AA);
		//cout <<P<< endl;//外接矩形尺寸

		//计算面积、周长、圆形度、周径比
		area = contourArea(contours[t]);//面积
		len = arcLength(contours[t], true);//周长
		roundness = (4 * CV_PI * area) / (len * len);//圆形度
		if ( area > 100) //圆形度大于0.6 则画出轮廓
			drawContours(srcMat, contours, static_cast<int>(t), Scalar(0, 255, 0), CV_FILLED);
		cout << "圆形度：" << area << endl;

	}
	//显示结果
	namedWindow("轮廓图", WINDOW_AUTOSIZE);
	imshow("轮廓图", dstMat);
	namedWindow("标记原图", WINDOW_AUTOSIZE);
	imshow("标记原图", srcMat);
	//drawContours(dstMat, contours, -1, Scalar(0, 255, 0), 1);
	//findContours(binMat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//CV_RETR_EXTERNAL:只检测外围轮廓 CV_RETR_LIST 检测所有轮廓包括内围外围
	//CV_CHAIN_APPROX_NONE保存边界上所有连续的轮廓点到contours向量内
	//CV_CHAIN_APPROX_SIMPLE仅保存拐点
	//			二值图·保存轮廓的向量·寻找模式·近似方法
	//drawContours(dstMat, contours, -1, Scalar(0, 255, 255), 1, 8);//轮廓;

	waitKey(0);
}
int main()
{
	test8_2();
	return 0;
}