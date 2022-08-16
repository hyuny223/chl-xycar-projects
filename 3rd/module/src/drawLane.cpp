#include <iostream>
#include <vector>
#include "opencv4/opencv2/opencv.hpp"
#include "drawLane.h"
#include "hyperparam.h"

extern HyperParam p;

void draw_lane(cv::Mat& image, cv::Mat& warp_img, cv::Mat& Minv,
	std::vector<double>& left_fit, std::vector<double>& right_fit)
{
	int yMax = warp_img.rows; // 240
	std::vector<cv::Point> pts(2*yMax);
	cv::Mat color_warp(cv::Size(warp_img.cols, warp_img.rows), CV_8UC3);


	// 구한 파라미터 값으로 y=0~240에 해당하는 x좌표를 구하여, 그 좌표값을 pts에 할당
	for (int i = 0; i < yMax; ++i)
	{
		double left_fitx = left_fit[2] * (i * i) + left_fit[1] * i + left_fit[0];
		double right_fitx = right_fit[2] * (i * i) + right_fit[1] * i + right_fit[0];

		pts[i] = cv::Point(left_fitx, i);
		pts[2*yMax - 1 - i] = cv::Point(right_fitx, i);
	}

	fillPoly(color_warp, pts, cv::Scalar(0, 255, 0));
	cv::Mat newwarp;
	warpPerspective(color_warp, newwarp, Minv, cv::Size(p.Width, p.Height));
	addWeighted(image, 1, newwarp, 0.3, 0, p.lane_img);
}
