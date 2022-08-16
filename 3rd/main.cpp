#include <iostream>
#include <vector>
#include "opencv4/opencv2/opencv.hpp"
#include "warpProcessImage.h"
#include "hyperparam.h"
#include "drawLane.h"


using namespace std;
using namespace cv;


HyperParam p;


int main()
{
	VideoCapture cap("/home/chl/Videos/subProject.avi");

	if (!cap.isOpened())
	{
		cerr << "Image open failed!\n";

		return -1;
	}

	Mat frame, M, Minv, warp_img;
	cv::Point2f warp_src[4], warp_dst[4];

	warp_src[0] = cv::Point2f(20 - p.warpx_margin, 300 - p.warpy_margin);
	warp_src[1] = cv::Point2f(20 - p.warpx_margin, 390 + p.warpy_margin);
	warp_src[2] = cv::Point2f(640-20 + p.warpx_margin, 300 - p.warpy_margin);
	warp_src[3] = cv::Point2f(640-20 + p.warpx_margin, 390 + p.warpy_margin);

	warp_dst[0] = cv::Point2f(0, 0);
	warp_dst[1] = cv::Point2f(0, p.warp_img_h);
	warp_dst[2] = cv::Point2f(p.warp_img_w, 0);
	warp_dst[3] = cv::Point2f(p.warp_img_w, p.warp_img_h);

	while (true)
	{
		cap >> frame;

		if (frame.empty())
		{
			cout << "Image is empty!\n";
			break;
		}

		cv::Mat M = cv::getPerspectiveTransform(warp_src, warp_dst);
		cv::Mat Minv = cv::getPerspectiveTransform(warp_dst, warp_src);
		cv::warpPerspective(frame, warp_img, M, Size(p.warp_img_w, p.warp_img_h));
		vector<vector<double>> fit = warp_process_image(warp_img);
		draw_lane(frame, warp_img, Minv, fit[0], fit[1]);

		cv::imshow("warp_img", warp_img);
		cv::imshow("camera", p.lane_img);
		if(cv::waitKey(1)==27) break;
	}
}
