#pragma once

#include <iostream>
#include <vector>
#include "opencv4/opencv2/opencv.hpp"
#include "hyperparam.h"
#include "polyfit.h"

using namespace std;
using namespace cv;


extern HyperParam p;

vector<vector<double>> warp_process_image(Mat& img);
