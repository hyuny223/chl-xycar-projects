#include "eigen-3.4.0/Eigen/Dense"
#include <vector>
#include "eigen-3.4.0/Eigen/QR"
#include "polyfit.h"

// v = y, T = x, a = W
void polyfit(const std::vector<double>& x,
			const std::vector<double>& y,
			std::vector<double>& W,
			int&& degree)
{
	// Create Matrix Placeholder of size n x k, n= number of datapoints, k = degree of polynomial, for exame k = 3 for cubic polynomial
	Eigen::MatrixXd X(x.size(), degree + 1); // ax^2 + bx + c라면, 3개의 파라미터가 필요
	Eigen::VectorXd Y = Eigen::VectorXd::Map(&y.front(), y.size()); // Map은 뭐지
	Eigen::VectorXd result;

	// check to make sure inputs are correct
	assert(x.size() == y.size()); //x에 대한 y의 수가 1:1인지
	assert(y.size() >= degree + 1); // ??

	// Populate the matrix
	for (size_t i = 0; i < x.size(); ++i)
	{
		for (size_t j = 0; j < degree + 1; ++j)
		{
			X(i, j) = pow(x.at(i), j); // x_i ** 0 + x_i ** 1 + x_i ** 2
		}
	}
	// std::cout << X << std::endl;

	// Solve for linear least square fit
	result = X.householderQr().solve(Y);
	W.resize(degree + 1);

	for (int k = 0; k < degree + 1; k++)
	{
		W[k] = result[k];
	}
}
