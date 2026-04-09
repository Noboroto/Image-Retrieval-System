#include "Histogram.hpp"
#include "SaveBinaryCV.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using cv::calcHist;
using cv::FileStorage;
using cv::NORM_MINMAX;
using cv::SaveMatBinary;
using std::cout;
using std::endl;

/**
 * The function computes and returns the histogram of an input image using OpenCV in C++.
 * 
 * @param src The `src` parameter is the input image for which you want to compute the histogram. It is
 * of type `cv::Mat` which represents an n-dimensional dense numerical array that can store images.
 * 
 * @return The `compute` function returns a `cv::Mat` object which represents the computed histogram of
 * the input `src` image.
 */
cv::Mat Histogram::compute(const cv::Mat& src)
{
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform = true;
	bool accumulate = false;
	cv::Mat hist;
	calcHist(&src, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
	cv::normalize(hist, hist, 0, src.rows, NORM_MINMAX, -1, cv::Mat());
	return hist.t();
}

void Histogram::extract(const string& inputFolder, const string& outputPath)
{
	vector<cv::String> files;
	cv::glob(inputFolder, files);
	
	string name = outputPath + "_histograms";
	int total = (int)files.size();
	cout << "Extracting " << name << "...\n";
	FileStorage fs(name + ".yaml", cv::FileStorage::WRITE);
	fs.write("mode", 'H');
	fs.write("name", "Histogram");
	fs.write("total", total);
	fs.write("files", files);
	fs.release();

	// read all images and compute histograms

	cv::Mat hist;
	for (int i = 0; i < total; i++)
	{
		cv::Mat src = cv::imread(files[i], cv::IMREAD_GRAYSCALE);
		hist.push_back(compute(src));
	}
	SaveMatBinary(name + ".bin", hist);
}