#ifndef command_hpp
#define command_hpp

#include <opencv2/core/utility.hpp>
#include <string>
#include <functional>

using cv::CommandLineParser;
using std::function;
using std::string;

class Command
{
public:
	Command(string name = "", string keys = "", function<void(const CommandLineParser &)> execute = nullptr, function<bool(const CommandLineParser &)> canExecute = nullptr);
	
	string getKeys() const;
	void setKeys(const string keys);

	string getName() const;
	void setName(const string keys);

	void execute(const CommandLineParser &parser) const;
	void setExecute(function<void(const CommandLineParser &)> execute);

	bool canExecute(const CommandLineParser &parser) const;
	void setCanExecute(function<bool(const CommandLineParser &)> canExecute);

private:
	string mName;
	string mKeys;
	function<void(const CommandLineParser &)> mExecute;
	function<bool(const CommandLineParser &)> mCanExecute;
};
#endif