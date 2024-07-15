#ifndef command_hpp
#define command_hpp

#include <opencv2/core/utility.hpp>
#include <string>
#include <functional>
#include <vector>

using cv::CommandLineParser;
using std::function;
using std::string;
using std::vector;

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

	void addSubCommand(const Command &command);

private:
	string mName;
	string mKeys;
	function<void(const CommandLineParser &)> mExecute;
	function<bool(const CommandLineParser &)> mCanExecute;
	vector<Command> mSubCommands;
};
#endif