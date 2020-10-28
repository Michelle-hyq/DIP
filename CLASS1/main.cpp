#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// 课堂四
//--------------------------------------------------------------------------------------------//
//练习1 图形形态学 先二值化 后分别腐蚀、膨胀、开运算、闭运算
#define ERODE_NUM 0
#define DILATE_NUM 1
#define OPEN_NUM 2
#define CLOSE_NUM 3
int ele_size = 3;
int trackNum = ERODE_NUM;//0腐蚀1膨胀2开运算3闭运算
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

	namedWindow("效果图", WINDOW_NORMAL);
	imshow("效果图", proOut);
}
void TrackNumChange(int, void*){ process(); }
void eleSizeChange(int, void*) { process(); }
void test4_1()
{
	srcMat = imread("D://image/timg3.jpg", 0);
	threshold(srcMat, dstMat, 100, 255, THRESH_BINARY);
	namedWindow("二值化", WINDOW_NORMAL);
	imshow("二值化", dstMat);
	process();
	/*
	createTrackbar(
		"threshold",//轨迹条的名字
		window_name,//轨迹条依附的窗口名字
		&lowTH,		//指向整型的指针，表示滑块的位置，在创建时，滑块的初始位置就是该变量的初始值
		maxTH,		//滑块可以到达的最大位置的值
		threshold_Mat,//指向回调函数的指针
		&gryMat		//用户传给回调函数的数据
	);/**/
	createTrackbar("腐膨开闭", "效果图", &trackNum, 3, TrackNumChange);//0腐蚀1膨胀2开运算3闭运算
	createTrackbar("内核尺寸", "效果图", &ele_size, 21, eleSizeChange);
	waitKey(0);
} 
//----------------------------------------------------------------------------------------------//
//练习2 连通域标记
void test4_2()//
{
	srcMat = imread("D://image/timg3.jpg", 0);
	cv::Mat src_img, labels, stats, centroids, img_color, img_gray;
	threshold(srcMat, dstMat, 100, 255, THRESH_BINARY);
	namedWindow("二值化", WINDOW_NORMAL);
	imshow("二值化", dstMat);
	int nccomps = cv::connectedComponentsWithStats(
		dstMat, //二值图像
		labels,     //和原图一样大的标记图
		stats, //nccomps×5的矩阵 表示每个连通区域的外接矩形和面积（pixel）
		centroids //nccomps×2的矩阵 表示每个连通区域的质心
	);
	printf("原图中连通区域数：%d\n", nccomps);//连通域包含背景
	waitKey(0);
}
//----------------------------------------------------------------------------------//
//练习3 大律法分割下图，并对其进行连通域标记，利用图像形态学中所学的知识实现自动计算原点个数。
void test4_3()
{
	int ele_size2 = 9;
	srcMat = imread("D://image/timg4.jpg", 0);
	cv::Mat  labels, stats, centroids,out_res;
	threshold(srcMat, dstMat, 100, 255, THRESH_BINARY);//大律法
	namedWindow("二值化", WINDOW_NORMAL);
	imshow("二值化", dstMat);
	element = getStructuringElement(MORPH_RECT, Size(2 * ele_size2 + 1, 2 * ele_size2 + 1), Point(ele_size2, ele_size2));
	dilate(dstMat, proOut, element);//先膨胀后腐蚀
	erode(proOut, proOut, element);
	namedWindow("腐蚀", WINDOW_NORMAL);
	imshow("腐蚀", proOut);
	bitwise_not(proOut, proOut);//反二值化
	int nccomps = cv::connectedComponentsWithStats(
		proOut, //二值图像
		labels,     //和原图一样大的标记图
		stats, //nccomps×5的矩阵 表示每个连通区域的外接矩形和面积（pixel）
		centroids //nccomps×2的矩阵 表示每个连通区域的质心
	);
	cout << "检测到原点个数: " << nccomps-1 << endl;//原点个数(不包含背景)
	//printf("连通区域数：%d\n", nccomps);//连通域包含背景
	waitKey(0);
}
//----------------------------------------------------------------------------------------//
//练习4 回形针进行自动计数 思路：过滤掉连通域较小的区域
void test4_4()
{
	int ele_size3 = 1;
	srcMat = imread("D://image/timg5.jpg", 0);
	cv::Mat  labels, stats, centroids, out_res;
	threshold(srcMat, dstMat, 100, 255, THRESH_BINARY);//
	namedWindow("二值化", WINDOW_NORMAL);
	imshow("二值化", dstMat);
	element = getStructuringElement(MORPH_RECT, Size(2 * ele_size3 + 1, 2 * ele_size3 + 1), Point(ele_size3, ele_size3));
	bitwise_not(dstMat, proOut);
	namedWindow("反二值化", WINDOW_NORMAL);
	imshow("反二值化", proOut);
	/*//轮廓尝试
	vector<vector<Point>> contours;
	Mat resultImg = Mat::zeros(proOut.size(), CV_8UC3);//相当于创建一张黑色的图，每个像素的每个通道都为0,Scalar(0,0,0)；
	RNG rng((int)time(0));//创建一个随机数rng
	findContours(proOut, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(-1, -1));//寻找轮廓
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));//得到随机颜色
		drawContours(resultImg, contours, static_cast<int>(i), color, -1, 8, Mat(), 0, Point(-1, -1));//将轮廓填充为随机色
	}
	//namedWindow("Final image", WINDOW_NORMAL);
	//imshow("Final image", resultImg);
	/**/
	int nccomps = cv::connectedComponentsWithStats(
		proOut, //二值图像
		labels,     //和原图一样大的标记图
		stats, //nccomps×5的矩阵 表示每个连通区域的外接矩形和面积（pixel）
		centroids //nccomps×2的矩阵 表示每个连通区域的质心
	);
	int invalid_num = 1;//无效连通域 初始值为1（背景）
	for (int i = 1; i < nccomps; i++)
	{
		int area = stats.at<int>(i, CC_STAT_AREA);
		cout << "count:" << i << "  area:" << area << endl;
		if (area < 6000||area>8000) invalid_num++;
	}
	cout << "检测到回形针个数: " << nccomps - invalid_num << endl;
	//printf("连通区域数：%d\n", nccomps);//连通域包含背景
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
	namedWindow("二值化", WINDOW_NORMAL);
	imshow("二值化", dstMat);
	//获取腐蚀自定义核
	cv::Mat erd_element = getStructuringElement(MORPH_RECT, Size(2*ele_size+1, 2*ele_size+1));
	cv::Mat erdOut;
	erode(dstMat, erdOut, erd_element);
	namedWindow("腐蚀", WINDOW_NORMAL);
	imshow("腐蚀", erdOut);
	//获取膨胀自定义核
	cv::Mat dlt_element = getStructuringElement(MORPH_RECT, Size(2*ele_size+1, 2*ele_size+1));
	cv::Mat dltOut;
	dilate(dstMat, dltOut, dlt_element);
	namedWindow("膨胀", WINDOW_NORMAL);
	imshow("膨胀", dltOut);
	//开运算，先腐蚀后膨胀
	cv::Mat open_element = getStructuringElement(MORPH_RECT, Size(2*ele_size+1, 2*ele_size+1));
	cv::Mat openOut;
	morphologyEx(dstMat, openOut, MORPH_OPEN, open_element);
	namedWindow("开运算-调用函数", WINDOW_NORMAL);
	imshow("开运算-调用函数", openOut);
	//闭运算，先膨胀后腐蚀
	cv::Mat close_element = getStructuringElement(MORPH_RECT, Size(2*ele_size+1, 2*ele_size+1));
	cv::Mat closeOut;
	morphologyEx(dstMat, closeOut, MORPH_CLOSE, close_element);
	namedWindow("闭运算-调用函数", WINDOW_NORMAL);
	imshow("闭运算-调用函数", closeOut);
	waitKey(0);
}/**/