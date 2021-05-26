#pragma once


#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


#include <string>


#ifndef M_PI
#define M_PI 3.14159265358979
#endif

namespace cube_cv
{
	enum paster_color_t {White, Red, Green, Yellow, Orange, Blue, InvalidColor = -1};


	const std::string paster_color_name[] = { "White", "Red", "Green", "Yellow", "Orange", "Blue" };



	// 把图片剪切为正方形
	cv::Mat im_make_square(const cv::Mat& a);


	long long pMat_to_int64(cv::Mat* p);

	cv::Mat* int64_to_pMat(long long a);


	void meshgrid(const cv::Range &xgv, const cv::Range &ygv, cv::Mat &X, cv::Mat &Y);


}