#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// ������
cv::Mat frame;
cv::Mat frame_out;
VideoCapture cap;
//--------------------------------------------------------------------------------------------//
//��ϰ1 Canny��Ե��ȡ ��ȡ����ͷͼ�񣬲�������ͷͼ����б�Ե��ȡ�ֱ���ȡx��y�����ϵı�Ե���۲����к�����
int Canny_size = 3;
cv::Mat gray,edge;
void Canny_proc() {
	bool rSucess = cap.read(frame);
	if (!rSucess) {
		std::cout << "���ܴ���Ƶ�ļ���ȡ֡" << std::endl;
	}
	namedWindow("��Եԭͼ", WINDOW_AUTOSIZE);
	imshow("��Եԭͼ", frame);
	

	frame_out.create(frame.size(), frame.type());
	cvtColor(frame, gray, CV_BGR2GRAY);//�Ҷ�ͼ
	blur(gray, edge, Size(3, 3));//3x3��ֵ�˲�����
	//canny��Ե��ȡ
	if (Canny_size == 0) Canny_size = 1;
	Canny(edge, frame_out, 3,9, Canny_size*2+1);//��һ���ͺ�����ֵ���ڶ�������
	namedWindow("canny", WINDOW_AUTOSIZE);
	imshow("canny", frame_out);
	waitKey(30);
}
void Canny_fil_sizeChange(int, void*)
{
	Canny_proc();
}
void test6_1()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "���ܴ���Ƶ�ļ�" << std::endl;
	}
	Canny_proc();
	createTrackbar("��Ե�ں�", "��Եԭͼ", &Canny_size, 3, Canny_fil_sizeChange);
	while (1)
	{
		Canny_proc();
		createTrackbar("��Ե�ں�", "��Եԭͼ", &Canny_size, 3, Canny_fil_sizeChange);
	}
}
//--------------------------------------------------------------------------------------------//
//��ϰ2 ��ת������
void test6_2()
{
	Mat dst, src;
	src= imread("D://image/timg6.jpg", 1);
	//if (src.empty()) return -1;
	//��ת-10�� ���ų߶�Ϊ
	float angle = -10.0, scale = 0.8;
	//��ת����Ϊͼ������
	cv::Point2f center(src.cols * 0.5, src.rows * 0.5);
	//��ñ任����
	const cv::Mat affine_matrix = cv::getRotationMatrix2D(center, angle, scale);
	cv::warpAffine(src, dst, affine_matrix, src.size());
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", src);
	namedWindow("dst", WINDOW_AUTOSIZE);
	imshow("dst", dst);
	waitKey(0);
}
//--------------------------------------------------------------------------------------------//
//��ϰ3 ����任
void test6_3()
{
	Mat dst, src;
	src = imread("D://image/timg6.jpg", 1);
	//if (src.empty()) return -1;
	//�任ǰ����������
	const cv::Point2f src_pt[] = {
		cv::Point2f(200, 200),
		cv::Point2f(250, 200),
		cv::Point2f(200, 100)
	};
	//�任�����������
	const cv::Point2f dst_pt[] = {
		cv::Point2f(300, 100),
		cv::Point2f(300, 50),
		cv::Point2f(200, 100)
	};/**///(314,416)
	/*const cv::Point2f src_pt[] = {
		cv::Point2f(32, 0),
		cv::Point2f(190, 315),
		cv::Point2f(0, 176)
	};
	//�任�����������
	const cv::Point2f dst_pt[] = {
		cv::Point2f(0, 0),
		cv::Point2f(313, 315),
		cv::Point2f(0, 315)
	};/**/
	//�������任
	const cv::Mat affine_matrix = cv::getAffineTransform(src_pt, dst_pt);
	cv::warpAffine(src, dst, affine_matrix, src.size());
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", src);
	namedWindow("dst", WINDOW_AUTOSIZE);
	imshow("dst", dst);
	waitKey(0);
}
//--------------------------------------------------------------------------------------------//
//��ϰ4 ͶӰ�任
void test6_4()
{
	Mat dst, src;
	src = imread("D://image/timg6.jpg", 1);
	//if (src.empty()) return -1;
	//�任ǰ���ĵ�����
	const cv::Point2f pts1[] = {
		cv::Point2f(150, 150),
		cv::Point2f(150, 300),
		cv::Point2f(350, 300),
		cv::Point2f(350, 150)
	};
	//�任����ĵ�����
	const cv::Point2f pts2[] = {
		cv::Point2f(200, 150),
		cv::Point2f(200, 300),
		cv::Point2f(340, 270),
		cv::Point2f(340, 180)
	};/**/
	/*const cv::Point2f pts1[] = {
		cv::Point2f(32, 0),
		cv::Point2f(190, 315),
		cv::Point2f(0, 176),
		cv::Point2f(313, 33)
	};
	//�任�����������
	const cv::Point2f pts2[] = {
		cv::Point2f(0, 0),
		cv::Point2f(313, 315),
		cv::Point2f(0, 315),
		cv::Point2f(313, 0)
	};/**/
	//͸�ӱ任���м���
	cv::Mat perspective_matrix = cv::getPerspectiveTransform(pts1, pts2);
	//�任
	cv::warpPerspective(src, dst, perspective_matrix, src.size());
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", src);
	namedWindow("dst", WINDOW_AUTOSIZE);
	imshow("dst", dst);
	waitKey(0);
}
//--------------------------------------------------------------------------------------------//
//��ϰ5 ͼ�����
void test6_5()
{
	Mat dst, src;
	src = imread("D://image/timg6.jpg", 0);
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", src);
	//if (src.empty()) return -1;
	int width, height, channels, p_sum,i,j;
	int color_temp, pt_cnt = 0;
	height = src.rows;  //��314��316��
	width = src.cols;
	p_sum = width * height;
	const cv::Point2f p1,p2,p3,p4;
	int yuzhi = 248;
	float x1, x2, x3, x4, y1, y2, y3, y4;
	printf("(width,height)=(%d,%d)\n", width, height);
	for (i = 0; i < height; i++) {//������  �϶�������
		uchar* data = src.ptr<uchar>(i);//��i�� ������
		for (j = 0; j < width; j++) {	//��j�� ������
			color_temp = data[j];
			//printf("gray1[%d]=%d\n",j, color_temp);
			if (color_temp < yuzhi) {
				pt_cnt++;
				printf("(x,y)=(%d,%d)\n", j,i);
				printf("gray1=%d\n", color_temp);
				x1 = j;
				y1 = i;
				//cv::Point2f p1(j, i);
				break;//(32,0)
			}
		}
		if(pt_cnt==1) break;
	}
	for (i = height-1; i >0; i--) {//�¶��� 
		uchar* data = src.ptr<uchar>(i);//��i�� ������
		for (j = 0; j < width; j++) {	//��j�� ������
			color_temp = data[j];
			//printf("gray2[%d]=%d\n", j, color_temp);
			if (color_temp < yuzhi) {
				pt_cnt++;
				printf("(x,y)=(%d,%d)\n", j, i);
				printf("gray2=%d\n", color_temp);
				//cv::Point2f p2(j, i);
				x2 = j;
				y2 = i;
				break;//(190,315)
			}
		}
		if (pt_cnt == 2) break;
	}
	for (j = 0; j < width; j++) {//�󶥵�
		for (i = 0; i < height; i++) {
			uchar* data = src.ptr<uchar>(i);
			color_temp = data[j];
			if (color_temp < yuzhi) {
				pt_cnt++;
				printf("(x,y)=(%d,%d)\n", j, i);
				printf("gray3=%d\n", color_temp);
				//cv::Point2f p3(j, i);
				x3 = j;
				y3 = i;
				break;//(190,315)
			}
		}
		if (pt_cnt == 3) break;
	}
	/*for (j = width-1; j >0; j--) {//�Ҷ���
		for (i = 0; i < height; i++) {
			uchar* data = src.ptr<uchar>(i);
			color_temp = data[j];
			if (color_temp < yuzhi) {
				pt_cnt++;
				printf("(x,y)=(%d,%d)\n", j, i);
				printf("gray4=%d\n", color_temp);
				//cv::Point2f p4(j, i);
				x4 = j;
				y4 = i;
				break;//(190,315)
			}
		}
		if (pt_cnt == 4) break;
	}/**/
	const cv::Point2f pts1[] = {
		//p1,p2,p3
		cv::Point2f(x1, y1),
		cv::Point2f(x2, y2),
		cv::Point2f(x3, y3)/**/
	};
	const cv::Point2f pts2[] = {
		cv::Point2f(0, 0),//����
		cv::Point2f(width-1, height-1),//����
		cv::Point2f(0, height-1)//����
		//cv::Point2f(width-1, 0)//����
	};

	Mat srcMat = imread("D://image/timg6.jpg",1);
	const cv::Mat affine_matrix = cv::getAffineTransform(pts1, pts2);
	cv::warpAffine(srcMat, dst, affine_matrix, srcMat.size());
	namedWindow("srcMat", WINDOW_AUTOSIZE);
	imshow("srcMat", srcMat);
	namedWindow("dst", WINDOW_AUTOSIZE);
	imshow("dst", dst);
	waitKey(0);
}
//--------------------------------------------------------------------------------------------//
//˼���⣺ͨ��OpenCV����ͼ�����ת�󣬳���ԭ�ߴ�Ĳ��֣��ᱻ�Զ��ü������ʵ��
//���Զ��ü���ͼ����ת
void test6_6()
{
	Mat dst, src;
	src = imread("D://image/timg6.jpg", 0);
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", src);
	//if (src.empty()) return -1;
	int width, height, channels, p_sum, i, j;
	int color_temp, pt_cnt = 0;
	height = src.rows;  //��314��316��
	width = src.cols;
	p_sum = width * height;
	const cv::Point2f p1, p2, p3, p4;
	int yuzhi = 248;
	float x1, x2, x3, x4, y1, y2, y3, y4;
	printf("(width,height)=(%d,%d)\n", width, height);
	for (i = 0; i < height; i++) {//������  �϶�������
		uchar* data = src.ptr<uchar>(i);//��i�� ������
		for (j = 0; j < width; j++) {	//��j�� ������
			color_temp = data[j];
			//printf("gray1[%d]=%d\n",j, color_temp);
			if (color_temp < yuzhi) {
				pt_cnt++;
				printf("(x,y)=(%d,%d)\n", j, i);
				printf("gray1=%d\n", color_temp);
				x1 = j;
				y1 = i;
				//cv::Point2f p1(j, i);
				break;//(32,0)
			}
		}
		if (pt_cnt == 1) break;
	}
	for (i = height - 1; i > 0; i--) {//�¶��� 
		uchar* data = src.ptr<uchar>(i);//��i�� ������
		for (j = 0; j < width; j++) {	//��j�� ������
			color_temp = data[j];
			//printf("gray2[%d]=%d\n", j, color_temp);
			if (color_temp < yuzhi) {
				pt_cnt++;
				printf("(x,y)=(%d,%d)\n", j, i);
				printf("gray2=%d\n", color_temp);
				//cv::Point2f p2(j, i);
				x2 = j;
				y2 = i;
				break;//(190,315)
			}
		}
		if (pt_cnt == 2) break;
	}
	for (j = 0; j < width; j++) {//�󶥵�
		for (i = 0; i < height; i++) {
			uchar* data = src.ptr<uchar>(i);
			color_temp = data[j];
			if (color_temp < yuzhi) {
				pt_cnt++;
				printf("(x,y)=(%d,%d)\n", j, i);
				printf("gray3=%d\n", color_temp);
				//cv::Point2f p3(j, i);
				x3 = j;
				y3 = i;
				break;//(190,315)
			}
		}
		if (pt_cnt == 3) break;
	}
	const cv::Point2f pts1[] = {
		//p1,p2,p3
		cv::Point2f(x1, y1),
		cv::Point2f(x2, y2),
		cv::Point2f(x3, y3)/**/
	};
	const cv::Point2f pts2[] = {
		cv::Point2f(0, 0),//����
		cv::Point2f(width - 1, height - 1),//����
		cv::Point2f(0, height - 1)//����
		//cv::Point2f(width-1, 0)//����
	};

	Mat srcMat = imread("D://image/timg6.jpg", 1);
	const cv::Mat affine_matrix = cv::getAffineTransform(pts1, pts2);
	cv::warpAffine(srcMat, dst, affine_matrix, srcMat.size());

	Mat dstMat;
	float angle = -10.0;
	float ab_angle = 10.0;

	float scale = abs((width/ height*sin(ab_angle)+cos(ab_angle)));
	printf("(scale=%f)\n", scale);
    //scale = 1;
	//��ת����Ϊͼ������
	cv::Point2f center(src.cols * 0.5, src.rows * 0.5);
	//��ñ任����
	const cv::Mat affine_matrix1 = cv::getRotationMatrix2D(center, angle, scale);
	cv::warpAffine(dst, dstMat, affine_matrix1, src.size());
	namedWindow("dst", WINDOW_AUTOSIZE);
	imshow("dst", dst);
	namedWindow("dstMat", WINDOW_AUTOSIZE);
	imshow("dstMat", dstMat);
	waitKey(0);
}
int main()
{
	test6_6();
	return 0;
}