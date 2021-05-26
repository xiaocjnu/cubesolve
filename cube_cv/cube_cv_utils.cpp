#include "cube_cv_utils.h"

using namespace cv;

namespace cube_cv
{
	cv::Mat im_make_square(const cv::Mat & A)
	{
		int m = A.rows;
		int n = A.cols;

		Mat B;
		if (m < n)
		{
			int r = n - m;
			int start = r / 2;
			B = A(Range::all(), Range(start, start + m));
		}
		else if (m > n)
		{
			int r = m - n;
			int start = r / 2;

			B = A(Range(start, start + n), Range::all());
		}
		else
		{
			B = A;
		}

		return B.clone();
	}

	long long pMat_to_int64(Mat * p)
	{
		return (long long)p;
	}

	cv::Mat * int64_to_pMat(long long a)
	{
		return (cv::Mat *)a;
	}



	void meshgrid(const cv::Range &xgv, const cv::Range &ygv, cv::Mat &X, cv::Mat &Y)
	{
		std::vector<int> t_x, t_y;
		for (int i = xgv.start; i < xgv.end; i++) t_x.push_back(i);
		for (int j = ygv.start; j < ygv.end; j++) t_y.push_back(j);

		cv::repeat(cv::Mat(t_x).t(), (int)t_y.size(), 1, X);
		cv::repeat(cv::Mat(t_y), 1, (int)t_x.size(), Y);
	}


	
}