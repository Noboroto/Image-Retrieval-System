#include "Command.hpp"

Command::Command(string name, string keys, function<void(const CommandLineParser &)> execute, function<bool(const CommandLineParser &)> canExecute)
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

void Command::execute(const CommandLineParser &parser) const
{
	mExecute(parser);
}

void Command::setExecute(function<void(const CommandLineParser &)> execute)
{
	mExecute = execute;
}

bool Command::canExecute(const CommandLineParser &parser) const
{
	return mCanExecute(parser);
}

void Command::setCanExecute(function<bool(const CommandLineParser &)> canExecute)
{
	mCanExecute = canExecute;
}

void Command::addSubCommand(const Command &command)
{
	mSubCommands.push_back(command);
}