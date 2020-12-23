#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
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
void test12_2()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "���ܴ���Ƶ�ļ�" << std::endl;
	}
	int cnt = 0;

	while (1) {
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
			threshold(subMat, bny_subMat, 45, 255, CV_THRESH_BINARY);
			imshow("b_subMat", bny_subMat);
			imshow("subMat", subMat);
			waitKey(30);
		}
		cnt = 1;
	}
}

int median_size = 5;
void med_fil_sizeChange(int, void*);
void med_proc() {
	bool rSucess = cap.read(frame);
	if (!rSucess) {
		std::cout << "���ܴ���Ƶ�ļ���ȡ֡" << std::endl;
	}
	namedWindow("��ֵԭͼ", WINDOW_NORMAL);
	imshow("��ֵԭͼ", frame);
	medianBlur(frame, frame_out, median_size * 2 + 1);//һֱ��������Ϊż��
	namedWindow("��ֵ�˲�", WINDOW_NORMAL);
	imshow("��ֵ�˲�", frame_out);
	waitKey(30);
}
void med_fil_sizeChange(int, void*)
{
	med_proc();
}
void test5_1()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "���ܴ���Ƶ�ļ�" << std::endl;
	}

	med_proc();
	createTrackbar("��ֵ�ߴ�", "��ֵ�˲�", &median_size, 10, med_fil_sizeChange);
	while (1)
	{
		med_proc();
		createTrackbar("��ֵ�ߴ�", "��ֵ�˲�", &median_size, 10, med_fil_sizeChange);
		//cvSetTrackbarPos("�˲��ߴ�", "��ֵ�˲�", median_size);	
	}
	/*
	createTrackbar(
		"threshold",//�켣��������
		window_name,//�켣�������Ĵ�������
		&lowTH,		//ָ�����͵�ָ�룬��ʾ�����λ�ã��ڴ���ʱ������ĳ�ʼλ�þ��Ǹñ����ĳ�ʼֵ
		maxTH,		//������Ե�������λ�õ�ֵ
		threshold_Mat,//ָ��ص�������ָ��
		&gryMat		//�û������ص�����������
	);/**/
}
//--------------------------------------------------------------------------------------------//
//��ϰ2 ��ֵ�˲� ��ȡ����ͷͼ�񣬲�������ͷͼ����о�ֵ�˲�
int avr_size = 3;
void avr_fil_sizeChange(int, void*);
void avr_proc() {
	bool rSucess = cap.read(frame);
	if (!rSucess) {
		std::cout << "���ܴ���Ƶ�ļ���ȡ֡" << std::endl;
	}
	namedWindow("��ֵԭͼ", WINDOW_NORMAL);
	imshow("��ֵԭͼ", frame);
	if (avr_size == 0) avr_size = 1;//�ߴ粻��Ϊ0
	blur(frame, frame_out, Size(avr_size, avr_size));
	namedWindow("��ֵ�˲�", WINDOW_NORMAL);
	imshow("��ֵ�˲�", frame_out);
	waitKey(30);
}
void avr_fil_sizeChange(int, void*)
{
	avr_proc();
}
void test5_2()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "���ܴ���Ƶ�ļ�" << std::endl;
	}
	avr_proc();
	createTrackbar("��ֵ�ߴ�", "��ֵ�˲�", &avr_size, 10, avr_fil_sizeChange);
	while (1)
	{
		avr_proc();
		createTrackbar("��ֵ�ߴ�", "��ֵ�˲�", &avr_size, 10, avr_fil_sizeChange);
	}
}
//--------------------------------------------------------------------------------------------//
//��ϰ3 ��ֵ�˲�-��˹�˲� ��ȡ����ͷͼ�񣬲�������ͷͼ����и�˹�˲�
int Gauss_size = 5;
void Gauss_fil_sizeChange(int, void*);
void Gauss_proc() {
	bool rSucess = cap.read(frame);
	if (!rSucess) {
		std::cout << "���ܴ���Ƶ�ļ���ȡ֡" << std::endl;
	}
	namedWindow("��˹ԭͼ", WINDOW_NORMAL);
	imshow("��˹ԭͼ", frame);
	if (Gauss_size == 0) Gauss_size = 1;//�ߴ粻��Ϊ0
	blur(frame, frame_out, Size(Gauss_size, Gauss_size));
	namedWindow("��˹�˲�", WINDOW_NORMAL);
	imshow("��˹�˲�", frame_out);
	waitKey(30);
}
void Gauss_fil_sizeChange(int, void*)
{
	Gauss_proc();
}
void test5_3()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "���ܴ���Ƶ�ļ�" << std::endl;
	}
	Gauss_proc();
	createTrackbar("��˹�ߴ�", "��˹�˲�", &Gauss_size, 10, Gauss_fil_sizeChange);
	while (1)
	{
		Gauss_proc();
		createTrackbar("��˹�ߴ�", "��˹�˲�", &Gauss_size, 10, Gauss_fil_sizeChange);
	}
}
//--------------------------------------------------------------------------------------------//
//��ϰ4 ��Ե��ȡ ��ȡ����ͷͼ�񣬲�������ͷͼ����б�Ե��ȡ�ֱ���ȡx��y�����ϵı�Ե���۲����к�����
int Sobel_size = 3;
cv::Mat frame_outx, frame_outy;
cv::Mat abs_grad_outx, abs_grad_outy;
void Sobel_fil_sizeChange(int, void*);
void Sobel_proc() {
	bool rSucess = cap.read(frame);
	if (!rSucess) {
		std::cout << "���ܴ���Ƶ�ļ���ȡ֡" << std::endl;
	}
	namedWindow("��Եԭͼ", WINDOW_NORMAL);
	imshow("��Եԭͼ", frame);
	cvtColor(frame, frame, CV_BGR2GRAY);
	//threshold(frame, frame, 100, 255, THRESH_BINARY);
	Sobel(frame, frame_outx, CV_16S, 1, 0, Sobel_size * 2 + 1);//���ͼ�����ȡ�x��ֽ�����y��ֽ�����sobel�˴�С������
	//convertScaleAbs(frame_outx, abs_grad_outx);
	namedWindow("x��Ե��ȡ", WINDOW_NORMAL);
	imshow("x��Ե��ȡ", frame_outx);
	Sobel(frame, frame_outy, CV_16S, 0, 1, Sobel_size * 2 + 1);//���ͼ�����ȡ�x��ֽ�����y��ֽ�����sobel�˴�С������
	//convertScaleAbs(frame_outy, abs_grad_outy);
	namedWindow("y��Ե��ȡ", WINDOW_NORMAL);
	imshow("y��Ե��ȡ", frame_outy);

	waitKey(30);
}
void Sobel_fil_sizeChange(int, void*)
{
	Sobel_proc();
}
void test5_4()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "���ܴ���Ƶ�ļ�" << std::endl;
	}
	Sobel_proc();
	createTrackbar("��Ե�ں�", "��Եԭͼ", &Sobel_size, 10, Sobel_fil_sizeChange);
	while (1)
	{
		Sobel_proc();
		createTrackbar("��Ե�ں�", "��Եԭͼ", &Sobel_size, 10, Sobel_fil_sizeChange);
	}
}
int main()
{
	test12_1();
	return 0;
}