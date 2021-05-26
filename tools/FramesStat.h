// CubeFrames 基本统计数据



#pragma once


#include "tools_typedef.h"
#include "CubeFrames.h"

class FramesStat
{
public:
	FramesStat();
	FramesStat(const CubeFrames& frames);


	// 分析记录，成功返回 true, 失败或空返回false
	bool analyze(const CubeFrames& frames);

	// 是否分析成功
	bool isAnalyzed() const { return m_isAnalyzed; };

	// 获取记录的总时间
	double getTotalTime() const { return m_totalTime; };

	// 获取记录的步数（不含转体）
	int getStep() const { return m_step; };

	// 获取记录的转体次数
	int getRot() const { return m_rot; };

	// 获取 TPS
	int getTps() const { return m_tps; };

	// 获取流畅指数
	double getFluence() const { return m_fluence; };





protected:
	bool m_isAnalyzed = false;	// 是否分析成功
	double m_totalTime;	// 总时间(s)
	int m_step = 0;	// 步数
	int m_rot = 0;	// 转体
	int m_tps = 0;	// TPS
	double m_fluence = 0; // 流畅指数



};