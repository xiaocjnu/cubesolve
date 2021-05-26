#pragma once

#include "util.h"
#include "AxisCubieCube.h"
#include "CubieCube.h"
#include "CoordCube.h"
#include <vector>
#include <tuple>


namespace cs
{
	class HumanSolver
	{
	public:

		void setCube(const CubieCube& cc);



		// 底层十字是否还原, 包括方向和位置
		static bool isDCrossSolved(const CubieCube& cc);

		// 仅仅判断底层四个角块是否归位且方向正确
		static bool isOnlyDCornerSolved(const CubieCube& cc);

		// 底面1层是否还原
		static bool isDLayerSolved(const CubieCube& cc);

		// 底面两层层是否还原
		static bool isF2LSolved(const CubieCube& cc);

		// 仅仅计算 F2L 还原了几对，忽略掉底层十字
		static int numPairF2LIgnoreDCross(const CubieCube& cc);

		// 计算 F2L 还原了几对， 4对等价于已复原 F2L
		static int numPairF2LSolved(const CubieCube& cc);

		// 忽略前2层，仅仅判断顶面十字是否还原，只考虑U色方向不考虑排列
		static bool isOnlyUCrossSolved(const CubieCube& cc);

		// 顶面十字是否还原，只考虑U色方向不考虑排列
		static bool isUCrossSolved(const CubieCube& cc);

		// 忽略前2层，仅仅判断顶面4个角块的方向，不考虑位置
		static bool isOnlyUCornerOriSolved(const CubieCube& cc);

		// 忽略前2层，仅仅判断顶面4个棱块的方向，不考虑位置
		static bool isOnlyUEdgeOriSolved(const CubieCube& cc);

		// 顶面是否还原，只考虑U色方向不考虑排列
		static bool isOLLSolved(const CubieCube& cc);


		// 仅仅判断顶面4个角块的位置
		static bool isOnlyUCornerPermSolved(const CubieCube& cc);

		// 顶面四个角块是否还原，考虑排列和方向
		static bool isUCornerSolved(const CubieCube& cc);


		// 仅仅判断顶面4个棱块的位置
		static bool isOnlyUEdgePermSolved(const CubieCube& cc);


		// 是否已经还原
		static bool isSolved(const CubieCube& cc);


		// 以下函数判断按照空间位置，不是按照颜色

		// 底面十字是否还原
		static bool isDCrossSolved(const AxisCubieCube& cc);

		// 底面1层是否还原
		static bool isDLayerSolved(const AxisCubieCube& cc);

		// 底面两层层是否还原
		static bool isF2LSolved(const AxisCubieCube& cc);

		// 计算 F2L 还原了几对， 4对等价于已复原 F2L
		static int numPairF2LSolved(const AxisCubieCube& cc);

		// 顶面十字是否还原，只考虑U色方向不考虑排列
		static bool isUCrossSolved(const AxisCubieCube& cc);

		// 顶面是否还原，只考虑U色方向不考虑排列
		static bool isOLLSolved(const AxisCubieCube& cc);

		// 是否还原
		static bool isSolved(const AxisCubieCube& cc);

		// 顶面四个角块是否还原，考虑排列和方向
		static bool isUCornerSolved(const AxisCubieCube& cc);


		// 将公式插入公式后面
		static void formula_append(std::vector<Move>& moves, Move m);
		static void formula_append(std::vector<Move>& moves, const std::vector<Move>& mm);


		// 将公式插入公式后面, 合并相同轴的公式
		static void formula_append_merge(std::vector<Move>& moves, Move m);
		static void formula_append_merge(std::vector<Move>& moves, const std::vector<Move>& mm);

		// 将公式插入公式后面, 合并整体转动相同轴的公式
		static void formula_append_merge_rot(std::vector<Move>& moves, Move m);
		static void formula_append_merge_rot(std::vector<Move>& moves, const std::vector<Move>& mm);


		// 合并连续重复轴的公式
		static std::vector<Move> mergeRepetedFormula(const std::vector<Move>& moves);


		// 合并连续整体转动重复轴的公式
		static std::vector<Move> mergeRepetedRotateFormula(const std::vector<Move>& moves);

