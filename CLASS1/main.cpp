// dip-class-demos.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv.hpp>
#include "funtions.h"

using namespace cv;
using namespace std;

void test_1()
{
	kMeansDemo();
}
void test_2()
{
	segColor();
}
void test_3()
{
	segColor_1();
}
int main()
{
	//��ʼ��ʱ
	double start = static_cast<double>(cvGetTickCount());
	
	test_3();
	//������ʱ
	double time = ((double)cvGetTickCount() - start) / cvGetTickFrequency();
	//��ʾʱ��
	cout << "processing time:" << time / 1000 << "ms" << endl;

	//�ȴ�������Ӧ�����������������
	system("pause");
	return 0;
}
