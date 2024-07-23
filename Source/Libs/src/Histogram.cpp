#include "histogram.hpp"
#include "SaveBinaryCV.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using cv::calcHist;
using cv::FileStorage;
using cv::imshow;
using cv::namedWindow;
using cv::NORM_MINMAX;
using cv::Point;
using cv::Scalar;
using cv::waitKey;
using std::cout;
using std::endl;

void showHistogram(cv::Mat src)
{
	string title = "Histogram Gray Image";
	namedWindow(title, 1);

	int histSize = 256;

	float range[] = {0, 256}; // the upper boundary is exclusive
	const float *histRange[] = {range};

	bool uniform = true, accumulate = false;

	Mat gray_hist;
	calcHist(&src, 1, 0, Mat(), gray_hist, 1, &histSize, histRange, uniform, accumulate);

	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	normalize(gray_hist, gray_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(gray_hist.at<float>(i - 1))),
			 Point(bin_w * (i), hist_h - cvRound(gray_hist.at<float>(i))),
			 Scalar(255, 255, 255), 2, 8, 0);
	}

	imshow(title, histImage);
}

cv::Mat getHistogram(cv::Mat src)
{
	int histSize = 256;

	float range[] = {0, 256}; // the upper boundary is exclusive
	const float *histRange[] = {range};

	bool uniform = true, accumulate = false;

	Mat gray_hist;
	calcHist(&src, 1, 0, Mat(), gray_hist, 1, &histSize, histRange, uniform, accumulate);
	
	return gray_hist;
}

void Histogram::extract(const vector<Mat> &images, const vector<string> &files, string outputName)
{
	string name = outputName + "_histograms";
	Mat histograms;
	int total = (int)images.size();
	cout << "Extracting " << name << "...\n";

	FileStorage fs(name + ".yaml", cv::FileStorage::WRITE);
	fs.write("mode", 'H');
	fs.write("name", "Histogram");
	fs.write("total", total);
	fs.write("files", files);

	for (int i = 0; i < total; i++)
	{
		Mat input;
		cv::cvtColor(images[i], input, cv::COLOR_BGR2GRAY);
		Mat hist = getHistogram(input).t();

		histograms.push_back(hist);

		cout << "Image " << i << "/" << total << " processed\n";
		cout << "Path: " << files[i] << endl;
	}

	fs.release();

	cv::SaveMatBinary(name + ".bin", histograms);
	
	cout << "Extract " << name << " done\n";
}