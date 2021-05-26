#include "good_tools.h"
#include <sstream>
#include <vector>
#include <set>
#include <cmath>
#include <math.h>
#include <algorithm>

using namespace std;
using namespace cs;




vector<uchar> CharVectorToUCharVector(const vector<char>& a)
{
	return reinterpret_cast<const vector<uchar>&>(a);
}

vector<char> UCharVectorToCharVector(const vector<uchar>& a)
{
	return reinterpret_cast<const vector<char>&>(a);
}

cs::CubieCube randomCube()
{
	return cs::CubieCube::randomCube();
}

std::vector<cs::Move> solveCube(const cs::CubieCube & cc)
{
	Search search;
	std::vector<cs::Move> moves = search.solve(cc);
	return moves;
}

std::vector<int> solveCubeIV(const cs::CubieCube & cc)
{
	return MoveVector2IntVector(solveCube(cc));
}

std::vector<cs::Move> solveCube(const cs::CubieCube & src, const cs::CubieCube & dst)
{
	Search search;
	std::vector<cs::Move> moves = search.solve(src, dst);
	return moves;
}

std::vector<int> solveCubeIV(const cs::CubieCube & src, const cs::CubieCube & dst)
{
	return MoveVector2IntVector(solveCube(src, dst));
}

std::vector<cs::Move> solveCubeSramble(const cs::CubieCube & cc)
{
	return solveCube(cc.inv());
}

std::vector<int> solveCubeSrambleIV(const cs::CubieCube & cc)
{
	return MoveVector2IntVector(solveCubeSramble(cc));
}

std::vector<cs::Move> solveCubeNoU(const cs::CubieCube & cc)
{
	Search5 search;
	std::vector<cs::Move> moves = search.solve_no_axis(cc, 0);
	return moves;
}

std::vector<cs::Move> solveCubeNoU(const cs::AxisCubieCube & cc)
{
	AxisCube axis = cc.axis.inv();
	vector<Move> ff = axis.getGenFormula();
	AxisCubieCube ss = AxisCubieCube::eye().move(ff);


	AxisCubieCube dd = ss * cc;

	assert(dd.axis == AxisCube::eye());

	return solveCubeNoU(dd.cubie);
}

std::vector<int> solveCubeNoU_IV(const cs::AxisCubieCube & cc)
{
	return MoveVector2IntVector(solveCubeNoU(cc));
}

std::vector<cs::Move> solveCubeNoU(const cs::AxisCubieCube & src, const cs::AxisCubieCube & dst)
{
	return solveCubeNoU(src.inv() * dst);
}

std::vector<int> solveCubeNoU_IV(const cs::AxisCubieCube & src, const cs::AxisCubieCube & dst)
{
	return MoveVector2IntVector(solveCubeNoU(src, dst));
}

std::vector<int> solveCubeNoU_IV(const cs::CubieCube & cc)
{
	return MoveVector2IntVector(solveCubeNoU(cc));
}

std::vector<cs::Move> solveCubeNoU(const cs::CubieCube & src, const cs::CubieCube & dst)
{
	Search5 search;
	std::vector<cs::Move> moves = search.solve_no_axis(src, dst, 0);
	return moves;
}

std::vector<int> solveCubeNoU_IV(const cs::CubieCube & src, const cs::CubieCube & dst)
{
	return MoveVector2IntVector(solveCubeNoU(src, dst));
}


std::vector<cs::Move> randomSrambleFormula()
{
	return solveCubeSramble(randomCube());
}

std::vector<int> randomSrambleFormulaIV()
{
	return MoveVector2IntVector(randomSrambleFormula());
}

