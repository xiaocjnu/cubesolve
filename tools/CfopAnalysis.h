// CFOP 解法分析
// 时间单位均使用秒



#pragma once


#include "tools_typedef.h"
#include "CubeFrames.h"
#include <vector>
#include "cs.h"
#include <string>


class CfopAnalysis
{
public:
	enum State {
		Init, Cross, F1, F2, F3, F2L, OLL, Solved, Invalid = -1
	};


	CfopAnalysis();



	// 分析记录，成功返回 true, 失败或空返回false
	bool analyze(cs::CubieCube& initCube, const CubeFrames& frames);
	bool analyze(std::string scramble, const CubeFrames& frames);

	// 初始魔方状态
	cs::CubieCube getInitCube() const { return m_initCube; };

	// 获取原始记录
	const std::vector<cf_rich_record_t>& getRawRecords() const { return m_rawRecords; };

	// 获取优化的记录
	const std::vector<cf_rich_record_t>& getOptimRecords() const { return m_optimRecords; };

	void setIsOptimize(bool b) { m_isOptimize = b; };
	bool getIsOptimize() const { return m_isOptimize; };

	// 底面颜色，0-5, 分别表示 U,R,F,D,L,B
	int getBottomColor() const { return m_bottomColor; };

	// C 开始的下标，恒等于0，C 分段下标范围是 [m_CStart, m_CEnd), 下标是 m_optimRecords 的下标
	int getCStart() const { return m_CStart; };

	// C 结束的下标（不含），C 分段下标范围是 [m_CStart, m_CEnd)
	int getCEnd() const { return m_F1Start; };

	// F1 开始的下标，F1 分段下标范围是 [m_F1Start, m_F1End)
	int getF1Start() const { return m_F1Start; };

	// F1 结束的下标（不含），F1 分段下标范围是 [m_F1Start, m_F1End)
	int getF1End() const { return m_F2Start; };

	// F2 开始的下标，F2 分段下标范围是 [m_F2Start, m_F2End)
	int getF2Start() const { return m_F2Start; };

	// F2 结束的下标（不含），F2 分段下标范围是 [m_F2Start, m_F2End)
	int getF2End() const { return m_F3Start; };

	// F3 开始的下标，F3 分段下标范围是 [m_F3Start, m_F3End)
	int getF3Start() const { return m_F3Start; };

	// F3 结束的下标（不含），F3 分段下标范围是 [m_F3Start, m_F3End)
	int getF3End() const { return m_F4Start; };

	// F4 开始的下标，F4 分段下标范围是 [m_F4Start, m_F4End)
	int getF4Start() const { return m_F4Start; };

	// F4 结束的下标（不含），F4 分段下标范围是 [m_F4Start, m_F4End)
	int getF4End() const { return m_OStart; };

	// O 开始的下标，O 分段下标范围是 [m_OStart, m_OEnd)
	int getOStart() const { return m_OStart; };

	// O 结束的下标（不含），O 分段下标范围是 [m_OStart, m_OEnd)
	int getOEnd() const { return m_PStart; };

	// P 开始的下标，P 分段下标范围是 [m_PStart, m_PEnd)
	int getPStart() const { return m_PStart; };

	// P 结束的下标（不含），P 分段下标范围是 [m_PStart, m_PEnd)
	int getPEnd() const { return m_PEnd; };

	// C 分段的时间(ms)
	int getCTime() const { return m_CTime; };

	// F 分段的时间(ms)
	int getFTime() const { return m_FTime; };

	// O 分段的时间(ms)
	int getOTime() const { return m_OTime; };

	// P 分段的时间(ms)
	int getPTime() const { return m_PTime; };

	// 总时间(ms)
	int getTolTime() const { return m_TolTime; };


	// C 分段的时间(s)， 精确到 0.01s
	double getCTimeSecR2() const { return m_CTimeSecR2; };

	// F 分段的时间(s)， 精确到 0.01s
	double getFTimeSecR2() const { return m_FTimeSecR2; };

	// O 分段的时间(s)， 精确到 0.01s
	double getOTimeSecR2() const { return m_OTimeSecR2; };

	// P 分段的时间(s)， 精确到 0.01s
	double getPTimeSecR2() const { return m_PTimeSecR2; };

	// 总时间
	double getTolTimeSecR2() const { return m_TolTimeSecR2; };

	// 步数
	int getStep() const { return m_step; };

	// 转体
	int getRot() const { return m_rot; };

	// TPS, step per second
	double getTps() const { return m_step / (m_TolTime / 1000.0); };

	// 流畅指数
	double getFluency() const { return m_fluency; };



