#include "CenterColorRecognize.h"

#include "FaceColorDetect.h"

#include <array>
#include <vector>
#include <algorithm>


using namespace std;
using namespace cv;

namespace cube_cv
{
	// 角度差，范围是 [-180, 180)
	static double _deltaAngle(double a, double b)
	{
		double d = a - b;

		while (d >= 180.0)
		{
			d -= 360.0;
		}

		while (d < -180.0)
		{
			d += 360.0;
		}

		return d;
	}



	static double _distance_ab(Vec3f a, Vec3f b)
	{
		double a1 = a[1];
		double a2 = a[2];

		double b1 = b[1];
		double b2 = b[2];

		double d1 = b1 - a1;
		double d2 = b2 - a2;

		return sqrt(d1 * d1 + d2 * d2);
	}


	// ==========================================================
	CenterColorRecognize::CenterColorRecognize()
	{
		clear();
	}

	void CenterColorRecognize::clear()
	{
		myInColor.clear();
		myOutColor.clear();
	}


	int CenterColorRecognize::push(cv::Vec3f color)
	{
		if (empty())
		{
			myInColor.push_back(color);
			myOutColor.push_back(_recognize_first(color));
			return 0;
		}



		paster_color_t raw_color = _recognize_first(color);

		if (raw_color == -1)
			return -1;


		int num = getNum();

		double cur_a = color[1];
		double cur_b = color[2];
		double cur_ang = atan2(cur_b, cur_a) * 180.0 / M_PI;
		double cur_d = sqrt(cur_a * cur_a + cur_b * cur_b);

		vector<double> dd(num), ang(num), aa(num), bb(num);

		for (int i = 0; i < num; ++i)
		{
			double ai = myInColor[i][1];
			double bi = myInColor[i][2];

			ang[i] = atan2(bi, ai) * 180.0 / M_PI;
			dd[i] = sqrt(ai*ai + bi * bi);
			aa[i] = ai;
			bb[i] = bi;
		}

		array<int, 6> color_idx = { -1,-1,-1,-1,-1,-1 };

		for (int i = 0; i < num; ++i)
		{
			for (int j = 0; j < 6; ++j)
			{
				if (myOutColor[i] == j)
					color_idx[j] = i;
			}
		}

		// 处理颜色
		if (raw_color == White || raw_color == Blue)	// 白色和蓝色
		{
			int idx = color_idx[raw_color];

			if (idx >= 0)	// 重复
			{
				myInColor[idx] = color;
				return idx;
			}
			else
			{
				myInColor.push_back(color);
				myOutColor.push_back(raw_color);
				return num;
			}
		}
		else // 红橙黄绿
		{
			// 重新计算颜色
			int royg[] = { Red, Orange, Yellow, Green };

			vector<int> idx;
			for (int j = 0; j < 4; ++j)
			{
				int ii = color_idx[royg[j]];

				if (ii >= 0)
					idx.push_back(ii);
			}


			if (idx.empty())
			{
				myInColor.push_back(color);
				myOutColor.push_back(raw_color);
				return num;
			}

			


			const double tol_ang = 5.0;
			const double tol_d = 10.0;

			if (raw_color == Green || raw_color == Yellow) // 绿黄
			{
				// 查找离当前 ang 最近			
				partial_sort(idx.begin(), idx.begin() + 1, idx.end(), [&](int i, int j)->bool {
					double ai = _deltaAngle(ang[i], cur_ang);
					double aj = _deltaAngle(ang[j], cur_ang);

					return abs(ai) < abs(aj);
				});

				int closest_idx = idx[0];


				if (abs(_deltaAngle(ang[closest_idx], cur_ang)) < tol_ang)	// 角度太小，认为一样
				{
					myInColor[closest_idx] = color;
					return closest_idx;
				}
				else // 角度不同
				{
					int idx_g = color_idx[Green];
					int idx_y = color_idx[Yellow];

					int _n = (idx_g >= 0 ? 1 : 0) + (idx_y >= 0 ? 1 : 0);

					if (_n == 0)
					{
						myInColor.push_back(color);
						myOutColor.push_back(raw_color);
						return num;
					}
					else if (_n == 1)
					{
						int idx1 = (idx_g >= 0 ? idx_g : idx_y);

						double da = _deltaAngle(cur_ang, ang[idx1]);

						if (da > 0)
						{
							myOutColor[idx1] = Yellow;

							myInColor.push_back(color);
							myOutColor.push_back(Green);
						}
						else
						{
							myOutColor[idx1] = Green;

							myInColor.push_back(color);
							myOutColor.push_back(Yellow);
						}

						return num;
					}
					else if (_n == 2)
					{
						double da_g = _deltaAngle(ang[idx_g], cur_ang);
						double da_y = _deltaAngle(ang[idx_y], cur_ang);

						if (abs(da_g) < abs(da_y))
						{
							myInColor[idx_g] = color;
							return idx_g;
						}
						else
						{
							myInColor[idx_y] = color;
							return idx_y;
						}
					}

					assert(false);
				}// if
			}
			else // 红橙
			{
				// 查找离当前距离最近			
				partial_sort(idx.begin(), idx.begin() + 1, idx.end(), [&](int i, int j)->bool {
					double di = _distance_ab(myInColor[i], color);
					double dj = _distance_ab(myInColor[j], color);
					return di < dj;
				});

				int closest_idx = idx[0];


				if (_distance_ab(myInColor[closest_idx], color) < tol_d)	// 距离太近
				{
					myInColor[closest_idx] = color;
					return closest_idx;
				}
				else // 距离足够远
				{
					int idx_r = color_idx[Red];
					int idx_o = color_idx[Orange];

					int _n = (idx_r >= 0 ? 1 : 0) + (idx_o >= 0 ? 1 : 0);

					if (_n == 0)
					{
						myInColor.push_back(color);
						myOutColor.push_back(raw_color);
						return num;
					}
					else if (_n == 1)
					{
						int idx1 = (idx_r >= 0 ? idx_r : idx_o);

						double da = _deltaAngle(cur_ang, ang[idx1]);

						if (da > tol_ang)
						{
							myOutColor[idx1] = Red;

							myInColor.push_back(color);
							myOutColor.push_back(Orange);
						}
						else if (da < -tol_ang)
						{
							myOutColor[idx1] = Orange;

							myInColor.push_back(color);
							myOutColor.push_back(Red);
						}
						else // 角度不能区分，离原点距离
						{
							double _d = cur_d - dd[idx1];

							if (_d > 0)
							{
								myOutColor[idx1] = Red;

								myInColor.push_back(color);
								myOutColor.push_back(Orange);
							}
							else
							{
								myOutColor[idx1] = Orange;

								myInColor.push_back(color);
								myOutColor.push_back(Red);
							}

						}

						return num;
					}
					else if (_n == 2)
					{
						double dd_r = _distance_ab(myInColor[idx_r], color);
						double dd_o = _distance_ab(myInColor[idx_o], color);

						if (dd_r < dd_o)
						{
							myInColor[idx_r] = color;
							return idx_r;
						}
						else
						{
							myInColor[idx_o] = color;
							return idx_o;
						}
					}

					assert(false);
				}// if

				assert(false);
			}// if




		
		}// if


		assert(false);
		return -1;

	}

