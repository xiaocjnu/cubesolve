#pragma once

#include <array>
#include <exception>
#include <assert.h>
#include <vector>
#include <array>
#include <string>
#include<limits>

#define GET_ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define GET_MATRIX_NROWS(x) (sizeof(x) / (sizeof(x[0]))
#define GET_MATRIX_NCOLS(x) (sizeof(x[0]) / sizeof(x[0][0])

namespace cs
{

	using int64 = long long;

	class CubieCube;
	//////////////////////////////////////////////////////////////////////////
	// 常量


	/*             face
	*  * The names of the facelet positions of the cube
	*  顺序： URFDLB
	*             |----------|
	*             | 0  1  2  |
	*             | 3  4  5  |
	*             | 6  7  8  |
	*  | ---------|----------|----------|----------|
	*  | 36 37 38 | 18 19 20 |  9 10 11 | 45 46 47 |
	*  | 39 40 41 | 21 22 23 | 12 13 14 | 48 49 50 |
	*  | 42 43 44 | 24 25 26 | 15 16 17 | 51 52 53 |
	*  | ---------|----------|----------|----------|
	*             | 27 28 29 |
	*             | 30 31 32 |
	*             | 33 34 35 |
	*             |----------|
	*
	*
	*
	*             |************|
	*             |*U1**U2**U3*|
	*             |************|
	*             |*U4**U5**U6*|
	*             |************|
	*             |*U7**U8**U9*|
	*             |************|
	* ************|************|************|************|
	* *L1**L2**L3*|*F1**F2**F3*|*R1**R2**F3*|*B1**B2**B3*|
	* ************|************|************|************|
	* *L4**L5**L6*|*F4**F5**F6*|*R4**R5**R6*|*B4**B5**B6*|
	* ************|************|************|************|
	* *L7**L8**L9*|*F7**F8**F9*|*R7**R8**R9*|*B7**B8**B9*|
	* ************|************|************|************|
	*             |************|
	*             |*D1**D2**D3*|
	*             |************|
	*             |*D4**D5**D6*|
	*             |************|
	*             |*D7**D8**D9*|
	*             |************|
	*/


	//Edges 
	//          0   1   2   3   4   5   6   7   8   9   10  11
	enum Edge { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };

	//Corners
	//           0    1    2    3    4    5    6    7
	enum Corner{ URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };


	//Moves
	//          0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17
	enum Move { Ux1, Ux2, Ux3, Rx1, Rx2, Rx3, Fx1, Fx2, Fx3, Dx1, Dx2, Dx3, Lx1, Lx2, Lx3, Bx1, Bx2, Bx3,
	//			18      19      20      21      22      23      24      25      26
				Rot_x1, Rot_x2, Rot_x3, Rot_y1, Rot_y2, Rot_y3, Rot_z1, Rot_z2, Rot_z3,

	//          27   28   29   30   31   32   33   34   35
				Ex1, Ex2, Ex3, Mx1, Mx2, Mx3, Sx1, Sx2, Sx3,
			
	//			36        37        38        39        40        41        42        43        44
				LowerUx1, LowerUx2, LowerUx3, LowerRx1, LowerRx2, LowerRx3, LowerFx1, LowerFx2, LowerFx3,
	//          45        46        47        48        49        50        51        52        53
				LowerDx1, LowerDx2, LowerDx3, LowerLx1, LowerLx2, LowerLx3, LowerBx1, LowerBx2, LowerBx3,

				
	//			逆时针转180度，第二个字节是1， 第一个字节一样，只有 Ux2n 等是合法的。
				Ux2n = 256 + 1, Rx2n = 256 + 4, Fx2n = 256 + 7, Dx2n = 256 + 10, Lx2n = 256 + 13, Bx2n = 256 + 16,

				Rot_x2n = 256 + 19, Rot_y2n = 256 + 22, Rot_z2n = 256 + 25,

				Ex2n = 256 +28, Mx2n = 256 + 31, Sx2n = 256 + 34,

				LowerUx2n = 256 + 37, LowerRx2n = 256 + 40, LowerFx2n = 256 + 43, 
		
				LowerDx2n = 256 + 46, LowerLx2n = 256 + 49, LowerBx2n = 256 + 52,

				InvalidMove = -1

				};

	// 逆时针转180度
	bool isMove2n(Move m);

	// 去除掉逆时针旋转 180度
	Move getPureMove(Move m);

