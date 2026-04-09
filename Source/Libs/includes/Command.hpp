#ifndef command_hpp
#define command_hpp

#include <opencv2/core/utility.hpp>
#include <string>
#include <functional>
#include <vector>
#include "lib_export.h"

using std::function;
using std::string;
using std::vector;

class Command
{
public:
	Command(string name = "", string keys = "", function<void(int argc, char **argv)> execute = nullptr, function<bool(int argc, char **argv)> canExecute = nullptr);

	string getKeys() const;
	void setKeys(const string keys);

	string getName() const;
	void setName(const string keys);

	void execute(int argc, char **argv) const;
	void setExecute(function<void(int argc, char **argv)> execute);

	bool canExecute(int argc, char **argv) const;
	void setCanExecute(function<bool(int argc, char **argv)> canExecute);

	void addSubCommand(const Command &command);

private:
	string mName;
	string mKeys;
	function<void(int argc, char **argv)> mExecute;
	function<bool(int argc, char **argv)> mCanExecute;
	vector<Command> mSubCommands;
};
#endif