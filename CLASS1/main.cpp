#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;
void test2_1()
{
	cv::Mat src_color = imread("D://image/timg2.jpg");
	std::vector<cv::Mat> channels;
	cv::split(src_color, channels);
	cv::Mat B = channels.at(0);
	cv::Mat G = channels.at(1);
	cv::Mat R = channels.at(2);
	cv::imshow("red", R);
	cv::imshow("blue", G);
	cv::imshow("green", B);
	cv::imshow("original Mat", src_color);
	waitKey(0);
}
void test2_2()
{
	VideoCapture cap;
	cap.open(0);
	while (!cap.isOpened());
	{
		cap.open(0);
		std::cout << "不能打开视频文件" << std::endl;
		
	}
	double fps = cap.get(CAP_PROP_FPS);
	std::cout << "fps" << fps << std::endl;
	while (1)
	{
		cv::Mat frame;
		bool rSucess = cap.read(frame);
		if (!rSucess)
		{
			std::cout << "不能从视频文件读取帧" << std::endl;
			break;
		}
		else
			cv::imshow("frame", frame);
		waitKey(30);
	}

}
void test2_3_1()//画圆
{
	cv::Point pt;
	cv::Mat srcMat = Mat::zeros(500, 500, CV_8UC3);
	pt.x = 100;
	pt.y = 100;
	circle(srcMat, pt, 50, CV_RGB(255, 0, 0), 1, 8, 0);
	imshow("circle", srcMat);
	waitKey(0);
}
void test2_3_2()//画线
{
	cv::Point pt1, pt2;
	cv::Mat srcMat = Mat::zeros(500, 500, CV_8UC3);
	pt1.x = 10;
	pt1.y = 10;
	pt2.x = 20;
	pt2.y = 10;
	line(srcMat, pt1, pt2, CV_RGB(255, 0, 0), 1, 8, 0);//红
	imshow("line", srcMat);
	waitKey(0);
}
void test2_3_3()//画矩形
{
	cv::Rect rect;
	cv::Mat srcMat = Mat::zeros(500, 500, CV_8UC3);
	rect.x = 10;
	rect.y = 10;
	rect.width = 3;
	rect.height = 4;
	rectangle(srcMat, rect, CV_RGB(255, 0, 0), 1, 8, 0);//红色
	imshow("rectangle", srcMat);
	waitKey(0);
}
void test2_3()//画圆、直线、矩形
{
	cv::Point pt;
	cv::Mat srcMat = Mat::zeros(500, 500, CV_8UC3);//生成黑色画布
	pt.x = 100;
	pt.y = 100;
	circle(srcMat, pt, 50, CV_RGB(255, 0, 0), 1, 8, 0);
	cv::Point pt1, pt2;
	pt1.x = 20;
	pt1.y = 250;
	pt2.x = 400;
	pt2.y = 100;
	line(srcMat, pt1, pt2, CV_RGB(255, 0, 0), 1, 8, 0);//红
	cv::Rect rect;
	rect.x = 200;
	rect.y = 300;
	rect.width = 150;
	rect.height = 80;
	rectangle(srcMat, rect, CV_RGB(255, 0, 0), 1, 8, 0);//红色
	imshow("circle", srcMat);
	waitKey(0);
}
int main()
{
	test2_3();
	return 0;
}/**/