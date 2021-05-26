#pragma once

#include "CubieCube.h"


namespace cs
{
	class CoordCubeTables;



	class CoordCube
	{
	public:
		static CoordCubeTables tables;
		static CubieTables& cuTbl;

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


		CoordCube();
		CoordCube(const CubieCube& cc);


		// 计算 prun 并缓存到 prun
		int calcPrunPhase1();
		int calcPrunPhase2();

		CoordCube movePhase1(int i) const;	// phase 1 move
		CoordCube movePhase2(int i) const;	// phase 2 move


		static void init();

	};



	// 单例
	class CoordCubeTables
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


		//////////////////////////////////////////////////////////////////////////
		// 辅助
		int permMult[24][24];	// perm<4> 乘法表



		//////////////////////////////////////////////////////////////////////////
		//XMove = Move Table
		//XPrun = Pruning Table
		//XConj = Conjugate Table

		// phase1
		//int UDSliceFlipMove [N_UDSLICEFLIP_SYM][N_MOVES];
		short TwistMoveF[N_TWIST][N_MOVES];
		short TwistConj[N_TWIST][16];

		//char[] UDSliceFlipTwistPrunP = null; //Search.USE_FULL_PRUN ? new byte[N_UDSLICEFLIP_SYM * N_TWIST / 5] : null;


		// move table
		//phase1
		short sliceMove[N_SLICE][N_MOVES];		// slice mean UDSlice Combination C(12,4)
		short sliceConj[N_SLICE][8];			// sliceConj[i][s] = conj(i, S^-1), where S = cubeSym[2*s]
		sym3_t twistMove[N_TWIST_SYM][N_MOVES];	// corner orientation
		sym3_t flipMove[N_FLIP_SYM][N_MOVES];


		//phase2
		sym4_t CPermMove[N_PERM_SYM][N_MOVES];
		sym4_t EPermMove[N_PERM_SYM][N_MOVES2];
		char MPermMove[N_MPERM][N_MOVES2];
		short MPermConj[N_MPERM][16];
		short MPermConj2[N_MPERM][16];
		char CCombMove[N_COMB][N_MOVES];
		short CCombConj[N_COMB][16];


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

		static CubieTables& cuTbl;
		CoordCubeTables();


		static void setPruning(char *table, int index, char value);

		// Extract pruning value
		static char getPruning(char* table, int index);

		void initPermMult();


		void initSliceMoveConj();
		void initTwistMove();
		void initFlipMove();



		void initCPermMove();
		void initEPermMove();
		void initMPermMoveConj();
		void initCCombMoveConj();



// 		void initUDSliceMoveConj();
// 		
// 		
// 
// 		//void initUDSliceFlipMove();
// 		void initTwistMoveConj();
// 		void initTwistFlipPrun();

 

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