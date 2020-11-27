#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
//vector<vector<Point>> contours;//m个<n个坐标点>
	//第0行（第一个轮廓的n个坐标点）：[x1,y1],[x2,y2]... 向量型的点 --n个点
	//第1行...
	//vector<Vec4i> hierarchy;//Vec4i:n个<4个整型变量>   的向量
	//输出轮廓的继承关系 int型[a1,b1,c1,d1],[a2,b2,c2,d2]...

	//vector<Rect>像素width*height from 位置(x,y)
	//Rect(x,y,width,height)
	//[width x height from(x,y)]

	//vector<RotatedRect>
	//angle:0 center:[0,0] size:[0 x 0]
//--------------------------------------------------------------------------------------------//
// 课堂九
//角点检测、SIFT、HOG特征检测行人
cv::Mat binMat, gryMat, minRectMat, canny_img,dstMat;
int nX,nY;
int bins = 810;
float ref_hist[810];
float pl_hist[810];
float bg_hist[810];/**/
//输入原图像、直方图、cell面积、cell边长
void calcHOG(cv::Mat src,float* tempHist,int nAngle,int cellSize)
{
	int i, j;
	Rect roi;
	roi.width = cellSize;
	roi.height = cellSize;
	//计算所有像素的梯度与角度方向，统计直方图
	Mat gx, gy;
	Sobel(src, gx, CV_32F, 1, 0, 1);
	Sobel(src, gy, CV_32F, 0, 1, 1);
	
	cv::Mat roiMat;
	cv::Mat roiMag;
	cv::Mat roiAgl;

	//默认是弧度radians，可选择角度degrees
	Mat mag, angle;//每个像素点都有幅值和方向
	cartToPolar(gx, gy, mag, angle, true);
	int cont = 0;
	for (int i = 0; i < nY; i++) {
		for (int j = 0; j < nX; j++) {
			cont++;
			roi.x = j * cellSize;
			roi.y = i * cellSize;
			//赋值图像 ROI感兴趣的区域
			roiMat = src(roi);
			roiMag = mag(roi);
			roiAgl = angle(roi);
			//当前cell第一个像素在数组中的位置
			int head = (i * nX + j) * nAngle;
			//ref_hist += head;
			
			//计算角度直方图
			//cell存储空间信息 不同空间有一个9维直方图
			//第0个-8个 代表梯度角度0 20 40...160 °
			//值表示梯度幅度
			int cnt = 0;
			for (int n = 0; n < roiMat.rows; n++) {
				for (int m = 0; m < roiMat.cols; m++) {
					cnt++;
					float temp_mag = ( roiMag.at<float>(m, n));//像素点的幅度值mag
					int temp_ang = (int)( roiAgl.at<float>(m, n));
					float show_ang = roiAgl.at<float>(m, n);
					//cout << "\temp_ang: " << show_ang << endl;
					
					if (temp_ang % 40 == 0)
						tempHist[head + temp_ang / 40] = temp_mag;
						//*(ref_hist + temp_ang / 40) = temp_mag;
					else if(temp_ang/40 < 9){
						tempHist[head + temp_ang / 40]= (float)(40 - (temp_ang % 40)) / 40.0 * temp_mag;
						tempHist[head + ((temp_ang / 40) == 8 ? (temp_ang / 40 + 1) : 0)] = (float)(temp_ang % 40) / 40 * temp_mag;/**/
						/**(tempHist + temp_ang / 40) = (float)(40-(temp_ang % 40)) / 40.0 * temp_mag;
						*(tempHist + ((temp_ang/40)==8? (temp_ang / 40+1):0)) = (float)(temp_ang % 40) / 40.0 * temp_mag;/**/
					}
					
				}
			}
		}
	}
}
void test9_1()
{
	Mat srcMat, plMat, bgMat;
	srcMat = imread("D://image/timg11.jpg");
	plMat = imread("D://image/timg12.jpg");
	bgMat = imread("D://image/timg13.jpg");
	cvtColor(srcMat, gryMat, COLOR_BGR2GRAY);//灰度图
	int wid = 0;
	int hei = 0;
	wid = srcMat.cols;
	hei = srcMat.rows;
	int area = wid * hei;
	//cout << "area: " << area << endl;
	//深复制
	Mat refMat;
	srcMat.copyTo(refMat);
	//图像划分为16*16的cell
	int blockSize = 16;
	nX = refMat.cols / blockSize;//横方向上有几个cell
	nY = refMat.rows / blockSize;//竖方向上有几个cell

	int nAangle = 9;//180°n等分 0 20 40 160 九等分
	int bins = nX * nY * nAangle;//每个cell需要9bit的直方统计图
	//cout << "bins: " << bins << endl;

	/*float* ref_hist = new float[bins];
	memset(ref_hist, 0, sizeof(float) * bins);
	float* pl_hist = new float[bins];
	memset(pl_hist, 0, sizeof(float) * bins);
	float* bg_hist = new float[bins];
	memset(bg_hist, 0, sizeof(float) * bins);/**/
	/*
	vector<float> hist_ref(bins);
	vector<float> hist_pl(bins);
	vector<float> hist_bg(bins);/**/
	//ref_hist[0] = 9;
	//cout << "\nmagnitude: " << ref_hist[0] << endl;
	int reCode = 0;
	//计算三张输入图片的HOG
	calcHOG(refMat, ref_hist,nAangle, blockSize);//ref_hist直方图
	calcHOG(plMat , pl_hist, nAangle, blockSize);
	calcHOG(bgMat, bg_hist,nAangle, blockSize);

	for (int i = 0; i < 10; i++) printf("%f ", ref_hist[i]);
	printf("\r\n ");
	for (int i = 0; i < 10; i++) printf("%f ", pl_hist[i]);
	printf("\r\n ");
	for (int i = 0; i < 10; i++) printf("%f ", bg_hist[i]);
	printf("\r\n");/**/

	//计算直方图距离，相似度
	//float dis0 = normL2Sqr(ref_hist, ref_hist, bins);
	float dis1 = normL2Sqr(ref_hist, pl_hist, bins);
	float dis2 = normL2Sqr(pl_hist, bg_hist, bins);
	//cout << dis0 << endl;
	cout << "dis1: " << dis1 << endl;
	cout << "dis2: " << dis2 << endl;

	/*delete[] ref_hist;
	delete[] pl_hist;
	delete[] bg_hist;/**/

	namedWindow("标记原图", WINDOW_AUTOSIZE);
	imshow("标记原图", srcMat);
	namedWindow("plMat", WINDOW_AUTOSIZE);
	imshow("plMat", plMat);
	namedWindow("bgMat", WINDOW_AUTOSIZE);
	imshow("bgMat", bgMat);

	waitKey(0);
}

int main()
{
	test9_1();
	return 0;
}