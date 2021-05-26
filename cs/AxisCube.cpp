#include "AxisCube.h"
#include "CubieCube.h"
#include <iostream>


namespace cs
{
	//bool AxisCube::myInited = false;

	const AxisCube AxisCube::valid_table[24] =
	{
		AxisCube({0,1,2,3,4,5}),	// 0  eye
		AxisCube({0,2,4,3,5,1}),	// 1  y'
		AxisCube({0,4,5,3,1,2}),	// 2  y2
		AxisCube({0,5,1,3,2,4}),	// 3  y
		AxisCube({1,0,5,4,3,2}),	// 4
		AxisCube({1,2,0,4,5,3}),	// 5
		AxisCube({1,3,2,4,0,5}),	// 6  z'
		AxisCube({1,5,3,4,2,0}),	// 7
		AxisCube({2,0,1,5,3,4}),	// 8
		AxisCube({2,1,3,5,4,0}),	// 9  x
		AxisCube({2,3,4,5,0,1}),	// 10
		AxisCube({2,4,0,5,1,3}),	// 11
		AxisCube({3,1,5,0,4,2}),	// 12  x2
		AxisCube({3,2,1,0,5,4}),	// 13
		AxisCube({3,4,2,0,1,5}),	// 14  z2
		AxisCube({3,5,4,0,2,1}),	// 15
		AxisCube({4,0,2,1,3,5}),	// 16  z
		AxisCube({4,2,3,1,5,0}),	// 17
		AxisCube({4,3,5,1,0,2}),	// 18
		AxisCube({4,5,0,1,2,3}),	// 19
		AxisCube({5,0,4,2,3,1}),	// 20
		AxisCube({5,1,0,2,4,3}),	// 21  x'
		AxisCube({5,3,1,2,0,4}),	// 22
		AxisCube({5,4,3,2,1,0})		// 23
	};


	const int AxisCube::inv_table[24] =
	{
		0, // 0 : {0,1,2,3,4,5} -> the same
		3, // 1 : {0,2,4,3,5,1} -> {0,5,1,3,2,4}
		2, // 2 : {0,4,5,3,1,2} -> the same
		1, // 3 : {0,5,1,3,2,4} -> {0,2,4,3,5,1}
		4, // 4 : {1,0,5,4,3,2} -> the same
		8, // 5 : {1,2,0,4,5,3} -> {2,0,1,5,3,4}
		16, // 6 : {1,3,2,4,0,5} -> {4,0,2,1,3,5}
		20, // 7 : {1,5,3,4,2,0} -> {5,0,4,2,3,1}
		5, // 8 : {2,0,1,5,3,4} -> {1,2,0,4,5,3}
		21, // 9 : {2,1,3,5,4,0} -> {5,1,0,2,4,3}
		19, // 10 : {2,3,4,5,0,1} -> {4,5,0,1,2,3}
		11, // 11 : {2,4,0,5,1,3} -> the same
		12, // 12 : {3,1,5,0,4,2} -> the same
		13, // 13 : {3,2,1,0,5,4} -> the same
		14, // 14 : {3,4,2,0,1,5} -> the same
		15, // 15 : {3,5,4,0,2,1} -> the same
		6, // 16 : {4,0,2,1,3,5} -> {1,3,2,4,0,5}
		22, // 17 : {4,2,3,1,5,0} -> {5,3,1,2,0,4}
		18, // 18 : {4,3,5,1,0,2} -> the same
		10, // 19 : {4,5,0,1,2,3} -> {2,3,4,5,0,1}
		7, // 20 : {5,0,4,2,3,1} -> {1,5,3,4,2,0}
		9, // 21 : {5,1,0,2,4,3} -> {2,1,3,5,4,0}
		17, // 22 : {5,3,1,2,0,4} -> {4,2,3,1,5,0}
		23 // 23 : {5,4,3,2,1,0} -> the same
	};

	// valid_table 生成表
	const Move AxisCube::gen_table[24][2]
	{
		InvalidMove, InvalidMove,	// {0,1,2,3,4,5} // 0  eye
		Rot_y3, InvalidMove,	// {0,2,4,3,5,1} // 1  y'
		Rot_y2, InvalidMove,	// {0,4,5,3,1,2} // 2  y2
		Rot_y1, InvalidMove,	 // {0,5,1,3,2,4} // 3  y
		Rot_z3, Rot_y2,			// {1,0,5,4,3,2} // 4
		Rot_z3, Rot_y3,			// {1,2,0,4,5,3} // 5
		Rot_z3, InvalidMove,	// {1,3,2,4,0,5} // 6  z'
		Rot_z3, Rot_y1,			// {1,5,3,4,2,0} // 7
		Rot_x1, Rot_y1,			// {2,0,1,5,3,4} // 8
		Rot_x1, InvalidMove,	// {2,1,3,5,4,0} // 9  x
		Rot_x1, Rot_y3,			// {2,3,4,5,0,1} // 10
		Rot_x1, Rot_y2,			// {2,4,0,5,1,3} // 11
		Rot_x2, InvalidMove,	// {3,1,5,0,4,2} // 12  x2
		Rot_x2, Rot_y1,			// {3,2,1,0,5,4} // 13
		Rot_z2, InvalidMove,	// {3,4,2,0,1,5} // 14  z2
		Rot_x2, Rot_y3,			// {3,5,4,0,2,1} // 15
		Rot_z1, InvalidMove,	// {4,0,2,1,3,5} // 16  z
		Rot_z1, Rot_y3,			// {4,2,3,1,5,0} // 17
		Rot_z1, Rot_y2,			// {4,3,5,1,0,2} // 18
		Rot_z1, Rot_y1,			// {4,5,0,1,2,3} // 19
		Rot_x3, Rot_y3,			// {5,0,4,2,3,1} // 20
		Rot_x3, InvalidMove,	// {5,1,0,2,4,3} // 21  x'
		Rot_x3, Rot_y1,			// {5,3,1,2,0,4} // 22
		Rot_x3, Rot_y2,			// {5,4,3,2,1,0} // 23
	};


