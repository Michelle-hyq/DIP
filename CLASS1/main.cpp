#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// ������
//--------------------------------------------------------------------------------------------//
//��ϰ1 ͼ����̬ѧ �ȶ�ֵ�� ��ֱ�ʴ�����͡������㡢������
#define ERODE_NUM 0
#define DILATE_NUM 1
#define OPEN_NUM 2
#define CLOSE_NUM 3
int ele_size = 3;
int trackNum = ERODE_NUM;//0��ʴ1����2������3������
cv::Mat srcMat;
cv::Mat dstMat;
cv::Mat element;
cv::Mat proOut;
void process()
{
	Mat element= getStructuringElement(MORPH_RECT, Size(2 * ele_size + 1, 2 * ele_size + 1),Point(ele_size, ele_size));
	if (trackNum == ERODE_NUM)		 erode(dstMat, proOut, element);
	else if(trackNum == DILATE_NUM)  dilate(dstMat, proOut, element);
	else if (trackNum == OPEN_NUM)	 morphologyEx(dstMat, proOut, MORPH_OPEN, element);
	else if (trackNum == CLOSE_NUM)  morphologyEx(dstMat, proOut, MORPH_CLOSE, element);

	namedWindow("Ч��ͼ", WINDOW_NORMAL);
	imshow("Ч��ͼ", proOut);
}
void TrackNumChange(int, void*){ process(); }
void eleSizeChange(int, void*) { process(); }
void test4_1()
{
	srcMat = imread("D://image/timg3.jpg", 0);
	threshold(srcMat, dstMat, 100, 255, THRESH_BINARY);
	namedWindow("��ֵ��", WINDOW_NORMAL);
	imshow("��ֵ��", dstMat);
	process();
	/*
	createTrackbar(
		"threshold",//�켣��������
		window_name,//�켣�������Ĵ�������
		&lowTH,		//ָ�����͵�ָ�룬��ʾ�����λ�ã��ڴ���ʱ������ĳ�ʼλ�þ��Ǹñ����ĳ�ʼֵ
		maxTH,		//������Ե�������λ�õ�ֵ
		threshold_Mat,//ָ��ص�������ָ��
		&gryMat		//�û������ص�����������
	);/**/
	createTrackbar("���򿪱�", "Ч��ͼ", &trackNum, 3, TrackNumChange);//0��ʴ1����2������3������
	createTrackbar("�ں˳ߴ�", "Ч��ͼ", &ele_size, 21, eleSizeChange);
	waitKey(0);
} 
//----------------------------------------------------------------------------------------------//
//��ϰ2 ��ͨ����
void test4_2()//
{
	srcMat = imread("D://image/timg3.jpg", 0);
	cv::Mat src_img, labels, stats, centroids, img_color, img_gray;
	threshold(srcMat, dstMat, 100, 255, THRESH_BINARY);
	namedWindow("��ֵ��", WINDOW_NORMAL);
	imshow("��ֵ��", dstMat);
	int nccomps = cv::connectedComponentsWithStats(
		dstMat, //��ֵͼ��
		labels,     //��ԭͼһ����ı��ͼ
		stats, //nccomps��5�ľ��� ��ʾÿ����ͨ�������Ӿ��κ������pixel��
		centroids //nccomps��2�ľ��� ��ʾÿ����ͨ���������
	);
	printf("ԭͼ����ͨ��������%d\n", nccomps);//��ͨ���������
	waitKey(0);
}
//----------------------------------------------------------------------------------//
//��ϰ3 ���ɷ��ָ���ͼ�������������ͨ���ǣ�����ͼ����̬ѧ����ѧ��֪ʶʵ���Զ�����ԭ�������
void test4_3()
{
	int ele_size2 = 9;
	srcMat = imread("D://image/timg4.jpg", 0);
	cv::Mat  labels, stats, centroids,out_res;
	threshold(srcMat, dstMat, 100, 255, THRESH_BINARY);//���ɷ�
	namedWindow("��ֵ��", WINDOW_NORMAL);
	imshow("��ֵ��", dstMat);
	element = getStructuringElement(MORPH_RECT, Size(2 * ele_size2 + 1, 2 * ele_size2 + 1), Point(ele_size2, ele_size2));
	dilate(dstMat, proOut, element);//�����ͺ�ʴ
	erode(proOut, proOut, element);
	namedWindow("��ʴ", WINDOW_NORMAL);
	imshow("��ʴ", proOut);
	bitwise_not(proOut, proOut);//����ֵ��
	int nccomps = cv::connectedComponentsWithStats(
		proOut, //��ֵͼ��
		labels,     //��ԭͼһ����ı��ͼ
		stats, //nccomps��5�ľ��� ��ʾÿ����ͨ�������Ӿ��κ������pixel��
		centroids //nccomps��2�ľ��� ��ʾÿ����ͨ���������
	);
	cout << "��⵽ԭ�����: " << nccomps-1 << endl;//ԭ�����(����������)
	//printf("��ͨ��������%d\n", nccomps);//��ͨ���������
	waitKey(0);
}
//----------------------------------------------------------------------------------------//
//��ϰ4 ����������Զ����� ˼·�����˵���ͨ���С������
void test4_4()
{
	int ele_size3 = 1;
	srcMat = imread("D://image/timg5.jpg", 0);
	cv::Mat  labels, stats, centroids, out_res;
	threshold(srcMat, dstMat, 100, 255, THRESH_BINARY);//
	namedWindow("��ֵ��", WINDOW_NORMAL);
	imshow("��ֵ��", dstMat);
	element = getStructuringElement(MORPH_RECT, Size(2 * ele_size3 + 1, 2 * ele_size3 + 1), Point(ele_size3, ele_size3));
	bitwise_not(dstMat, proOut);
	namedWindow("����ֵ��", WINDOW_NORMAL);
	imshow("����ֵ��", proOut);
	/*//��������
	vector<vector<Point>> contours;
	Mat resultImg = Mat::zeros(proOut.size(), CV_8UC3);//�൱�ڴ���һ�ź�ɫ��ͼ��ÿ�����ص�ÿ��ͨ����Ϊ0,Scalar(0,0,0)��
	RNG rng((int)time(0));//����һ�������rng
	findContours(proOut, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(-1, -1));//Ѱ������
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));//�õ������ɫ
		drawContours(resultImg, contours, static_cast<int>(i), color, -1, 8, Mat(), 0, Point(-1, -1));//���������Ϊ���ɫ
	}
	//namedWindow("Final image", WINDOW_NORMAL);
	//imshow("Final image", resultImg);
	/**/
	int nccomps = cv::connectedComponentsWithStats(
		proOut, //��ֵͼ��
		labels,     //��ԭͼһ����ı��ͼ
		stats, //nccomps��5�ľ��� ��ʾÿ����ͨ�������Ӿ��κ������pixel��
		centroids //nccomps��2�ľ��� ��ʾÿ����ͨ���������
	);
	int invalid_num = 1;//��Ч��ͨ�� ��ʼֵΪ1��������
	for (int i = 1; i < nccomps; i++)
	{
		int area = stats.at<int>(i, CC_STAT_AREA);
		cout << "count:" << i << "  area:" << area << endl;
		if (area < 6000||area>8000) invalid_num++;
	}
	cout << "��⵽���������: " << nccomps - invalid_num << endl;
	//printf("��ͨ��������%d\n", nccomps);//��ͨ���������
	waitKey(0);
}
int main()
{
	test4_4();
	return 0;
}/**/
/*
void test4_1()
{	
	threshold(srcMat, dstMat, 100, 255, THRESH_BINARY);
	namedWindow("��ֵ��", WINDOW_NORMAL);
	imshow("��ֵ��", dstMat);
	//��ȡ��ʴ�Զ����
	cv::Mat erd_element = getStructuringElement(MORPH_RECT, Size(2*ele_size+1, 2*ele_size+1));
	cv::Mat erdOut;
	erode(dstMat, erdOut, erd_element);
	namedWindow("��ʴ", WINDOW_NORMAL);
	imshow("��ʴ", erdOut);
	//��ȡ�����Զ����
	cv::Mat dlt_element = getStructuringElement(MORPH_RECT, Size(2*ele_size+1, 2*ele_size+1));
	cv::Mat dltOut;
	dilate(dstMat, dltOut, dlt_element);
	namedWindow("����", WINDOW_NORMAL);
	imshow("����", dltOut);
	//�����㣬�ȸ�ʴ������
	cv::Mat open_element = getStructuringElement(MORPH_RECT, Size(2*ele_size+1, 2*ele_size+1));
	cv::Mat openOut;
	morphologyEx(dstMat, openOut, MORPH_OPEN, open_element);
	namedWindow("������-���ú���", WINDOW_NORMAL);
	imshow("������-���ú���", openOut);
	//�����㣬�����ͺ�ʴ
	cv::Mat close_element = getStructuringElement(MORPH_RECT, Size(2*ele_size+1, 2*ele_size+1));
	cv::Mat closeOut;
	morphologyEx(dstMat, closeOut, MORPH_CLOSE, close_element);
	namedWindow("������-���ú���", WINDOW_NORMAL);
	imshow("������-���ú���", closeOut);
	waitKey(0);
}/**/