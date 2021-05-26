
#include "CfopAnalysis.h"
#include "cs.h"
#include <cassert>
#include <algorithm>
#include "good_tools.h"
#include <array>
#include <cmath>

using namespace std;
using namespace cs;

using HS = cs::HumanSolver;

bool CfopAnalysis::_is_inited = false;
std::array<cs::AxisCubieCube, 54> CfopAnalysis::_acc_move_table;

CfopAnalysis::CfopAnalysis()
{
	_init();
	_reset_result();
}

bool CfopAnalysis::analyze(cs::CubieCube& initCube, const CubeFrames & frames)
{
	m_initCube = initCube;
	_reset_result();

	bool check = check_scramble_and_frames(initCube, frames);

	if (!check || frames.empty())	// 不能复原或者为空
	{
		return false;
	}


	// 处理分段
	const array<int, 6> start_axi = {
		12,	// AxisCube({3,1,5,0,4,2}),	// 12  x2
		16,	// AxisCube({4,0,2,1,3,5}),	// 16  z
		20,	// AxisCube({5,0,4,2,3,1}),	// 20
		0,	// AxisCube({0,1,2,3,4,5}),	// 0  eye
		4,	// AxisCube({1,0,5,4,3,2}),	// 4
		8	// AxisCube({2,0,1,5,3,4}),	// 8
	};


	array<vector<int>, 6> idxs;		// 首次达到某个状态的下标
	array<int, 6> eq_bot = { 0,0,0,0,0,0 };			// 底面方向相同计数
	array<double, 6> score = { 0,0,0,0,0,0 };		// 分数


	int best_k = -1;
	double best_score = 0.0;

	for (int k = 0; k < 6; ++k)
	{
		AxisCubieCube acc;
		acc.axis = AxisCube::fromValidIdx(start_axi[k]);
		acc.setAbsCubie(initCube);

		vector<int>& idx = idxs[k];
		idx = vector<int>((int)Solved + 1, -1);

		// 魔方复原进度
		State state = Init;

		for (size_t i = 0; i < frames.records.size(); ++i)
		{
			AxisCube axc = AxisCube::fromValidIdx(frames.records[i].axis_cube);

			if (axc[3] == acc.axis[3])
				++(eq_bot[k]);

			auto mm = frames.records[i].move.getMoves();

			for (Move m : mm)
			{
				if (0 <= m && m < 18)
				{
					acc = acc.moveAbs(m);

					State curState = getSolveState(acc);

					if (curState > state)
					{
						idx[curState] = (int)i;
						state = curState;
					}

				}
			}
			
		}// for i


		int n = 0;

		for (int j = 1; j <= (int)Solved; ++j)
		{
			if (idx[j] > 0)
				++n;
		}

		double s1 = 1.0 *n / (int)Solved;
		double s2 = 1.0 * eq_bot[k] / frames.records.size();

		double s = 0.6 * s1 + 0.4 * s2;   // 底判断分数权重， s2 本来难以是1，可以更加偏向于s1

		score[k] = s;

		if (s > best_score)
		{
			best_k = k;
			best_score = s;
		}
	}// for k


	if (best_k < 0)
		return false;


	// 临时保存分段结果
	const vector<int>& idx = idxs[best_k];

	m_bottomColor = best_k;


	m_PEnd = (int)frames.records.size();
	m_PStart = idx[OLL] > 0 ? idx[OLL] + 1 : m_PEnd;
	m_OStart = idx[F2L] > 0 ? idx[F2L] + 1 : m_PStart;
	m_F4Start = idx[F3] > 0 ? idx[F3] + 1 : m_OStart;
	m_F3Start = idx[F2] > 0 ? idx[F2] + 1 : m_F4Start;
	m_F2Start = idx[F1] > 0 ? idx[F1] + 1 : m_F3Start;
	m_F1Start = idx[Cross] > 0 ? idx[Cross] + 1 : m_F2Start;
	m_CStart = 0;


	// 分段优化
	m_rawRecords = frames.toRichRecords();

	auto seg_C = _sub_record(m_rawRecords, m_CStart, m_F1Start);
	auto seg_F1 = _sub_record(m_rawRecords, m_F1Start, m_F2Start);
	auto seg_F2 = _sub_record(m_rawRecords, m_F2Start, m_F3Start);
	auto seg_F3 = _sub_record(m_rawRecords, m_F3Start, m_F4Start);
	auto seg_F4 = _sub_record(m_rawRecords, m_F4Start, m_OStart);
	auto seg_OLL = _sub_record(m_rawRecords, m_OStart, m_PStart);
	auto seg_PLL = _sub_record(m_rawRecords, m_PStart, m_PEnd);

	int init_axis_F1 = seg_C.empty() ? 0 : (*seg_C.rbegin()).axis_cube;
	int init_axis_F2 = seg_F1.empty() ? init_axis_F1 : (*seg_F1.rbegin()).axis_cube;
	int init_axis_F3 = seg_F2.empty() ? init_axis_F2 : (*seg_F2.rbegin()).axis_cube;
	int init_axis_F4 = seg_F3.empty() ? init_axis_F3 : (*seg_F3.rbegin()).axis_cube;
	int init_axis_OLL = seg_F4.empty() ? init_axis_F4 : (*seg_F4.rbegin()).axis_cube;
	int init_axis_PLL = seg_OLL.empty() ? init_axis_OLL : (*seg_OLL.rbegin()).axis_cube;

	seg_C = _fixRelateMove(0, seg_C);
	seg_F1 = _fixRelateMove(init_axis_F1, seg_F1);
	seg_F2 = _fixRelateMove(init_axis_F2, seg_F2);
	seg_F3 = _fixRelateMove(init_axis_F3, seg_F3);
	seg_F4 = _fixRelateMove(init_axis_F4, seg_F4);
	seg_OLL = _fixRelateMove(init_axis_OLL, seg_OLL);
	seg_PLL = _fixRelateMove(init_axis_PLL, seg_PLL);


#ifdef _DEBUG
	AxisCubieCube a0;
	a0.setAbsCubie(initCube);

	AxisCubieCube a1 = _move_record_abs(a0, seg_C);
	AxisCubieCube a2 = _move_record_abs(a1, seg_F1);
	AxisCubieCube a3 = _move_record_abs(a2, seg_F2);
	AxisCubieCube a4 = _move_record_abs(a3, seg_F3);
	AxisCubieCube a5 = _move_record_abs(a4, seg_F4);
	AxisCubieCube a6 = _move_record_abs(a5, seg_OLL);
	AxisCubieCube a7 = _move_record_abs(a6, seg_PLL);
#endif


	if (m_isOptimize)
	{
		seg_C = _optimize(0, seg_C);
		seg_F1 = _optimize(init_axis_F1, seg_F1);
		seg_F2 = _optimize(init_axis_F2, seg_F2);
		seg_F3 = _optimize(init_axis_F3, seg_F3);
		seg_F4 = _optimize(init_axis_F4, seg_F4);
		seg_OLL = _optimize(init_axis_OLL, seg_OLL);
		seg_PLL = _optimize(init_axis_PLL, seg_PLL);
	}



#ifdef _DEBUG
	AxisCubieCube b0;
	b0.setAbsCubie(initCube);

	AxisCubieCube b1 = _move_record_abs(b0, seg_C);
	AxisCubieCube b2 = _move_record_abs(b1, seg_F1);
	AxisCubieCube b3 = _move_record_abs(b2, seg_F2);
	AxisCubieCube b4 = _move_record_abs(b3, seg_F3);
	AxisCubieCube b5 = _move_record_abs(b4, seg_F4);
	AxisCubieCube b6 = _move_record_abs(b5, seg_OLL);
	AxisCubieCube b7 = _move_record_abs(b6, seg_PLL);

	assert(a1 == b1);
	assert(a2 == b2);
	assert(a3 == b3);
	assert(a4 == b4);
	assert(a5 == b5);
	assert(a6 == b6);
	assert(a7 == b7);


#endif

#ifdef _DEBUG
	AxisCubieCube dd;
	dd.setAbsCubie(initCube);

	dd = _move_record_abs(dd, seg_C);
	dd = _move_record_abs(dd, seg_F1);
	dd = _move_record_abs(dd, seg_F2);
	dd = _move_record_abs(dd, seg_F3);
	dd = _move_record_abs(dd, seg_F4);
	dd = _move_record_abs(dd, seg_OLL);
	dd = _move_record_abs(dd, seg_PLL);

	assert(dd.isSolved());

	dd.setAbsCubie(initCube);
	dd = _move_record_rel(dd, seg_C);
	dd = _move_record_rel(dd, seg_F1);
	dd = _move_record_rel(dd, seg_F2);
	dd = _move_record_rel(dd, seg_F3);
	dd = _move_record_rel(dd, seg_F4);
	dd = _move_record_rel(dd, seg_OLL);
	dd = _move_record_rel(dd, seg_PLL);

	assert(dd.isSolved());
#endif



	// 更新 optimRecords
	m_optimRecords.clear();

	for (auto& a : seg_C)
		m_optimRecords.push_back(a);

	m_F1Start = (int)m_optimRecords.size();
	for (auto& a : seg_F1)
		m_optimRecords.push_back(a);

	m_F2Start = (int)m_optimRecords.size();
	for (auto& a : seg_F2)
		m_optimRecords.push_back(a);

	m_F3Start = (int)m_optimRecords.size();
	for (auto& a : seg_F3)
		m_optimRecords.push_back(a);

	m_F4Start = (int)m_optimRecords.size();
	for (auto& a : seg_F4)
		m_optimRecords.push_back(a);

	m_OStart = (int)m_optimRecords.size();
	for (auto& a : seg_OLL)
		m_optimRecords.push_back(a);

	m_PStart = (int)m_optimRecords.size();
	for (auto& a : seg_PLL)
		m_optimRecords.push_back(a);

	m_PEnd = (int)m_optimRecords.size();

	// 更新分段时间
	m_CTime = _sum_record_ds_time(seg_C);
	m_FTime = _sum_record_ds_time(seg_F1) + _sum_record_ds_time(seg_F2) + _sum_record_ds_time(seg_F3) + _sum_record_ds_time(seg_F4);
	m_OTime = _sum_record_ds_time(seg_OLL);
	m_PTime = _sum_record_ds_time(seg_PLL);

	m_TolTime = _sum_record_ds_time(m_rawRecords);

	assert(m_CTime + m_FTime + m_OTime + m_PTime == m_TolTime);

	_cal_seg_timeR2();

#ifdef _DEBUG
	int _tolTime = _sum_record_ds_time(frames.records);
	assert(m_TolTime == _tolTime);
#endif


	// 更新步数和转体和流畅指数
	m_step = 0;
	m_rot = 0;

	for (const auto& a : m_optimRecords)
	{
		if (Rot_x1 <= a.rel_move  && a.rel_move <= Rot_z3)
			++m_rot;
		else if (0 <= a.rel_move  && a.rel_move < 54)
			++m_step;
	}

	_cal_fluency();
	

	return true;
}

