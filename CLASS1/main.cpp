#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;
// ����ʮ��
//--------------------------------------------------------------------------------------------//
//��ϰ1 �������
cv::Mat frame, bgMat, subMat, bny_subMat;
VideoCapture cap;
cv::Mat frame_out;
void test12_1()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "���ܴ���Ƶ�ļ�" << std::endl;
	}
	int cnt = 0;

	while(1){
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (cnt == 0) {
			frame.copyTo(bgMat);
		}
		else {
			//�ڶ�֡��ʼ�������
			//���
			absdiff(frame, bgMat, subMat);
			//absdiff(bgMat, frame, subMat);
			//��ֽ����ֵ��
			threshold(subMat, bny_subMat, 50, 255, CV_THRESH_BINARY);
			imshow("b_subMat", bny_subMat);
			imshow("subMat", subMat);
			waitKey(30);
		}
		cnt = 1;
	}
}
//--------------------------------------------------------------------------------------------//
//��˹��ģ���������
constexpr auto N = 50;
cv::Mat datMat[N];
int a = 10;//����ǰ���ϵ��
Mat aveTemp(480, 640, CV_16UC3, Scalar(0, 0, 0));//��Ƶͼ�������������
//ע�����ﶨ��16λ����ͨ��Mat ���ڼ���50֡������ƽ��ֵ����Ȼ���ݻ����
Mat aveMat(480, 640, CV_8UC3, Scalar(0, 0, 0));
Mat showMat(480, 640, CV_8UC3, Scalar(0, 0, 0));
double sigma = 0;
void aChange(int, void*)
{
	for (int i = 0; i < 480; i++) { //һ�����ص�Ϊ��λ���д���
		for (int j = 0; j < 640; j++)
		{
			int data0 = abs(frame.at<Vec3b>(i, j)[0] - aveMat.at<Vec3b>(i, j)[0]);
			int data1 = abs(frame.at<Vec3b>(i, j)[1] - aveMat.at<Vec3b>(i, j)[1]);
			int data2 = abs(frame.at<Vec3b>(i, j)[2] - aveMat.at<Vec3b>(i, j)[2]);
			int data = (data0 + data1 + data2) / 3;
			//if (abs(sigma0 - data0) > a * sigma0|| abs(sigma1 - data1) > a * sigma1|| abs(sigma1 - data1) > a * sigma1) //����һ����Χ ����Ϊ���˶�������
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
	namedWindow("Ŀ������", WINDOW_AUTOSIZE);
	imshow("Ŀ������", showMat);
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
	int a=10;//����ǰ���ϵ��
	Mat aveTemp(480, 640, CV_16UC3, Scalar(0, 0, 0));//��Ƶͼ�������������
   //ע�����ﶨ��16λ����ͨ��Mat ���ڼ���50֡������ƽ��ֵ����Ȼ���ݻ����
	Mat aveMat(480, 640, CV_8UC3, Scalar(0, 0, 0));
	Mat showMat(480, 640, CV_8UC3, Scalar(0, 0, 0));
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "���ܴ���Ƶ�ļ�" << std::endl;
	}
	int cnt = 0;
	int frame_cnt = 0;
	while (1) {
		cap >> frame;
		createTrackbar("��ֵϵ��", "Ŀ������", &a, 20, aChange);
		//cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (frame_cnt == 0) {
			wid = frame.cols;//640
			hei = frame.rows;//480
			area = wid * hei;
			frame.copyTo(datMat[frame_cnt]);//��һ��ͼҲ������������datMat[0]�
			frame_cnt++;
		}
		else if(frame_cnt<N){//����50��ͼ��
			frame.copyTo(datMat[frame_cnt]);
			frame_cnt++;
		}
		else if(frame_cnt==N){//�ĵ�50�ţ�������50��ͼ��
			for (i = 0; i < hei; i++) { //480
				for (j = 0; j < wid; j++)//640
				{
					//ushort a1 = aveTemp.at<Vec3w>(480, 0)[0];//(480,0)����--��640,480������
					//ushort a2 = datMat[0].at<Vec3b>(0, 480)[0];//(0,480)����--��480,640������
					for (cnt = 0; cnt < N; cnt++) {//50֡��� ���ֵu				
						aveTemp.at<Vec3w>(i, j)[0] += datMat[cnt].at<Vec3b>(i, j)[0];//i��j�е�k��ͨ�� 
						aveTemp.at<Vec3w>(i, j)[1] += datMat[cnt].at<Vec3b>(i, j)[1];//(640,480)
						aveTemp.at<Vec3w>(i, j)[2] += datMat[cnt].at<Vec3b>(i, j)[2];
						//add(aveMat, datMat[cnt], aveMat);
					}
					aveMat.at<Vec3b>(i, j) = aveTemp.at<Vec3w>(i, j) /N;//��ÿһ�����ص��50����ֵ--��˹�ֲ�
					//�󷽲�sigma
					for (cnt = 0; cnt < N; cnt++) {//�ƣ�X-u)^2
						sigma0 += pow(abs(datMat[cnt].at<Vec3b>(i, j)[0] - aveMat.at<Vec3b>(i, j)[0]), 2);
						sigma1 += pow(abs(datMat[cnt].at<Vec3b>(i, j)[1] - aveMat.at<Vec3b>(i, j)[1]), 2);
						sigma2 += pow(abs(datMat[cnt].at<Vec3b>(i, j)[2] - aveMat.at<Vec3b>(i, j)[2]), 2);
						//add(aveMat, datMat[cnt], aveMat);
					}//sigma^2=�ƣ�X-u)^2/N;
					sigma0 = pow(sigma0 / N, 0.5);
					sigma1 = pow(sigma1 / N, 0.5);
					sigma2 = pow(sigma2 / N, 0.5);
					sigma= (sigma0 + sigma1 + sigma2) / 3;//��ÿһ�����ص��50������--��˹�ֲ�
					//cout << "i:" << i<< endl;
					//cout << "j:" << j << endl;
				}
			}
			frame_cnt++;
		}
		else {//�Ѿ��������ÿһ�����ص�ľ�ֵ�Լ����巽�������������������Ƶ����������--��ԭ����ͼ�������ж�Զ
			for (i = 0; i < hei; i++) { //һ�����ص�Ϊ��λ���д���
				for (j = 0; j < wid; j++)
				{
					int data0 = abs(frame.at<Vec3b>(i, j)[0] - aveMat.at<Vec3b>(i, j)[0]);
					int data1 = abs(frame.at<Vec3b>(i, j)[1] - aveMat.at<Vec3b>(i, j)[1]);
					int data2 = abs(frame.at<Vec3b>(i, j)[2] - aveMat.at<Vec3b>(i, j)[2]);
					int data = (data0 + data1 + data2) / 3;
					//if (abs(sigma0 - data0) > a * sigma0|| abs(sigma1 - data1) > a * sigma1|| abs(sigma1 - data1) > a * sigma1) //����һ����Χ ����Ϊ���˶�������
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
			namedWindow("Ŀ������", WINDOW_AUTOSIZE);
			imshow("Ŀ������", showMat);
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