	cv::Vec3f CenterColorRecognize::centerColor(const cv::Mat & I, cv::Rect rect)
	{
		Rect rect2(rect.x + rect.width / 3, rect.y + rect.height / 3, rect.width / 3, rect.height / 3);

		Mat piece = I(rect2);


		// 转为float, 提出RGB 3通道

		if (piece.depth() != CV_32F || piece.channels() != 3)
		{
			Mat I2 = Mat(piece.rows, piece.cols, CV_MAKETYPE(piece.depth(), 3));
			mixChannels(piece, I2, vector<int>{0, 0, 1, 1, 2, 2});
			I2.convertTo(I2, CV_32FC3);
			I2 /= 255.0;

			piece = I2;
		}

		Mat piece_erode;
		Mat se = Mat::ones(5, 5, CV_32F);
		erode(1.0f - piece, piece_erode, se);
		piece_erode = 1.0f - piece_erode;


		return FaceColorDetect::pieceColor(piece_erode);

	}

	paster_color_t CenterColorRecognize::_recognize_first(cv::Vec3f color)
	{

		double L = color[0];
		double a = color[1];
		double b = color[2];



		// 靠近原点且亮度足够大的是白色
		double d = sqrt(a*a + b * b);
		if (L >= 30.0 && d <= 20.0)
			return White;

		// 太黑忽略
		if (L < 10.0)
			return InvalidColor;


		// 按角度区分颜色
		const double boundary_gb = -135.0;
		const double boundary_br = -45.0;
		const double boundary_ro = 22.5;
		const double boundary_oy = 75.0;
		const double boundary_yg = 120.0;



		double ang = atan2(b, a) * 180 / M_PI;

		if (boundary_gb <= ang && ang < boundary_br)
			return Blue;

		if (boundary_br <= ang && ang < boundary_ro)
			return Red;

		if (boundary_ro <= ang && ang < boundary_oy)
			return Orange;

		if (boundary_oy <= ang && ang < boundary_yg)
			return Yellow;

		return Green;
	}

}