	std::vector<cf_rich_record_t> getCrossRecords() const { return _sub_record(m_optimRecords, getCStart(), getCEnd()); };
	std::vector<cf_rich_record_t> getF1Records() const { return _sub_record(m_optimRecords, getF1Start(), getF1End()); };
	std::vector<cf_rich_record_t> getF2Records() const { return _sub_record(m_optimRecords, getF2Start(), getF2End()); };
	std::vector<cf_rich_record_t> getF3Records() const { return _sub_record(m_optimRecords, getF3Start(), getF3End()); };
	std::vector<cf_rich_record_t> getF4Records() const { return _sub_record(m_optimRecords, getF4Start(), getF4End()); };
	std::vector<cf_rich_record_t> getF2LRecords() const { return _sub_record(m_optimRecords, getF1Start(), getF4End()); };
	std::vector<cf_rich_record_t> getOLLRecords() const { return _sub_record(m_optimRecords, getOStart(), getOEnd()); };
	std::vector<cf_rich_record_t> getPLLRecords() const { return _sub_record(m_optimRecords, getPStart(), getPEnd()); };


	// 获取当前复原的进展状态
	static State getSolveState(const cs::CubieCube& cc);
	static State getSolveState(const cs::AxisCubieCube& cc);

	static std::vector<cs::Move> recordsToMoves(const std::vector<cf_rich_record_t>& records);


protected:

	void _reset_result();

	// 获取当前进展，即最高达到了那个状态。
	State _getCurrState(const cs::CubieCube& acc);



	// 修正 m_rawRecords 相对公式，会增加返回的大小
	static std::vector<cf_rich_record_t> _fixRelateMove(int init_axis, const std::vector<cf_rich_record_t>& src);

	// 公式优化
	static std::vector<cf_rich_record_t> _optimize(int init_axis, const std::vector<cf_rich_record_t>& src);

	static std::vector<cf_rich_record_t> _sub_record(const std::vector<cf_rich_record_t>& src, size_t _begin, size_t _end);

	static int _sum_record_ds_time(const std::vector<cf_rich_record_t>& src);
	static int _sum_record_ds_time(const std::vector<cf_record_t>& src);

	static cs::AxisCubieCube _move_record_abs(const cs::AxisCubieCube& src, const std::vector<cf_rich_record_t>& records);
	static cs::AxisCubieCube _move_record_rel(const cs::AxisCubieCube& src, const std::vector<cf_rich_record_t>& records);


	void _cal_seg_timeR2();
	void _cal_fluency();



	void _init();
	static bool _is_inited;
	static std::array<cs::AxisCubieCube,54> _acc_move_table;

protected:
	cs::CubieCube m_initCube;	// 初始魔方状态



	// 结果
	std::vector<cf_rich_record_t> m_rawRecords;	// 原始记录
	std::vector<cf_rich_record_t> m_optimRecords;	// 优化的记录，每步相对公式都有意义

	bool m_isOptimize = true;

	int m_bottomColor;	// 底面颜色，0-5, 分别表示 U,R,F,D,L,B

	int m_CStart;	// C 开始的下标，恒等于0，C 分段下标范围是 [m_CStart, m_CEnd)
	int m_F1Start;	// F1 开始的下标，F1 分段下标范围是 [m_F1Start, m_F1End)
	int m_F2Start;	// F2 开始的下标，F2 分段下标范围是 [m_F2Start, m_F2End)
	int m_F3Start;	// F3 开始的下标，F3 分段下标范围是 [m_F3Start, m_F3End)
	int m_F4Start;	// F4 开始的下标，F4 分段下标范围是 [m_F4Start, m_F4End)
	int m_OStart;	// O 开始的下标，O 分段下标范围是 [m_OStart, m_OEnd)
	int m_PStart;	// P 开始的下标，P 分段下标范围是 [m_PStart, m_PEnd)
	int m_PEnd;		// P 结束的下标（不含），等价于总步数. P 分段下标范围是 [m_PStart, m_PEnd)

	int m_CTime;	// C 分段的时间(ms)
	int m_FTime;	// F 分段的时间(ms)
	int m_OTime;	// O 分段的时间(ms)
	int m_PTime;	// P 分段的时间(ms)
	int m_TolTime;  // 总时间

	double m_CTimeSecR2;	// C 分段的时间(s)， 精确到 0.01s
	double m_FTimeSecR2;	// F 分段的时间(s)， 精确到 0.01s
	double m_OTimeSecR2;	// O 分段的时间(s)， 精确到 0.01s
	double m_PTimeSecR2;	// P 分段的时间(s)， 精确到 0.01s
	double m_TolTimeSecR2;  // 总时间(s)， 精确到 0.01s


	int m_step;		// 步数
	int m_rot;		// 转体
	double m_fluency;  // 流畅指数

};