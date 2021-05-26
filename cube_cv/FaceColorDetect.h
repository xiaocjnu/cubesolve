#pragma once


#include "cube_cv_utils.h"
#include <array>

namespace cube_cv
{


	class FaceColorDetect
	{
	public:

		// 检测颜色
		bool detect(const cv::Mat& I, cv::Rect rect);
		bool detect_kmeans(const cv::Mat& I, cv::Rect rect);


		// k means 直方图统计
		static std::vector<int> histcounts(const cv::Mat_<int>& I, int k);

		static void imkmeans(const cv::Mat & I, int num_kmeans, double tolX, int max_iter, cv::Mat & labels, cv::Mat & centers);

		// 计算每块的颜色，返回 Lab
		static cv::Vec3f pieceColor(const cv::Mat& I);

		// 识别颜色，失败返回空的 vector
		static std::vector<paster_color_t> color_recognize(const std::array<cv::Vec3f, 54>& colors);


		const cv::Mat& getRaw() const { return myRaw; };
		const cv::Rect& getRect() const { return myRect; };
		const cv::Mat& getLab() const { return myLab; };

		const std::array<int, 4>& getPieceGridX() const { return myPieceGridX; };
		const std::array<int, 4>& getPieceGridY() const { return myPieceGridY; };
		const std::array<cv::Rect, 9>& getPieceRect() const { return myPieceRect; };
		const std::array<cv::Mat, 9>& getPiece() const { return myPiece; };
		const std::array<cv::Mat, 9>& getFeatureSet() const { return myFeatureSet; };
		const std::array<cv::Vec3f, 9>& getPieceColor() const { return myPieceColor; };


		const bool isFound() const { return myFound; };





	protected:
		// 参数


		// 中间结果
		cv::Mat myRaw;
		cv::Rect myRect;
		cv::Mat myLab;

		std::array<int, 4> myPieceGridX;
		std::array<int, 4> myPieceGridY;
		std::array<cv::Rect, 9> myPieceRect;
		std::array<cv::Mat, 9> myPiece;
		std::array<cv::Mat, 9> myFeatureSet;
		std::array<cv::Vec3f, 9> myPieceColor;		// Lab 空间颜色

		bool myFound = false;
		

	};
}