#pragma once


#include <array>
#include <exception>
#include <assert.h>
#include <vector>

#include "util.h"
#include "CubieCube.h"

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


	//////////////////////////////////////////////////////////////////////////
	// CubieTables, 单例


	class CubieTables
	{
	public:

		//////////////////////////////////////////////////////////////////////////
		// move 的 URF.
		const int urfMove[6][18] =
		{
			{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 },	// m
			{ 6, 7, 8, 0, 1, 2, 3, 4, 5, 15, 16, 17, 9, 10, 11, 12, 13, 14 },	// m * urf^-1
			{ 3, 4, 5, 6, 7, 8, 0, 1, 2, 12, 13, 14, 15, 16, 17, 9, 10, 11 },	// m * urf^-2
			{ 2, 1, 0, 5, 4, 3, 8, 7, 6, 11, 10, 9, 14, 13, 12, 17, 16, 15 },	// m^-1
			{ 8, 7, 6, 2, 1, 0, 5, 4, 3, 17, 16, 15, 11, 10, 9, 14, 13, 12 },	// m^-1 * urf^-1
			{ 5, 4, 3, 8, 7, 6, 2, 1, 0, 14, 13, 12, 17, 16, 15, 11, 10, 9 }	// m^-1 * urf^-2
		};


		const int preMove[9] = { -1, Rx1, Rx3, Fx1, Fx3, Lx1, Lx3, Bx1, Bx3 };

		// phase 2 move to std move
		const int ud2std[10] = { Ux1, Ux2, Ux3, Rx2, Fx2, Dx1, Dx2, Dx3, Lx2, Bx2 };

		int std2ud[18];			// std move 2 phase 2 move, 无则保持-1


		// phase2 的 skipmove, 当前不得与上一步相对，或逆序相对。
		// ckmv2[i][j] = true 表示 a) ix和jx 相等或 b) ix和jx相对且逆序(ix>jx). 
		// b 即 ix 是 DLB, jx 是 URF 并与 ix 相对
		// ix 表示上一步ud2std[i] 的轴, jx 表示当前步ud2std[j] 的轴.
		// ckmv2[10][:] = false
		bool ckmv2[11][10];


		//////////////////////////////////////////////////////////////////////////
		// symmetry cuble	

		// 沿 URF-DLB axis 顺时针旋转 120 度
		const CubieCube s_urf = { 2531, 1373, 67026819, 1367 };

		// 沿着 U-D axis 顺时针旋转 90 度
		const CubieCube s_f2 = { 28783, 0, 259268407, 0 };

		// 相对 LR Slice 平面镜像
		const CubieCube s_u4 = { 15138, 0, 119765538, 7 };

		// 沿着 F-B axis 顺时针旋转 180 度
		const CubieCube s_lr2 ={
			{ 1,0,3,2,5,4,7,6 },{ 3, 3, 3, 3, 3, 3, 3, 3 },
			{ 2,1,0,3,6,5,4,7,9,8,11,10 },{ 0,0,0,0,0,0,0,0,0,0,0,0 }};



		CubieCube moveCube[18];  // U(Ux1, Ux2, Ux3), R L D L B
		int64 moveCubeSym[18];	// moveCube 的 selfSymmetry 标志位

		// 标志位(18位)，对应 18 种 move
		// for 0 <= s <= 15, firstMoveSym[s].bit(i) = symMove[s][i] < i
		// for 16 <= s <= 31, firstMoveSym[s].bit(16+i) = symMove[s][j] < i, where j = urfMove[2][i]
		// for 32 <= s <= 47, firstMoveSym[s].bit(32+i) = symMove[s][j] < i, where j = urfMove[2][urfMove[2][i]]
		int firstMoveSym[48];	


		CubieCube cubeSym[16];	// 由 s_f2, s_lr2, s_u4 生成， 16 = 2 * 2 * 4

		int symInv[16];	// symInv[i] = j 表示 cubeSym[i] * cubeSym[j] == eye
		int symMult[16][16];	// symMult[i][j] = k 表示 S[i] * S[j] == S[k], where S[i] = cubeSym[i]
		int symMove[16][18];	// symMult[i][j] = k 表示 S * moveCube[j] * S^-1 = moveCube[k], where S = cubeSym[i]
		int symMultInv[16][16];	// symMultInv[i][j] = k 表示 S[i] * S[j].inv() == S[k], where S[i] = cubeSym[i]
		int sym8Mult[8][8];		// sym8Mult[i][j] = SymMult[2*i][2*j]/2
		int sym8Move[8][18];		// sym8Move[i][j] = symMove[2*i][j]
		int sym8MultInv[8][8];	// sym8MultInv[i][j] = SymMult[2*i][symInv(2*j)]/2
		int symMoveUD[16][10];	// 类似 symMove, 但仅 phase 2.

		// Sym-Coordnate to Raw-Coordnate
		// xS2R[count] = raw0,		指向其中某一个
		// xR2S[idx] = {count, s(低3位)} means   idx = conj(raw0, S)  i.e.  raw0 = conj(idx, S);  S means cubeSym[2*s] or cubeSym[s]
		// symState[count].bit(j) = 1 means conj(raw0,S) = raw0, s means  cubeSym[2*s] or cubeSym[s]


		short flipS2R[336];
		short twistS2R[324];
		unsigned short epermS2R[2768];

		short flipS2RF[336*8];
		short twistS2RF[324*8];

		// Sym State
		char symStateFlip[336];	//
		char symStateTwist[324];
		short symStatePerm[2768];	// means EPerm, CPerm 也通用。


		// Raw-Coordnate to Sym-Coordnate, only for speeding up initializaion.
		// Sym-Coordnate: 高位, count 值，flip/twist低3位 s, 2*s 表示关于 symCube[2*s], perm 低4位，16种可能。
		sym3_t flipR2S[2048];		// 2^11 = 2048, cubeSym[2*s]
		sym3_t twistR2S[2187];	// 3^7 = 2187, cubeSym[2*s]
		sym4_t epermR2S[40320];	// 8! = 40320, cubeSym[s]


		/**
		* Notice that Edge Perm Coordnate and Corner Perm Coordnate are the same symmetry structure.
		* So their ClassIndexToRepresentantArray are the same.
		* And when x is RawEdgePermCoordnate, y*16+k is SymEdgePermCoordnate, y*16+(k^e2c[k]) will
		* be the SymCornerPermCoordnate of the State whose RawCornerPermCoordnate is x.

		k:         0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  10, 11,12, 13, 14, 15
		k^e2c[k]:  0, 1, 2, 3, 5, 6, 7, 4, 9, 10, 11, 8, 12, 13, 14, 15}

		*/
		//const char e2c[16] = { 0, 0, 0, 0, 1, 3, 1, 3, 1, 3, 1, 3, 0, 0, 0, 0 };

		//                0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  10, 11,12, 13, 14, 15
		int s_e2c[16] = { 0, 1, 2, 3, 5, 6, 7, 4, 9, 10, 11, 8, 12, 13, 14, 15 };
		int s_c2e[16] = { 0, 1, 2, 3, 7, 4, 5, 6, 11, 8, 9, 10, 12, 13, 14, 15 };

		sym4_t MtoEPerm[40320];
		char perm2comb[2768];		// EPermSym to U4Comb [0,70), C(8,4)

		CubieTables();

		corner_t conjugate(const corner_t& a, int i);	// b = S^-1 * a * S where S = cubeSym[i].corner
		edge_t conjugate(const edge_t& a, int i);	// b = S^-1 * a * S where S = cubeSym[i].edge


		CubieCube conjugate(const CubieCube& a, const CubieCube& S);	// b = S^-1 * a * S
		CubieCube conjugate(const CubieCube& a, int i);	// b = S^-1 * a * S where S = cubeSym[i]


		// 返回标志位(49位)
		// 前48位：conj(b,symInv(s)) == a, s = [0,15]; 共16*3=48种 symmetry，b 分别对应 a, a.conjURF, a.conjURF.conjURF
		// bit 48 : 满足任意 conj(b.inv,symInv(s)) == a,  s,b 定义如上
		int64 selfSymmetry(const CubieCube& a);

		

		void init_std2ud_ckmv();
		void initMove();
		void initSym();
		void initFlipSym2Raw();
		void initTwistSym2Raw();
		void initPermSym2Raw();


	private:
		static bool inited;
	};





}