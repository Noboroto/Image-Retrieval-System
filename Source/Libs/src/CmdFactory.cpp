#include "CmdFactory.hpp"
#include "Histogram.hpp"
#include "SIFT.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

// ref at https://docs.opencv.org/3.4/d0/d2e/classcv_1_1CommandLineParser.html
using cv::CommandLineParser;
using cv::glob;
using std::cout;

Command createExtractCmd();
Command createHistogramCmd();
Command createSIFTCmd();

CmdFactory *CmdFactory::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new CmdFactory();
		// add the commands
		sInstance->mCommands.push_back(createExtractCmd());
		sInstance->mCommands.push_back(createHistogramCmd());
		sInstance->mCommands.push_back(createSIFTCmd());
	}

	return sInstance;
}

CmdFactory::CmdFactory()
{
}

string CmdFactory::getKeys()
{
	string keys = "{help h usage ? | | print this message }";
	for (const Command &command : mCommands)
	{
		keys += command.getKeys();
	}
	return keys;
}

void CmdFactory::execute(int argc, char **argv)
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

void readFiles(string imgPath, vector<Mat> &images, vector<string> filenames)
{
	vector<string> files;

	glob(imgPath, files);
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
}

Command createExtractCmd()
{
	Command extractCmd = Command("extract", "{extract |. | path to the images folder to extract all features }");

	extractCmd.setCanExecute([](const CommandLineParser &parser) -> bool
							 { return parser.has("extract"); });

	extractCmd.setExecute([](const CommandLineParser &parser)
						  {
		string imgPath = parser.get<string>("extract");
		string outputName = imgPath.substr(imgPath.find_last_of("/\\") + 1);

		// get the list of images in the folder
		vector<string> filenames;
		// read the images
		vector<Mat> images;

		readFiles(imgPath, images, filenames);

		// extract features
		Histogram::extract(images, filenames, outputName);
		SIFT::extract(images, filenames, outputName); });

	return extractCmd;
}

Command createSIFTCmd()
{
	Command extractCmd = Command("sift", "{sift |. | path to the images folder to extract SIFT features }");

	extractCmd.setCanExecute(
		[](const CommandLineParser &parser) -> bool
		{ return parser.has("sift"); });

	extractCmd.setExecute(
		[](const CommandLineParser &parser)
		{
		string imgPath = parser.get<string>("sift");
		string outputName = imgPath.substr(imgPath.find_last_of("/\\") + 1);

		// get the list of images in the folder
		vector<string> filenames;
		// read the images
		vector<Mat> images;

		readFiles(imgPath, images, filenames);

		// extract features
		SIFT::extract(images, filenames, outputName); });

	return extractCmd;
}

Command createHistogramCmd()
{
	Command extractCmd = Command("histogram", "{histogram |. | path to the images folder to extract histogram features }");

	extractCmd.setCanExecute(
		[](const CommandLineParser &parser) -> bool
		{ return parser.has("histogram"); });

	extractCmd.setExecute(
		[](const CommandLineParser &parser)
		{
		string imgPath = parser.get<string>("histogram");
		string outputName = imgPath.substr(imgPath.find_last_of("/\\") + 1);

		// get the list of images in the folder
		vector<string> filenames;
		// read the images
		vector<Mat> images;

		readFiles(imgPath, images, filenames);

		// extract features
		Histogram::extract(images, filenames, outputName); });
	return extractCmd;
}