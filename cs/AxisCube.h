#pragma once
#include "util.h"
#include "CubieCube.h"
#include <vector>
#include<limits>

// 处理三阶魔方轴变换的类


namespace cs
{
	class AxisCube : public permutation<6>
	{
	private:
		typedef permutation<6> _base;

	public:

		static const int num_valid_table = 24;

		// 合法状态表
		static const AxisCube valid_table[24];

		// 合法状态表 的逆 valid_table[i].inv() = valid_table[inv_table[i]]
		static const int inv_table[24];

		// valid_table 生成表
		static const Move gen_table[24][2];

		// 整体旋转 move table, 用 valid_table 下标表示 
		static const int move_table[9];

		// 以下表需要调用 init() 初始化 

		// 生成valid_table 的公式(即gen_table) 所对应的 CubieCube （由同样公式生成）
		static CubieCube gen_cubie_table[24];

		// 基本move 的共轭move, move_conj_table[i,j] = axis[j] * move[i] * axis[j]^-1, 相对公式 -> 绝对公式
		static int move_conj_table_i[18][24];

		// 基本move 的共轭move, move_conj_table[i,j] = axis[j]^-1 * move[i] * axis[j], 绝对公式 -> 相对公式
		static int move_conj_table[18][24];





		AxisCube();
		AxisCube(const AxisCube& _right);
		AxisCube(const permutation<6>& _right);

		// 只所有公式，非法公式忽略
		AxisCube move(Move mv) const;
		AxisCube move(int mv) const { return move(Move(mv)); };

		AxisCube move(std::vector<Move> mv) const;

		bool isValid() const;

		//static void init();	 // 初始化静态表


		AxisCube inv() const { return _base::inv(); };
		AxisCube operator* (const AxisCube& b) const { return _base::operator*(b); };


		// 从 0 - 23 合法表中查找
		static AxisCube fromValidIdx(int idx);


		// 获取当前 axis 在合法表中的下标, 非法返回 -1
		int getValidIdx() const;


		// 获取当前AxisCube 的生成公式
		std::vector<Move> getGenFormula() const;

		// 获取当前下标是 idx 的 AxisCube 的生成公式
		static std::vector<Move> getGenFormula(int idx);

		static AxisCube eye() { return permutation<6>::eye(); };

		static void init();
	protected:
		// 只支持整体转动公式
		AxisCube _move(Move mv) const;

		static bool inited;
	};
}