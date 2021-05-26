#pragma once


#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

namespace cube_cv
{
	class FaceRectDetect
	{
	public:
		FaceRectDetect() {};


		// 对 row vector 进行移动， delta > 0 右移， delta < 0 左移， delta == 0 返回引用
		static cv::Mat shiftRowVector(const cv::Mat& a, int delta);

		// 查找 row vector 最大值下标
		static int findMaxInRowVector(const cv::Mat& a);


		// 查找极点，并按值降序输出下标
		static std::vector<int> findLocalPeak1d(const cv::Mat& x, int radius);


		// 检测
		bool detect(const cv::Mat& I);

		cv::Rect getRect() const { return myRect; };
		bool isFound() const { return myFound; };



		
		cv::Scalar getRectLineColor() const { return myRectLineColor; };
		void setRectLineColor(const cv::Scalar& c) { myRectLineColor = c; };


		const cv::Mat& getRaw() const { return myRaw; };
		const cv::Mat& getSquared() const { return mySquared; };
		const cv::Mat& getGray() const { return myGray; };
		const cv::Mat& getEdge() const { return myEdge; };
		const cv::Mat& getEdgePx() const { return myEdgePx; };
		const cv::Mat& getEdgePy() const { return myEdgePy; };
		int getEdgePyShiftDelta() const { return myEdgePyShiftDelta; };
		const cv::Mat& getEdgePyShift() const { return myEdgePyShift; };
		const cv::Mat& getEdgePm() const { return myEdgePm; };
		const std::vector<int>& getPmPeakIdx() const { return myPmPeakIdx; };
		const cv::Mat& getSquaredWithRect() const { return mySquaredWithRect; };


	protected:
		bool _verify(const cv::Mat& I, cv::Rect rect);




	protected:
		// 参数
		cv::Scalar myRectLineColor = { 255,255,255 };		// myRawWithRect 画线颜色


		// 检测的中间变量
		cv::Mat myRaw;		// 原始图片
		cv::Mat mySquared;	// 正方形剪切后图片
		cv::Mat myGray;		// 灰度图像
		cv::Mat myEdge;		// 边沿灰度图像
		cv::Mat myEdgePx;	// myEdge x 轴的累积
		cv::Mat myEdgePy;	// myEdge x 轴的累积的转置，row vector
		int myEdgePyShiftDelta;
		cv::Mat myEdgePyShift;	// myEdgePy shift
		cv::Mat myEdgePm;	// myEdgePx, myEdgePy（移动后） 相加
		std::vector<int> myPmPeakIdx;
		cv::Mat mySquaredWithRect;	// 原始图像加入矩阵


		bool myFound = false;	// 是否找到魔方

		cv::Rect myRect;


	};
}