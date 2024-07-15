#ifndef CmdFactory_hpp
#define CmdFactory_hpp

#include "Command.hpp"
#include "lib_export.h"
#include <vector>

using std::vector;
class Command;
class DLIB_EXPORT CmdFactory
{
public:
	CmdFactory(const CmdFactory&) = delete;
	void execute(int argc, char **argv);

	static CmdFactory* getInstance();
private:
	string getKeys();

	CmdFactory();
	inline static CmdFactory* sInstance = nullptr;
	vector<Command> mCommands;
};

#endif