bool CfopAnalysis::analyze(std::string scramble, const CubeFrames & frames)
{
	auto mv = scramble_formula_str_to_moves(scramble);
	CubieCube cc;
	cc = cc.move(mv);
	return analyze(cc, frames);
}

CfopAnalysis::State CfopAnalysis::getSolveState(const CubieCube & cc)
{
	State state = Init;

	if (HS::isDCrossSolved(cc))
		state = Cross;
	else
		return state;


	int n = HS::numPairF2LIgnoreDCross(cc);

	if (n == 0)
		return state;
	else if (n == 1)
		state = F1;
	else if (n == 2)
		state = F2;
	else if (n == 3)
		state = F3;
	else if (n == 4)
		state = F2L;
	else
		throw "unreachable";

	if (n < 4)
		return state;

	if (HS::isOnlyUCornerOriSolved(cc) && HS::isOnlyUEdgeOriSolved(cc))
		state = OLL;
	else
		return state;


	if (HS::isOnlyUCornerPermSolved(cc) && HS::isOnlyUEdgePermSolved(cc))
	{
		assert(cc.iseye());
		state = Solved;
	}

	return state;
}

CfopAnalysis::State CfopAnalysis::getSolveState(const cs::AxisCubieCube & cc)
{
	vector<Move> ff = cc.axis.inv().getGenFormula();
	CubieCube ss = CubieCube::eye().move(ff);
	return getSolveState(ss * cc.cubie);
}

