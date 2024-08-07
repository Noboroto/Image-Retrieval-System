#include <opencv2/core/utils/logger.hpp>
#include "CmdFactory.hpp"
#include <ctime>

using std::cout;
using std::endl;
using std::string;

int main(int argc, char **argv)
{
	// The logging level can be set with the following function
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

	CmdFactory::getInstance()->execute(argc, argv);

	return 0;
}