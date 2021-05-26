
#include "cube_cv_utils.h"
#include "FaceRectDetect.h"


using namespace std;
using namespace cv;

namespace cube_cv
{
	cv::Mat FaceRectDetect::shiftRowVector(const cv::Mat& a, int delta)
	{
		assert(a.rows == 1);


		Mat_<double> result;

		if (delta == 0)
			result = a;
		else if (delta > 0) // 右移
		{
			result = Mat_<double>::zeros(1, a.cols);

			for (int i = delta; i < a.cols; ++i)
			{
				result(0, i) = a.at<double>(0, i - delta);
			}
		}
		else // 左移
		{
			delta = -delta;
			result = Mat_<double>::zeros(1, a.cols);

			for (int i = 0; i < a.cols - delta; ++i)
			{
				result(0, i) = a.at<double>(0, i + delta);
			}
		}

		return result;
	}
	int FaceRectDetect::findMaxInRowVector(const Mat& a)
	{
		assert(a.rows == 1);


		int idx = -1;
		double max_a = -std::numeric_limits<double>::infinity();

		for (int i = 0; i < a.cols; ++i)
		{
			double ai = a.at<double>(0, i);
			if (ai > max_a)
			{
				idx = i;
				max_a = ai;
			}
		}

		return idx;
	}
	std::vector<int> FaceRectDetect::findLocalPeak1d(const Mat& x, int radius)
	{
		assert(x.rows == 1);


		std::vector<int> idx;

		int m = x.cols;


		for (int i = 0; i < m; ++i)
		{


			bool is_peak = true;

			for (int j = -radius; j <= radius; ++j)
			{
				int jj = i + j;

				if (jj >= 0 && jj < m && jj != i)
				{
					if (x.at<double>(i) <= x.at<double>(jj))
					{
						is_peak = false;
						break;
					}
				}
			}

			if (is_peak)
			{
				idx.push_back(i);
			}
		}


		// 排序

		struct pv_t
		{
			int idx;
			double v;
		};

		vector<pv_t> pv(idx.size());

		for (int i = 0; i < (int)idx.size(); ++i)
		{
			pv[i].idx = idx[i];
			pv[i].v = x.at<double>(0, idx[i]);
		}



		std::sort(pv.begin(), pv.end(), [](pv_t a, pv_t b) {
			return a.v > b.v;
		});


		for (int i = 0; i < (int)pv.size(); ++i)
		{
			idx[i] = pv[i].idx;
		}

		return idx;
	}
	bool FaceRectDetect::detect(const cv::Mat& I)
	{
		myRaw = I.clone();
		mySquared = im_make_square(myRaw);

		assert(mySquared.rows == mySquared.cols);
		int M = mySquared.rows;


		if (mySquared.channels() > 1)
		{
			cvtColor(mySquared, myGray, COLOR_RGB2GRAY);
		}
		else
		{
			myGray = mySquared.clone();
		}

		Mat h = Mat_<double>{ 0.166666666666667, 0.666666666666667, 0.166666666666667,
			 0.666666666666667, -3.33333333333333, 0.666666666666667,
			 0.166666666666667, 0.666666666666667, 0.166666666666667
		}.reshape(0, 3);

		filter2D(myGray, myEdge, -1, h, Point(-1, -1), 0, BORDER_REPLICATE);

		Mat px;
		Mat py;

		reduce(myEdge, px, 0, REDUCE_SUM, CV_64F);
		reduce(myEdge, py, 1, REDUCE_SUM, CV_64F);

		py = py.t();


		Mat win = Mat_<double>{ 0.0221905, 0.045589, 0.0798114, 0.119065, 0.151361, 0.163967, 0.151361, 0.119065, 0.0798114, 0.045589, 0.0221905 }.reshape(0, 1);
		Mat win2 = win(Range(0, 1), Range(0, 9)).reshape(0, 3);


		Mat px1, py1;


		filter2D(px, px1, -1, win2, Point(-1, 0), 0.0, BORDER_REPLICATE);
		filter2D(py, py1, -1, win, Point(-1, 0), 0.0, BORDER_REPLICATE);


		myEdgePx = px1;
		myEdgePy = py1;



		Mat cc;
		filter2D(px1, cc, -1, py1, Point(-1, 0), 0.0, BORDER_CONSTANT);

		int mid = py1.cols / 2;
		int idx_peak_cc = findMaxInRowVector(cc);
		int delta = idx_peak_cc - mid;


		

		
		Mat py2 = shiftRowVector(py1, delta);


		int c1 = py1.channels();
		int t1 = py1.type();
		int c2 = py2.channels();
		int t2 = py2.type();
		int cx1 = px1.channels();
		int tx1 = px1.type();

		Mat pm = px1 + py2;

		myEdgePyShiftDelta = delta;
		myEdgePyShift = py2;
		myEdgePm = pm;

		vector<int> idx_peak = findLocalPeak1d(pm, 30);

		myPmPeakIdx = idx_peak;

		mySquaredWithRect = mySquared.clone();
		myRect = Rect(0, 0, 0, 0);

		bool found = false;
		
		if (idx_peak.size() >= 4 && pm.at<double>(idx_peak[0]) > 10.0 * M)
		{
			vector<int> idx_cand(4);

			for (int i = 0; i < 4; ++i)
			{
				idx_cand[i] = idx_peak[i];
			}

			sort(idx_cand.begin(), idx_cand.end());

			int dd[3] = { idx_cand[1] - idx_cand[0], idx_cand[2] - idx_cand[1], idx_cand[3] - idx_cand[2] };
			int max_dd = std::max(dd[0], std::max(dd[1], dd[2]));
			int min_dd = std::min(dd[0], std::min(dd[1], dd[2]));



			if (max_dd - min_dd < 50)
			{
				

				int x1 = idx_cand[0];
				int x2 = idx_cand[3];
				int dx = x2 - x1;
				int y1 = x1 - delta;
				int y2 = y1 + dx;

				Rect rect = Rect(x1, y1, dx, dx);


				bool valid = _verify(mySquared, rect);

				if (valid)
				{
					myRect = rect;

					found = true;
					rectangle(mySquaredWithRect, Point(x1, y1), Point(x2, y2), myRectLineColor);
				}


			}// if

		}// if


		myFound = found;

		return myFound;
	}