vector<Move> CfopAnalysis::recordsToMoves(const vector<cf_rich_record_t>& records)
{
	vector<Move> moves;
	for (const auto& a : records)
		moves.push_back(a.rel_move);

	return moves;
}

void CfopAnalysis::_reset_result()
{
	m_rawRecords.clear();
	m_optimRecords.clear();

	m_bottomColor = 3;

	m_CStart = 0;
	m_F1Start = 0;
	m_F2Start = 0;
	m_F3Start = 0;
	m_F4Start = 0;
	m_OStart = 0;
	m_PStart = 0;
	m_PEnd = 0;

	m_CTime = 0;
	m_FTime = 0;
	m_OTime = 0;
	m_PTime = 0;

}


CfopAnalysis::State CfopAnalysis::_getCurrState(const CubieCube & acc)
{
	State state;

	if (acc.iseye())	// 是否复原
	{
		state = Solved;
	}
	else if (HS::isOLLSolved(acc))
	{
		state = OLL;
	}
	else if (HS::isF2LSolved(acc))
	{
		state = F2L;
	}
	else if (HS::numPairF2LSolved(acc) == 3)
	{
		state = F3;
	}
	else if (HS::numPairF2LSolved(acc) == 2)
	{
		state = F2;
	}
	else if (HS::numPairF2LSolved(acc) == 1)
	{
		state = F1;
	}
	else if (HS::isDCrossSolved(acc))
	{
		state = Cross;
	}
	else
	{
		state = Init;
	}

	return state;
}

