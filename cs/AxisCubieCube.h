#pragma once
#include "util.h"
#include "AxisCube.h"
#include "CubieCube.h"

// 处理三阶魔方轴变换的类

namespace cs
{
	//////////////////////////////////////////////////////////////////////////
	// 包含 axisCubie 和 CubieCubie, 整体转动时，CubieCube 也转动
	class AxisCubieCube
	{
	public:
		AxisCube axis = AxisCube::eye();
		CubieCube cubie = CubieCube::eye();
		
		static int move_conj[54][24];	// 绝对公式 -> 相对公式, 需要初始化

		AxisCubieCube();
		AxisCubieCube(const AxisCube& ax, const CubieCube& cc);

		// 执行相对公式，公式相对物理空间，不相对axis的颜色,支持所有公式，x,y,z, E,M,S, 小写转两层
		AxisCubieCube move(Move m) const;
		AxisCubieCube move(int m) const;
		AxisCubieCube move(std::vector<Move> moves) const;

		// 执行绝对公式，相对于绝对的 axis颜色
		AxisCubieCube moveAbs(Move m) const;
		AxisCubieCube moveAbs(int m) const;
		AxisCubieCube moveAbs(std::vector<Move> moves) const;


		// 整体转动，至 axisCube 等于目标的设定(AxisCube 或者 下标)
		AxisCubieCube rotateToAxis(AxisCube ac) const;
		AxisCubieCube rotateToAxis(int ac) const;


		// 绝对公式转相对公式
		Move formulaA2R(Move m) const;
		// 相对公式转绝对公式
		Move formulaR2A(Move m) const;
		std::vector<Move> formulaR2A(std::vector<Move> m) const;
		std::vector<Move> formulaA2R(std::vector<Move> m) const;

		// 绝对公式转相对公式, 相对于 axis_cube
		static Move formulaA2R(int axis_cube, Move m);

		// 相对公式转绝对公式, 相对于 axis_cube
		static Move formulaR2A(int axis_cube, Move m);

		static AxisCubieCube eye();

		
		bool iseye() const;

		// 判断魔方是否复原，axis cube 可以任意合法方向
		bool isSolved() const;

		// c = a * b
		AxisCubieCube mul(const AxisCubieCube& b) const;
		AxisCubieCube operator*(const AxisCubieCube& b) const;

		AxisCubieCube inv() const;


		bool operator==(const AxisCubieCube& b) const;


		// 获取绝对的 CubieCube，（所谓绝对，就是相对于中心块）
		CubieCube getAbsCubie() const;
		void setAbsCubie(const CubieCube& cc);


		ColorCube toColorCube() const;

		static void init();

	protected:
		static bool inited;

		

	};
}