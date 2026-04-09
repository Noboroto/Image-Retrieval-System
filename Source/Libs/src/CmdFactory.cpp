#include "CmdFactory.hpp"
#include "SaveBinaryCV.hpp"
#include "Histogram.hpp"
#include "SIFT.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

// ref at https://docs.opencv.org/3.4/d0/d2e/classcv_1_1CommandLineParser.html
using cv::CommandLineParser;
using cv::glob;
using std::cout;

Command createExtractCmd();
Command visualizeCmd();
Command KMean();
Command FindByHis();

void verifyPath(const string &path)
{
	std::ifstream file(path);
	if (!file)
	{
		std::cerr << "File not found: " << path << std::endl;
		exit(404);
	}
}

CmdFactory *CmdFactory::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new CmdFactory();
		// add the commands
		sInstance->mCommands.push_back(createExtractCmd());
		sInstance->mCommands.push_back(visualizeCmd());
		sInstance->mCommands.push_back(KMean());
		sInstance->mCommands.push_back(FindByHis());
	}

	return sInstance;
}

CmdFactory::CmdFactory()
{
}

string CmdFactory::getKeys()
{
	string keys = "{help h usage ? | | print this message}";
	for (const Command &command : mCommands)
	{
		keys += command.getKeys();
	}
	return keys;
}

void CmdFactory::execute(int argc, char **argv)
{
	CommandLineParser parser(argc, argv, getKeys());

	if (!parser.check())
	{
		parser.printErrors();
		return;
	}

	for (const Command &command : mCommands)
	{
		if (command.canExecute(argc, argv))
		{
			cout << "Running command: " << command.getName() << std::endl;
			command.execute(argc, argv);
			return;
		}
	}

	if (parser.has("help"))
	{
		parser.printMessage();
		return;
	}
}

void readFiles(string imgPath, vector<Mat> &images, vector<string> &filenames)
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

		size_t pos = file.find_last_of("/\\");
		string name = (pos != string::npos) ? file.substr(pos + 1) : file;
		filenames.push_back(name);
		images.push_back(img);
	}
}

Command createExtractCmd()
{
	Command extractCmd = Command("extract", "{extract | | turns into extract mode}");

	string outParams = extractCmd.getKeys();
	string selfParam = outParams +
					   "{help h usage ? | | print this message}"
					   "{input_path | | path to the input images folder}"
					   "{output_path |.| path to the folder store feautures}"
					   "{mode |all| mode to extract features}";

	extractCmd.setCanExecute([outParams](int argc, char **argv) -> bool
							 { 
								CommandLineParser parser(argc, argv, outParams);
								return parser.has("extract"); });

	extractCmd.setExecute([selfParam](int argc, char **argv)
						  {
		CommandLineParser parser(argc, argv, selfParam);

		for (int i = 0; i < argc; i++)
		{
			cout << argv[i] << std::endl;
		}

		if (!parser.check())
		{
			parser.printErrors();
			return;
		}

		if (parser.has("help"))
		{
			parser.printMessage();
			return;
		}

		string imgPath = parser.get<string>("input_path");
		string outputFolder = parser.get<string>("output_path");
		string mode = parser.get<string>("mode");

		// get the list of images in the folder
		vector<string> filenames;
		// read the images
		vector<Mat> images;

		readFiles(imgPath, images, filenames);

		if (mode == "histogram" || mode == "all")
		{
			Histogram::extract(imgPath, outputFolder);
		}
		if (mode == "sift" || mode == "all")
		{
			SIFT::extract(images, filenames, outputFolder);
		} });

	return extractCmd;
}

cv::Mat plotPoints(const cv::Mat &points)
{
	// Perform PCA, reducing to 2 dimensions
	cv::PCA pca(points, cv::Mat(), cv::PCA::DATA_AS_ROW, 2);

	// Project the original data into the PCA space (2D)
	cv::Mat result = pca.project(points);

	// Normalize the result to fit within the display window
	double minVal, maxVal;
	cv::minMaxLoc(result, &minVal, &maxVal);
	cv::Mat normalizedResult;
	result.convertTo(normalizedResult, CV_32F, 255 / (maxVal - minVal), -minVal * 255 / (maxVal - minVal));

	// Create an image to display the points
	cv::Mat displayImage = cv::Mat::zeros(600, 600, CV_8UC3);

	// Plot each point
	for (int i = 0; i < normalizedResult.rows; i++)
	{
		cv::Point point(static_cast<int>(normalizedResult.at<float>(i, 0)), static_cast<int>(normalizedResult.at<float>(i, 1)));
		cv::circle(displayImage, point, 5, cv::Scalar(0, 0, 255), cv::FILLED);
	}

	return displayImage;
}

