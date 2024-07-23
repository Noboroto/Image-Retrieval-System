#include "SIFT.hpp"
#include "SaveBinaryCV.hpp"
#include <opencv2/features2d.hpp>
#include <opencv2/core.hpp>

#include <vector>
#include <iostream>

using cv::FileStorage;
using cv::glob;
using cv::Mat;
using std::cout;
using std::endl;
using std::vector;

void SIFT::extract(const vector<Mat> &images, const vector<string> &files, string outputName)
{
	int total = (int)images.size();
	string name = outputName + "_SIFT";

	Mat sizes;
	Mat descriptors;

	cout << "Extracting " << name << " features...\n";
	cv::Ptr<cv::Feature2D> sift = cv::SIFT::create();

	FileStorage fs(name + ".yaml", FileStorage::WRITE);
	fs.write("mode", 'S');
	fs.write("name", "SIFT");
	fs.write("total", total);
	fs.write("files", files);

	for (int i = 0; i < images.size(); i++)
	{
		Mat input = images[i];
		Mat gray;
		cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
		vector<cv::KeyPoint> kps;
		Mat des;
		sift->detectAndCompute(gray, Mat(), kps, des);

		descriptors.push_back(des);

		cout << "Image " << i << "/" << images.size() << " processed\n";
		cout << "Path: " << files[i] << endl;

		sizes.push_back((int)kps.size());
	}
	fs.write("sizes", sizes);

	fs.release();

	cv::SaveMatBinary(name + ".bin", descriptors);

	cout << "Extract " << name << " features done\n";
}