	bool FaceRectDetect::_verify(const cv::Mat & I, cv::Rect rect)
	{

		const double cut_ratio = 0.25;
		const double sgm_thres = 15.0;
		const double gray_thres = 20.0;
		const int min_width = 6;



		int m = I.rows;
		int n = I.cols;

		if (rect.width != rect.height)
			return false;

		if (rect.width < min_width)
			return false;

		if (rect.x < 0 || rect.y < 0)
			return false;

		if (rect.x + rect.width > n)
			return false;

		if (rect.y + rect.height > m)
			return false;


		int w = rect.width;


		int x[3];
		int y[3];

		x[0] = rect.x;
		x[1] = rect.x + w / 3;
		x[2] = rect.x + 2 * w / 3;

		y[0] = rect.y;
		y[1] = rect.y + w / 3;
		y[2] = rect.y + 2 * w / 3;

		double max_sgm = 0;



		int piece_w = w / 3;
		int win_shift = (int)(piece_w * cut_ratio);
		int win_w = (int)(piece_w * (1.0 - 2 * cut_ratio));


		bool valid = true;


		for (int i = 0; i < 3 && valid; ++i)
		{
			for (int j = 0; j < 3 && valid; ++j)
			{
				if (i == 1 && j == 1)	// 忽略中心块 Logo
					continue;

				Rect rect2(x[i] + win_shift, y[j] + win_shift, win_w, win_w);

				Mat II = I(rect2);
				Mat gray;
				cvtColor(II, gray, COLOR_RGB2GRAY);


				int N = II.rows * II.cols;

				double sgm[3] = { 0,0,0 };
				double tmp[3] = { 0,0,0 };


				for (auto it = II.begin<Vec3b>(); it != II.end<Vec3b>(); ++it)
				{
					double r = (*it)[0];
					double g = (*it)[1];
					double b = (*it)[2];

					sgm[0] += r * r;
					sgm[1] += g * g;
					sgm[2] += b * b;


					tmp[0] += r;
					tmp[1] += g;
					tmp[2] += b;
				}

				for (int k = 0; k < 3; ++k)
				{
					sgm[k] /= N;
					tmp[k] /= N;
					sgm[k] = sgm[k] - tmp[k] * tmp[k];
					sgm[k] = sqrt(sgm[k]);

					if (sgm[k] > max_sgm) {
						max_sgm = sgm[k];
					}
				}


				if (max_sgm > sgm_thres)
				{
					valid = false;
				}

				double av_gray = 0.0;
				for (auto it = gray.begin<uchar>(); it != gray.end<uchar>(); ++it)
				{
					av_gray += (*it);
				}
				av_gray /= N;

				if (av_gray < gray_thres)
				{
					valid = false;
				}
			}
		}







		return valid;
	}

}