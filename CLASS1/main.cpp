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
	cvtColor(srcMat, gryMat, COLOR_BGR2GRAY);//�Ҷ�ͼ
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
	int ele_size = 2;
	Mat proOut;
	srcMat = imread("D://image/timg8.jpg");
	cvtColor(srcMat, gryMat, COLOR_BGR2GRAY);//�Ҷ�ͼ
	//��Ե�˲�
	/*Canny(srcMat, canny_img, canny_d, 110, 3);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(canny_img, canny_img, MORPH_CLOSE, kernel);
	imshow("��Ե�˲�", canny_img);/**/
	//������
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * ele_size + 1, 2 * ele_size + 1), Point(ele_size, ele_size));
	erode(gryMat, proOut, element);
	namedWindow("��ʴ", WINDOW_AUTOSIZE);
	imshow("��ʴ", proOut);
	//��ֵ��
	Mat binary_image;
	threshold(proOut, binary_image, 100, 255, THRESH_BINARY);
	namedWindow("��ֵ��", WINDOW_AUTOSIZE);
	imshow("��ֵ��", binary_image);
	//������������
	vector<vector<Point>> contours;//m��<n�������>
	vector<Vec4i> hireachy;//Vec4i:n��<4�����ͱ���>   ������
	//д����
	findContours(binary_image, contours, RETR_TREE, CHAIN_APPROX_NONE);
	cout << contours.size() << endl;
	//��ɫ���ò�ͬ��ɫ�����ҵ��Ĳ�ͬ����
	Scalar color[] = { Scalar(0,0,255), Scalar(0,255,0), Scalar(255,0,0), Scalar(255,255,0) ,Scalar(255,0,255) };
	//ѭ���ҳ����з�������������
	for (int t = 0; t < contours.size(); t++)
	{
		//���������˵�С�ĸ�������
		Rect rect = boundingRect(contours[t]);//��������
		cout << rect << endl;
		if (rect.width > 50&& rect.width <100) {
			//�����ҵ�����������t��
			//drawContours(dstMat, contours, static_cast<int>(t), Scalar(255, 255, 255), 1, 8, hireachy);

			//���Ƹ���������С��Ӿ���
			RotatedRect minrect = minAreaRect(contours[t]);
			Point2f P[4];
			minrect.points(P);
			for (int i = 0; i < 4; i++) cv::line(srcMat, P[i], P[i < 3 ? i + 1 : 0], color[t % 5], 1, CV_AA);
		}
	}

	namedWindow("���ԭͼ", WINDOW_AUTOSIZE);
	imshow("���ԭͼ", srcMat);
	waitKey(0);
}
Mat colorFilter(CvMat* inputImage, CvMat*& outputImage)
{
	//inputImageΪָ������ͼƬ��ַ��ָ��,outputImage Ϊָ��һ��cvMat��ָ��
	int i, j;
	IplImage* image = cvCreateImage(cvGetSize(inputImage), 8, 3);  //ָ���ͼ���ָ��
	cvGetImage(inputImage, image);
	IplImage* hsv = cvCreateImage(cvGetSize(image), 8, 3);
	cvCvtColor(image, hsv, CV_BGR2HSV);              //BGRת����HSV�ռ�
	int width = hsv->width;
	int height = hsv->height;
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			CvScalar s_hsv = cvGet2D(hsv, i, j);//��ȡ���ص�Ϊ��j, i�����HSV��ֵ 
			/*
				opencv ��H��Χ��0~180����ɫ��H��Χ�����(0~8)��(160,180)
				S�Ǳ��Ͷȣ�һ���Ǵ���һ��ֵ,S���;��ǻ�ɫ���ο�ֵS>80)��
				V�����ȣ����;��Ǻ�ɫ�����߾��ǰ�ɫ(�ο�ֵ220>V>50)��
			*/
			CvScalar s;
			if (!((((s_hsv.val[0] > 0) && (s_hsv.val[0] < 10)) || (s_hsv.val[0] > 120) && (s_hsv.val[0] < 180))&& (s_hsv.val[2] > 70) && (s_hsv.val[2] < 220) && (s_hsv.val[1] > 80) ))
			{
				s.val[0] = 0;
				s.val[1] = 0;
				s.val[2] = 0;
				cvSet2D(hsv, i, j, s);    //����ͼ��s��ֵ������hsvָ����ָ���ͼ��
			}
			else {
				s.val[0] = 255;
				s.val[1] = 255;
				s.val[2] = 255;
				cvSet2D(hsv, i, j, s);    //����ͼ��s��ֵ������hsvָ����ָ���ͼ��
			}
		}
	outputImage = cvCreateMat(hsv->height, hsv->width, CV_8UC3);
	cvConvert(hsv, outputImage);         //��IplImageͼ��hsv��ֵת������outputImage
	Mat output = cvarrToMat(hsv);
	return output;
}
void test8_3()
{
	int i;
	int ele_size = 6;
	Mat hsvMat,image,proOut;

	srcMat = imread("D://image/timg10.jpg");
	cvtColor(srcMat, gryMat,COLOR_BGR2GRAY);
	//gryMat = imread("D://image/timg10.jpg",0);
	//Step1.��ͼƬת��ΪHSV��ɫ�ռ䲢����ͼ��ָ������ɫ����
	//ת��Ϊhsv�ռ䲢��ȡ��ɫ���򣬷��ظ�img_red
	CvMat img_temp = srcMat;
	CvMat* output;
	Mat img_red = colorFilter(&img_temp, output);
	namedWindow("img_red", WINDOW_NORMAL);
	imshow("img_red", img_red);
	cvtColor(img_red, img_red, COLOR_BGR2GRAY);
	//����
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * ele_size + 1, 2 * ele_size + 1), Point(ele_size, ele_size));
	dilate(img_red, proOut, element);//����Ҷ�ͼ
	namedWindow("����", WINDOW_NORMAL);
	imshow("����", proOut);/**/
	//��ֵ��
	Mat binary_image;
	threshold(proOut, binary_image, 130, 255, THRESH_BINARY);//����Ҷ�ͼ
	//adaptiveThreshold(gryMat, binary_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 51, 10);
	namedWindow("��ֵ��", WINDOW_NORMAL);
	imshow("��ֵ��", binary_image);/**/
	//������������
	vector<vector<Point>> contours;//m��<n�������>
	vector<Vec4i> hireachy;//Vec4i:n��<4�����ͱ���>   ������
	//д����
	findContours(binary_image, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	//cout << contours.size() << endl;
	//��ɫ���ò�ͬ��ɫ�����ҵ��Ĳ�ͬ����
	Scalar color[] = { Scalar(0,0,255), Scalar(0,255,0), Scalar(255,0,0), Scalar(255,255,0) ,Scalar(255,0,255) };
	//������ͼ ����
	//Mat dstMat = Mat(srcMat.rows, srcMat.cols, CV_8UC3, Scalar(0, 0, 0));
	//��ʼ���ܳ��������Բ�ζȡ��ܾ���
	double len = 0, area = 0, roundness = 0, lenratio = 0;
	//ѭ���ҳ����з�������������
	for (int t = 0; t < contours.size(); t++)
	{
		//���������˵�С�ĸ�������
		Rect rect = boundingRect(contours[t]);//��������
		//drawContours(dstMat, contours, static_cast<int>(t), Scalar(255, 255, 255), 1, 8, hireachy);
		//cout << rect << endl;
		if (rect.width > 1000) {
			//���Ƹ���������С��Ӿ���
			RotatedRect minrect = minAreaRect(contours[t]);
			Point2f P[4];
			minrect.points(P);
			for (int i = 0; i < 4; i++) cv::line(srcMat, P[i], P[i < 3 ? i + 1 : 0], color[t % 5], 6, CV_AA);
		}
	}/**/
	//namedWindow("���ͼ", WINDOW_NORMAL);
	//imshow("���ͼ", dstMat);
	namedWindow("���ԭͼ", WINDOW_NORMAL);
	imshow("���ԭͼ", srcMat);//**/
	waitKey(0);
}
int main()
{
	test8_1();
	return 0;
}