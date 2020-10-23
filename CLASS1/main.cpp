#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
void test_1()//���ò�ͬ��������HSVɫ�ʿռ��ϵĲ�ͬɫ��ʵ��Ŀ�����ص���ȡ��
{
	VideoCapture cap(0);
	double scale = 0.5;

	//0-180��ɫ
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
		resize(frame, rFrame,ResImgSiz, INTER_LINEAR);//˫���Բ�ֵ��
		cvtColor(rFrame, hsvMat, COLOR_BGR2HSV);
		rFrame.copyTo(detectMat);
		cv::inRange(hsvMat, Scalar(i_minH, i_minS, i_minV), Scalar(i_maxH, i_maxV, i_maxV), detectMat);
		imshow("while: in the range", detectMat);
		imshow("frame", rFrame);
		waitKey(30);
	}
}
void test_2()//���ֶ�ֵ����������
{
	cv::Mat srcMat = imread("D://image/timg1.jpg",0);
	Mat dstMst;
	//threshold(srcMat, dstMst, 100, 255, THRESH_BINARY);//��ֵ��Ԥ���������������ֵ
	adaptiveThreshold(srcMat, dstMst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 121 , 5
	);
	//-------------------------------����Ӧ��ֵ�㷨--��������ֵ�򷴶�������ֵ--�ֲ�����ߴ�3/5/7---10�������㷨�й�
	namedWindow("dstMst", WINDOW_NORMAL);
	imshow("dstMst", dstMst);
	waitKey(0);
}

string window_name = "binaryMat";
void threshod_Mat(int th, void* data)
{
	Mat src = *(Mat*)(data);
	Mat dst;
	//��ֵ��
	threshold(src, dst, th, 255, 0);
	imshow(window_name, dst);
}
int test_3()//�ص�����
{
	Mat srcMat = imread("D://image/timg1.jpg");
	Mat gryMat;
	int LowTh = 30;
	int maxTh = 255;
	if (!srcMat.data)
	{
		cout << "ͼ������ʧ��" << endl;
		return 0;
	}
	cvtColor(srcMat, gryMat, CV_BGR2GRAY);//ת�ɻҶ�ͼ
	imshow(window_name, gryMat);
	createTrackbar( //����������+�ص�����
		"threshold",//���������ƣ���ֵ
		window_name,//��������
		&LowTh,		//Ĭ�ϳ�ʼֵ ָ�����͵�ָ�� ��ʾ����λ�� ��ʼλ��Ϊ��ǰֵ
		maxTh,		//����ɵ�������λ�õ�ֵ ��СĬ��Ϊ0
		threshod_Mat,//�ص�������������λ�øı�ʱ���ٴε��ô˺���
		&gryMat);	//���뺯��������

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
//�ص�����
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
	//	createTrackbar("��ֵ", "coin",&t,255,on_threshold);            //    ����������
	threshold(coin, res, t, 255, THRESH_BINARY);
	imshow("coin", res);
	shu = connectedComponentsWithStats(res, labels, stats, centroids, 8, CV_32S);
	cout << "����������" << shu - 1 << endl;
	waitKey(0);
	return 0;
}