std::vector<cf_rich_record_t> CfopAnalysis::_fixRelateMove(int init_axis, const std::vector<cf_rich_record_t>& src)
{
	std::vector<cf_rich_record_t> dst;

	for (size_t i = 0; i < src.size(); ++i)
	{
		if (src[i].rel_move != InvalidMove)
		{
			dst.push_back(src[i]);
		}
		else
		{
			vector<Move> abs_moves;
			if (src[i].abs_move != InvalidMove)
				abs_moves.push_back(src[i].abs_move);

			if (src[i].abs_move2 != InvalidMove)
				abs_moves.push_back(src[i].abs_move2);

			AxisCubieCube pre_acc, acc;

			if (i > 0)
				pre_acc.axis = AxisCube::fromValidIdx(src[i - 1].axis_cube);
			else
				pre_acc.axis = AxisCube::fromValidIdx(init_axis);

			acc.axis = AxisCube::fromValidIdx(src[i].axis_cube);

			auto rot_moves = (pre_acc.axis.inv() * acc.axis).getGenFormula();
			

			if (abs_moves.empty() && rot_moves.empty())  // 完全不动
			{
				dst.push_back(src[i]);
			}
			else
			{

				AxisCubieCube dd = pre_acc;
				cf_rich_record_t rec;

				bool first_time = true;

				for (size_t j = 0; j < rot_moves.size(); ++j)
				{
					dd = dd.move(rot_moves[j]);

					rec.abs_move = InvalidMove;
					rec.abs_move2 = InvalidMove;
					rec.rel_move = rot_moves[j];
					rec.axis_cube = dd.axis.getValidIdx();

					if (first_time)
					{
						first_time = false;
						rec.ds_time = src[i].ds_time;
					}
					else
						rec.ds_time = 0;
					

					dst.push_back(rec);
				}

				assert(dd.axis.getValidIdx() == src[i].axis_cube);


				if (abs_moves.size() >= 1)
				{
					rec.abs_move = abs_moves[0];
					rec.abs_move2 = InvalidMove;
					rec.rel_move = dd.formulaA2R(rec.abs_move);
					rec.axis_cube = src[i].axis_cube;

					rec.ds_time = first_time ? src[i].ds_time : 0;

					dst.push_back(rec);
				}

				if (abs_moves.size() == 2)
				{
					assert(!first_time);

					rec.abs_move = InvalidMove;
					rec.abs_move2 = abs_moves[1];
					rec.rel_move = dd.formulaA2R(rec.abs_move);
					rec.axis_cube = src[i].axis_cube;
					rec.ds_time = 0;

					dst.push_back(rec);
				}
			}
		}
	}

	return dst;
}