	// 名字
	const std::string move2str[54] = { "U", "U2", "U\'","R", "R2", "R\'", "F", "F2", "F\'",
		"D", "D2", "D\'","L", "L2", "L\'", "B", "B2", "B\'", "x", "x2", "x\'", "y", "y2", "y\'","z", "z2", "z\'",
		"E", "E2", "E\'", "M", "M2", "M\'", "S", "S2", "S\'", 
		"u", "u2", "u\'","r", "r2", "r\'", "f", "f2", "f\'",
		"d", "d2", "d\'","l", "l2", "l\'", "b", "b2", "b\'" };

	// 程序员格式名字
	const std::string move2strP[54] = {
		"Ux1", "Ux2", "Ux3", "Rx1", "Rx2", "Rx3", "Fx1", "Fx2", "Fx3", "Dx1", "Dx2", "Dx3", "Lx1", "Lx2", "Lx3", "Bx1", "Bx2", "Bx3",
		"Rot_x1", "Rot_x2", "Rot_x3", "Rot_y1", "Rot_y2", "Rot_y3", "Rot_z1", "Rot_z2", "Rot_z3",
		"Ex1", "Ex2", "Ex3", "Mx1", "Mx2", "Mx3", "Sx1", "Sx2", "Sx3",
		"LowerUx1", "LowerUx2", "LowerUx3", "LowerRx1", "LowerRx2", "LowerRx3", "LowerFx1", "LowerFx2", "LowerFx3",
		"LowerDx1", "LowerDx2", "LowerDx3", "LowerLx1", "LowerLx2", "LowerLx3", "LowerBx1", "LowerBx2", "LowerBx3"
	};



	// 转中间层的分解表
	const int move_middle_table[9][3] =
	{
		// move1, move2, rot, 右手规则，四指弯曲，拇指伸直，四指旋转方向是中间层旋转方向，拇指方向是 move1
		Ux1, Dx3, Rot_y3, // 0 Ex1
		Ux2, Dx2, Rot_y2, // 1 Ex2
		Ux3, Dx1, Rot_y1, // 2 Ex3
		Rx1, Lx3, Rot_x3, // 3 Mx1
		Rx2, Lx2, Rot_x2, // 4 Mx2
		Rx3, Lx1, Rot_x1, // 5 Mx3
		Fx3, Bx1, Rot_z1, // 6 Sx1
		Fx2, Bx2, Rot_z2, // 7 Sx2
		Fx1, Bx3, Rot_z3, // 8 Sx3
	};


	// 转2层的分解表
	const int move_lower_table[18][2] =
	{
		// move, rot
		Dx1, Rot_y1, // 0 LowerUx1
		Dx2, Rot_y2, // 1 LowerUx2
		Dx3, Rot_y3, // 2 LowerUx3
		Lx1, Rot_x1, // 3 LowerRx1
		Lx2, Rot_x2, // 4 LowerRx2
		Lx3, Rot_x3, // 5 LowerRx3
		Bx1, Rot_z1, // 6 LowerFx1
		Bx2, Rot_z2, // 7 LowerFx2
		Bx3, Rot_z3, // 8 LowerFx3
		Ux1, Rot_y3, // 9 LowerDx1
		Ux2, Rot_y2, // 10 LowerDx2
		Ux3, Rot_y1, // 11 LowerDx3
		Rx1, Rot_x3, // 12 LowerLx1
		Rx2, Rot_x2, // 13 LowerLx2
		Rx3, Rot_x1, // 14 LowerLx3
		Fx1, Rot_z3, // 15 LowerBx1
		Fx2, Rot_z2, // 16 LowerBx2
		Fx3, Rot_z1, // 17 LowerBx3
	};


	// 公式分解为基本公式和转体，转体在后面, 仅支持  0 - 53, 否则返回空
	std::vector<Move> move_decompose(Move m);

	// 公式分解为基本公式和转体，转体在后面, 仅支持  0 - 53, 否则返回 (空,-1)
	// 返回值中第一个是基本公式，第二个是转体
	std::array<std::vector<Move>, 2> move_decompose2(Move m);


