#ifndef sift_hpp
#define sift_hpp
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
using cv::Mat;
using std::string;
using std::vector;

namespace SIFT
{
	void extract(const vector<Mat> &images, const vector<string> &files, string outputName);
}
#endif