#ifndef histogram_hpp
#define histogram_hpp
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "lib_export.h"

using std::string;
using std::vector;
using cv::Mat;

namespace Histogram
{
	void extract(const vector<Mat> &images, const vector<string> &files, string outputName);
}
#endif