#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
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
constexpr auto N = 50;
cv::Mat datMat[N];
int a = 10;//方差前面的系数
Mat aveTemp(480, 640, CV_16UC3, Scalar(0, 0, 0));//视频图像的行数和列数
//注意这里定义16位的三通道Mat 用于计算50帧的像素平均值，不然数据会溢出
Mat aveMat(480, 640, CV_8UC3, Scalar(0, 0, 0));
Mat showMat(480, 640, CV_8UC3, Scalar(0, 0, 0));
double sigma = 0;
void aChange(int, void*)
{
	for (int i = 0; i < 480; i++) { //一个像素点为单位进行处理
		for (int j = 0; j < 640; j++)
		{
			int data0 = abs(frame.at<Vec3b>(i, j)[0] - aveMat.at<Vec3b>(i, j)[0]);
			int data1 = abs(frame.at<Vec3b>(i, j)[1] - aveMat.at<Vec3b>(i, j)[1]);
			int data2 = abs(frame.at<Vec3b>(i, j)[2] - aveMat.at<Vec3b>(i, j)[2]);
			int data = (data0 + data1 + data2) / 3;
			//if (abs(sigma0 - data0) > a * sigma0|| abs(sigma1 - data1) > a * sigma1|| abs(sigma1 - data1) > a * sigma1) //超过一定范围 则认为是运动的物体
			if (abs(sigma - data) > a * sigma)
			{
				showMat.at<Vec3b>(i, j)[0] = 255;
				showMat.at<Vec3b>(i, j)[1] = 255;
				showMat.at<Vec3b>(i, j)[2] = 255;
			}
			else {
				showMat.at<Vec3b>(i, j)[0] = 0;
				showMat.at<Vec3b>(i, j)[1] = 0;
				showMat.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	namedWindow("目标物体", WINDOW_AUTOSIZE);
	imshow("目标物体", showMat);
}

void test12_2()
{
	int i, j;
	int wid = 0;
	int hei = 0;
	int area = 0;
	double sigma0 = 0;
	double sigma1 = 0;
	double sigma2 = 0;
	int a=10;//方差前面的系数
	Mat aveTemp(480, 640, CV_16UC3, Scalar(0, 0, 0));//视频图像的行数和列数
   //注意这里定义16位的三通道Mat 用于计算50帧的像素平均值，不然数据会溢出
	Mat aveMat(480, 640, CV_8UC3, Scalar(0, 0, 0));
	Mat showMat(480, 640, CV_8UC3, Scalar(0, 0, 0));
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "不能打开视频文件" << std::endl;
	}
	int cnt = 0;
	int frame_cnt = 0;
	while (1) {
		cap >> frame;
		createTrackbar("阈值系数", "目标物体", &a, 20, aChange);
		//cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (frame_cnt == 0) {
			wid = frame.cols;//640
			hei = frame.rows;//480
			area = wid * hei;
			frame.copyTo(datMat[frame_cnt]);//第一幅图也保存下来存在datMat[0]里；
			frame_cnt++;
		}
		else if(frame_cnt<N){//保存50张图像
			frame.copyTo(datMat[frame_cnt]);
			frame_cnt++;
		}
		else if(frame_cnt==N){//拍到50张，保存了50张图像
			for (i = 0; i < hei; i++) { //480
				for (j = 0; j < wid; j++)//640
				{
					//ushort a1 = aveTemp.at<Vec3w>(480, 0)[0];//(480,0)可以--（640,480）坐标
					//ushort a2 = datMat[0].at<Vec3b>(0, 480)[0];//(0,480)可以--（480,640）行列
					for (cnt = 0; cnt < N; cnt++) {//50帧相加 求均值u				
						aveTemp.at<Vec3w>(i, j)[0] += datMat[cnt].at<Vec3b>(i, j)[0];//i行j列第k个通道 
						aveTemp.at<Vec3w>(i, j)[1] += datMat[cnt].at<Vec3b>(i, j)[1];//(640,480)
						aveTemp.at<Vec3w>(i, j)[2] += datMat[cnt].at<Vec3b>(i, j)[2];
						//add(aveMat, datMat[cnt], aveMat);
					}
					aveMat.at<Vec3b>(i, j) = aveTemp.at<Vec3w>(i, j) /N;//求每一个像素点的50个均值--高斯分布
					//求方差sigma
					for (cnt = 0; cnt < N; cnt++) {//∑（X-u)^2
						sigma0 += pow(abs(datMat[cnt].at<Vec3b>(i, j)[0] - aveMat.at<Vec3b>(i, j)[0]), 2);
						sigma1 += pow(abs(datMat[cnt].at<Vec3b>(i, j)[1] - aveMat.at<Vec3b>(i, j)[1]), 2);
						sigma2 += pow(abs(datMat[cnt].at<Vec3b>(i, j)[2] - aveMat.at<Vec3b>(i, j)[2]), 2);
						//add(aveMat, datMat[cnt], aveMat);
					}//sigma^2=∑（X-u)^2/N;
					sigma0 = pow(sigma0 / N, 0.5);
					sigma1 = pow(sigma1 / N, 0.5);
					sigma2 = pow(sigma2 / N, 0.5);
					sigma= (sigma0 + sigma1 + sigma2) / 3;//求每一个像素点的50个方差--高斯分布
					//cout << "i:" << i<< endl;
					//cout << "j:" << j << endl;
				}
			}
			frame_cnt++;
		}
		else {//已经计算出了每一个像素点的均值以及整体方差，接下来将正常处理视频流，看方差--离原来的图像像素有多远
			for (i = 0; i < hei; i++) { //一个像素点为单位进行处理
				for (j = 0; j < wid; j++)
				{
					int data0 = abs(frame.at<Vec3b>(i, j)[0] - aveMat.at<Vec3b>(i, j)[0]);
					int data1 = abs(frame.at<Vec3b>(i, j)[1] - aveMat.at<Vec3b>(i, j)[1]);
					int data2 = abs(frame.at<Vec3b>(i, j)[2] - aveMat.at<Vec3b>(i, j)[2]);
					int data = (data0 + data1 + data2) / 3;
					//if (abs(sigma0 - data0) > a * sigma0|| abs(sigma1 - data1) > a * sigma1|| abs(sigma1 - data1) > a * sigma1) //超过一定范围 则认为是运动的物体
					if(abs(sigma - data) > a * sigma)
					{
						showMat.at<Vec3b>(i, j)[0] = 255;
						showMat.at<Vec3b>(i, j)[1] = 255;
						showMat.at<Vec3b>(i, j)[2] = 255;
					}
					else {
						showMat.at<Vec3b>(i, j)[0] = 0;
						showMat.at<Vec3b>(i, j)[1] = 0;
						showMat.at<Vec3b>(i, j)[2] = 0;
					}
				}
			}
			namedWindow("目标物体", WINDOW_AUTOSIZE);
			imshow("目标物体", showMat);
		}		
		namedWindow("frame", WINDOW_AUTOSIZE);
		imshow("frame", frame);
		waitKey(30);
	}
}


int main()
{
	test12_2();
	return 0;
}