	//Facelets
	enum Facelet
	{
		U1, U2, U3, U4, U5, U6, U7, U8, U9,		// 0 -- 8
		R1, R2, R3, R4, R5, R6, R7, R8, R9,		// 9 -- 17
		F1, F2, F3, F4, F5, F6, F7, F8, F9,		// 18 -- 26
		D1, D2, D3, D4, D5, D6, D7, D8, D9,		// 27 -- 35
		L1, L2, L3, L4, L5, L6, L7, L8, L9,		// 36 -- 44
		B1, B2, B3, B4, B5, B6, B7, B8, B9		// 45 -- 53
	};

	//Colors
	//           0  1  2  3  4  5
	enum Color { U, R, F, D, L, B };

	//////////////////////////////////////////////////////////////////////////
	const char colorNames[6] = { 'U', 'R', 'F', 'D', 'L', 'B' };
	const char colorNames2[6] = { 'W', 'R', 'G', 'Y', 'O', 'B' };

	const Facelet cornerFacelet[8][3] =
	{
		{ U9, R1, F3 },{ U7, F1, L3 },{ U1, L1, B3 },{ U3, B1, R3 },
	{ D3, F9, R7 },{ D1, L9, F7 },{ D7, B9, L7 },{ D9, R9, B7 }
	};

	const Facelet edgeFacelet[12][2] =
	{
		{ U6, R2 },{ U8, F2 },{ U4, L2 },{ U2, B2 },{ D6, R8 },{ D2, F8 },
	{ D4, L8 },{ D8, B8 },{ F6, R4 },{ F4, L6 },{ B6, L4 },{ B4, R6 }
	};


	const Color cornerColor[8][3] = { { U, R, F },{ U, F, L },{ U, L, B },{ U, B, R },{ D, F, R },{ D, L, F },
	{ D, B, L },{ D, R, B } };

	const Color edgeColor[12][2] = { { U, R },{ U, F },{ U, L },{ U, B },{ D, R },{ D, F },{ D, L },{ D, B },
	{ F, R },{ F, L },{ B, L },{ B, R } };



	const int face_move_table[15][54]
	{
	{ 6, 3, 0, 7, 4, 1, 8, 5, 2, 45, 46, 47, 12, 13, 14, 15, 16, 17, 9, 10, 11, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 18, 19, 20, 39, 40, 41, 42, 43, 44, 36, 37, 38, 48, 49, 50, 51, 52, 53, },	// U
	{ 0, 1, 20, 3, 4, 23, 6, 7, 26, 15, 12, 9, 16, 13, 10, 17, 14, 11, 18, 19, 29, 21, 22, 32, 24, 25, 35, 27, 28, 51, 30, 31, 48, 33, 34, 45, 36, 37, 38, 39, 40, 41, 42, 43, 44, 8, 46, 47, 5, 49, 50, 2, 52, 53, },	// R
	{ 0, 1, 2, 3, 4, 5, 44, 41, 38, 6, 10, 11, 7, 13, 14, 8, 16, 17, 24, 21, 18, 25, 22, 19, 26, 23, 20, 15, 12, 9, 30, 31, 32, 33, 34, 35, 36, 37, 27, 39, 40, 28, 42, 43, 29, 45, 46, 47, 48, 49, 50, 51, 52, 53, },	// F
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 24, 25, 26, 18, 19, 20, 21, 22, 23, 42, 43, 44, 33, 30, 27, 34, 31, 28, 35, 32, 29, 36, 37, 38, 39, 40, 41, 51, 52, 53, 45, 46, 47, 48, 49, 50, 15, 16, 17, },	// D
	{ 53, 1, 2, 50, 4, 5, 47, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 19, 20, 3, 22, 23, 6, 25, 26, 18, 28, 29, 21, 31, 32, 24, 34, 35, 42, 39, 36, 43, 40, 37, 44, 41, 38, 45, 46, 33, 48, 49, 30, 51, 52, 27, },	// L
	{ 11, 14, 17, 3, 4, 5, 6, 7, 8, 9, 10, 35, 12, 13, 34, 15, 16, 33, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 36, 39, 42, 2, 37, 38, 1, 40, 41, 0, 43, 44, 51, 48, 45, 52, 49, 46, 53, 50, 47, },	// B

