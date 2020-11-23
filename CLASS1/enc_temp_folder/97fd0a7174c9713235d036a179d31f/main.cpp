#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// ���ð�
//���� �����ж�
cv::Mat frame;
cv::Mat frame_out;
VideoCapture cap;
//--------------------------------------------------------------------------------------------//
//��ϰ1��Ѱ������
cv::Mat srcMat, binMat, gryMat,minRectMat, canny_img;
int canny_d= 150;
void canny_change(int, void*)
{
	Canny(srcMat, canny_img, canny_d, 110, 3);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(canny_img, canny_img, MORPH_CLOSE, kernel);
	imshow("��Ե�˲�", canny_img);
}
void test8_1()
{
	int i;
	//vector<vector<Point>> contours;//m��<n�������>
	//��0�У���һ��������n������㣩��[x1,y1],[x2,y2]... �����͵ĵ� --n����
	//��1��...
	//vector<Vec4i> hierarchy;//Vec4i:n��<4�����ͱ���>   ������
	//��������ļ̳й�ϵ int��[a1,b1,c1,d1],[a2,b2,c2,d2]...

	//vector<Rect>����width*height from λ��(x,y)
	//[width x height from(x,y)]

	//vector<RotatedRect>
	//angle:0 center:[0,0] size:[0 x 0]
	srcMat = imread("D://image/timg9.jpg");
	namedWindow("ԭͼ", WINDOW_AUTOSIZE);
	imshow("ԭͼ", srcMat);
	gryMat = imread("D://image/timg9.jpg",0);//�Ҷ�ͼ
	//��ֵ��
	Mat binary_image;
	threshold(gryMat, binary_image, 100, 255, THRESH_BINARY);
	imshow("binary", binary_image);
	//������������
	vector<vector<Point>> contours;//m��<n�������>
	vector<Vec4i> hireachy;//Vec4i:n��<4�����ͱ���>   ������
	//д��һ
	//findContours(binMat, contours,hireachy, RETR_TREE, CHAIN_APPROX_NONE);
	//for (int i = 0; i < contours.size(); i++) {
	//    drawContours(dst, contours, i, Scalar(0, 0, 255), 1, 8, hierarchy);
	//}
	//д����
	findContours(binary_image, contours, RETR_TREE, CHAIN_APPROX_NONE);
	//������ͼ ����
	Mat dstMat = Mat(srcMat.rows, srcMat.cols, CV_8UC3, Scalar(0, 0, 0));
	//��ɫ���ò�ͬ��ɫ�����ҵ��Ĳ�ͬ����
	Scalar color[] = { Scalar(0,0,255), Scalar(0,255,0), Scalar(255,0,0), Scalar(255,255,0) ,Scalar(255,0,255) };
	//��ʼ���ܳ��������Բ�ζȡ��ܾ���
	double len = 0, area = 0, roundness = 0, lenratio = 0;
	cout << contours.size() << endl;
	//ѭ���ҳ����з�������������
	for (int t = 0; t < contours.size(); t++)
	{
		//���������˵�С�ĸ�������
		Rect rect = boundingRect(contours[t]);//��������
		if (rect.width < 10)
			continue;
		//�����ҵ�����������t��
		drawContours(dstMat, contours, static_cast<int>(t), Scalar(255, 255, 255), 1, 8, hireachy);

		//���Ƹ���������С��Ӿ���
		RotatedRect minrect = minAreaRect(contours[t]);
		Point2f P[4];
		minrect.points(P);
		for (int i = 0; i < 4; i++) cv::line(dstMat, P[i], P[i < 3 ? i + 1 : 0], color[t%5], 1, CV_AA);
		//cout <<P<< endl;//��Ӿ��γߴ�

		//����������ܳ���Բ�ζȡ��ܾ���
		area = contourArea(contours[t]);//���
		len = arcLength(contours[t], true);//�ܳ�
		roundness = (4 * CV_PI * area) / (len * len);//Բ�ζ�
		if(roundness>0.55&& area>100) //Բ�ζȴ���0.6 �򻭳�����
			drawContours(srcMat, contours, static_cast<int>(t), Scalar(0, 255, 0), CV_FILLED);
		cout << "Բ�ζȣ�" << area << endl;
		
	}
	//��ʾ���
	namedWindow("����ͼ", WINDOW_AUTOSIZE);
	imshow("����ͼ", dstMat);
	namedWindow("���ԭͼ", WINDOW_AUTOSIZE);
	imshow("���ԭͼ", srcMat);
	//drawContours(dstMat, contours, -1, Scalar(0, 255, 0), 1);
	//findContours(binMat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//CV_RETR_EXTERNAL:ֻ�����Χ���� CV_RETR_LIST �����������������Χ��Χ
	//CV_CHAIN_APPROX_NONE����߽������������������㵽contours������
	//CV_CHAIN_APPROX_SIMPLE������յ�
	//			��ֵͼ������������������Ѱ��ģʽ�����Ʒ���
	//drawContours(dstMat, contours, -1, Scalar(0, 255, 255), 1, 8);//����;
	
	waitKey(0);
}