//////////////////////////////////////////////////////////////////////////
vector<int> round_vector_eq_sum(const vector<double>& aa, int total)
{
	int num = (int)aa.size();


	if (num == 0)
		return vector<int>();



	// 修正和
	vector<double> a(num);

	double _sum = 0.0;
	for (size_t i = 0; i < aa.size(); ++i)
	{
		_sum += aa[i];
	}

	double factor = total / _sum;

	for (size_t i = 0; i < a.size(); ++i)
	{
		a[i] = aa[i] * factor;
	}

	// 修正
	vector<int> ra(num);
	vector<double> abs_e(num);

	vector<int> pos_e, neg_e;
	pos_e.reserve(num);
	neg_e.reserve(num);

	double sum_a = 0.0;
	int sum_ra = 0;

	for (int i = 0; i < num; ++i)
	{
		ra[i] = (int)std::round(a[i]);

		double ei = a[i] - ra[i];
		abs_e[i] = abs(ei);

		if (ei < 0)
			neg_e.push_back(i);
		else
			pos_e.push_back(i);

		sum_a += a[i];
		sum_ra += ra[i];
	}

	int r_sum_a = (int)std::round(sum_a);

	assert(abs(r_sum_a - total) <= 1);

	int delta = sum_ra - total;

	if (delta == 0)
		return ra;

	int num_delta = abs(delta);

	if (delta < 0)
	{
		assert((int)pos_e.size() >= num_delta);

		partial_sort(pos_e.begin(), pos_e.begin() + num_delta, pos_e.end(),
			[abs_e](int i, int j) {return abs_e[i] > abs_e[j]; });


		for (int j = 0; j < num_delta; ++j)
		{
			ra[pos_e[j]]++;
		}
	}
	else
	{
		assert((int)neg_e.size() >= num_delta);

		partial_sort(neg_e.begin(), neg_e.begin() + num_delta, neg_e.end(),
			[abs_e](int i, int j) {return abs_e[i] > abs_e[j]; });


		for (int j = 0; j < num_delta; ++j)
		{
			ra[neg_e[j]]--;
		}
	}


#ifdef _DEBUG

	int __sum = 0;

	for (int el : ra)
	{
		__sum += el;
	}

	assert(__sum == total);

#endif

	return ra;
}


//////////////////////////////////////////////////////////////////////////

static std::string _formulaToStr(std::string seperator, const int* moves, size_t _size)
{
	stringstream ss;

	for (size_t i = 0; i < _size; ++i)
	{
		int m = moves[i];

		bool is_move_2n = false;
		if (isMove2n((Move)m))
		{
			m = getPureMove((Move)m);
			is_move_2n = true;
		}


		if (i > 0)
			ss << seperator;

		if (0 <= m && m < 54)
		{
			string a = move2str[m];

			if (is_move_2n)
			{
				assert(a.size() >= 2 && a[a.size() - 1] == '2');
				ss << a.substr(0, a.size() - 1) << "'2";
			}
			else
			{
				ss << a;
			}
		}
		else
		{
			ss << "~";
		}



	}

	return ss.str();
}

static std::string _formulaToStrP(std::string seperator, const int* moves, size_t _size)
{
	stringstream ss;

	for (size_t i = 0; i < _size; ++i)
	{
		int m = moves[i];

		bool is_move_2n = false;
		if (isMove2n((Move)m))
		{
			m = getPureMove((Move)m);
			is_move_2n = true;
		}

		if (m < 0 || m >= 54)	// 非法返回空
			return string();

		if (i > 0)
			ss << seperator;


		string a = move2strP[m];
		ss << a;

		if (is_move_2n)
			ss << 'n';
	}

	return ss.str();
}


std::string formulaToStr(const std::vector<cs::Move>& moves, std::string seperator)
{
	if (moves.empty())
		return "";

	return _formulaToStr(seperator, (int*)&moves[0], moves.size());
}

std::string formulaToStr(const std::vector<int>& moves, std::string seperator)
{
	if (moves.empty())
		return "";

	return _formulaToStr(seperator, &moves[0], moves.size());
}

std::string formulaToCodeStr(const std::vector<cs::Move>& moves, std::string seperator)
{
	if (moves.empty())
		return "";

	return _formulaToStrP(seperator, (int*)&moves[0], moves.size());
}

std::string formulaToCodeStr(const std::vector<int>& moves, std::string seperator)
{
	if (moves.empty())
		return "";

	return _formulaToStrP(seperator, &moves[0], moves.size());
}