	protected:
		CubieCube myCubieCubie;
	};


	//////////////////////////////////////////////////////////////////////////
	// 层先法，每一步单独求解
	class HumanSolverLBL
	{
	public:
		static Move conjMove[4];		// Invalid, y1, y2, y3
		static int conjInv[4];
		static AxisCubieCube conjCube[4];	// eye, y1, y2, y3
		static int moveConj[54][4];	// moveConj[i][j] =  S(j).inv() * move(i) * S(j) = conjCube[j]


		// 初始化静态表，调用 solve 会自动调用
		static void init();

		// 求解魔方，从D颜色开始
		std::vector<Move> solve(const CubieCube& cc);

		// 求解魔方，开始的颜色有AxisCube 决定
		std::vector<Move> solve(const AxisCubieCube& cc);

		std::vector<Move> solve(const AxisCube& axis, const CubieCube& cc);

		std::vector<Move> solveDCross();
		std::vector<Move> solveDLayer();
		std::vector<Move> solveMLayer();
		std::vector<Move> solveUCross();
		std::vector<Move> solveUFace();
		std::vector<Move> solveUCorner();
		std::vector<Move> solveFinal();

	protected:

		// 求解 DFR(4) 角块
		static std::vector<Move> _solveDLayerDFR(const AxisCubieCube& cc);

		// 求解 FR(8) 棱块
		static std::vector<Move> _solveMLayerFR(const AxisCubieCube& cc);

		static void _init_moveConj();


	protected:
		static bool inited;

		int myError;
		bool myIsSolved = false;
		AxisCubieCube mySrcCube;

		std::vector<Move> myDCrossMoves;
		std::vector<Move> myDLayerMoves;
		std::vector<Move> myMLayerMoves;
		std::vector<Move> myUCrossMoves;
		std::vector<Move> myUFaceMoves;
		std::vector<Move> myUCornerMoves;
		std::vector<Move> myFinalMoves;

		std::vector<Move> myMoves;

		AxisCubieCube myDCrossCube;
		AxisCubieCube myDLayerCube;
		AxisCubieCube myMLayerCube;
		AxisCubieCube myUCrossCube;
		AxisCubieCube myUFaceCube;
		AxisCubieCube myUCornerCube;
		AxisCubieCube myFinalCube;
	};


	// 层先法，每一步单独搜索求解
	class HumanSolverLBLSearch
	{
	public:
		static int moveConjY2[18];		// y2 * mv * y2

		static int DCrossMove[528][18];		// DR(4),DF(5): A(12,2) * 2^2 = 132 * 4 = 528
		static int DCrossPrun[528];

		static int DLayerMove[504][18];		// DFR(4), DLF(5):  A(8,2) * 3^2 = 56 * 9 = 504
		static int DLayerPrun[504];		


		// 初始化静态表
		static void init();

		void setCube(const CubieCube& cc) { mySrcCube = cc; };
		std::vector<Move> solve(const CubieCube& cc);

		std::vector<Move> solveDCross();
		std::vector<Move> solveDLayer();


	//protected:

		// 底面2个棱块(DR(4), DF(5)的排列和方向
		static int toDCrossIdx(const CubieCube& cc);
		static CubieCube fromDCrossIdx(int idx);


		// 底面2个角块(DFR(4), DLF(5)的排列和方向
		static int toDLayerIdx(const CubieCube& cc);
		static CubieCube fromDLayerIdx(int idx);


		
		// 两个相连的棱块(edgeNo, edgeNo+1) 的排列和方向
		static int toTwoEdgeIdx(int edgeNo, const CubieCube& cc);
		static CubieCube fromTwoEdgeIdx(int edgeNo, int idx);

		// 两个相连的棱块(cornerNo, cornerNo+1) 的排列和方向
		static int toTwoCornerIdx(int cornerNo, const CubieCube& cc);
		static CubieCube fromTwoCornerIdx(int cornerNo, int idx);



		int getDCrossNumTry() const { return myDCrossNumTry; };
		int getDLayerNumTry() const { return myDLayerNumTry; };

	protected:
		static void init_moveConjY2();

