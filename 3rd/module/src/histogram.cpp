// #include <iostream>
// #include <cmath>
// #include <vector>
// #include "opencv4/opencv2/opencv.hpp"
// #include "eigen-3.4.0/Eigen/Dense"
// #include "eigen-3.4.0/Eigen/QR"

// using namespace std;
// using namespace cv;

// int nwindows = 20, margin = 20, minpix = 5;
// int Width = 640, Height = 480;
// int warp_img_w = 320, warp_img_h = 240;
// int warpx_margin = 20, warpy_margin = 3;
// int lane_bin_th = 145;
// int coordinate_threshold = 5;
// Mat lane_img;


// // v = y, T = x, a = W
// void polyfit(const std::vector<double>& x,
// 			const std::vector<double>& y,
// 			std::vector<double>& W,
// 			int&& degree)
// {
// 	// Create Matrix Placeholder of size n x k, n= number of datapoints, k = degree of polynomial, for exame k = 3 for cubic polynomial
// 	Eigen::MatrixXd X(x.size(), degree + 1); // ax^2 + bx + c라면, 3개의 파라미터가 필요
// 	Eigen::VectorXd Y = Eigen::VectorXd::Map(&y.front(), y.size()); // Map은 뭐지
// 	Eigen::VectorXd result;

// 	// check to make sure inputs are correct
// 	assert(x.size() == y.size()); //x에 대한 y의 수가 1:1인지
// 	assert(y.size() >= degree + 1); // ??

// 	// Populate the matrix
// 	for (size_t i = 0; i < x.size(); ++i)
// 	{
// 		for (size_t j = 0; j < degree + 1; ++j)
// 		{
// 			X(i, j) = pow(x.at(i), j); // x_i ** 0 + x_i ** 1 + x_i ** 2
// 		}
// 	}
// 	// std::cout << X << std::endl;

// 	// Solve for linear least square fit
// 	result = X.householderQr().solve(Y);
// 	W.resize(degree + 1);

// 	for (int k = 0; k < degree + 1; k++)
// 	{
// 		W[k] = result[k];
// 	}
// }

// vector<vector<double>> warp_process_image(Mat& img)
// {
// 	Mat blur, dst, lane;
// 	vector<Mat> planes(3);

// 	// warp_img 영역 평균값 구하기
// 	Scalar m = mean(img);


// 	//roi 영역 평균 밝기 조정
// 	add(img, -m + Scalar(70,70,70), img);

// 	GaussianBlur(img, blur, Size(0, 0), 3.5);
// 	// GaussianBlur(img, blur, Size(0, 0), 3.5);

// 	cvtColor(blur, dst, COLOR_BGR2HLS);
// 	split(dst, planes);

// 	threshold(planes[1],lane, 40, 255, THRESH_BINARY);
// 	imshow("lane", lane);


// 	// histogram
// 	int col = lane.cols, row = lane.rows;	// col = 320, row = 240;
// 	vector<int> hist(col);

// 	for (int y = 0; y < row; y++)
// 	{
// 		for (int x = 0; x < col; x++)
// 		{
// 			if (lane.at<uchar>(y, x) == 0)
// 			{
// 				hist[x]++;
// 			}
// 		}
// 	}

// 	//for (int i = 0; i < hist.size(); ++i)
// 	//{
// 	//	cout << "index : " << i << " and value : " << hist[i] << '\n';
// 	//}


// 	// argmax 구하기!
// 	int midpoint = col / 2;

// 	auto leftx_current = max_element(hist.begin(), hist.begin() + midpoint) - hist.begin();
// 	auto rightx_current = max_element(hist.begin() + midpoint, hist.end()) - hist.begin();
// 	int window_height = row / nwindows;



// 	// lane.nonzero(), nz[0]은 0이 아닌 row값, nz[1]은 0이 아닌 col값
// 	vector<vector<int>> nz(2);

// 	for (int y = 0; y < row; ++y)
// 	{
// 		for (int x = 0; x < col; ++x)
// 		{
// 			if (lane.at<uchar>(y,x) == 0)
// 			{
// 				nz[0].push_back(y);
// 				nz[1].push_back(x);
// 			}
// 		}
// 	}

// 	vector<vector<int>> left_lane_inds, right_lane_inds;
// 	vector<double> lx(nwindows), ly(nwindows), rx(nwindows), ry(nwindows); // 윈도우를 대표하는 x,y 좌표값을 담을 윈도우 갯수만큼의 벡터

// 	Mat out_img = Mat::zeros(row, col, CV_8UC3);



