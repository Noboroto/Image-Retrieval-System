#ifndef factory_hpp
#define factory_hpp

#include <opencv2/core/utility.hpp>
#include "command.hpp"
#include <vector>

using cv::CommandLineParser;
using std::vector;

class Factory
{
public:
	Factory(const Factory&) = delete;

	inline static Factory* getInstance();
	void execute(const CommandLineParser &parser);

private:
	Factory();
	inline static Factory* sInstance = nullptr;
	vector<Command> mCommands;
};

#endif