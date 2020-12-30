#include "stdafx.h"
#include "funtions.h"
#include "parameters.h"


VideoCapture createInput(bool useCamera, std::string videoPath)
{
	//
	VideoCapture capVideo;
	if (useCamera) {
		capVideo.open(0);
	}
	else {
		capVideo.open(videoPath);
	}
	return capVideo;
}
int createMaskByKmeans(cv::Mat src, cv::Mat& mask);
int kMeansDemo()
{
	const int MAX_CLUSTERS = 5;
	Scalar colorTab[] =
	{
		Scalar(0, 0, 255),
		Scalar(0,255,0),
		Scalar(255,100,100),
		Scalar(255,0,255),
		Scalar(0,255,255)
	};

	Mat img(500, 500, CV_8UC3);
	RNG rng(12345);

	for (;;)
	{
		int k, clusterCount = rng.uniform(2, MAX_CLUSTERS + 1);
		int i, sampleCount = rng.uniform(1, 1001);
		Mat points(sampleCount, 1, CV_32FC2), labels;

		clusterCount = MIN(clusterCount, sampleCount);
		std::vector<Point2f> centers;

		/* generate random sample from multigaussian distribution */
		for (k = 0; k < clusterCount; k++)
		{
			Point center;
			center.x = rng.uniform(0, img.cols);
			center.y = rng.uniform(0, img.rows);
			Mat pointChunk = points.rowRange(k * sampleCount / clusterCount,
				k == clusterCount - 1 ? sampleCount :
				(k + 1) * sampleCount / clusterCount);
			rng.fill(pointChunk, RNG::NORMAL, Scalar(center.x, center.y), Scalar(img.cols * 0.05, img.rows * 0.05));
		}

		randShuffle(points, 1, &rng);

		double compactness = kmeans(points, clusterCount, labels,
			TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0),
			3, KMEANS_PP_CENTERS, centers);

		img = Scalar::all(0);

		for (i = 0; i < sampleCount; i++)
		{
			int clusterIdx = labels.at<int>(i);
			Point ipt = points.at<Point2f>(i);
			circle(img, ipt, 2, colorTab[clusterIdx], FILLED, LINE_AA);
		}
		for (i = 0; i < (int)centers.size(); ++i)
		{
			Point2f c = centers[i];
			circle(img, c, 40, colorTab[i], 1, LINE_AA);
		}
		cout << "Compactness: " << compactness << endl;

		imshow("clusters", img);

		char key = (char)waitKey();
		if (key == 27 || key == 'q' || key == 'Q') // 'ESC'
			break;
	}

	return 0;
}
void segColor()
{

	Mat src = imread("D://image/timg24.jpg");

	Mat mask = Mat::zeros(src.size(), CV_8UC1);
	createMaskByKmeans(src, mask);

	imshow("src", src);
	imshow("mask", mask);

	waitKey(0);

}
int createMaskByKmeans(cv::Mat src, cv::Mat& mask)
{
	if ((mask.type() != CV_8UC1)
		|| (src.size() != mask.size())
		) {
		return 0;
	}

	int width = src.cols;
	int height = src.rows;

	int pixNum = width * height;
	int clusterCount;
	Mat labels;
	Mat centers;

	//制作kmeans用的数据
	Mat sampleData = src.reshape(3, pixNum);
	Mat km_data;
	sampleData.convertTo(km_data, CV_32F);

	//执行kmeans
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1);
	kmeans(km_data, 2, labels, criteria, 1, KMEANS_PP_CENTERS, centers);

	//制作mask
	uchar fg[2] = { 0,255 };
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			mask.at<uchar>(row, col) = fg[labels.at<int>(row * width + col)];
		}
	}

	return 0;
}
void segColor_1()
{
	VideoCapture cap2;
	cap2.open("D://image/bg.mp4");
	//获取视频的帧速率（一般是30或者60）
	double frame_rate2 = cap2.get(CV_CAP_PROP_FPS);
	cout << frame_rate2 << endl;
	//获取视频的总帧数目
	long num_frame2 = static_cast<long>(cap2.get(CV_CAP_PROP_FRAME_COUNT));
	cout << num_frame2 << endl;
	//从特定帧开始
	//auto position2 = num_frame2 / 2;
	cap2.set(CV_CAP_PROP_POS_FRAMES, 0);
	Mat frame2;
	//根据帧速率计算帧之间的等待时间，单位ms
	int delat2 = 1000 / frame_rate2;


	VideoCapture cap;
	cap.open("D://image/Dragon_1.mp4");
	//open（0）时打开本地USB摄像头，输入网址可以加在web上的视频
	//检查是否打开成功
	if (!cap.isOpened())
	{
		cout << "视频没有打开" << endl;
	}
	//获取视频的帧速率（一般是30或者60）
	double frame_rate = cap.get(CV_CAP_PROP_FPS);
	cout << frame_rate << endl;
	//获取视频的总帧数目
	long num_frame = static_cast<long>(cap.get(CV_CAP_PROP_FRAME_COUNT));
	cout << num_frame << endl;
	//从特定帧开始
	//auto position = num_frame / 2;
	cap.set(CV_CAP_PROP_POS_FRAMES, 0);
	Mat frame;
	namedWindow("vedeo", WINDOW_AUTOSIZE);
	//根据帧速率计算帧之间的等待时间，单位ms
	int delat = 1000 / frame_rate;
	cap2 >> frame2;
	int width = frame2.cols;//272
	int height = frame2.rows;//480
	Mat mask = Mat::zeros(frame2.size(), CV_8UC1);
	
	Mat result;
	frame2.copyTo(result);
	//循环遍历视频中的全部帧
	while (1)
	{
		cap >> frame;
		cap2 >> frame2;
		if (!frame.empty()&& !frame2.empty())//如果读完就结束
		{
			resize(frame, frame, frame2.size());
			createMaskByKmeans(frame, mask);
			//erode(mask, mask, 3);
			//morphologyEx(mask, mask, MORPH_OPEN, 3);
			for (int row = 0; row < height; row++) {
				for (int col = 0; col < width; col++) {
					if (mask.at<uchar>(479, 0) <= 100) {
						if (mask.at<uchar>(row, col) <= 100) {//黑色
							result.at<Vec3b>(row, col) = frame2.at<Vec3b>(row, col);
						}
						else {
							result.at<Vec3b>(row, col) = frame.at<Vec3b>(row, col);
						}
					}
					else {
						if (mask.at<uchar>(row, col) <= 100) {//黑色
							result.at<Vec3b>(row, col) = frame.at<Vec3b>(row, col);
						}
						else {
							result.at<Vec3b>(row, col) = frame2.at<Vec3b>(row, col);
						}
					}
				}
			}
			imshow("result", result); 
			imshow("mask", mask);
			imshow("vedeo",frame);
		}
		else
		{
			break;
		}

		waitKey(1);//要有这句，才会输出视频
		//在显示每一帧都采用了延时方法。延时的时长取决于视频的帧频率（fps为帧速率，1000/fps为两帧之间的毫秒数）
		//通过修改delat的值，可以使视频快进或慢进
		//将delat设置为0，按照用户按键，才播放。
	}

	waitKey(0);

}
int createMaskByKmeans_1(cv::Mat src, cv::Mat& mask)
{
	if ((mask.type() != CV_8UC1)
		|| (src.size() != mask.size())
		) {
		return 0;
	}

	int width = src.cols;
	int height = src.rows;

	int pixNum = width * height;
	int clusterCount;
	Mat labels;
	Mat centers;

	//制作kmeans用的数据
	Mat sampleData = src.reshape(3, pixNum);
	Mat km_data;
	sampleData.convertTo(km_data, CV_32F);

	//执行kmeans
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1);
	kmeans(km_data, 2, labels, criteria, 10, KMEANS_PP_CENTERS, centers);

	//制作mask
	uchar fg[2] = { 0,255 };
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			mask.at<uchar>(row, col) = fg[labels.at<int>(row * width + col)];
		}
	}

	return 0;
}