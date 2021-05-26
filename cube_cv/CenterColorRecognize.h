#pragma once

#include <vector>
#include "cube_cv_utils.h"

namespace cube_cv
{


	// 中心块颜色识别，每次加入一个中心块颜色(Lab 格式), 自动识别颜色，直至6个位置。
	// 未满足6个颜色时，已经识别的颜色还可能变化, 因为颜色是相对的，不是绝对的
	class CenterColorRecognize
	{
	public:

		CenterColorRecognize();

		void clear();	// 恢复初始状态


		bool empty() const { return myInColor.empty(); };


		int getNum() const { return (int)myInColor.size(); };

		const std::vector<cv::Vec3f>& getInColor() const { return myInColor; };
		const std::vector<paster_color_t>& getOutColor() const { return myOutColor; };


		// 加入一个颜色，如果颜色相同会覆盖
		// 返回 color 存储在 myInColor的下标, -1 表示忽略
		int push(cv::Vec3f color);


		// 计算中心块颜色(Lab)
		static cv::Vec3f centerColor(const cv::Mat& I, cv::Rect rect);





	protected:

		// 识别第单个颜色
		paster_color_t _recognize_first(cv::Vec3f color);



	protected:

		// 数据
		std::vector<cv::Vec3f> myInColor;
		std::vector<paster_color_t> myOutColor;


	};
}