// 	// 윈도우 설정
// 	for (int window = 0; window < nwindows; ++window)
// 	{
// 		int win_yl = row - (window + 1) * window_height; // 윈도우의 윗쪽 y
// 		int win_yh = row - window * window_height; // 윈도우의 아랫쪽 y

// 		int win_xll = leftx_current - margin; // 왼왼 xll
// 		int win_xlr = leftx_current + margin; // 왼오 xlr
//  		int win_xrl = rightx_current - margin; // 오왼 xrl
// 		int win_xrr = rightx_current + margin; // 오오 xrr


// 		// 윈도우 쌓기
// 		rectangle(out_img, Point(win_xll, win_yl), Point(win_xlr, win_yh), Scalar(0, 255, 0), 2);
// 		rectangle(out_img, Point(win_xrl, win_yl), Point(win_xrr, win_yh), Scalar(0, 255, 0), 2);


// 		vector<int> good_left_inds, good_right_inds;
// 		vector<int> tmp_row;
// 		vector<int> tmp_col_l;
// 		vector<int> tmp_col_r;


// 		// 윈도우 범위에 있는 y좌표를 1로, 그 외는 0으로 만들기
// 		for (auto& nz_row : nz[0])
// 		{
// 			if (win_yl <= nz_row && nz_row < win_yh)
// 			{
// 				tmp_row.push_back(1);
// 			}
// 			else
// 			{
// 				tmp_row.push_back(0);
// 			}
// 		}


// 		// 윈도우 범위에 있는 x좌표를 1로, 그 외는 0으로 만들기
// 		// 아마 이 부분에서 좌우 x 좌표가 뭉치는? 현상이 생기는 것 같다. 이 부분 처리를 잘 해야할 듯
// 		for (auto& nz_col : nz[1])
// 		{
// 			if (win_xll <= nz_col && nz_col < win_xlr)
// 			{
// 				tmp_col_l.push_back(1); // true
// 			}
// 			else
// 			{
// 				tmp_col_l.push_back(0); // false
// 			}

// 			if (win_xrl <= nz_col && nz_col < win_xrr)
// 			{
// 				tmp_col_r.push_back(1); // true
// 			}
// 			else
// 			{
// 				tmp_col_r.push_back(0); // false
// 			}
// 		}


// 		// 범위 내에 있는 x,y 값 찾기. index가 겹치면(즉, 1이 되는 요소는) 좌표가 되는 것이다.
// 		cv::bitwise_and(tmp_row, tmp_col_l, tmp_col_l);
// 		cv::bitwise_and(tmp_row, tmp_col_r, tmp_col_r);

// 		// find nonzero index. nz[0][index]는 y값, nz[1][index]는 x값이 된다.
// 		for (int i = 0; i < tmp_col_l.size(); ++i)
// 		{
// 			if (tmp_col_l[i] == 1)
// 			{
// 				good_left_inds.push_back(i);
// 			}

// 			if (tmp_col_r[i] == 1)
// 			{
// 				good_right_inds.push_back(i);
// 			}
// 		}

// 		left_lane_inds.push_back(good_left_inds);
// 		right_lane_inds.push_back(good_right_inds);


// 		// 좌표가 coordinate_threshold개 초과하면 leftx_current 갱신
// 		// lx 값의 평균
// 		if (good_left_inds.size() > coordinate_threshold)
// 		{
// 			leftx_current = 0;
// 			for (auto lx : good_left_inds)
// 			{
// 				leftx_current += nz[1][lx];
// 			}

// 			leftx_current = (int)(leftx_current / good_left_inds.size());
// 		}

// 		// 좌표가 coordinate_threshold개 초과하면 rightx_current 갱신
// 		// rx 값의 평균
// 		if (good_right_inds.size() > coordinate_threshold)
// 		{
// 			rightx_current = 0;
// 			for (auto rx : good_right_inds)
// 			{
// 				rightx_current += nz[1][rx];
// 			}
// 			rightx_current = (int)(rightx_current / good_right_inds.size());
// 		}


// 		// 각 대표 값을 lx, ly, rx, ry에 할당한다.
// 		double h = (win_yl + win_yh) / 2;
// 		lx[window] = (double)leftx_current;
// 		ly[window] = h;

// 		rx[window] = (double)rightx_current;
// 		ry[window] = h;
// 	}


// 	// nz[0][index] = y좌표, nz[1][index] = x좌표. 즉 점의 위치의 스칼라 값을 250, 0, 0으로 변경
// 	for (auto& l : left_lane_inds)
// 	{
// 		for (auto& e : l)
// 		{
// 			out_img.at<Vec3b>(nz[0][e], nz[1][e]) = Vec3b(255, 0, 0);
// 		}
// 	}


