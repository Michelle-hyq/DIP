
// dip-class-demos.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include <opencv.hpp>
#include "funtions.h"
#include "stdafx.h"
//#include <time.h>

using namespace cv;
using namespace std;

int main()
{
	//��ʼ��ʱ
	double start = static_cast<double>(cvGetTickCount());

	//dftDemo();
	//mouseROI();
	ifftDemo();
	ifftDemo2();
	ADD2();
	//������ʱ
	double time = ((double)cvGetTickCount() - start) / cvGetTickFrequency();
	//��ʾʱ��
	cout << "processing time:" << time / 1000 << "ms" << endl;

	//�ȴ�������Ӧ�����������������
	system("pause");
	return 0;
}