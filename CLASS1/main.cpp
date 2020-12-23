#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// ����ʮ�� 
//ģ��ƥ�书�ܵ�����ϰ
//--------------------------------------------------------------------------------------------//
//��ϰ1 
cv::Mat frame, tempMat, refMat, resultMat;
VideoCapture cap;
cv::Mat frame_out;
void test13_1()
{
	cap.open(0);
	while (!cap.isOpened()); {
		cap.open(0);
		std::cout << "���ܴ���Ƶ�ļ�" << std::endl;
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
			//�ڶ�֡��ʼ

			matchTemplate(frame, refMat, resultMat, match_method);//Ѱ��ƥ������
			//imshow("resultMat", resultMat);
			normalize(resultMat, resultMat, 0, 1, NORM_MINMAX);//��һ��
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
	//�����������ص��ݶ���Ƕȷ���ͳ��ֱ��ͼ
	Mat gx, gy;
	Sobel(src, gx, CV_32F, 1, 0, 1);
	Sobel(src, gy, CV_32F, 0, 1, 1);

	cv::Mat roiMat;
	cv::Mat roiMag;
	cv::Mat roiAgl;

	//Ĭ���ǻ���radians����ѡ��Ƕ�degrees
	Mat mag, angle;//ÿ�����ص㶼�з�ֵ�ͷ���
	cartToPolar(gx, gy, mag, angle, true);
	int cont = 0;
	for (int i = 0; i < nY; i++) {
		for (int j = 0; j < nX; j++) {
			cont++;
			roi.x = j * cellSize;
			roi.y = i * cellSize;
			//��ֵͼ�� ROI����Ȥ������
			roiMat = src(roi);
			roiMag = mag(roi);
			roiAgl = angle(roi);
			//��ǰcell��һ�������������е�λ��
			int head = (i * nX + j) * nAngle;
			//ref_hist += head;

			//����Ƕ�ֱ��ͼ
			//cell�洢�ռ���Ϣ ��ͬ�ռ���һ��9άֱ��ͼ
			//��0��-8�� �����ݶȽǶ�0 20 40...160 ��
			//ֵ��ʾ�ݶȷ���
			int cnt = 0;
			for (int n = 0; n < roiMat.rows; n++) {
				for (int m = 0; m < roiMat.cols; m++) {
					cnt++;
					float temp_mag = (roiMag.at<float>(m, n));//���ص�ķ���ֵmag
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
	cvtColor(srcMat, srcMat, COLOR_BGR2GRAY);//�Ҷ�ͼ
	//���ͼ�����Ϣ����ͼ
	int WID = 0;//646
	int HEI = 0;//482
	WID = srcMat.cols;
	HEI = srcMat.rows;
	int AREA = WID * HEI;
	cout << "WID: " << WID << endl;
	cout << "HEI: " << HEI << endl;
	cout << "AREA: " << AREA << endl;
	//Ŀ��ͼ�����Ϣ��Сͼ���ο�ͼ��
	int wid = 0;//211
	int hei = 0;//93
	wid = refMat.cols;
	hei = refMat.rows;
	int area = wid * hei;
	cout << "wid: " << wid << endl;
	cout << "hei: " << hei << endl;
	cout << "area: " << area << endl;

	//ͼ�񻮷�Ϊ16*16��cell
	int blockSize = 16;
	nX = refMat.cols / blockSize;//�᷽�����м���cell
	nY = refMat.rows / blockSize;//���������м���cell
	int nAangle = 9;//180��n�ȷ� 0 20 40 160 �ŵȷ�
	//int bins = nX * nY * nAangle;//ÿ��cell��Ҫ9bit��ֱ��ͳ��ͼ
	//cout << "bins: " << bins << endl;
	//������������ͼƬ��HOG
	calcHOG(refMat, ref_hist, nAangle, blockSize);//ref_histֱ��ͼ
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