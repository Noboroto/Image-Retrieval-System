#include "Command.hpp"

Command::Command(string name, string keys, function<void(int argc, char **argv)> execute, function<bool(int argc, char **argv)> canExecute)
{
	mName = name;
	mKeys = keys;
	mExecute = execute;
	mCanExecute = canExecute;
}

string Command::getKeys() const
{
	return mKeys;
}

void Command::setKeys(const string keys)
{
	mKeys = keys;
}

string Command::getName() const
{
	return mName;
}

void Command::setName(const string name)
{
	mName = name;
}

void Command::execute(int argc, char **argv) const
{
	if (mExecute)
		mExecute(argc, argv);
}

void Command::setExecute(function<void(int argc, char **argv)> execute)
{
	mExecute = execute;
}

bool Command::canExecute(int argc, char **argv) const
{
	return mCanExecute ? mCanExecute(argc, argv) : false;
}

void Command::setCanExecute(function<bool(int argc, char **argv)> canExecute)
{
	mCanExecute = canExecute;
}

void Command::addSubCommand(const Command &command)
{
	mSubCommands.push_back(command);
}