		static void init_MoveTable(int nSize, int* moveTable,
			int(*fcnToIdx)(const CubieCube&),
			CubieCube(*fcnFromIdx)(int));

		static void init_PrunTable(int nSize, int* prunTable, 
			int(*fcnToIdx)(const CubieCube&) ,
			CubieCube(*fcnFromIdx)(int));


		struct PrunInfo
		{
			int (*fcnToIdx) (const CubieCube&);
			CubieCube (*fcnFromIdx) (int);
			int* moveTable;
			int* prunTable;
			bool is_conj;
		};


		bool _search(const CubieCube & cc, const std::vector<PrunInfo>& prunInfos, int max_depth, std::vector<Move>& moves, int& num_try);


	protected:
		static bool inited;

		bool myIsSolved = false;
		CubieCube mySrcCube;

		std::vector<Move> myDCrossMoves;
		std::vector<Move> myDLayerMoves;

		CubieCube myDCrossCube;
		CubieCube myDLayerCube;

		int myDCrossNumTry;
		int myDLayerNumTry;



	};

	class HumanSolverCFOP
	{
	public:
		// F2L 公式表，第一个元素是角块DFR(4)的位置和方向, 第二个元素是FR(8)的位置和方向, 第三个是公式
		// 角块表示 perm * 3 + ori, 棱块表示 perm * 2 + ori
		static std::vector<std::tuple<int, int, std::vector<Move>>> F2L_table;

		// OLL 公式表，第一个元素是角块方向idx, 第二个元素是棱块方向idx, 第三个是公式
		// 角块表示 a0 + a1*3 + a2*9 + a3*27, 棱块表示 a0 + a1*2 + a2*4 + a3*8
		static std::vector<std::tuple<int, int, std::vector<Move>>> OLL_table;

		// OLL 公式表，第一个元素是角块排列idx, 第二个元素是棱块排列idx, 第三个是公式
		static std::vector<std::tuple<int, int, std::vector<Move>>> PLL_table;

		// 初始化静态表，调用 solve 会自动调用
		static void init();

		// 求解魔方，从D颜色开始
		std::vector<Move> solve(const CubieCube& cc);

		// 求解魔方，开始的颜色有AxisCube 决定
		std::vector<Move> solve(const AxisCubieCube& cc);
		std::vector<Move> solve(const AxisCube& axis, const CubieCube& cc);


		// 仅仅求解到 Cross
		std::vector<Move> solveCross(const CubieCube& cc);
		std::vector<Move> solveCross(const AxisCubieCube& cc);
		std::vector<Move> solveCross(const AxisCube& axis, const CubieCube& cc);

		// 仅仅求解到 F2L
		std::vector<Move> solveF2L(const CubieCube& cc);
		std::vector<Move> solveF2L(const AxisCubieCube& cc);
		std::vector<Move> solveF2L(const AxisCube& axis, const CubieCube& cc);

		// 仅仅求解到 OLL
		std::vector<Move> solveOLL(const CubieCube& cc);
		std::vector<Move> solveOLL(const AxisCubieCube& cc);
		std::vector<Move> solveOLL(const AxisCube& axis, const CubieCube& cc);

	protected:
		static std::vector<Move> _solveF2L_internal(const AxisCubieCube& cc);

		std::vector<Move> _solveCross();
		std::vector<Move> _solveF2L();
		std::vector<Move> _solveOLL();
		std::vector<Move> _solvePLL();


		// 求解， progress 取值 1,2,3,4 分别只求解到代表 Cross, F2L, OLL, PLL
		std::vector<Move> _solve(const AxisCubieCube & cc, int progress);


	protected:
		static bool inited;
		int myError;
		bool myIsSolved = false;
		AxisCubieCube mySrcCube;

		std::vector<Move> myCrossMoves;
		std::vector<Move> myF2LMoves;
		std::vector<Move> myOLLMoves;
		std::vector<Move> myPLLMoves;

		std::vector<Move> myMoves;

		AxisCubieCube myCrossCube;
		AxisCubieCube myF2LCube;
		AxisCubieCube myOLLCube;
		AxisCubieCube myPLLCube;
	};
}