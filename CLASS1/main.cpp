#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// 课堂六
cv::Mat frame;
cv::Mat frame_out;
VideoCapture cap;
//--------------------------------------------------------------------------------------------//
//练习1 Canny边缘提取 读取摄像头图像，并对摄像头图像进行边缘提取分别提取x，y方向上的边缘，观察结果有何区别。
int Canny_size = 3;
cv::Mat gray,edge;
void Canny_proc() {
	bool rSucess = cap.read(frame);
	if (!rSucess) {
		std::cout << "不能从视频文件读取帧" << std::endl;
	}
	namedWindow("边缘原图", WINDOW_AUTOSIZE);
	imshow("边缘原图", frame);
	

	frame_out.create(frame.size(), frame.type());
	cvtColor(frame, gray, CV_BGR2GRAY);//灰度图
	blur(gray, edge, Size(3, 3));//3x3均值滤波降噪
	//canny边缘提取
	if (Canny_size == 0) Canny_size = 1;
	Canny(edge, frame_out, 3,9, Canny_size*2+1);//第一个滞后性阈值，第二个。。
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
		std::cout << "不能打开视频文件" << std::endl;
	}
	Canny_proc();
	createTrackbar("边缘内核", "边缘原图", &Canny_size, 3, Canny_fil_sizeChange);
	while (1)
	{
		Canny_proc();
		createTrackbar("边缘内核", "边缘原图", &Canny_size, 3, Canny_fil_sizeChange);
	}
}
//--------------------------------------------------------------------------------------------//
//练习2 旋转及缩放
void test6_2()
{
	Mat dst, src;
	src= imread("D://image/timg6.jpg", 1);
	//if (src.empty()) return -1;
	//旋转-10° 缩放尺度为
	float angle = -10.0, scale = 0.8;
	//旋转中心为图像中心
	cv::Point2f center(src.cols * 0.5, src.rows * 0.5);
	//获得变换矩阵
	const cv::Mat affine_matrix = cv::getRotationMatrix2D(center, angle, scale);
	cv::warpAffine(src, dst, affine_matrix, src.size());
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", src);
	namedWindow("dst", WINDOW_AUTOSIZE);
	imshow("dst", dst);
	waitKey(0);
}
//--------------------------------------------------------------------------------------------//
//练习3 仿射变换
void test6_3()
{
	Mat dst, src;
	src = imread("D://image/timg6.jpg", 1);
	//if (src.empty()) return -1;
	//变换前的三点坐标
	const cv::Point2f src_pt[] = {
		cv::Point2f(200, 200),
		cv::Point2f(250, 200),
		cv::Point2f(200, 100)
	};
	//变换后的三点坐标
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
	//变换后的三点坐标
	const cv::Point2f dst_pt[] = {
		cv::Point2f(0, 0),
		cv::Point2f(313, 315),
		cv::Point2f(0, 315)
	};/**/
	//计算仿射变换
	const cv::Mat affine_matrix = cv::getAffineTransform(src_pt, dst_pt);
	cv::warpAffine(src, dst, affine_matrix, src.size());
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", src);
	namedWindow("dst", WINDOW_AUTOSIZE);
	imshow("dst", dst);
	waitKey(0);
}
//--------------------------------------------------------------------------------------------//
//练习4 投影变换
void test6_4()
{
	Mat dst, src;
	src = imread("D://image/timg6.jpg", 1);
	//if (src.empty()) return -1;
	//变换前的四点坐标
	const cv::Point2f pts1[] = {
		cv::Point2f(150, 150),
		cv::Point2f(150, 300),
		cv::Point2f(350, 300),
		cv::Point2f(350, 150)
	};
	//变换后的四点坐标
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
	//变换后的三点坐标
	const cv::Point2f pts2[] = {
		cv::Point2f(0, 0),
		cv::Point2f(313, 315),
		cv::Point2f(0, 315),
		cv::Point2f(313, 0)
	};/**/
	//透视变换行列计算
	cv::Mat perspective_matrix = cv::getPerspectiveTransform(pts1, pts2);
	//变换
	cv::warpPerspective(src, dst, perspective_matrix, src.size());
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", src);
	namedWindow("dst", WINDOW_AUTOSIZE);
	imshow("dst", dst);
	waitKey(0);
}
//--------------------------------------------------------------------------------------------//
//练习5 图像矫正
void test6_5()
{
	Mat dst, src;
	src = imread("D://image/timg6.jpg", 0);
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", src);
	//if (src.empty()) return -1;
	int width, height, channels, p_sum,i,j;
	int color_temp, pt_cnt = 0;
	height = src.rows;  //（314，316）
	width = src.cols;
	p_sum = width * height;
	const cv::Point2f p1,p2,p3,p4;
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
	for (i = height-1; i >0; i--) {//下顶点 
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
	/*for (j = width-1; j >0; j--) {//右顶点
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
		cv::Point2f(0, 0),//左上
		cv::Point2f(width-1, height-1),//右下
		cv::Point2f(0, height-1)//左下
		//cv::Point2f(width-1, 0)//右上
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
//思考题：通过OpenCV进行图像的旋转后，超出原尺寸的部分，会被自动裁剪，如何实现
//不自动裁剪的图像旋转
void test6_6()
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

	float scale = abs((width/ height*sin(ab_angle)+cos(ab_angle)));
	printf("(scale=%f)\n", scale);
    //scale = 1;
	//旋转中心为图像中心
	cv::Point2f center(src.cols * 0.5, src.rows * 0.5);
	//获得变换矩阵
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