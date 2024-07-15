#include "Factory.hpp"
#include <algorithm>
#include "Histogram.hpp"
#include "SIFT.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

// ref at https://docs.opencv.org/3.4/d0/d2e/classcv_1_1CommandLineParser.html
using cv::CommandLineParser;
using cv::glob;
using std::cout;
using std::sort;

Command createExtractCmd();

Factory *Factory::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new Factory();
	}

	// add the commands
	sInstance->mCommands.push_back(createExtractCmd());

	// sort the commands by name
	sort(sInstance->mCommands.begin(), sInstance->mCommands.end(), [](const Command &a, const Command &b) -> bool
	{
		return a.getName() < b.getName();
	});
	return sInstance;
}

Factory::Factory()
{
	
}

string Factory::getKeys()
{
	string keys = "{help h usage ? | | print this message }";
	for (const Command &command : mCommands)
	{
		keys += command.getKeys();
	}
	return keys;
}

void Factory::execute(int argc, char **argv)
{
	CommandLineParser parser(argc, argv, getKeys());

	if (parser.has("help"))
	{
		parser.printMessage();
		return;
	}

	if (!parser.check())
	{
		parser.printErrors();
		return;
	}

	for (const Command &command : mCommands)
	{
		if (command.canExecute(parser))
		{
			command.execute(parser);
			break;
		}
	}
}

Command createExtractCmd()
{
	Command extractCmd = Command("extract", "{extract |. | path to the images folder to extract features }");

	extractCmd.setCanExecute([](const CommandLineParser &parser) -> bool
	{
		return parser.has("extract");
	});

	extractCmd.setExecute([](const CommandLineParser &parser)
	{
		string imgPath = parser.get<string>("extract");
		string outputName = imgPath.substr(imgPath.find_last_of("/\\") + 1);

		// get the list of images in the folder
		vector<string> files;
		vector<string> filenames;
		glob(imgPath, files);

		// read the images
		vector<Mat> images;
		for (auto &file : files)
		{
			Mat img = cv::imread(file);
			if (img.empty())
			{
				cout << "Could not read the image: " << file << std::endl;
				continue;
			}

			string name = file.replace(file.find(imgPath), imgPath.length() + 1, "");
			name = name.substr(1, name.size() - 1);
			filenames.push_back(name);
			images.push_back(img);
		}

		// extract features
		Histogram::extract(images, filenames, outputName);
		SIFT::extract(images, filenames, outputName);
	});

	return extractCmd;
}