#pragma once

#include "CubieCube.h"

namespace cs
{
	class CoordCubeRawTables;

	class CoordCubeRaw
	{
	public:
		static CoordCubeRawTables tables;

		// phase 1
		int twist;
		int flip;
		int slice;

		// phase 2
		int slice2;
		int cperm;
		int eperm;



		CoordCubeRaw();
		CoordCubeRaw(const CubieCube& cc);

		int getPrunPhase1() const;
		int getPrunPhase2() const;


		CoordCubeRaw movePhase1(int i) const;	// phase 1 move
		CoordCubeRaw movePhase2(int i) const;	// phase 2 move

		static void init();
	};


	// 单例
	class CoordCubeRawTables
	{
	public:
		static const int N_MOVES = 18;
		static const int N_MOVES2 = 10;

		static const int N_SLICE1 = 495;		// C(12,4)
		static const int N_SLICE2 = 24;		// 4!
		static const int N_TWIST = 2187;	// 3^7
		static const int N_FLIP = 2048;		// 2^11
		static const int N_PERM = 40320;	// 8!

		static const int N_COMB = 70;		// C(8,4)

		//////////////////////////////////////////////////////////////////////////
		// 辅助
		int permMult[24][24];	// perm<4> 乘法表

		// phase 1 move table
		int sliceMove[N_SLICE1][N_MOVES];	// phase1 UD slice comb
		int twistMove[N_TWIST][N_MOVES];
		int flipMove[N_FLIP][N_MOVES];


		// phase 2 move table
		int slice2Move[N_SLICE2][N_MOVES2];		// phase2 UD slice perm
		int CPermMove[N_PERM][N_MOVES2];
		int EPermMove[N_PERM][N_MOVES2];


		// phase1 pruning table
		pruning_table<N_SLICE1, N_TWIST> sliceTwistPrun;
		pruning_table<N_SLICE1, N_FLIP> sliceFlipPrun;
		
		// phase2 pruning table
		pruning_table<N_SLICE2, N_PERM> sliceCPermPrun;
		pruning_table<N_SLICE2, N_PERM> sliceEPermPrun;

		// for research
		int max_sliceTwistPrun;
		int max_sliceFlipPrun;
		int max_sliceCPermPrun;
		int max_sliceEPermPrun;
		

		void initPermMult();

		void initSliceMove();
		void initTwistMove();
		void initFlipMove();

		void initSlice2Move();
		void initCPermMove();
		void initEPermMove();

		void initSliceTwistPrun();
		void initSliceFlipPrun();
		void initSliceCPermPrun();
		void initSliceEPermPrun();


		void init();
		

	private:
		static bool inited;
		static CubieTables& cubieTables;
	};


}