std::vector<cf_rich_record_t> CfopAnalysis::_optimize(int init_axis, const std::vector<cf_rich_record_t>& src)
{
	const int merge_time = 100;  // 毫秒，如果可以小于此时间多步公式合并成一个相对公式，仅用于中间层
	const int merge_time_other = 50;   // 用于合并转两层的时间

	if (src.empty() || src.size() == 1)
		return src;

	std::vector<cf_rich_record_t> dst;

	dst.push_back(src[0]);

	for (int i = 1; i < (int)src.size(); ++i)
	{
		const cf_rich_record_t& b = src[i];

		if (src[i].ds_time > merge_time)  // 时间太大，不能合并
		{
			dst.push_back(b);
		}
		else // 时间较小，有可能合并
		{
			cf_rich_record_t& a = dst[dst.size() - 1];


			if (a.rel_move == InvalidMove && b.rel_move == InvalidMove)  // 没有动作，合并
			{
				assert(a.abs_move == InvalidMove);
				assert(a.abs_move2 == InvalidMove);
				assert(b.abs_move == InvalidMove);
				assert(b.abs_move2 == InvalidMove);
				assert(a.axis_cube == b.axis_cube);

				a.ds_time += b.ds_time;
			}
			else if (a.rel_move != InvalidMove && b.rel_move == InvalidMove)
			{
				dst.push_back(b);
			}
			else if (a.ds_time + b.ds_time <= merge_time)  // 时间较短，有可能要合并
			{
				AxisCubieCube dd;
				dd = dd.move(a.rel_move);
				dd = dd.move(b.rel_move);


				Move merge_move = InvalidMove;

				for (size_t j = 0; j < _acc_move_table.size(); ++j)
				{
					if (_acc_move_table[j] == dd)
					{
						merge_move = (Move)j;
						break;
					}
				}


				if (merge_move != InvalidMove &&
					(Ex1 << merge_move && merge_move <= Sx3 || a.ds_time + b.ds_time <= merge_time_other)
					)  // 合并
				{

					AxisCubieCube pre_a;

					if (dst.size() >= 2)
						pre_a.axis = AxisCube::fromValidIdx(dst[dst.size()-2].axis_cube);
					else
						pre_a.axis = AxisCube::fromValidIdx(init_axis);

					auto dec_moves = move_decompose(merge_move);

					if (dec_moves.size() == 1)
					{
						if (0 <= dec_moves[0] && dec_moves[0] < 18)
						{
							a.abs_move = pre_a.formulaR2A(dec_moves[0]);
							a.abs_move2 = InvalidMove;
						}
						else
						{
							a.abs_move = InvalidMove;
							a.abs_move2 = InvalidMove;
						}

					}
					else if (dec_moves.size() == 2)
					{
						a.abs_move = pre_a.formulaR2A(dec_moves[0]);
						a.abs_move2 = InvalidMove;
					}
					else if (dec_moves.size() == 3)
					{
						a.abs_move = pre_a.formulaR2A(dec_moves[0]);
						a.abs_move2 = pre_a.formulaR2A(dec_moves[1]);
					}
					else
						throw "unreachable";

					a.rel_move = merge_move;
					a.ds_time += b.ds_time;
					a.axis_cube = b.axis_cube;
				}
				else
				{
					dst.push_back(b);
				}


			}
			else
			{
				dst.push_back(b);
			}

		}
	}


	return dst;
}

