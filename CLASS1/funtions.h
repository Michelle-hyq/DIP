#pragma once
#include <opencv.hpp>

using namespace cv;
using namespace std;

VideoCapture createInput(bool useCamera, std::string videoPath);



void segColor();
int kMeansDemo();
void segColor_1();
int kMeansDemo_1();

int createMaskByKmeans(cv::Mat src, cv::Mat& mask);