	// move table, 用 valid_table 下标表示 
	// table index:                        0  1  2   3   4   5   6   7   8
	// (int)Move:                         18  19 20  21  22  23  24  25  26
	// Move enum (Rot_**):                x1, x2 x3, y1, y2, y3, z1, z2, z3
	const int AxisCube::move_table[9] = { 9, 12, 21, 3,  2,  1,  16, 14, 6 };


	// 生成valid_table 的公式(即gen_table) 所对应的 CubieCube （由同样公式生成）
	CubieCube AxisCube::gen_cubie_table[24];

	// move_conj_table[i,j] = axis[j] * move[i] * axis[j]^-1
	int AxisCube::move_conj_table_i[18][24];

	// move_conj_table[i,j] = axis[j]^-1 * move[i] * axis[j]
	int AxisCube::move_conj_table[18][24];


	//////////////////////////////////////////////////////////////////////////
	bool AxisCube::inited = false;

	AxisCube::AxisCube()
		: _base()
	{

	}
	AxisCube::AxisCube(const AxisCube & _right)
		: _base((const _base&) _right)
	{
	}
	AxisCube::AxisCube(const permutation<6>& _right)
		: _base(_right)
	{
	}

	AxisCube AxisCube::move(Move mv) const
	{
		mv = (Move)(mv & 0xff);

		AxisCube dd = *this;
		if (Rot_x1 <= mv && mv <= Rot_z3) // 基本公式和整体转动
		{
			dd = dd._move(mv);
		}
		else if (Ex1 <= mv && mv <= Sx3)	// 中间层
		{
			int idx = mv - Ex1;
			dd = dd.move(CubieCube::middle_table[idx][2]);
		}
		else if (LowerUx1 <= mv && mv <= LowerBx3)	// 转两层
		{
			int idx = mv - LowerUx1;
			dd = dd.move(CubieCube::lower_table[idx][1]);
		}

		return dd;
	}
	AxisCube AxisCube::_move(Move mv) const
	{
		AxisCube b = valid_table[move_table[(int)mv - (int)Rot_x1]];
		AxisCube c = (*this) * b;

		return c;


	}

	AxisCube AxisCube::move(std::vector<Move> mv) const
	{
		AxisCube dd = *this;

		for (Move m : mv)
		{
			dd = dd.move(m);
		}
		return dd;
	}

	bool AxisCube::isValid() const
	{
		bool ret = false;
		for (int i = 0; i < num_valid_table; ++i)
		{
			if ((*this) == valid_table[i])
			{
				ret = true;
				break;
			}
		}
		

		return ret;
	}

	AxisCube AxisCube::fromValidIdx(int idx)
	{
		return valid_table[idx];
	}

	int AxisCube::getValidIdx() const
	{
		int idx = -1;
		for (int i = 0; i < 24; ++i)
		{
			if (valid_table[i] == *this)
			{
				idx = i;
				break;
			}
		}

		return idx;
	}

	std::vector<Move> AxisCube::getGenFormula() const
	{
		return getGenFormula(getValidIdx());
	}


	std::vector<Move> AxisCube::getGenFormula(int idx)
	{
		std::vector<Move> mm;

		for (int j = 0; j < 2; ++j)
		{
			Move m = gen_table[idx][j];

			if (m != InvalidMove)
				mm.push_back(m);
		}

		return mm;
	}

	void AxisCube::init()
	{
		if (inited)
			return;

		inited = true;

		// gen_cubie_table
		CubieCube eye = CubieCube::eye();
		for (int i = 0; i < 24; ++i)
		{
			CubieCube cc = eye;
			for (int j = 0; j < 2; ++j)
			{
				if (gen_table[i][j] != InvalidMove)
					cc = cc.move(gen_table[i][j]);
			}
			gen_cubie_table[i] = cc;
		}

		// move_conj_table
		CubieCube moveTbl[18];

		for (int i = 0; i<18; ++i)
		{
			moveTbl[i] = eye.move(i);
		}


		for (int i = 0; i < 18; ++i)
		{
			for (int j = 0; j < 24; ++j)
			{
				CubieCube cc = gen_cubie_table[j] * moveTbl[i] * gen_cubie_table[j].inv();

				for (int k = 0; k < 18; ++k)
				{
					if (cc == moveTbl[k])
					{
						move_conj_table_i[i][j] = k;
						break;
					}
				}


				CubieCube cc2 = gen_cubie_table[j].inv() * moveTbl[i] * gen_cubie_table[j];

				for (int k = 0; k < 18; ++k)
				{
					if (cc2 == moveTbl[k])
					{
						move_conj_table[i][j] = k;
						break;
					}
				}
			}
		}

	}

}