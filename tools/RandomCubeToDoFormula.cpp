// 用来生成 CFOP 和 层先法公式练习所用的状态


#include "RandomCubeToDoFormula.h"
#include <tuple>


using namespace std;
using namespace cs;

AxisCubieCube randomCubeToDoFormulaF2L(int axis, int idx)
{
	AxisCubieCube cc;

	if (idx < 0 || idx >= 41 || axis < 0 || axis >= 24)
		return cc;

	cc.cubie = CubieCube::randomCube();

	cc = cc.rotateToAxis(axis);


	HumanSolverCFOP solver;

	vector<Move> moves = solver.solveF2L(cc);

	cc = cc.move(moves);

	cc = cc.rotateToAxis(axis);

	vector<Move> mvf = get<2>(HumanSolverCFOP::F2L_table[idx]);

	AxisCubieCube aa = cc.move(mvf);

	cc = cc.rotateToAxis(aa.axis);

	AxisCubieCube dd;
	dd = dd.move(mvf);

	cc = cc * dd.inv();

	return cc;
}

AxisCubieCube randomCubeToDoFormulaF2L(int axis)
{
	int idx = randInt(0, 40);
	return randomCubeToDoFormulaF2L(axis, idx);
}


static int _rand_axis_with_bottom(int bottom)
{
	vector<int> axis;

	for (int i = 0; i < 24; ++i)
	{
		AxisCube ac = AxisCube::fromValidIdx(i);

		if (ac[3] == bottom)
			axis.push_back(i);
	}

	int j = randInt(0, axis.size() - 1);
	return axis[j];
}

cs::AxisCubieCube randomCubeToDoFormulaF2L_v2(int bottom, int idx)
{
	if (bottom < 0 || bottom >= 6)
		return cs::AxisCubieCube();

	int axis = _rand_axis_with_bottom(bottom);

	AxisCubieCube cc = randomCubeToDoFormulaF2L(axis, idx);

	int n = randInt(0, 3);

	for (int i = 0; i < n; ++i)
	{
		cc = cc.move(Ux1);
	}

	return cc;
}

cs::AxisCubieCube randomCubeToDoFormulaF2L_v2(int bottom)
{
	int idx = randInt(0, 40);
	return randomCubeToDoFormulaF2L_v2(bottom, idx);
}

cs::AxisCubieCube randomCubeToDoFormulaOLL(int axis, int idx)
{
	AxisCubieCube cc;

	if (idx < 0 || idx >= 57 || axis < 0 || axis >= 24)
		return cc;

	cc.cubie = CubieCube::randomCube();

	cc = cc.rotateToAxis(axis);


	HumanSolverCFOP solver;

	vector<Move> moves = solver.solveOLL(cc);

	cc = cc.move(moves);

	cc = cc.rotateToAxis(axis);

	vector<Move> mvf = get<2>(HumanSolverCFOP::OLL_table[idx]);

	AxisCubieCube aa = cc.move(mvf);

	cc = cc.rotateToAxis(aa.axis);

	AxisCubieCube dd;
	dd = dd.move(mvf);

	cc = cc * dd.inv();

	return cc;
}

cs::AxisCubieCube randomCubeToDoFormulaOLL(int axis)
{
	int idx = randInt(0, 56);
	return randomCubeToDoFormulaOLL(axis, idx);
}

cs::AxisCubieCube randomCubeToDoFormulaOLL_v2(int bottom, int idx)
{
	if (bottom < 0 || bottom >= 6)
		return cs::AxisCubieCube();

	int axis = _rand_axis_with_bottom(bottom);

	AxisCubieCube cc = randomCubeToDoFormulaOLL(axis, idx);


	int n = randInt(0, 3);

	for (int i = 0; i < n; ++i)
	{
		cc = cc.move(Ux1);
	}


	return cc;
}

cs::AxisCubieCube randomCubeToDoFormulaOLL_v2(int bottom)
{
	int idx = randInt(0, 56);
	return randomCubeToDoFormulaOLL_v2(bottom, idx);
}

cs::AxisCubieCube randomCubeToDoFormulaPLL(int axis, int idx)
{
	AxisCubieCube cc;

	if (idx < 0 || idx >= 21 || axis < 0 || axis >= 24)
		return cc;


	cc = cc.rotateToAxis(axis);

	vector<Move> mvf = get<2>(HumanSolverCFOP::PLL_table[idx]);

	AxisCubieCube aa = cc.move(mvf);

	cc = cc.rotateToAxis(aa.axis);

	AxisCubieCube dd;
	dd = dd.move(mvf);

	cc = cc * dd.inv();

	return cc;
}

cs::AxisCubieCube randomCubeToDoFormulaPLL(int axis)
{
	int idx = randInt(0, 20);
	return randomCubeToDoFormulaPLL(axis, idx);
}

cs::AxisCubieCube randomCubeToDoFormulaPLL_v2(int bottom, int idx)
{
	if (bottom < 0 || bottom >= 6)
		return cs::AxisCubieCube();

	int axis = _rand_axis_with_bottom(bottom);

	AxisCubieCube cc = randomCubeToDoFormulaPLL(axis, idx);


	int n = randInt(0, 3);

	for (int i = 0; i < n; ++i)
	{
		cc = cc.move(Ux1);
	}


	return cc;
}

cs::AxisCubieCube randomCubeToDoFormulaPLL_v2(int bottom)
{
	int idx = randInt(0, 20);
	return randomCubeToDoFormulaPLL_v2(bottom, idx);
}