	{ 18, 19, 20, 21, 22, 23, 24, 25, 26, 15, 12, 9, 16, 13, 10, 17, 14, 11, 27, 28, 29, 30, 31, 32, 33, 34, 35, 53, 52, 51, 50, 49, 48, 47, 46, 45, 38, 41, 44, 37, 40, 43, 36, 39, 42, 8, 7, 6, 5, 4, 3, 2, 1, 0, },	// x
	{ 6, 3, 0, 7, 4, 1, 8, 5, 2, 45, 46, 47, 48, 49, 50, 51, 52, 53, 9, 10, 11, 12, 13, 14, 15, 16, 17, 29, 32, 35, 28, 31, 34, 27, 30, 33, 18, 19, 20, 21, 22, 23, 24, 25, 26, 36, 37, 38, 39, 40, 41, 42, 43, 44, },	// y
	{ 42, 39, 36, 43, 40, 37, 44, 41, 38, 6, 3, 0, 7, 4, 1, 8, 5, 2, 24, 21, 18, 25, 22, 19, 26, 23, 20, 15, 12, 9, 16, 13, 10, 17, 14, 11, 33, 30, 27, 34, 31, 28, 35, 32, 29, 47, 50, 53, 46, 49, 52, 45, 48, 51, },	// z

	{ 6, 3, 0, 7, 4, 1, 8, 5, 2, 45, 46, 47, 48, 49, 50, 15, 16, 17, 9, 10, 11, 12, 13, 14, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 18, 19, 20, 21, 22, 23, 42, 43, 44, 36, 37, 38, 39, 40, 41, 51, 52, 53, },	// Uw
	{ 0, 19, 20, 3, 22, 23, 6, 25, 26, 15, 12, 9, 16, 13, 10, 17, 14, 11, 18, 28, 29, 21, 31, 32, 24, 34, 35, 27, 52, 51, 30, 49, 48, 33, 46, 45, 36, 37, 38, 39, 40, 41, 42, 43, 44, 8, 7, 47, 5, 4, 50, 2, 1, 53, },	// Rw
	{ 0, 1, 2, 43, 40, 37, 44, 41, 38, 6, 3, 11, 7, 4, 14, 8, 5, 17, 24, 21, 18, 25, 22, 19, 26, 23, 20, 15, 12, 9, 16, 13, 10, 33, 34, 35, 36, 30, 27, 39, 31, 28, 42, 32, 29, 45, 46, 47, 48, 49, 50, 51, 52, 53, },	// Fw
	{ 8, 7, 6, 5, 4, 3, 2, 1, 0, 36, 37, 38, 21, 22, 23, 24, 25, 26, 45, 46, 47, 39, 40, 41, 42, 43, 44, 33, 30, 27, 34, 31, 28, 35, 32, 29, 9, 10, 11, 48, 49, 50, 51, 52, 53, 18, 19, 20, 12, 13, 14, 15, 16, 17, },	// Dw
	{ 53, 52, 29, 50, 49, 32, 47, 46, 35, 17, 16, 15, 14, 13, 12, 11, 10, 9, 0, 1, 51, 3, 4, 48, 6, 7, 45, 18, 19, 2, 21, 22, 5, 24, 25, 8, 42, 39, 36, 43, 40, 37, 44, 41, 38, 26, 34, 33, 23, 31, 30, 20, 28, 27, },	// Lw
	{ 11, 14, 17, 10, 13, 16, 29, 28, 27, 44, 32, 35, 41, 31, 34, 38, 30, 33, 26, 25, 24, 23, 22, 21, 20, 19, 18, 8, 7, 6, 37, 40, 43, 36, 39, 42, 2, 5, 15, 1, 4, 12, 0, 3, 9, 51, 48, 45, 52, 49, 46, 53, 50, 47, }	// Bw
	};


	//////////////////////////////////////////////////////////////////////////
	// 函数

	// n choose k , C(n,k)
	int nchoosek(int n, int k);


	// 计算 perm<n>(idx) 的奇偶性
	int getNParity(int idx, int n);

	// [a, b] 之间的随机数
	int randInt(int a, int b);

	double random(double a, double b);


	std::vector<int> MoveVector2IntVector(const std::vector<Move>& mm);
	std::vector<Move> IntVector2MoveVector(const std::vector<int>& vv);

	
	//////////////////////////////////////////////////////////////////////////
	// 全排列 A(n,n)
	template <int N>
	class permutation : public std::array<int, N>
	{
	public:
		typedef std::array<int, N> _base;

		static permutation eye()
		{
			permutation e;
			return e;
		}

		bool iseye() const
		{
			return *this == eye();
		}

		permutation()
		{
			for (int i = 0; i < N; ++i)
			{
				this->at(i) = i;
			}
		}

		permutation(int _p[N])
		{
			for (int i = 0; i < N; ++i)
			{
				this->at(i) = _p[i];
			}
		}


		permutation(const _base& src)
			: _base(src)
		{}


		permutation(const std::initializer_list<int> &t)
		{
			int i = 0;

			for (int val : t)
			{
				this->at(i++) = val;
			}

			assert(i == N);
		}


		permutation(int idx)
		{
			setIndex(idx);
		}

		// c = a * b = a(b)
		permutation<N> mul(const permutation& b) const
		{
			permutation<N> c;

			for (int i = 0; i < N; ++i)
			{
				c[i] = this->at(b[i]);
			}

			return c;
		}

		permutation<N> operator * (const permutation& b) const
		{
			return mul(b);
		}

		// b = inv(b), b(a) = e
		permutation<N> inv() const
		{
			permutation<N> b;

			for (int i = 0; i < N; ++i)
			{
				b[this->at(i)] = i;
			}

			return b;
		}


		bool operator ==(const permutation& src)
		{
			bool ret = true;

			for (int i = 0; i < N; ++i)
			{
				ret &= this->at(i) == src[i];
			}

			return ret;
		}


		// 验证是否合法，值必须是 0 to N-1, 且唯一
		bool verify() const
		{
			if (this->at(0) < 0 || this->at(0) >= N)
				return false;


			for (int i = 1; i < N; ++i)
			{
				int a = this->at(i);

				if (a < 0 || a >= N)
					return false;

				for (int j = 0; j < i; ++j)
				{
					int b = this->at(j);

					if (a == b)
						return false;
				}
			}

			return true;
		}

		// 计算排列对应的下标数
		// 取值范围是 0 to (N!-1)

		int getIndex() const
		{
			// 初始为 0,1,...,N-1
			std::array<int, N> seq;
			for (int i = 0; i < N; ++i)
				seq[i] = i;



			int idx = 0;
			for (int i = 0; i < N-1; ++i)
			{
				int s = seq[this->at(i)];   // 序号
				idx = (N - i) * idx + s;

				// 更新seq, j > at(i) 的 seq[j] 都减1
				for (int j = this->at(i)+1; j < N; ++j)
				{
					seq[j] -= 1;
				}
			}


			return idx;
		}

		// 另一种方式的下标，从高位算起
		int getIndex2() const
		{
			// 初始为 N-1,...,0
			std::array<int, N> seq;
			for (int i = 0; i < N; ++i)
				seq[i] = N-1-i;



			int idx = 0;
			for (int i = N-1; i >= 1; --i)
			{
				int s = seq[this->at(i)];   // 序号
				idx = (i+1) * idx + s;

				// 更新seq, j < at(i) 的 seq[j] 都减1
				for (int j = this->at(i) - 1; j >= 0; --j)
				{
					seq[j] -= 1;
				}
			}


			return idx;
		}

		// 通过序号来设置排列
		void setIndex(int idx)
		{
			// 序号
			std::array<int, N - 1> sn;
			for (int i = 2; i <= N; ++i)
			{
				int a = idx / i;
				int b = idx % i;
				sn[N - i] = b;
				idx = a;
			}

			// 初始为 0,1,...,N-1
			std::array<int, N> seq;
			for (int i = 0; i < N; ++i)
				seq[i] = i;

			// 更新排列
			for (int i = 0; i < N-1; ++i)
			{
				this->at(i) = seq[sn[i]];

				// seq sn[i] 右边的项向左移位
				for (int j = sn[i]; j < N - 1 - i; ++j)
				{
					seq[j] = seq[j + 1];
				}
			}

			this->at(N - 1) = seq[0];

		}


		void setIndex2(int idx)
		{
			// 序号
			std::array<int, N-1> sn;
			for (int i = 2; i <= N; ++i)
			{
				int a = idx / i;
				int b = idx % i;
				sn[N-i] = b;
				idx = a;
			}

			// 初始为 N-1,...,0
			std::array<int, N> seq;
			for (int i = 0; i < N; ++i)
				seq[i] = N-1-i;

			// 更新排列
			for (int i = N-1; i >= 1; --i)
			{
				this->at(i) = seq[sn[N-1-i]];

				// seq sn[N-1-i] 右边的项向左移位
				for (int j = sn[N-1-i]; j < i; ++j)
				{
					seq[j] = seq[j + 1];
				}
			}

			this->at(0) = seq[0];

		}
	};

	//////////////////////////////////////////////////////////////////////////
	// 排列 A(n,k) 1<=k<n
	// index 计算与permutation 类似
	template <int N, int K>
	class permutation_Ank : public std::array<int, K>
	{
	private:
		typedef std::array<int, K> _base;

	public:

		permutation_Ank()
		{
			for (int i = 0; i < K; ++i)
			{
				this->at(i) = i;
			}
		}

		permutation_Ank(int _p[K])
		{
			for (int i = 0; i < K; ++i)
			{
				this->at(i) = _p[i];
			}
		}


		permutation_Ank(const _base& src)
			: _base(src)
		{}

		permutation_Ank(const std::initializer_list<int> &t)
		{
			int i = 0;

			for (int val : t)
			{
				this->at(i++) = val;
			}

			assert(i == K);
		}


		permutation_Ank(int idx)
		{
			setIndex(idx);
		}


		bool operator ==(const permutation_Ank& src)
		{
			bool ret = true;

			for (int i = 0; i < K; ++i)
			{
				ret &= this->at(i) == src[i];
			}

			return ret;
		}


		// 验证是否合法，值必须是 0 to N-1, 且唯一
		bool verify() const
		{
			if (this->at(0) < 0 || this->at(0) >= N)
				return false;


			for (int i = 1; i < K; ++i)
			{
				int a = this->at(i);

				if (a < 0 || a >= N)
					return false;

				for (int j = 0; j < i; ++j)
				{
					int b = this->at(j);

					if (a == b)
						return false;
				}
			}

			return true;
		}

		// 计算排列对应的下标数
		// 取值范围是 0 to (A(N,K)-1)

		int getIndex() const
		{
			// 初始为 0,1,...,N-1
			std::array<int, N> seq;
			for (int i = 0; i < N; ++i)
				seq[i] = i;

			int idx = 0;
			for (int i = 0; i < K; ++i)
			{
				int s = seq[this->at(i)];   // 序号
				idx = (N - i) * idx + s;

				// 更新seq, j > at(i) 的 seq[j] 都减1
				for (int j = this->at(i) + 1; j < N; ++j)
				{
					seq[j] -= 1;
				}
			}


			return idx;
		}

		// 通过序号来设置排列
		void setIndex(int idx)
		{
			// 序号
			std::array<int, K> sn;
			for (int i = N - K + 1; i <= N; ++i)
			{
				int a = idx / i;
				int b = idx % i;
				sn[N-i] = b;
				idx = a;
			}

			// 初始为 0,1,...,N-1
			std::array<int, N> seq;
			for (int i = 0; i < N; ++i)
				seq[i] = i;

			// 更新排列
			for (int i = 0; i < K; ++i)
			{
				this->at(i) = seq[sn[i]];

				// seq sn[i] 右边的项向左移位
				for (int j = sn[i]; j < N - 1 - i; ++j)
				{
					seq[j] = seq[j + 1];
				}
			}

		}
	};



	//////////////////////////////////////////////////////////////////////////
	// 组合, C(n,k), 1 <= k < n
	template<int N, int K>
	class combination : public std::array<bool, N>
	{
	public:
		using _base = std::array<bool, N>;
		using my_t = combination;
		
		combination()
		{
			for (int i = 0; i < N-K; ++i)
			{
				this->at(i) = false;
			}

			for (int i = N-K; i < N; ++i)
			{
				this->at(i) = true;
			}
		}

		combination(const std::initializer_list<bool>& t)
		{
			int i = 0;
			for (auto a : t)
			{
				this->at(i++) = a;
			}

			assert(i == N);
		}

		combination(int idx)
		{
			setIndex(idx);
		}


		combination(const std::array<int, K>& number)
		{
			for (int i = 0; i < N; ++i)
			{
				this->at(i) = false;
			}

			for (int i = 0; i < K; ++i)
			{
				this->at(number[i]) = true;
			}
		}

		my_t zeroComb() const
		{
			combination c;
			return c;
		}


		bool operator==(const my_t& src)
		{
			bool ret = true;
			for (int i = 0; i < N; ++i)
			{
				ret &= (this->at(i) == src[i]);
			}

			return ret;
		}

		// 验证， true 的个数必须是 K
		bool verify() const
		{
			int count = 0;
			for (int i = 0; i < N; ++i)
			{
				count += this->at(i) ? 1 : 0;
			}

			return count == K;
		}

		// 获取下标, 以 C(12,4) 为例
		// a =  0 0,1,0,0,1,0,1,0,0,1,0)
		// 		i	0	1	2	3	4	5	6	7	8	9	10	11
		// 		a	0	0	1	0	0	1	0	1	0	0	1	0
		// 		k	-	-	-	0	0	-	1	-	2	2	-	3
		// idx = C(11,3) + C(9,2) + C(8,2) + C(6,1) + C(4,0) + C(3,0) = 237
		int getIndex() const
		{
			int k = K - 1;

			int idx = 0;
			for (int n = N - 1; n >= 0 && k >=0; --n)
			{
				if (this->at(n))
					--k;
				else
					idx += nchoosek(n, k);

			}

			return idx;
		}

		// 由下标设定
		void setIndex(int idx)
		{
			int k = K - 1;

			int n;
			for (n = N - 1; n >= 0 && k >= 0; --n)
			{
				int rem = idx - nchoosek(n, k);

				if (rem >= 0)  // => a[i] = false
				{
					this->at(n) = false;
					idx = rem;
				}
				else // rem < 0  => a[i] = true;
				{
					this->at(n) = true;
					--k;
				}
			}

			for (; n>=0; --n)
			{
				this->at(n) = false;
			}

		}


		// 获取被选择(即 true)对应的下标数字
		std::array<int, K> toNumber() const
		{
			std::array<int, K> ret;

			int n = 0;
			for (int i = 0; i < N; ++i)
			{
				if (this->at(i))
					ret[n++] = i;
			}

			assert(n == K);

			return ret;
		}

	private:
		std::array<bool, N> my_choose;
	};


	//////////////////////////////////////////////////////////////////////////
	// 取值只能是 0, 1
	class eo_t
	{
	public:
		static const int nbase = 2;


		eo_t();
		eo_t(int v);
		eo_t(const eo_t& src);

		int get() const { return val; };
		void set(int v);


		// c = a + b
		// a\b| 0 | 1
		// ---+---+----
		//  0 | 0   1
		//  1 | 1   0
		eo_t operator+(const eo_t& b) const;
		bool operator==(const eo_t& b) const;


		// 满足 a + a.neg = 0 <=> a.neg = a
		eo_t neg() const;

	private:
		char val;

	};

	//////////////////////////////////////////////////////////////////////////
	// 角度方向， 0, 1, 2 分别表示 U/D 颜色的facelet 相对 U/D 方向顺时针顺转 0, 120, 240 deg.
	// 角度 3, 4, 5 表示镜像，表示先旋转 0, 120, 240 deg, 然后再相对 LR-slice 反射镜像。
	class co_t
	{
	public:
		static const int nbase = 6;


		co_t();
		co_t(int v);
		co_t(const co_t& src);


		int get() const { return val; };
		void set(int v);

		// c = a + b
		// a\b| 0 | 1 | 2 | 3 | 4 | 5
		// ---+---+---+---+---+---+---
		//  0 | 0   1   2 | 3   4   5
		//  1 | 1   2   0 | 4   5   3
		//  2 | 2   0   1 | 5   3   4
		// ---+-----------+-----------
		//  3 | 3   5   4 | 0   2   1
		//  4 | 4   3   5 | 1   0   2
		//  5 | 5   4   3 | 2   1   0
		co_t operator+(const co_t& b) const;
		bool operator==(const co_t& b) const;


		// 满足 a + a.neg() = 0
		// a\b| 0 | 1 | 2 | 3 | 4 | 5
		// ---+---+---+---+---+---+---
		//  0 | 0   2   1 | 3   4   5
		co_t neg() const;

	private:
		char val;
	};


	//////////////////////////////////////////////////////////////////////////
	class edge_orientation : public std::array<eo_t, 12>
	{
	public:
		using _base = std::array<eo_t, 12>;


		edge_orientation();

		edge_orientation(const std::initializer_list<int>& t);

		edge_orientation(int idx) { setIndex(idx); };

		static edge_orientation zeros() { return edge_orientation(); };



		// 求 index, 取值 [0, 2048), 2048 = 2^11, 因为需保持奇偶性
		// index = a0 + a1 * 2^1 + a2 * 2^2 + ... + a10 * 2^10
		int getIndex() const;

		// 根据 index 设置
		void setIndex(int idx);

	};


	//////////////////////////////////////////////////////////////////////////
	class corner_orientation : public std::array<co_t, 8>
	{
	public:
		using _base = std::array<co_t, 8>;


		corner_orientation();

		corner_orientation(const std::initializer_list<int>& t);


		corner_orientation(int idx) { setIndex(idx); }

		static corner_orientation zeros() { return corner_orientation(); };


		// 求 index, 不考虑 reflection 的情况，即 co 取值只能是 0,1,2
		// 取值 [0, 2187 ), 2187 = 3^7, 因为保持奇偶性
		// 如 orientation<base3,8> a = (1,2,0,1,2,0,1,0)
		// index = a0 + a1 * 3^1 + a2 * 3^2 + ... + a6 * 3^6
		int getIndex() const;

		// 根据 index 设置
		void setIndex(int idx);

	};

	//////////////////////////////////////////////////////////////////////////
	template < typename orientation_t, int N>
	class permutation_orientation_t
	{
	public:
		using perm_t = permutation<N>;
		using my_t = permutation_orientation_t<orientation_t, N>;



		perm_t perm;
		orientation_t ori;

		permutation_orientation_t()
		{
		};

		permutation_orientation_t(perm_t _perm, orientation_t _ori)
			: perm(_perm)
			, ori(_ori)
		{}


		permutation_orientation_t(const std::initializer_list<std::initializer_list<int>>& src)
			: perm(*(src.begin()))
			, ori(*(src.begin() + 1))
		{
		}

		static my_t eye()
		{
			return my_t();
		}


		void set_perm(const permutation<N>& _perm)
		{
			perm = _perm;
		}

		void set_perm(int idx)
		{
			perm.setIndex(idx);
		}

		void set_ori(const orientation_t& _ori)
		{
			ori = _ori;
		}

		void set_ori(int idx)
		{
			ori.setIndex(idx);
		}


		// c = a * b
		// c.perm = a.perm * b.perm
		// c.ori = a.ori(b.perm) (+) b.ori
		my_t mul(const my_t& b) const
		{
			permutation_orientation_t<orientation_t, N> c;
			c.perm = perm * b.perm;

			for (int i = 0; i < N; ++i)
			{
				c.ori[i] = ori[(int)b.perm[i]] + b.ori[i];
			}

			return c;
		}


		my_t operator * (const my_t& b) const
		{
			return mul(b);
		}


		//b.perm = inv(a.perm), 即 b.perm(a.perm) = e
		// b.ori = 0 (-) a.ori(b.perm)

		my_t inv() const
		{
			my_t b;

			b.perm = perm.inv();

			for (int i = 0; i < N; ++i)
			{
				b.ori[i] = ori[b.perm[i]].neg();
			}

			return b;
		}


		bool operator == (const my_t& src) const
		{
			return perm == src.perm && ori == src.ori;
		}


		const my_t& operator=(const my_t& src)
		{
			this->perm = src.perm;
			this->ori = src.ori;
			return *this;
		}

	};


	using corner_t = permutation_orientation_t<corner_orientation, 8>;
	using edge_t = permutation_orientation_t<edge_orientation, 12>;

	//////////////////////////////////////////////////////////////////////////
	template <int N>
	class pruning_array : public std::array<char, (N+1)/2>
	{
	public:
		static inline int size()
		{
			return N;
		}


		static inline int nbytes()
		{
			return (N + 1) / 2;
		}

		void set(int index, char value)
		{
			int s = ((index & 1) << 2);
			char mask = 0xf << s;
			(*this)[index >> 1] = (~mask & (*this)[index >> 1]) | (mask & (value << s));
		}

		// Extract pruning value
		char get(int index) const
		{
			int s = (index & 1) << 2;
			return ((*this)[index >> 1] >> s) & 0x0f;;
		}
	};



	template<int M, int N>
	class pruning_table : public pruning_array<M*N>
	{
	public:


		static inline int size_m() { return M; };
		static inline int size_n() { return N; };
		static inline int numel() { return M * N; };


		void set(int i, int j, char value)
		{
			pruning_array<M*N>::set(i*N + j, value);
		}

		// Extract pruning value
		char get(int i, int j) const
		{
			return pruning_array<M*N>::get(i*N + j);
		}


		int calc_max() const
		{
			int _max = 0;
			for (int i = 0; i < M; ++i)
			{
				for (int j = 0; j < N; ++j)
				{
					int v = get(i, j);
					if (v > _max)
						_max = v;
				}
			}

			return _max;
		}


	};

}