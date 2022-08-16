#pragma once
#include "eigen-3.4.0/Eigen/Dense"
#include <vector>
#include "eigen-3.4.0/Eigen/QR"

// v = y, T = x, a = W
void polyfit(const std::vector<double>& x,
			const std::vector<double>& y,
			std::vector<double>& W,
			int&& degree);
