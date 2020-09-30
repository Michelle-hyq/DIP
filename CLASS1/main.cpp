#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;
void test1_1()//ƽ��
{
	cv::Mat srcMat = imread("D://image/timg1.jpg");
	//imshow("src", srcMat);
	int height = srcMat.rows;
	int width = srcMat.cols;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			uchar average = (srcMat.at<Vec3b>(j, i)[0] + srcMat.at<Vec3b>(j, i)[1] + srcMat.at<Vec3b>(j, i)[2] / 3);
			srcMat.at<Vec3b>(j, i)[0] = average;
			srcMat.at<Vec3b>(j, i)[1] = average;
			srcMat.at<Vec3b>(j, i)[2] = average;
		}
	}
	namedWindow("src", WINDOW_NORMAL);
	imshow("src", srcMat);
	waitKey(0);
}
void test1_2()//�Ҷ�ͼ
{
	cv::Mat srcMat = imread("D://image/timg1.jpg", 0);
	//imshow("src", srcMat);
	int height = srcMat.rows;
	int width = srcMat.cols;
	namedWindow("src", WINDOW_NORMAL);
	imshow("src", srcMat);
	waitKey(0);
}
void test1_3()//�Ҷ�ͼ
{
	cv::Mat srcMat = imread("D://image/timg1.jpg");
	int height = srcMat.rows;
	int width = srcMat.cols;
	uchar threshold = 100;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			uchar average = (srcMat.at<Vec3b>(j, i)[0] + srcMat.at<Vec3b>(j, i)[1] + srcMat.at<Vec3b>(j, i)[2] / 3);
			if (average > threshold) average = 255;
			else average = 0;
			srcMat.at<Vec3b>(j, i)[0] = average;
			srcMat.at<Vec3b>(j, i)[1] = average;
			srcMat.at<Vec3b>(j, i)[2] = average;
		}
	}
	namedWindow("src", WINDOW_NORMAL);
	imshow("src", srcMat);
	waitKey(0);
}
void test1_4()//��� ǳ����
{
	cv::Mat srcMat = imread("D://image/timg1.jpg");
	cv::Mat deepMat;
	srcMat.copyTo(deepMat);
	cv::Mat shallowMat = srcMat;
	int height = srcMat.rows;
	int width = srcMat.cols;
	uchar threshold = 100;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			uchar average = (srcMat.at<Vec3b>(j, i)[0] + srcMat.at<Vec3b>(j, i)[1] + srcMat.at<Vec3b>(j, i)[2] / 3);
			if (average > threshold) average = 255;
			else average = 0;
			srcMat.at<Vec3b>(j, i)[0] = average;
			srcMat.at<Vec3b>(j, i)[1] = average;
			srcMat.at<Vec3b>(j, i)[2] = average;
		}
	}
	namedWindow("src", WINDOW_NORMAL);
	imshow("src", shallowMat);
	waitKey(0);
}
int main()
{
	test1_1();
	return 0;
}/**/