Command visualizeCmd()
{
	Command result = Command("visualize", "{visualize | | visualize mode }");
	string outParam = result.getKeys();
	string selfParam = outParam +
					   "{help h usage ? | | print this message}"
					   "{input_path | | path to the input images folder}"
					   "{output_path |.| path to the folder store feautures}";

	result.setCanExecute(
		[outParam](int argc, char **argv) -> bool
		{
			CommandLineParser parser(argc, argv, outParam);
			return parser.has("visualize");
		});

	result.setExecute(
		[selfParam](int argc, char **argv)
		{
			CommandLineParser parser(argc, argv, selfParam);

			if (!parser.check())
			{
				parser.printErrors();
				return;
			}

			if (parser.has("help"))
			{
				parser.printMessage();
				return;
			}

			string filename = parser.get<string>("input_path");
			string outputFolder = parser.get<string>("output_path");

			verifyPath(filename);
			cout << "Visualizing " << filename << std::endl;

			cv::Mat points;
			cv::LoadMatBinary(filename, points);
			cv::Mat plot = plotPoints(points);
			cv::imshow("plot", plot);
			cv::waitKey(0);
		});

	return result;
}

Command KMean()
{
	Command result = Command("kmean", "{kmean | | clustering mode by kmean}");
	string outParam = result.getKeys();
	string selfParam = outParam +
					   "{help h usage ? | | print this message}"
					   "{input_path | | path to the input images folder}"
					   "{output_path |.| path to the folder store feautures}"
					   "{clusters |256| number of clusters}";

	result.setCanExecute(
		[outParam](int argc, char **argv) -> bool
		{
			CommandLineParser parser(argc, argv, outParam);
			return parser.has("kmean");
		});

	result.setExecute(
		[selfParam](int argc, char **argv)
		{
			CommandLineParser parser(argc, argv, selfParam);

			if (!parser.check())
			{
				parser.printErrors();
				return;
			}

			if (parser.has("help"))
			{
				parser.printMessage();
				return;
			}

			string filename = parser.get<string>("input_path");
			verifyPath(filename);

			string outFolder = parser.get<string>("output_path");

			cout << "Clustering " << filename << std::endl;

			int clusters = parser.get<int>("clusters");
			cout << "Number of clusters: " << clusters << std::endl;
			cv::Mat points;
			cv::LoadMatBinary(filename, points);

			cv::Mat labels;
			cv::Mat centers;
			cv::kmeans(points, clusters, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0), 3, cv::KMEANS_PP_CENTERS, centers);

			// Save the result
			cv::SaveMatBinary("centers.bin", centers);

			// Plot the result
			cv::Mat plot = plotPoints(centers);
			cv::imshow("plot", plot);
			cv::waitKey(0);
		});

	return result;
}

Command FindByHis()
{
	string outParam = "{find_by_hist | | path to the MAT binary file to find by histogram}";
	string selfParam = outParam +
					   "{help h usage ? | | print this message}"
					   "{input_path | | path to the input images folder}"
					   "{output_path |.| path to the folder store feautures}"
					   "{clusters |256| number of clusters}"
					   "{histogram | | path to the MAT binary file to find by histogram}"
					   "{metadata_hist | | path to the YAML file to load the metadata}"
					   "{threshold |0.1| threshold to consider a match}";

	Command result = Command("find_by_hist", outParam);

	result.setCanExecute(
		[outParam](int argc, char **argv) -> bool
		{
			CommandLineParser parser(argc, argv, outParam);
			return parser.has("find_by_hist"); });

	result.setExecute(
		[selfParam](int argc, char **argv)
		{
			CommandLineParser parser(argc, argv, selfParam);
			string filename = parser.get<string>("input_path");
			verifyPath(filename);
			cout << "Finding by histogram " << filename << std::endl;

			string histFilename = parser.get<string>("histogram");
			verifyPath(histFilename);
			cout << "Using histogram " << histFilename << std::endl;

			string metadataFilename = parser.get<string>("metadata_hist");
			verifyPath(metadataFilename);
			cout << "Using metadata " << metadataFilename << std::endl;

			cv::Mat points;
			cv::LoadMatBinary(filename, points);

			cv::Mat hist;
			cv::LoadMatBinary(histFilename, hist);

			cv::FileStorage fs(metadataFilename, cv::FileStorage::READ);
			std::vector<std::string> filenames;
			fs["files"] >> filenames;
			fs.release();

			const double THRESHOLD = 0.1;

			// sort the images by similarity
			std::vector<std::pair<std::string, double>> similarities;
			for (int i = 0; i < points.rows; i++)
			{
				// compute the similarity by euclidean distance
				double similarity = cv::norm(hist, points.row(i), cv::NORM_L2);
				// norm it to [0, 1]
				similarity /= cv::norm(hist, cv::NORM_L2);
				// if it is below the threshold, add it to the list
				if (similarity < THRESHOLD)
				{
					similarities.push_back(std::make_pair(filenames[i], similarity));
				}
			}

			// sort the images by similarity
			std::sort(similarities.begin(), similarities.end(), [](const std::pair<std::string, double> &a, const std::pair<std::string, double> &b) -> bool
					  { return a.second > b.second; });

			// print the results
			for (const auto &similarity : similarities)
			{
				cout << similarity.first << " - " << similarity.second << std::endl;
			}

			// convert the result to vector double to save
			vector<double> result;
			for (const auto &similarity : similarities)
			{
				result.push_back(similarity.second);
			}

			// save the result to YAML
			cv::FileStorage ResultFile("result.yaml", cv::FileStorage::WRITE);
			ResultFile << "result" << result;
			ResultFile.release();
		});

	return result;
}