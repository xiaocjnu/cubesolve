// 只允许转5个轴 URFDL, 没有B


#pragma once
#include "CubieCube.h"
#include "CoordCube.h"


namespace cs
{
	class CoordCubeTables5;

	class CoordCube5
	{
	public:
		static CoordCubeTables5 tables5;

		// phase 1
		sym3_t twist;
		sym3_t flip;
		int slice;		// UDSlice Comb

		// phase 2
		sym4_t cperm;		// corner perm
		sym4_t eperm;		// UD 8 edge perm
		int mperm;		// M(i.e. UDSlice) perm
		int ccomb;		// U corner combination

		int prun;


		CoordCube5();
		CoordCube5(const CubieCube& cc);


		// 计算 prun 并缓存到 prun
		int calcPrunPhase1();
		int calcPrunPhase2();

		CoordCube5 movePhase1(int i) const;	// phase 1 move
		CoordCube5 movePhase2(int i) const;	// phase 2 move


		static void init();

	};



	// 单例
	class CoordCubeTables5
	{
	public:
		static const int N_MOVES = 18;
		static const int N_MOVES2 = 10;

		static const int N_SLICE = 495;		// C(12,4)
		static const int N_TWIST = 2187;	// 3^7
		static const int N_TWIST_SYM = 324;
		static const int N_FLIP = 2048;		// 2^11
		static const int N_FLIP_SYM = 336;
		static const int N_PERM = 40320;	// 8!
		static const int N_PERM_SYM = 2768;
		static const int N_MPERM = 24;		// 4!
		static const int N_COMB = 70;		// C(8,4), phase 2, Upper Corner Combination
											//static const int N_UDSLICEFLIP_SYM = 64430;

		// pruning table

		// phase 1
		pruning_table<N_SLICE, N_TWIST_SYM> sliceTwistPrun;	// UDSlice and twist
		pruning_table<N_SLICE, N_FLIP_SYM> sliceFlipPrun;		// UDSlice and flip
		pruning_table<N_TWIST_SYM, N_FLIP> twistFlipPrun;		// twist(sym) ans flip(raw)

																// phase 2
		pruning_table<N_MPERM, N_PERM_SYM> MCPermPrun;		// MPerm and CPerm
		pruning_table<N_MPERM, N_PERM_SYM> MEPermPrun;		// MPerm and EPerm
		pruning_table<N_COMB, N_PERM_SYM> CCombEPermPrun;		// CComb and EPerm

		// for research
		int max_sliceTwistPrun;
		int max_sliceFlipPrun;
		int max_MCPermPrun;
		int max_MEPermPrun;
		int max_CCombEPermPrun;
		int max_twistFlipPrun;

		CoordCubeTables5();


		static void setPruning(char *table, int index, char value);

		// Extract pruning value
		static char getPruning(char* table, int index);


		void initSliceTwistPrun();
		void initSliceFlipPrun();
		void initTwistFlipPrun();

		void initMCPermPrun();
		void initMEPermPrun();
		void initCCombEPermPrun();


		void init();

	private:
		static bool inited;
	};

}