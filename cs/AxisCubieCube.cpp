#include "AxisCube.h"
#include "CubieCube.h"
#include <iostream>
#include "AxisCubieCube.h"
#include <vector>


using namespace std;

namespace cs
{



	int AxisCubieCube::move_conj[54][24];
	

	//////////////////////////////////////////////////////////////////////////
	bool AxisCubieCube::inited = false;

	AxisCubieCube::AxisCubieCube()
	{
	}

	AxisCubieCube::AxisCubieCube(const AxisCube & ax, const CubieCube & cc)
		: axis(ax)
		, cubie(cc)
	{
	}

	AxisCubieCube AxisCubieCube::move(Move m) const
	{
		return AxisCubieCube(axis.move(m), cubie.move(m));
	}

	AxisCubieCube AxisCubieCube::move(int m) const
	{
		return move((Move)m);
	}

	AxisCubieCube AxisCubieCube::move(std::vector<Move> moves) const
	{
		AxisCubieCube B = *this;
		for (Move m : moves)
		{
			B = B.move(m);
		}
		return B;
	}

	AxisCubieCube AxisCubieCube::moveAbs(Move m) const
	{
		Move m2 = formulaA2R(m);

		return move(m2);
	}

	AxisCubieCube AxisCubieCube::moveAbs(int m) const
	{
		return moveAbs((Move)m);
	}

	AxisCubieCube AxisCubieCube::moveAbs(std::vector<Move> moves) const
	{
		AxisCubieCube B = *this;
		for (Move m : moves)
		{
			B = B.moveAbs(m);
		}
		return B;
	}

	AxisCubieCube AxisCubieCube::rotateToAxis(AxisCube ac) const
	{
		AxisCube da = axis.inv() * ac;
		auto gen = da.getGenFormula();
		AxisCubieCube dd = this->move(gen);

		return dd;
	}

	AxisCubieCube AxisCubieCube::rotateToAxis(int ac) const
	{
		return rotateToAxis(AxisCube::fromValidIdx(ac));
	}

	Move AxisCubieCube::formulaA2R(Move m) const
	{
		if (m < 0)
			return m;

		int idx = axis.getValidIdx();
		return (Move)move_conj[(int)m][idx];
	}

	std::vector<Move> AxisCubieCube::formulaA2R(std::vector<Move> m) const
	{
		std::vector<Move> m2(m.size());


		AxisCubieCube dd = *this;
		for (int i = 0; i < (int)m2.size(); ++i)
		{
			m2[i] = dd.formulaA2R(m[i]);
			dd = dd.move(m2[i]);
		}

		return m2;
	}

	Move AxisCubieCube::formulaR2A(Move m) const
	{
		if (m < 0)
			return m;

		int idx = AxisCube::inv_table[axis.getValidIdx()];
		return (Move)move_conj[(int)m][idx];
	}

	std::vector<Move> AxisCubieCube::formulaR2A(std::vector<Move> m) const
	{
		std::vector<Move> m2(m.size());

		AxisCubieCube dd = *this;
		for (int i = 0; i < (int)m2.size(); ++i)
		{
			m2[i] = dd.formulaR2A(m[i]);
			dd = dd.move(m[i]);
		}

		return m2;
	}

	Move AxisCubieCube::formulaA2R(int axis_cube, Move m)
	{
		if (m < 0)
			return m;

		if (axis_cube < 0 || axis_cube > 24)
			return InvalidMove;

		return (Move)move_conj[(int)m][axis_cube];
	}

	Move AxisCubieCube::formulaR2A(int axis_cube, Move m)
	{
		if (m < 0)
			return m;

		if (axis_cube < 0 || axis_cube > 24)
			return InvalidMove;

		int idx = AxisCube::inv_table[axis_cube];
		return (Move)move_conj[(int)m][idx];
	}

	AxisCubieCube AxisCubieCube::eye()
	{
		return AxisCubieCube();
	}

	bool AxisCubieCube::iseye() const
	{
		return axis.iseye() && cubie.iseye();
	}

	bool AxisCubieCube::isSolved() const
	{
		return getAbsCubie().iseye();
	}

	AxisCubieCube AxisCubieCube::mul(const AxisCubieCube & b) const
	{
		return AxisCubieCube(axis * b.axis, cubie * b.cubie);
	}

	AxisCubieCube AxisCubieCube::operator*(const AxisCubieCube& b) const
	{
		return mul(b);
	}

	AxisCubieCube AxisCubieCube::inv() const
	{
		return AxisCubieCube(axis.inv(), cubie.inv());
	}

	bool AxisCubieCube::operator==(const AxisCubieCube & b) const
	{
		return axis == b.axis && cubie == b.cubie;
	}

	CubieCube AxisCubieCube::getAbsCubie() const
	{
		AxisCube pa = axis.inv();
		int idx = pa.getValidIdx();
		CubieCube cc = cubie;


		for (int i = 0; i < 2; ++i)
		{
			int m = AxisCube::gen_table[idx][i];
			if (m != InvalidMove)
				cc = cc.move(m);
		}

		return cc;
	}

	void AxisCubieCube::setAbsCubie(const CubieCube & cc)
	{
		int idx = axis.getValidIdx();
		CubieCube dd = cc;

		for (int i = 0; i < 2; ++i)
		{
			int m = AxisCube::gen_table[idx][i];
			if (m != InvalidMove)
				dd = dd.move(m);
		}

		cubie = dd;
	}

	ColorCube AxisCubieCube::toColorCube() const
	{
		ColorCube cc = cubie.toColorCube();

		for (int i = 0; i < 6; ++i)
		{
			cc[4 + i * 9] = axis[i];
		}

		return cc;
		
	}

	void AxisCubieCube::init()
	{
		if (inited)
			return;

		inited = true;

		// move_conj

		AxisCubieCube eye = AxisCubieCube::eye();

		vector<AxisCubieCube> moves(54);
		for (int i = 0; i < 54; ++i)
		{
			moves[i] = eye.move(i);
		}

		for (int i = 0; i < 54; ++i)
		{
			for (int j = 0; j < 24; ++j)
			{
				AxisCubieCube S = eye;

				for (int k = 0; k < 2; ++k)
				{
					int m = AxisCube::gen_table[j][k];

					if (m != InvalidMove)
						S = S.move(m);
				}

				AxisCubieCube dd = S.inv() * moves[i] * S;


				for (int k = 0; k < 54; ++k)
				{
					if (moves[k] == dd)
					{
						move_conj[i][j] = k;
						break;
					}
				}
			}
		}
	}


}