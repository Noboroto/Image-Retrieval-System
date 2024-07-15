#ifndef factory_hpp
#define factory_hpp

#include "Command.hpp"
#include <vector>

using std::vector;

class Factory
{
public:
	Factory(const Factory&) = delete;
	void execute(int argc, char **argv);

	static Factory* getInstance();
private:
	string getKeys();

	Factory();
	inline static Factory* sInstance = nullptr;
	vector<Command> mCommands;
};

#endif