// 	// nz[0][index] = y좌표, nz[1][index] = x좌표. 즉 점의 위치의 스칼라 값을 250, 0, 0으로 변경
// 	for (auto& r : right_lane_inds)
// 	{
// 		for (auto& e : r)
// 		{
// 			out_img.at<Vec3b>(nz[0][e], nz[1][e]) = Vec3b(0, 0, 255);
// 		}
// 	}
// 	/* 위 부분에서 점이 굵게 찍힌다는 것은, 좌표값에 대한 필터링이 필요하다는 것*/
// 	/* 위 부분에서 점이 굵게 찍힌다는 것은, 좌표값에 대한 필터링이 필요하다는 것*/
// 	/* 위 부분에서 점이 굵게 찍힌다는 것은, 좌표값에 대한 필터링이 필요하다는 것*/


// 	// 파라미터 근사값 구하기. 즉 가장 잘 나타내는 2차 함수 파라미터 값 구하기
// 	vector<double> lfit, rfit;
// 	polyfit(ly, lx, lfit, 2);
// 	polyfit(ry, rx, rfit, 2);

// 	cv::imshow("viewer", out_img);

// 	return { lfit, rfit };
// }

// void draw_lane(Mat& image, Mat& warp_img, Mat& Minv,
// 	vector<double>& left_fit, vector<double>& right_fit)
// {
// 	int yMax = warp_img.rows; // 240
// 	vector<Point> pts(2*yMax);
// 	Mat color_warp(Size(warp_img.cols, warp_img.rows), CV_8UC3);


// 	// 구한 파라미터 값으로 y=0~240에 해당하는 x좌표를 구하여, 그 좌표값을 pts에 할당
// 	for (int i = 0; i < yMax; ++i)
// 	{
// 		double left_fitx = left_fit[2] * (i * i) + left_fit[1] * i + left_fit[0];
// 		double right_fitx = right_fit[2] * (i * i) + right_fit[1] * i + right_fit[0];

// 		pts[i] = Point(left_fitx, i);
// 		pts[2*yMax - 1 - i] = Point(right_fitx, i);
// 	}

// 	fillPoly(color_warp, pts, Scalar(0, 255, 0));
// 	Mat newwarp;
// 	warpPerspective(color_warp, newwarp, Minv, Size(Width, Height));
// 	addWeighted(image, 1, newwarp, 0.3, 0, lane_img);
// }

// int main()
// {
// 	VideoCapture cap("/home/chl/Videos/subProject.avi");

// 	if (!cap.isOpened())
// 	{
// 		cerr << "Image open failed!\n";

// 		return -1;
// 	}

// 	Mat frame, M, Minv, warp_img;
// 	cv::Point2f warp_src[4], warp_dst[4];

// 	// 설정 중요!!!
// 	// 캘리브레이션 필!!!
// 	warp_src[0] = cv::Point2f(20 - warpx_margin, 300 - warpy_margin);
// 	warp_src[1] = cv::Point2f(20 - warpx_margin, 390 + warpy_margin);
// 	warp_src[2] = cv::Point2f(640-20 + warpx_margin, 300 - warpy_margin);
// 	warp_src[3] = cv::Point2f(640-20 + warpx_margin, 390 + warpy_margin);

// 	warp_dst[0] = cv::Point2f(0, 0);
// 	warp_dst[1] = cv::Point2f(0, warp_img_h);
// 	warp_dst[2] = cv::Point2f(warp_img_w, 0);
// 	warp_dst[3] = cv::Point2f(warp_img_w, warp_img_h);

// 	while (true)
// 	{
// 		cap >> frame;

// 		if (frame.empty())
// 		{
// 			cout << "Image is empty!\n";
// 			break;
// 		}

// 		cv::Mat M = cv::getPerspectiveTransform(warp_src, warp_dst);
// 		cv::Mat Minv = cv::getPerspectiveTransform(warp_dst, warp_src);
// 		cv::warpPerspective(frame, warp_img, M, Size(warp_img_w, warp_img_h));
// 		vector<vector<double>> fit = warp_process_image(warp_img);
// 		draw_lane(frame, warp_img, Minv, fit[0], fit[1]);

// 		cv::imshow("warp_img", warp_img);
// 		cv::imshow("camera", lane_img);
// 		if (waitKey(0) == 27) break;
// 	}
// }
