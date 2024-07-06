#include "factory.hpp"
#include <algorithm>
using std::sort;

Factory *Factory::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new Factory();
	}

	sort(sInstance->mCommands.begin(), sInstance->mCommands.end(), [](const Command &a, const Command &b) -> bool
	{
		return a.getName() < b.getName();
	});
	return sInstance;
}