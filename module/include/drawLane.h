#pragma once

#include <iostream>
#include <vector>
#include "opencv4/opencv2/opencv.hpp"
#include "hyperparam.h"

extern HyperParam p;

void draw_lane(cv::Mat& image, cv::Mat& warp_img, cv::Mat& Minv,
	std::vector<double>& left_fit, std::vector<double>& right_fit);