vector<cf_rich_record_t> CfopAnalysis::_sub_record(const vector<cf_rich_record_t>& src, size_t _begin, size_t _end)
{
	vector<cf_rich_record_t> result(_end - _begin);
	copy(src.begin() + _begin, src.begin() + _end, result.begin());
	return result;
}

int CfopAnalysis::_sum_record_ds_time(const std::vector<cf_rich_record_t>& src)
{
	int _sum = 0;

	for (const auto& a : src)
		_sum += a.ds_time;

	return _sum;
}

int CfopAnalysis::_sum_record_ds_time(const std::vector<cf_record_t>& src)
{
	int _sum = 0;

	for (const auto& a : src)
		_sum += a.ds_time;

	return _sum;
}

AxisCubieCube CfopAnalysis::_move_record_abs(const AxisCubieCube & src, const std::vector<cf_rich_record_t>& records)
{
	AxisCubieCube dd = src;

	for (auto& rec : records)
	{
		dd = dd.moveAbs(rec.abs_move);
		dd = dd.moveAbs(rec.abs_move2);
	}

	return dd;
}

AxisCubieCube CfopAnalysis::_move_record_rel(const AxisCubieCube & src, const std::vector<cf_rich_record_t>& records)
{
	AxisCubieCube dd = src;

	for (auto& rec : records)
	{
		dd = dd.move(rec.rel_move);
	}

	return dd;
}

void CfopAnalysis::_cal_seg_timeR2()
{
	double c = m_CTime / 10.0;
	double f = m_FTime / 10.0;
	double o = m_OTime / 10.0;
	double p = m_PTime / 10.0;


	int _tol = (int)round(m_TolTime / 10.0);

	m_TolTimeSecR2 = _tol / 100.0;

	vector<double> aa = { c,f,o,p };
	auto bb = round_vector_eq_sum(aa, _tol);

	m_CTimeSecR2 = bb[0] / 100.0;
	m_FTimeSecR2 = bb[1] / 100.0;
	m_OTimeSecR2 = bb[2] / 100.0;
	m_PTimeSecR2 = bb[3] / 100.0;

	


	assert(abs(m_TolTimeSecR2 - m_CTimeSecR2 - m_FTimeSecR2 - m_OTimeSecR2 - m_PTimeSecR2) < 1e-6);




}

void CfopAnalysis::_cal_fluency()
{
	int min_step = 4;

	if ((int)m_optimRecords.size() < min_step)
	{
		m_fluency = 0.0;
		return;
	}


	// 忽略第一步
	double _sum = 0.0;
	for (size_t i = 1; i < m_optimRecords.size(); ++i)
	{
		double a =  m_optimRecords[i].ds_time / 1000.0;
		_sum += a * a;
	}

	m_fluency = sqrt(_sum / (m_optimRecords.size() - 2));

}

void CfopAnalysis::_init()
{
	if (_is_inited)
		return;

	_is_inited = true;

	AxisCubieCube e;

	for (int i = 0; i < 54; ++i)
	{
		_acc_move_table[i] = e.move(i);
	}


}


