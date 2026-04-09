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
	Mat compute(const Mat &image);
	void extract(const string& inputFolder, const string& outputPath);
}
#endif