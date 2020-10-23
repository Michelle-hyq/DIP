#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// ������
//-------��ϰ1 ����HSVģ��ʵ��Ŀ�����ص���ȡ--------------------------------------------------------//
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
//-------��ϰ2 ��ֵ�������ַ���--------------------------------------------------------//
void test3_2()//��ֵ�������ַ���
{
	cv::Mat srcMat = imread("D://image/timg1.jpg", 0);
	cv::Mat dstMat;
	//threshold(srcMat, dstMat, 100, 255, THRESH_BINARY);
	//-----------------100��ֵ--255Ԥ���������������ֵ--ʹ�õĶ�ֵ���㷨
	adaptiveThreshold(srcMat, dstMat, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 15, 8);
	//--------255Ԥ���������������ֵ------����Ӧ��ֵ�㷨-------�����ƻ򷴶�������ֵ--�ֲ�����ߴ�һ��3/5/7��--10���㷨�й�
	namedWindow("dstMat", WINDOW_NORMAL);
	imshow("dstMat", dstMat);
	waitKey(0);
}
//------��ϰ3-�ص�����------------------------------------------------//
string window_name = "binaryMat";
void threshold_Mat(int th, void* data)
{
	Mat src = *(Mat*)(data);
	Mat dst;
	//��ֵ��
	threshold(src, dst, th, 255, 0);
	//------th��ֵ--255Ԥ���������������ֵ--ʹ�õĶ�ֵ���㷨
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
		cout << "ͼ������ʧ��" << endl;
		return 0;
	}
	//cvtColor(srcMat, gryMat, CV_BGR2GRAY);
	threshold_Mat(lowTH, &gryMat);
	//namedWindow("gryMat", WINDOW_NORMAL);
	//imshow(window_name, gryMat);
	createTrackbar(
		"threshold",//�켣��������
		window_name,//�켣�������Ĵ�������
		&lowTH,		//ָ�����͵�ָ�룬��ʾ�����λ�ã��ڴ���ʱ������ĳ�ʼλ�þ��Ǹñ����ĳ�ʼֵ
		maxTH,		//������Ե�������λ�õ�ֵ
		threshold_Mat,//ָ��ص�������ָ��
		&gryMat		//�û������ص�����������
	);
	waitKey(0);
	return 0;
}
int main()
{
	test3_3();
	return 0;
}/**/