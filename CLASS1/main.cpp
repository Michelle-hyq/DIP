#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// 课堂七
cv::Mat frame;
cv::Mat frame_out;
VideoCapture cap;
//--------------------------------------------------------------------------------------------//
//练习1：通过OpenCV进行图像的旋转后，超出原尺寸的部分，会被自动裁剪，如何实现
//不自动裁剪的图像旋转
void test7_1()
{
	Mat dst, src;
	src = imread("D://image/timg6.jpg", 0);
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", src);
	//if (src.empty()) return -1;
	int width, height, channels, p_sum, i, j;
	int color_temp, pt_cnt = 0;
	height = src.rows;  //（314，316）
	width = src.cols;
	p_sum = width * height;
	const cv::Point2f p1, p2, p3, p4;
	int yuzhi = 248;
	float x1, x2, x3, x4, y1, y2, y3, y4;
	printf("(width,height)=(%d,%d)\n", width, height);
	for (i = 0; i < height; i++) {//从左到右  上顶点坐标
		uchar* data = src.ptr<uchar>(i);//第i行 纵坐标
		for (j = 0; j < width; j++) {	//第j列 横坐标
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
	for (i = height - 1; i > 0; i--) {//下顶点 
		uchar* data = src.ptr<uchar>(i);//第i行 纵坐标
		for (j = 0; j < width; j++) {	//第j列 横坐标
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
	for (j = 0; j < width; j++) {//左顶点
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
		cv::Point2f(0, 0),//左上
		cv::Point2f(width - 1, height - 1),//右下
		cv::Point2f(0, height - 1)//左下
		//cv::Point2f(width-1, 0)//右上
	};

	Mat srcMat = imread("D://image/timg6.jpg", 1);
	const cv::Mat affine_matrix = cv::getAffineTransform(pts1, pts2);
	cv::warpAffine(srcMat, dst, affine_matrix, srcMat.size());

	Mat dstMat;
	float angle = -10.0;
	float ab_angle = 10.0;
	float scale = 1;
	cv::Point2f center(src.cols * 0.5, src.rows * 0.5);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, scale);
	//获得外界四边形
	cv::Rect bbox = cv::RotatedRect(center, dst.size(), angle).boundingRect();
	//调整仿射矩阵参数
	rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;
	//仿射变换
	cv::warpAffine(dst, dstMat, rot, bbox.size());
	namedWindow("dst", WINDOW_AUTOSIZE);
	imshow("dst", dst);
	namedWindow("dstMat", WINDOW_AUTOSIZE);
	imshow("dstMat", dstMat);
	waitKey(0);
}
void test7_2()
{
	Mat dst, src,temp;
	Point p_start,p_end;
	src = imread("D://image/timg7.jpg",0);
	//Canny(src, temp, 50, 200, 3);
	namedWindow("temp", WINDOW_AUTOSIZE);
	imshow("temp", temp);
	cvtColor(temp, dst, CV_GRAY2BGR);//转化边缘检测后的图为灰度图
	//霍夫线变换
	vector<	Vec2f > lines;
	HoughLines(temp, lines, 1, CV_PI / 180, 250);
	//绘制出每条线段
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));//返回与参数最接近的整数
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 + 1000 * (-b));
		pt2.y = cvRound(y0 + 1000 * (a));
		line(dst, pt1, pt2, Scalar(55, 100, 196), 1, LINE_AA);
		//line(src, pt1, pt2, Scalar(255, 255, 255), 1, LINE_AA);
		/*if (i == 0) { p_start.x = pt1.x; p_start.y = pt1.y; }
		else if(i== lines.size()) { p_end.x = pt2.x; p_end.y = pt2.y; }/**/
	}
	
	namedWindow("原始图", WINDOW_AUTOSIZE);
	imshow("原始图", src);
	namedWindow("效果图", WINDOW_AUTOSIZE);
	imshow("效果图", dst);
	waitKey(0);
}
int main()
{
	test7_2();
	return 0;
}