#pragma once
#include "util.h"
#include "FaceCubeBase.h"

namespace cs
{
	// sym, s占3位
	struct sym3_t
	{
		unsigned short val;

		sym3_t() {};
		sym3_t(short count, char s) { set(count, s); };

		void set(short count, char s) { val = count << 3 | s & 7; };
		void set_s(char s) { val = (val & 0xfff8) | (s & 7); };


		char get_s() const { return val & 7; };
		short get_count() const { return val >> 3; };
	};


	// sym, s占4位
	struct sym4_t
	{
		unsigned short val;


		sym4_t() {};
		sym4_t(short count, char s) { set(count, s); };

		void set(short count, char s) { val = count << 4 | s & 0xf; };
		void set_s(char s) { val = (val & 0xfff0) | (s & 0xf); };


		char get_s() const { return val & 0xf; };
		short get_count() const { return val >> 4; };
	};

	//////////////////////////////////////////////////////////////////////////
	class ColorCube;
	class CubieTables;


	class CubieCube
	{
	public:
		corner_t corner;
		edge_t edge;

		static const CubieCube move_table[9];  // URFDLBxyz
		static const int middle_table[9][3];	// 中间层公式表
		static const int lower_table[18][2];	// 转两层公式表

		static CubieTables tables;


	public:
		CubieCube();



		// 一个魔方的状态由四个数完全确定
		// cperm : corner permutation
		// twist : corner orientation
		// eperm : edge permutation
		// flip : edge orientation
		CubieCube(int cperm, int twist, int eperm, int flip);

		CubieCube(const std::initializer_list<std::initializer_list<int>>& lt);

		CubieCube(const ColorCube& fc);


		~CubieCube();


		static CubieCube eye() { return CubieCube(); };

		// 简单通过随机打乱产生随机魔方, m 打乱步数，不保证状态是随机分布，也不处理重复的打乱步骤
		static CubieCube randomScramble(int m = 30);


		// 真正随机的状态
		static CubieCube randomCube();

		bool iseye() const { return (*this) == eye(); };

		// c = a * b
		CubieCube mul(const CubieCube& b) const;

		CubieCube operator* (const CubieCube& b) const
		{
			return mul(b);
		}


		// b = inv(a)
		CubieCube inv() const;


		bool operator == (const CubieCube& b) const
		{
			return corner == b.corner && edge == b.edge;
		}

		bool equalsCorn(const CubieCube& b) const
		{
			return corner == b.corner;
		}

		bool equalsEdge(const CubieCube& b) const
		{
			return edge == b.edge;
		}


		const CubieCube& operator=(const CubieCube& src);

		// ********************************************* Get and set coordinates *********************************************
		// XSym : Symmetry Coordnate of X. MUST be called after initialization of ClassIndexToRepresentantArrays.

		// ++++++++++++++++++++ Phase 1 Coordnates ++++++++++++++++++++

		// Flip : Orientation of 12 Edges. Raw[0, 2048) Sym[0, 336 * 8)
		// Twist : Orientation of 8 Corners. Raw[0, 2187) Sym[0, 324 * 8)
		// UDSlice : Positions of the 4 UDSlice edges, the order is ignored. [0, 495)

		// flip means edge orientation
		int getFlip() const;
		void setFlip(int idx);
		sym3_t getFlipSym() const;
		void setFlipSym(sym3_t s);

		// twist : corner orientation
		int getTwist() const;
		void setTwist(int idx);
		sym3_t getTwistSym() const;
		void setTwistSym(sym3_t s);

		// UDSlice 中间层4个边块 (8,9,10,11) 所在位置组合的index. index 等于 0 表示中间层已经归位，但位置不一定准. 用于phase 1
		// 范围：(0, 495)
		int getUDSliceComb() const;

		// UDSlice 中间层4个边块的排列排列的index [0,24)
		int getUDSlicePerm() const;

		// 设置中间层，顺序全部按升序。
		void setUDSlice(int c, int p);


		// 顶层边块 (0,1,2,3) 所在位置组合， index = 0 表示顶层边块归位, 但顺序不一定。
		int getU4Comb();


		// 低层边块 (4,5,6,7) 所在位置组合， index = 0 表示底层边块归位, 但顺序不一定。
		int getD4Comb();

		// 顶层边块 (0,1,2,3) 单独提取出来的排列
		int getU4Perm();

		// 低层边块 (4,5,6,7) 单独提取出来的排列
		int getD4Perm();


		// ++++++++++++++++++++ Phase 2 Coordnates ++++++++++++++++++++
		// EPerm : Permutations of 8 UD Edges. Raw[0, 40320) Sym[0, 2187 * 16)
		// CPerm : Permutations of 8 Corners. Raw[0, 40320) Sym[0, 2187 * 16)
		// MPerm : Permutations of 4 UDSlice Edges. [0, 24)



		int getCPerm() const;
		void setCPerm(int idx);
		sym4_t getCPermSym() const;
		void setCPermSym(sym4_t s);

		int getEPerm() const;	// 获取 0 to 7 边块的permutation, 8-11 必须先归位，否则返回 -1
		void setEPerm(int idx);	// 设置 0 to 7 边块的permutation， 剩余边块不处理
		sym4_t getEPermSym() const;
		void setEPermSym(sym4_t s);

		// 中间层的排列, [0, 24)
		int getMPerm() const;
		void setMPerm(int idx);


		// 8 choose 4, U corner, [0, 70), 0 表示已归位（忽略顺序）
		int getCComb() const;
		void setCComb(int idx);


		//////////////////////////////////////////////////////////////////////////
		// other function

		std::string toString() const;
		std::string toString2() const;

		std::string idxtoString() const;

		FaceCube toFaceCube() const;
		ColorCube toColorCube() const;

		// 执行公式，支持所有公式，非法公式则忽略
		CubieCube move(Move mv) const;
		CubieCube move(int mv) const { return move(Move(mv));};

		CubieCube move(std::vector<Move> mv) const;


		/**
		* Check a cubiecube for solvability. Return the error code.
		* 0: Cube is solvable
		* -2: Not all 12 edges exist exactly once
		* -3: Flip error: One edge has to be flipped
		* -4: Not all corners exist exactly once
		* -5: Twist error: One corner has to be twisted
		* -6: Parity error: Two corners or two edges have to be exchanged
		*/
		int verify() const;

	protected:

		// 只支持基本公式和整体转动
		CubieCube _move(Move mv) const;
	};
}

//////////////////////////////////////////////////////////////////////////
#include "CubieTables.h"

