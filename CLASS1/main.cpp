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
	while(1){
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
			rectangle(frame, minLoc, Point2i(minLoc.x + tempMat.cols, minLoc.y + tempMat.rows),Scalar(0,0,0),2,8,0);
			namedWindow("frame", WINDOW_AUTOSIZE);
			imshow("frame", frame);
			//imshow("subMat", subMat);
			waitKey(30);	
		}
	}
}
cv::Mat binMat, gryMat, minRectMat, canny_img, dstMat;
int nX, nY;
int bins = 810;
float ref_hist[810];
float pl_hist[810];
float bg_hist[810];/**/
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
	int cnt = 0;
	int match_method = 0;
	double minVal, maxVal;
	Point matchLoc;
	Point minLoc, maxLoc;

	frame = imread("D://image/timg7.jpg");
	cvtColor(frame, frame, COLOR_BGR2GRAY);

	Mat srcMat, plMat, bgMat;
	srcMat = imread("D://image/timg7.jpg");
	cvtColor(srcMat, gryMat, COLOR_BGR2GRAY);//�Ҷ�ͼ
	int wid = 0;
	int hei = 0;
	wid = srcMat.cols;
	hei = srcMat.rows;
	int area = wid * hei;

	Rect2d r;
	r = selectROI(frame, true);
	tempMat = frame(r);
	tempMat.copyTo(refMat);
	//srcMat.copyTo(refMat);
	//ͼ�񻮷�Ϊ16*16��cell
	int blockSize = 16;
	nX = refMat.cols / blockSize;//�᷽�����м���cell
	nY = refMat.rows / blockSize;//���������м���cell
	int nAangle = 9;//180��n�ȷ� 0 20 40 160 �ŵȷ�
	int bins = nX * nY * nAangle;//ÿ��cell��Ҫ9bit��ֱ��ͳ��ͼ
	cout << "bins: " << bins << endl;
	while (1) {	
		
		//������������ͼƬ��HOG
		calcHOG(refMat, ref_hist, nAangle, blockSize);//ref_histֱ��ͼ
		for (int i = 0; i < 10; i++) printf("%f ", ref_hist[i]);
		printf("\r\n ");
		//����ֱ��ͼ���룬���ƶ�
		//float dis0 = normL2Sqr(ref_hist, ref_hist, bins);
		float dis1 = normL2Sqr(ref_hist, pl_hist, bins);
		cout << "dis1: " << dis1 << endl;
		namedWindow("���ԭͼ", WINDOW_AUTOSIZE);
		imshow("���ԭͼ", srcMat);
		waitKey(0);

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

int main()
{
	test13_1();
	return 0;
}