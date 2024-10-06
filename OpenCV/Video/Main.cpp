#include <cstdlib>
#include <iostream>
#include <format>
#include <filesystem>

#include "../CV.h"

int main() 
{
	//!< �r���h���
	std::cout << cv::getBuildInformation() << std::endl;

#if true
	//!< Gstreamer
	//cv::VideoCapture Capture("gst-launch-1.0 videotestsrc ! videoconvert ! appsink", cv::CAP_GSTREAMER);
	cv::VideoCapture Capture("gst-launch-1.0 videotestsrc ! appsink", cv::CAP_GSTREAMER);
#elif false
	//!< �J�����摜
	//cv::VideoCapture Capture(0);
#else
	//!< ����
	cv::VideoCapture Capture("01.mp4");
#endif

	if (!Capture.isOpened()) {
		std::cerr << "VideoCapture open failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	cv::Mat Frame;
	constexpr auto ESC = 27;
	while (true) {
		Capture >> Frame;
		
		cv::imshow("Video", Frame);

		if (ESC == cv::pollKey()) {
			break;
		}
	}
}