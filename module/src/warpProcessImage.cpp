#include <iostream>
#include <vector>
#include "opencv4/opencv2/opencv.hpp"
#include "hyperparam.h"
#include "polyfit.h"
#include "warpProcessImage.h"

using namespace std;
using namespace cv;


extern HyperParam p;

vector<vector<double>> warp_process_image(Mat& img)
{
	++p.frame;
	Mat blur, dst, lane;
	vector<Mat> planes(3);

	// warp_img 영역 평균값 구하기
	Scalar m = mean(img);


	//roi 영역 평균 밝기 조정
	add(img, -m + Scalar(60,60,60), img);

	GaussianBlur(img, blur, Size(1, 0), 2);

	cvtColor(blur, dst, COLOR_BGR2HLS);
	split(dst, planes);

	threshold(planes[1],lane, 20, 255, THRESH_BINARY);
	imshow("lane", lane);


	// histogram
	int col = lane.cols, row = lane.rows;	// col = 320, row = 240;
	int leftx_current = 0, rightx_current = col;
	int ll = 0, rr = 0;
	int window_height = row / p.nwindows;
	bool l_flag = false, r_flag = false;
	bool ll_f = false, rr_f = false;
	for (int x = p.center; x > 0; --x)
	{

		if ( lane.at<uchar>(row - 10, x) == 0 )
		{

			leftx_current = x;
			l_flag = true;
			break;
		}
	}

	for (int x = p.center; x < col; ++x)
	{

		if ( lane.at<uchar>(row - 10, x) == 0 )
		{
			rightx_current = x;
			r_flag = true;
			break;
		}
	}

	// 오차로 값 추정
	if (l_flag == false && r_flag == true)
	{
		leftx_current = max(0, rightx_current - 260);
	}

	if (l_flag == true && r_flag == false)
	{
		rightx_current = min(rightx_current + 260, col);
	}

	cout << "l_pos : " << leftx_current << ", rpos : " << rightx_current << "\n";
	cout << "----------------------------------\n";



	// lane.nonzero(), nz[0]은 0이 아닌 row값, nz[1]은 0이 아닌 col값
	vector<vector<int>> nz(2);

	for (int y = 0; y < row; ++y)
	{
		for (int x = 0; x < col; ++x)
		{
			if (lane.at<uchar>(y,x) == 0)
			{
				nz[0].push_back(y);
				nz[1].push_back(x);
			}
		}
	}

	vector<vector<int>> left_lane_inds, right_lane_inds;
	vector<double> lx(p.nwindows), ly(p.nwindows), rx(p.nwindows), ry(p.nwindows); // 윈도우를 대표하는 x,y 좌표값을 담을 윈도우 갯수만큼의 벡터

	Mat out_img = Mat::zeros(row, col, CV_8UC3);



	// 윈도우 설정
	for (int window = 0; window < p.nwindows; ++window)
	{
		int win_yl = row - (window + 1) * window_height; // 윈도우의 윗쪽 y
		int win_yh = row - window * window_height; // 윈도우의 아랫쪽 y

		int win_xll = leftx_current - p.margin; // 왼왼 xll
		int win_xlr = leftx_current + p.margin; // 왼오 xlr
 		int win_xrl = rightx_current - p.margin; // 오왼 xrl
		int win_xrr = rightx_current + p.margin; // 오오 xrr


		// 윈도우 쌓기
		rectangle(out_img, Point(win_xll, win_yl), Point(win_xlr, win_yh), Scalar(0, 255, 0), 2);
		rectangle(out_img, Point(win_xrl, win_yl), Point(win_xrr, win_yh), Scalar(0, 255, 0), 2);


		vector<int> good_left_inds, good_right_inds;
		vector<int> tmp_row;
		vector<int> tmp_col_l;
		vector<int> tmp_col_r;



		// 윈도우 범위에 있는 y좌표를 1로, 그 외는 0으로 만들기
		for (auto& nz_row : nz[0])
		{
			if (win_yl <= nz_row && nz_row < win_yh)
			{
				tmp_row.push_back(1);
			}
			else
			{
				tmp_row.push_back(0);
			}
		}


		// 윈도우 범위에 있는 x좌표를 1로, 그 외는 0으로 만들기
		// 아마 이 부분에서 좌우 x 좌표가 뭉치는? 현상이 생기는 것 같다. 이 부분 처리를 잘 해야할 듯
		for (auto& nz_col : nz[1])
		{
			if (win_xll <= nz_col && nz_col < win_xlr)
			{
				tmp_col_l.push_back(1); // true
			}
			else
			{
				tmp_col_l.push_back(0); // false
			}

			if (win_xrl <= nz_col && nz_col < win_xrr)
			{
				tmp_col_r.push_back(1); // true
			}
			else
			{
				tmp_col_r.push_back(0); // false
			}
		}


		// 범위 내에 있는 x,y 값 찾기. index가 겹치면(즉, 1이 되는 요소는) 좌표가 되는 것이다.
		cv::bitwise_and(tmp_row, tmp_col_l, tmp_col_l);
		cv::bitwise_and(tmp_row, tmp_col_r, tmp_col_r);

		// find nonzero index. nz[0][index]는 y값, nz[1][index]는 x값이 된다.
		for (int i = 0; i < tmp_col_l.size(); ++i)
		{
			if (tmp_col_l[i] == 1)
			{
				good_left_inds.push_back(i);
			}

			if (tmp_col_r[i] == 1)
			{
				good_right_inds.push_back(i);
			}
		}

		left_lane_inds.push_back(good_left_inds);
		right_lane_inds.push_back(good_right_inds);

		if (good_left_inds.size() > p.coordinate_threshold)
		{
			for (auto lx : good_left_inds)
			{
				leftx_current = max(leftx_current, nz[1][lx]);
			}
		}

		if (good_right_inds.size() > p.coordinate_threshold)
		{
			for (auto rx : good_right_inds)
			{
				rightx_current = min(rightx_current, nz[1][rx]);
			}
		}

		// 각 대표 값을 lx, ly, rx, ry에 할당한다.
		double h = (win_yl + win_yh) / 2;
		lx[window] = (double)leftx_current;
		ly[window] = h;

		rx[window] = (double)rightx_current;
		ry[window] = h;
	}


	// nz[0][index] = y좌표, nz[1][index] = x좌표. 즉 점의 위치의 스칼라 값을 250, 0, 0으로 변경
	for (auto& l : left_lane_inds)
	{
		for (auto& e : l)
		{
			out_img.at<Vec3b>(nz[0][e], nz[1][e]) = Vec3b(255, 0, 0);
		}
	}


	// nz[0][index] = y좌표, nz[1][index] = x좌표. 즉 점의 위치의 스칼라 값을 250, 0, 0으로 변경
	for (auto& r : right_lane_inds)
	{
		for (auto& e : r)
		{
			out_img.at<Vec3b>(nz[0][e], nz[1][e]) = Vec3b(0, 0, 255);
		}
	}
	/* 위 부분에서 점이 굵게 찍힌다는 것은, 좌표값에 대한 필터링이 필요하다는 것*/
	/* 위 부분에서 점이 굵게 찍힌다는 것은, 좌표값에 대한 필터링이 필요하다는 것*/
	/* 위 부분에서 점이 굵게 찍힌다는 것은, 좌표값에 대한 필터링이 필요하다는 것*/


	// 파라미터 근사값 구하기. 즉 가장 잘 나타내는 2차 함수 파라미터 값 구하기
	vector<double> lfit, rfit;
	polyfit(ly, lx, lfit, 2);
	polyfit(ry, rx, rfit, 2);

	cv::imshow("viewer", out_img);

	return { lfit, rfit };
}
