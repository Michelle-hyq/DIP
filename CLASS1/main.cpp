#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
//vector<vector<Point>> contours;//m��<n�������>
	//��0�У���һ��������n������㣩��[x1,y1],[x2,y2]... �����͵ĵ� --n����
	//��1��...
	//vector<Vec4i> hierarchy;//Vec4i:n��<4�����ͱ���>   ������
	//��������ļ̳й�ϵ int��[a1,b1,c1,d1],[a2,b2,c2,d2]...

	//vector<Rect>����width*height from λ��(x,y)
	//Rect(x,y,width,height)
	//[width x height from(x,y)]

	//vector<RotatedRect>
	//angle:0 center:[0,0] size:[0 x 0]
//--------------------------------------------------------------------------------------------//
// ���þ�
//�ǵ��⡢SIFT��HOG�����������
cv::Mat binMat, gryMat, minRectMat, canny_img,dstMat;
int nX,nY;
int bins = 810;
float ref_hist[810];
float pl_hist[810];
float bg_hist[810];/**/
//����ԭͼ��ֱ��ͼ��cell�����cell�߳�
void calcHOG(cv::Mat src,float* tempHist,int nAngle,int cellSize)
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
					float temp_mag = ( roiMag.at<float>(m, n));//���ص�ķ���ֵmag
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
	cvtColor(srcMat, gryMat, COLOR_BGR2GRAY);//�Ҷ�ͼ
	int wid = 0;
	int hei = 0;
	wid = srcMat.cols;
	hei = srcMat.rows;
	int area = wid * hei;
	//cout << "area: " << area << endl;
	//���
	Mat refMat;
	srcMat.copyTo(refMat);
	//ͼ�񻮷�Ϊ16*16��cell
	int blockSize = 16;
	nX = refMat.cols / blockSize;//�᷽�����м���cell
	nY = refMat.rows / blockSize;//���������м���cell

	int nAangle = 9;//180��n�ȷ� 0 20 40 160 �ŵȷ�
	int bins = nX * nY * nAangle;//ÿ��cell��Ҫ9bit��ֱ��ͳ��ͼ
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
	//������������ͼƬ��HOG
	calcHOG(refMat, ref_hist,nAangle, blockSize);//ref_histֱ��ͼ
	calcHOG(plMat , pl_hist, nAangle, blockSize);
	calcHOG(bgMat, bg_hist,nAangle, blockSize);

	for (int i = 0; i < 10; i++) printf("%f ", ref_hist[i]);
	printf("\r\n ");
	for (int i = 0; i < 10; i++) printf("%f ", pl_hist[i]);
	printf("\r\n ");
	for (int i = 0; i < 10; i++) printf("%f ", bg_hist[i]);
	printf("\r\n");/**/

	//����ֱ��ͼ���룬���ƶ�
	//float dis0 = normL2Sqr(ref_hist, ref_hist, bins);
	float dis1 = normL2Sqr(ref_hist, pl_hist, bins);
	float dis2 = normL2Sqr(pl_hist, bg_hist, bins);
	//cout << dis0 << endl;
	cout << "dis1: " << dis1 << endl;
	cout << "dis2: " << dis2 << endl;

	/*delete[] ref_hist;
	delete[] pl_hist;
	delete[] bg_hist;/**/

	namedWindow("���ԭͼ", WINDOW_AUTOSIZE);
	imshow("���ԭͼ", srcMat);
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