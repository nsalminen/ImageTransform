#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

Mat rotate(Mat src, int angle) {
	Mat dst;
	Point2f pc(src.cols / 2., src.rows / 2.);
	Mat r = getRotationMatrix2D(pc, angle, 1.0);
	warpAffine(src, dst, r, src.size());
	return dst;
}

void showImage(Mat img) {
	namedWindow("Image", WINDOW_AUTOSIZE);
	imshow("Image", img);
	waitKey(0);
}

void processImages(po::variables_map vm) {
	fs::path srcDir(vm["src"].as<string>());
	fs::path dstDir(vm["dst"].as<string>());
	fs::directory_iterator it(srcDir), eod;
	string filename;

	BOOST_FOREACH(fs::path const & path, std::make_pair(it, eod)) {
		filename = path.filename().string();
		if (fs::is_regular_file(path) && filename[0] != '.') {
			cout << path.string() << endl;
			Mat srcImage = imread(path.string(), 1);
			Mat dstImage = rotate(srcImage, -45);
			imwrite(dstDir.string() + '/' + filename, dstImage);
		}
	}
}

po::variables_map parseArguments(int argc, char const *argv[]) {
	po::variables_map vm;
	po::options_description desc("Allowed options");
	desc.add_options()
	("help", "produce help message")
	("src", po::value<string>(), "set source directory")
	("dst", po::value<string>(), "set destination directory")
	;

	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return vm;
	}

	if (vm.count("src")) {
		cout << "Source directory was set to \""
		     << vm["src"].as<string>() << "\".\n";
	} else {
		cout << "Source directory was not set. Use option \'--src arg\'\n";
	}

	if (vm.count("dst")) {
		cout << "Destination directory was set to \""
		     << vm["dst"].as<string>() << "\".\n";
		if (!fs::is_directory(vm["dst"].as<string>())) {
			cout << "Destination directory was not found but will be created." << endl;
			fs::create_directory(vm["dst"].as<string>());
		}

	} else {
		cout << "Destination directory was not set. Use option \'--dst arg\'\n";
	}
	return vm;
}

int main(int argc, char const *argv[]) {
	processImages(parseArguments(argc, argv));
	return 0;
}