std::vector<Move> toVirtualMoves(const std::vector<Move>& moves)
{
	std::vector<Move> result;


	for (Move m : moves)
	{

		if (m == -1)
			int tmp = 1;

		m = getPureMove(m);


		if ((Ux1 <= m && m <= Bx3) || 
			(Rot_x1 <= m && m <= Rot_z3) ||
			(Mx1 <= m && m <= Mx3) ||
			m == InvalidMove
			)
		{
			int ax = m / 3;
			int po = m % 3 + 1;

			if (po == 2)
			{
				result.push_back((Move)(ax * 3));
				result.push_back((Move)(ax * 3));
			}
			else
			{
				result.push_back(m);
			}
		}
		else if (
			(LowerUx1 <= m && m <= LowerBx3) ||
			(Ex1 <= m && m <= Ex1) ||
			(Sx1 <= m && m <= Sx1)
		)
		{
			std::vector<Move> mm = move_decompose(m);

			for (Move a : mm)
			{
				int ax = a / 3;
				int po = a % 3 + 1;

				if (po == 2)
				{
					result.push_back((Move)(ax * 3));
					result.push_back((Move)(ax * 3));
				}
				else
				{
					result.push_back(a);
				}
			}
		}
		else
		{
			throw "Unhandled error.";
		}
	}


	return result;
}

std::vector<int> toVirtualMoves(const std::vector<int>& moves)
{
    std::vector<Move> move = toVirtualMoves(reinterpret_cast<const vector<Move>&>(moves));
	return reinterpret_cast<vector<int>&>(move);
}

std::set<cs::Move> getValidVirtualMoves()
{
	return std::set<cs::Move> {
		Ux1, Ux3, Rx1, Rx3, Fx1, Fx3, Dx1, Dx3, Lx1, Lx3, Bx1, Bx3, Mx1, Mx3, Rot_x1, Rot_x3, Rot_y1, Rot_y3, Rot_z1, Rot_z3
	};
}


//////////////////////////////////////////////////////////////////////////
CubeFrames correct_old_type_data_of_virtual_cube(const CubeFrames & frames)
{
	if (frames.empty())
		return CubeFrames();


	CubeFrames new_frames;
	new_frames.identify = frames.identify;
	new_frames.version = frames.version;


	AxisCubieCube cube;	// 假设初始 axis 等于 eye
	//cube.axis = AxisCube::fromValidIdx(CubeFrames::getPreAxis((Move)frames.records[0].move.raw, frames.records[0].axis_cube));

	new_frames.records.resize(frames.records.size());

	for (size_t i = 0; i < frames.records.size(); ++i)
	{
		int m = frames.records[i].move.raw;

		auto moves = move_decompose2((Move)m);
		auto moves_basic = moves[0];

		if (moves_basic.size() > 0)	// 有基本公式
		{
			vector<Move> moves_abs(moves_basic.size());	// 绝对公式

			for (size_t j = 0; j < moves_abs.size(); ++j)
			{
				int ma = cube.formulaR2A((Move)m);
				moves_abs[j] = (Move)ma;
			}

			new_frames.records[i].move.setMoves(moves_abs);
		}
		else
		{
			new_frames.records[i].move.setMove(InvalidMove);
		}

		int ax = frames.records[i].axis_cube;
		cube = cube.move(m);

		int ax2 = cube.axis.getValidIdx();

		//assert(ax == ax2);

		new_frames.records[i].axis_cube = ax2;
		new_frames.records[i].ds_time = frames.records[i].ds_time;
	}


	return new_frames;
}

CubeFrames correct_old_type_data_of_smart_cube(const CubeFrames & frames)
{
	if (frames.empty())
		return CubeFrames();


	CubeFrames new_frames;
	new_frames.identify = frames.identify;
	new_frames.version = frames.version;


	new_frames.records.resize(frames.records.size());

	for (size_t i = 0; i < frames.records.size(); ++i)
	{
		int m = frames.records[i].move.raw;

		new_frames.records[i].move.setMove((Move)m);
		new_frames.records[i].axis_cube = frames.records[i].axis_cube;
		new_frames.records[i].ds_time = frames.records[i].ds_time;
	}


	return new_frames;
}

std::string scramble_formula_moves_to_str(std::vector<Move> moves)
{
	stringstream ss;

	for (int m : moves)
	{
		if (0 <= m && m < 18)
		{
			int ax = m / 3;
			int po = m % 3 + 1;

			ss << cs::colorNames[ax] << po;
		}
		else
		{
			return std::string();
		}
	}

	string s = ss.str();

	assert(s.size() == 2 * moves.size());

	return s;
}

