#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;
void test2_1()
{
	cv::Mat src_color = imread("D://image/timg2.jpg");
	std::vector<cv::Mat> channels;
	cv::split(src_color, channels);
	cv::Mat B = channels.at(0);
	cv::Mat G = channels.at(1);
	cv::Mat R = channels.at(2);
	cv::imshow("red", R);
	cv::imshow("blue", G);
	cv::imshow("green", B);
	cv::imshow("original Mat", src_color);
	waitKey(0);
}
void test2_2()
{
	VideoCapture cap;
	cap.open(0);
	while (!cap.isOpened());
	{
		cap.open(0);
		std::cout << "不能打开视频文件" << std::endl;
		
	}
	double fps = cap.get(CAP_PROP_FPS);
	std::cout << "fps" << fps << std::endl;
	while (1)
	{
		cv::Mat frame;
		bool rSucess = cap.read(frame);
		if (!rSucess)
		{
			std::cout << "不能从视频文件读取帧" << std::endl;
			break;
		}
		else
			cv::imshow("frame", frame);
		waitKey(30);
	}

}
int main()
{
	test2_2();
	return 0;
}/**/