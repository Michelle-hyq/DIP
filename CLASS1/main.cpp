#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// 课堂十三 
//模板匹配功能调用练习
//--------------------------------------------------------------------------------------------//
//练习1 
cv::Mat frame, tempMat, refMat, resultMat;
VideoCapture cap;
cv::Mat frame_out;
void test13_1()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "不能打开视频文件" << std::endl;
	}
	int cnt = 0;
	int match_method = 0;
	double minVal, maxVal;
	Point matchLoc;
	Point minLoc, maxLoc;
	while (1) {
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (cnt == 0) {
			Rect2d r;
			r = selectROI(frame, true);
			tempMat = frame(r);
			tempMat.copyTo(refMat);
			//namedWindow("tempMat", WINDOW_AUTOSIZE);
			//imshow("tempMat", tempMat);
			//waitKey(0);
			destroyAllWindows();
			cnt = 1;
		}
		else {
			//第二帧开始

			matchTemplate(frame, refMat, resultMat, match_method);//寻找匹配区域
			//imshow("resultMat", resultMat);
			normalize(resultMat, resultMat, 0, 1, NORM_MINMAX);//归一化
			//imshow("resultMat2", resultMat);
			minMaxLoc(resultMat, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
			rectangle(frame, minLoc, Point2i(minLoc.x + tempMat.cols, minLoc.y + tempMat.rows), Scalar(0, 0, 0), 2, 8, 0);
			namedWindow("frame", WINDOW_AUTOSIZE);
			imshow("frame", frame);
			//imshow("subMat", subMat);
			waitKey(30);
		}
	}
}
cv::Mat binMat, gryMat, minRectMat, canny_img, dstMat;
int nX, nY;
int bins = 585;
float ref_hist[585];
float pl_hist[585];
float bg_hist[585];/**/
void calcHOG(cv::Mat src, float* tempHist, int nAngle, int cellSize)
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
					float temp_mag = (roiMag.at<float>(m, n));//像素点的幅度值mag
					int temp_ang = (int)(roiAgl.at<float>(m, n));
					float show_ang = roiAgl.at<float>(m, n);
					//cout << "\temp_ang: " << show_ang << endl;

					if (temp_ang % 40 == 0)
						tempHist[head + temp_ang / 40] = temp_mag;
					//*(ref_hist + temp_ang / 40) = temp_mag;
					else if (temp_ang / 40 < 9) {
						tempHist[head + temp_ang / 40] = (float)(40 - (temp_ang % 40)) / 40.0 * temp_mag;
						tempHist[head + ((temp_ang / 40) == 8 ? (temp_ang / 40 + 1) : 0)] = (float)(temp_ang % 40) / 40 * temp_mag;/**/
						/**(tempHist + temp_ang / 40) = (float)(40-(temp_ang % 40)) / 40.0 * temp_mag;
						*(tempHist + ((temp_ang/40)==8? (temp_ang / 40+1):0)) = (float)(temp_ang % 40) / 40.0 * temp_mag;/**/
					}

				}
			}
		}
	}
}
void test13_2()
{
	Mat srcMat, plMat, bgMat;
	srcMat = imread("D://image/timg7.jpg");
	refMat = imread("D://image/timg17.jpg");
	cvtColor(refMat, refMat, COLOR_BGR2GRAY);
	cvtColor(srcMat, srcMat, COLOR_BGR2GRAY);//灰度图
	//检测图像的信息，大图
	int WID = 0;//646
	int HEI = 0;//482
	WID = srcMat.cols;
	HEI = srcMat.rows;
	int AREA = WID * HEI;
	cout << "WID: " << WID << endl;
	cout << "HEI: " << HEI << endl;
	cout << "AREA: " << AREA << endl;
	//目标图像的信息，小图（参考图像）
	int wid = 0;//211
	int hei = 0;//93
	wid = refMat.cols;
	hei = refMat.rows;
	int area = wid * hei;
	cout << "wid: " << wid << endl;
	cout << "hei: " << hei << endl;
	cout << "area: " << area << endl;

	//图像划分为16*16的cell
	int blockSize = 16;
	nX = refMat.cols / blockSize;//横方向上有几个cell
	nY = refMat.rows / blockSize;//竖方向上有几个cell
	int nAangle = 9;//180°n等分 0 20 40 160 九等分
	//int bins = nX * nY * nAangle;//每个cell需要9bit的直方统计图
	//cout << "bins: " << bins << endl;
	//计算三张输入图片的HOG
	calcHOG(refMat, ref_hist, nAangle, blockSize);//ref_hist直方图
	for (int i = 0; i < 10; i++) printf("%f ", ref_hist[i]);
	//int dist = normL2Sqr(ref_hist, ref_hist, bins);
	//cout << "dist:" << dist << endl;
	//printf("\r\n ");
	int mov_dx = wid / 4;//52
	int mov_dy = hei / 4;//23
	int MOV_X = WID - wid - 1;//434
	int MOV_Y = HEI - hei - 1;//388
	int MOV_Xcnt = WID / mov_dx;//12
	int MOV_Ycnt = HEI / mov_dy;//20
	cout << "mov_dx:" << mov_dx << " mov_dy:" << mov_dy << " MOV_X:" << MOV_X << " MOV_Y:" << MOV_Y << " MOV_Xcnt:" << MOV_Xcnt << " MOV_Ycnt:" << MOV_Ycnt << endl;
	float src_hist[240][585];
	float dis[240];
	float dis_max = 0;
	float dis_min = 2000000;
	Mat tempMat;
	Rect2i rr;
	int i = 0, j = 0, cnt = 0;
	for (i = 0; i < MOV_Ycnt; i++)
	{
		for (j = 0; j < MOV_Xcnt; j++)
		{
			Rect2i r(j * mov_dy, i * mov_dx, wid, hei);
			tempMat = srcMat(r);
			calcHOG(tempMat, src_hist[i * 12 + j], nAangle, blockSize);
			dis[i * 12 + j] = normL2Sqr(ref_hist, src_hist[i * 12 + j], bins) / 1000;
			//if (dis[i * 12 + j] > dis_max)
			//	dis_max = dis[i * 12 + j];
			if (dis[i * 12 + j] < dis_min) {
				dis_min = dis[i * 12 + j];
				rr = r;
				cnt = 1;
				cout << "cnt:" << i * 12 + j << "  " << dis[i * 12 + j] << endl;
				cout << "dis_min:" << dis_min << endl;
				break;
			}
			cout << "dis" << i * 12 + j << " = " << dis[i * 12 + j] << endl;
			cout << "dis_min:" << dis_min << endl;
		}
		if (cnt == 1)break;
	}
	rectangle(srcMat, rr, Scalar(255, 0, 0), 2, LINE_8, 0);
	namedWindow("srcMat", WINDOW_AUTOSIZE);
	imshow("srcMat", srcMat);
	waitKey(0);
}

int main()
{
	test13_2();
	return 0;
}