std::vector<cs::Move> scramble_formula_str_to_moves(std::string s)
{

	if (s.size() % 2 != 0)
		return vector<Move>();

	vector<Move> moves;

	for (size_t i = 0; i < s.size(); i+=2)
	{
		char ax_s = s[i];
		int ax = -1;

		if (ax_s == 'U')
			ax = 0;
		else if (ax_s == 'R')
			ax = 1;
		else if (ax_s == 'F')
			ax = 2;
		else if (ax_s == 'D')
			ax = 3;
		else if (ax_s == 'L')
			ax = 4;
		else if (ax_s == 'B')
			ax = 5;
		else
			return vector<Move>();
		
		int po;
		try {
			po = std::stoi(s.substr(i + 1, 1)) - 1;
		}
		catch (exception e)
		{
			return vector<Move>();
		}
		
		moves.push_back(static_cast<Move>(3 * ax + po));


	}

	assert(moves.size() * 2 == s.size());

	return moves;
}


bool check_scramble_and_frames(const cs::CubieCube & cube, const CubeFrames & frames)
{
	CubieCube dd = cube;

	for (size_t i = 0; i < frames.records.size(); ++i)
	{
		dd = dd.move(frames.records[i].move.getMoves());
	}

	return dd.iseye();
}

bool check_scramble_and_frames(const cs::CubieCube & cube, const std::vector<char>& frames)
{
	CubeFrames cf = CubeFrames::load(frames);
	return check_scramble_and_frames(cube, cf);
}

bool check_scramble_and_frames(const std::vector<Move>& scramble, const CubeFrames & frames)
{
	AxisCubieCube acc;
	acc = acc.move(scramble);
	CubieCube cube = acc.getAbsCubie();
	return check_scramble_and_frames(cube, frames);
}

bool check_scramble_and_frames(const std::vector<Move>& scramble, const std::vector<char>& frames)
{
	CubeFrames cf = CubeFrames::load(frames);
	return check_scramble_and_frames(scramble, cf);
}

bool check_scramble_and_frames(const std::string & scramble, const CubeFrames & frames)
{
	vector<Move> moves = scramble_formula_str_to_moves(scramble);
	return check_scramble_and_frames(moves, frames);
}

bool check_scramble_and_frames(const std::string & scramble, const std::vector<char>& frames)
{
	vector<Move> moves = scramble_formula_str_to_moves(scramble);
	CubeFrames cf = CubeFrames::load(frames);
	return check_scramble_and_frames(moves, cf);
}

CubeFrames correct_frames_ds_time(int total, const CubeFrames& frames)
{
	if (frames.empty())
		return CubeFrames();

	vector<double> ds_times(frames.records.size());

	double sum = 0.0;

	for (size_t i = 0; i < frames.records.size(); ++i)
	{
		double a = frames.records[i].ds_time;
		ds_times[i] = a;
		sum += a;
	}

	double factor = total / sum;


	CubeFrames result = frames;

	if (factor >= 0.9 && factor <= 1.1)
	{
		double sum2 = 0.0;

		for (size_t i = 0; i < ds_times.size(); ++i)
		{
			double a = ds_times[i] * factor;
			ds_times[i] = a;
			sum2 += a;
		}

		auto new_ds_times = round_vector_eq_sum(ds_times, total);

		int sum3 = 0;
		for (size_t i = 0; i < frames.records.size(); ++i)
		{
			int a = new_ds_times[i];
			result.records[i].ds_time = a;
			sum3 += a;
		}

		assert(sum3 == total);

		return result;
	}
	else
		return CubeFrames();
}

CubeFrames correct_frames_ds_time_old_data(int total, const CubeFrames & frames)
{
	if (frames.empty())
		return CubeFrames();

	CubeFrames cf = correct_frames_ds_time(total, frames);

	if (!cf.empty())
		return cf;

	// 重新尝试
	int sum = 0;

	for (size_t i = 0; i < frames.records.size(); ++i)
	{
		sum += frames.records[i].ds_time;
	}

	double factor = total / sum;


	CubeFrames result = frames;

	if (factor > 1.1 && frames.records[0].ds_time == 0)
	{
		result.records[0].ds_time = total - sum;	// 小概率到达
	}
	else if (factor < 0.9 && frames.records[0].ds_time > 1000)
	{
		result.records[0].ds_time = 0;
	}

	return correct_frames_ds_time(total, result);
}


bool check_frames_ds_time(int total, const CubeFrames& frames)
{
	if (frames.empty())
		return false;

	int sum = 0;
	for (size_t i = 0; i < frames.records.size(); ++i)
	{
		int a = frames.records[i].ds_time;
		sum += a;
	}

	return total == sum;
}
