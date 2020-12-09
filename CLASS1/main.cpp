#include "stdafx.h"
#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>

using namespace cv;
using namespace std;
//课堂十一

void MyGammaCorrection(Mat& src, Mat& dst, float fGamma)
{
	CV_Assert(src.data);

	// accept only char type matrices
	CV_Assert(src.depth() != sizeof(uchar));

	// build look up table
	unsigned char lut[256];
	for (int i = 0; i < 256; i++)
	{
		lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0), fGamma) * 255.0f);
	}

	//result = cv2.LUT(src, lut);
	//cv2.imshow("Result", result);
	dst = src.clone();
	const int channels = dst.channels();
	switch (channels)
	{
	case 1:
	{

		MatIterator_<uchar> it, end;
		for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
			//*it = pow((float)(((*it))/255.0), fGamma) * 255.0;
			*it = lut[(*it)];

		break;
	}
	case 3:
	{

		MatIterator_<Vec3b> it, end;
		for (it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++)
		{
			//(*it)[0] = pow((float)(((*it)[0])/255.0), fGamma) * 255.0;
			//(*it)[1] = pow((float)(((*it)[1])/255.0), fGamma) * 255.0;
			//(*it)[2] = pow((float)(((*it)[2])/255.0), fGamma) * 255.0;
			(*it)[0] = lut[((*it)[0])];
			(*it)[1] = lut[((*it)[1])];
			(*it)[2] = lut[((*it)[2])];
		}

		break;

	}
	}
}
void test11_1()//gamma矫正 对比度明显 
{
	cv::Mat srcMat = imread("D://image/timg14.jpg",0);
	cv::Mat dstMat;
	MyGammaCorrection(srcMat, dstMat, 0.5);
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", srcMat);
	namedWindow("dstMat", WINDOW_AUTOSIZE);
	imshow("dstMat", dstMat);
	waitKey(0);
}
void test11_2()//
{
	Mat srcMat = imread("D://image/timg15.jpg");
	Mat dstMat;
	////////////////////////////////////////////////////////////////////
	//步骤一：分通道显示
	vector<Mat>bgr_planes;
	split(srcMat, bgr_planes);
	const char* output = "histogram iamge";
	char input[] = "input image";
	namedWindow(input, CV_WINDOW_AUTOSIZE);
	namedWindow(output, CV_WINDOW_AUTOSIZE);
	imshow(input, srcMat);
	int histsize = 256;
	float range[] = { 0,256 };
	const float* histRanges = { range };
	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histsize, &histRanges, true, false);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histsize, &histRanges, true, false);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histsize, &histRanges, true, false);

	//归一化
	int hist_h = 400;//直方图的图像的高
	int hist_w = 512; //直方图的图像的宽
	int bin_w = hist_w / histsize;//直方图的等级
	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));//绘制直方图显示的图像
	normalize(b_hist, b_hist, 0, hist_h, NORM_MINMAX, -1, Mat());//归一化
	normalize(g_hist, g_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, hist_h, NORM_MINMAX, -1, Mat());

	//步骤三：绘制直方图（render histogram chart）
	for (int i = 1; i < histsize; i++)
	{
		//绘制蓝色分量直方图
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, CV_AA);
		//绘制绿色分量直方图
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, CV_AA);
		//绘制红色分量直方图
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, CV_AA);
	}
	imshow("out1", histImage);
	////////////////////////////////////////////////////////////////////////////////////
	//分割通道
	vector<Mat>channels;
	split(srcMat, channels);

	Mat blue, green, red;
	blue = channels.at(0);
	green = channels.at(1);
	red = channels.at(2);
	//分别对BGR通道做直方图均衡化
	equalizeHist(blue, blue);
	equalizeHist(green, green);
	equalizeHist(red, red);
	//合并通道
	merge(channels, dstMat);
	imshow(output, dstMat);
	////////////////////////////////////////////////////////////////////////////////////
	split(dstMat, bgr_planes);
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histsize, &histRanges, true, false);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histsize, &histRanges, true, false);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histsize, &histRanges, true, false);
	Mat histIma(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));//绘制直方图显示的图像
	normalize(b_hist, b_hist, 0, hist_h, NORM_MINMAX, -1, Mat());//归一化
	normalize(g_hist, g_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	//步骤三：绘制直方图（render histogram chart）
	for (int i = 1; i < histsize; i++)
	{
		//绘制蓝色分量直方图
		line(histIma, Point((i - 1) * bin_w, hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, CV_AA);
		//绘制绿色分量直方图
		line(histIma, Point((i - 1) * bin_w, hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, CV_AA);
		//绘制红色分量直方图
		line(histIma, Point((i - 1) * bin_w, hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, CV_AA);
	}
	imshow("out2", histIma);
	/**/

	//namedWindow("dstMat", WINDOW_AUTOSIZE);
	//imshow("dstMat", dstMat);
	waitKey(0);
}
int gamma = 150;
cv::Mat dstMat, srcMat, grayMat, result;
void gamma_Change(int, void*)
{
	MyGammaCorrection(grayMat, result, (float)gamma/100);
	namedWindow("gamma矫正", WINDOW_NORMAL);
	imshow("gamma矫正", result);
}
void test11_3()//gamma矫正 对比度明显 
{
	srcMat = imread("D://image/timg15.jpg");
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", srcMat);
	//gamma矫正
	cvtColor(srcMat, grayMat, COLOR_BGR2GRAY);//灰度图
	MyGammaCorrection(grayMat, result, 1.5);
	
	namedWindow("gamma矫正", WINDOW_NORMAL);
	imshow("gamma矫正", result);
	createTrackbar("gamma", "gamma矫正", &gamma, 200, gamma_Change);
	
	waitKey(0);
}
int main()
{
	test11_3();
	return 0;
}/**/