void test8_2()
{
	int i;
	//vector<vector<Point>> contours;//m��<n�������>
	//��0�У���һ��������n������㣩��[x1,y1],[x2,y2]... �����͵ĵ� --n����
	//��1��...
	//vector<Vec4i> hierarchy;//Vec4i:n��<4�����ͱ���>   ������
	//��������ļ̳й�ϵ int��[a1,b1,c1,d1],[a2,b2,c2,d2]...

	//vector<Rect>����width*height from λ��(x,y)
	//[width x height from(x,y)]

	//vector<RotatedRect>
	//angle:0 center:[0,0] size:[0 x 0]
	srcMat = imread("D://image/timg8.jpg");
	namedWindow("ԭͼ", WINDOW_AUTOSIZE);
	imshow("ԭͼ", srcMat);
	gryMat = imread("D://image/timg8.jpg", 0);//�Ҷ�ͼ
	//��ֵ��
	Mat binary_image;
	threshold(gryMat, binary_image, 100, 255, THRESH_BINARY);
	imshow("binary", binary_image);
	//������������
	vector<vector<Point>> contours;//m��<n�������>
	vector<Vec4i> hireachy;//Vec4i:n��<4�����ͱ���>   ������
	//д��һ
	//findContours(binMat, contours,hireachy, RETR_TREE, CHAIN_APPROX_NONE);
	//for (int i = 0; i < contours.size(); i++) {
	//    drawContours(dst, contours, i, Scalar(0, 0, 255), 1, 8, hierarchy);
	//}
	//д����
	findContours(binary_image, contours, RETR_TREE, CHAIN_APPROX_NONE);
	//������ͼ ����
	Mat dstMat = Mat(srcMat.rows, srcMat.cols, CV_8UC3, Scalar(0, 0, 0));
	//��ɫ���ò�ͬ��ɫ�����ҵ��Ĳ�ͬ����
	Scalar color[] = { Scalar(0,0,255), Scalar(0,255,0), Scalar(255,0,0), Scalar(255,255,0) ,Scalar(255,0,255) };
	//��ʼ���ܳ��������Բ�ζȡ��ܾ���
	double len = 0, area = 0, roundness = 0, lenratio = 0;
	cout << contours.size() << endl;
	//ѭ���ҳ����з�������������
	for (int t = 0; t < contours.size(); t++)
	{
		//���������˵�С�ĸ�������
		Rect rect = boundingRect(contours[t]);//��������
		if (rect.width < 10)
			continue;
		//�����ҵ�����������t��
		drawContours(dstMat, contours, static_cast<int>(t), Scalar(255, 255, 255), 1, 8, hireachy);

		//���Ƹ���������С��Ӿ���
		RotatedRect minrect = minAreaRect(contours[t]);
		Point2f P[4];
		minrect.points(P);
		for (int i = 0; i < 4; i++) cv::line(dstMat, P[i], P[i < 3 ? i + 1 : 0], color[t % 5], 1, CV_AA);
		//cout <<P<< endl;//��Ӿ��γߴ�

		//����������ܳ���Բ�ζȡ��ܾ���
		area = contourArea(contours[t]);//���
		len = arcLength(contours[t], true);//�ܳ�
		roundness = (4 * CV_PI * area) / (len * len);//Բ�ζ�
		if ( area > 100) //Բ�ζȴ���0.6 �򻭳�����
			drawContours(srcMat, contours, static_cast<int>(t), Scalar(0, 255, 0), CV_FILLED);
		cout << "Բ�ζȣ�" << area << endl;

	}
	//��ʾ���
	namedWindow("����ͼ", WINDOW_AUTOSIZE);
	imshow("����ͼ", dstMat);
	namedWindow("���ԭͼ", WINDOW_AUTOSIZE);
	imshow("���ԭͼ", srcMat);
	//drawContours(dstMat, contours, -1, Scalar(0, 255, 0), 1);
	//findContours(binMat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//CV_RETR_EXTERNAL:ֻ�����Χ���� CV_RETR_LIST �����������������Χ��Χ
	//CV_CHAIN_APPROX_NONE����߽������������������㵽contours������
	//CV_CHAIN_APPROX_SIMPLE������յ�
	//			��ֵͼ������������������Ѱ��ģʽ�����Ʒ���
	//drawContours(dstMat, contours, -1, Scalar(0, 255, 255), 1, 8);//����;

	waitKey(0);
}
int main()
{
	test8_2();
	return 0;
}