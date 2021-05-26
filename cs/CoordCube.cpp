
#include "CoordCube.h"
#include <time.h>
#include <iostream>

using namespace std;

namespace cs
{
	bool CoordCubeTables::inited = false;
	CubieTables& CoordCubeTables::cuTbl = CubieCube::tables;
	CubieTables& CoordCube::cuTbl = CubieCube::tables;
	CoordCubeTables CoordCube::tables;

	CoordCube::CoordCube()
	{

	}

	CoordCube::CoordCube(const CubieCube & cc)
	{

		twist = cc.getTwistSym();
		flip = cc.getFlipSym();

		slice = cc.getUDSliceComb();

		cperm = cc.getCPermSym();
		eperm = cc.getEPermSym();
		mperm = cc.getMPerm();
		ccomb = cc.getCComb();
	}

	int CoordCube::calcPrunPhase1()
	{


		int h1 = tables.sliceTwistPrun.get(tables.sliceConj[slice][twist.get_s()], twist.get_count());
		int h2 = tables.sliceFlipPrun.get(tables.sliceConj[slice][flip.get_s()], flip.get_count());

		int flipRaw = cuTbl.flipS2RF[sym3_t(flip.get_count(), cuTbl.sym8MultInv[flip.get_s()][twist.get_s()]).val];
		int h3 = tables.twistFlipPrun.get(twist.get_count(), flipRaw);
		int h = max(max(h1, h2),h3);
		prun = h;
		return h;
	}

	int CoordCube::calcPrunPhase2()
	{
		int h1 = tables.MCPermPrun.get(tables.MPermConj[mperm][cperm.get_s()], cperm.get_count());
		int h2 = tables.MEPermPrun.get(tables.MPermConj[mperm][eperm.get_s()], eperm.get_count());
		int h3 = tables.CCombEPermPrun.get(tables.CCombConj[ccomb][eperm.get_s()], eperm.get_count());
		
		int h = max(max(h1, h2), h3);
		prun = h;
		return h;
	}

	CoordCube CoordCube::movePhase1(int i) const
	{
		CoordCube dd = *this;

		dd.twist = tables.twistMove[twist.get_count()][cuTbl.sym8Move[twist.get_s()][i]];
		dd.twist.set_s(cuTbl.sym8Mult[dd.twist.get_s()][twist.get_s()]);

		dd.flip = tables.flipMove[flip.get_count()][cuTbl.sym8Move[flip.get_s()][i]];
		dd.flip.set_s(cuTbl.sym8Mult[dd.flip.get_s()][flip .get_s()]);

		dd.slice = tables.sliceMove[slice][i];

		return dd;
	}

	CoordCube CoordCube::movePhase2(int i) const
	{
		CoordCube dd = *this;


		int n = cuTbl.symMove[cperm.get_s()][cuTbl.ud2std[i]];
		dd.cperm = tables.CPermMove[cperm.get_count()][n];
		dd.cperm.set_s(cuTbl.symMult[dd.cperm.get_s()][cperm.get_s()]);


		dd.eperm = tables.EPermMove[eperm.get_count()][cuTbl.symMoveUD[eperm.get_s()][i]];
		dd.eperm.set_s(cuTbl.symMult[dd.eperm.get_s()][eperm.get_s()]);

		dd.mperm = tables.MPermMove[mperm][i];
		dd.ccomb = tables.CCombMove[ccomb][cuTbl.ud2std[i]];

		return dd;
	}

	void CoordCube::init()
	{
		tables.init();
	}

	//////////////////////////////////////////////////////////////////////////


	CoordCubeTables::CoordCubeTables()
	{
	}


	void CoordCubeTables::setPruning(char *table, int index, char value)
	{
		int s = ((index & 1) << 2);
		char mask = 0xf << s;
		table[index >> 1] = (~mask & table[index >> 1]) | (mask & (value << s));
	}

	// Extract pruning value
	char CoordCubeTables::getPruning(char *table, int index)
	{
		int s = (index & 1) << 2;
		return (table[index >> 1] >> s) & 0x0f;;
	}

	void CoordCubeTables::initPermMult()
	{
		for (int i = 0; i < 24; ++i)
		{
			permutation<4> a(i);
			for (int j = 0; j < 24; ++j)
			{
				permutation<4> b(j);
				permutation<4> c = a * b;
				permMult[i][j] = c.getIndex();
			}
		}
	}

	void CoordCubeTables::initSliceMoveConj()
	{
		CubieCube cc;
		CubieCube dd;

		for (int i = 0; i < N_SLICE; ++i)
		{
			cc.setUDSlice(i, 0);
			for (int j = 0; j < N_MOVES; ++j)
			{
				dd.edge = cc.edge * cuTbl.moveCube[j].edge;
				sliceMove[i][j] = dd.getUDSliceComb();
			}

			for (int s = 0; s < 16; s+=2)
			{
				dd.edge = cuTbl.conjugate(cc.edge, cuTbl.symInv[s]);
				sliceConj[i][s>>1] = dd.getUDSliceComb();
			}
		}
	}

	void CoordCubeTables::initTwistMove()
	{
		CubieCube c;
		CubieCube d;
		for (int i = 0; i < N_TWIST_SYM; i++)
		{
			c.setTwist(cuTbl.twistS2R[i]);
			for (int j = 0; j < N_MOVES; j++)
			{
				d.corner = c.corner * cuTbl.moveCube[j].corner;
				twistMove[i][j] = d.getTwistSym();
			}
		}
	}

	void CoordCubeTables::initFlipMove()
	{
		CubieCube c;
		CubieCube d;
		for (int i = 0; i < N_FLIP_SYM; i++) {
			c.setFlip(cuTbl.flipS2R[i]);
			for (int j = 0; j < N_MOVES; j++) {
				d.edge = c.edge * cuTbl.moveCube[j].edge;
				flipMove[i][j] = d.getFlipSym();
			}
		}
	}

// 	void CoordTables::initUDSliceMoveConj()
// 	{
// 		CubieCube c;
// 		CubieCube d;
// 
// 		for (int i = 0; i < N_SLICE; i++) {
// 			c.setUDSlice(i, 0);
// 			for (int j = 0; j < N_MOVES; ++j)
// 			{
// 				d.edge = c.edge * cubieTables.moveCube[j].edge;
// 				UDSliceMove[i][j] = (short)d.getUDSliceComb();
// 				UDSlicePermMove[i][j] = (short)d.getUDSlicePerm();
// 			}
// 			for (int j = 0; j < 16; j += 2)
// 			{
// 				d.edge = cubieTables.conjugate(c.edge, cubieTables.symInv[j]);
// 				UDSliceConj[i][j >> 1] = (short)d.getUDSliceComb();
// 			}
// 		}
// 
// 
// 		// 		for (int i = 0; i < N_SLICE; i++) {
// 		// 			c.setUDSlice(i, 0);
// 		// 			for (int j = 0; j < N_MOVES; j += 3)
// 		// 			{
// 		// 				d.edge = c.edge * cubieTables.moveCube[j].edge;
// 		// 				UDSliceMove[i][j] = (short)d.getUDSliceComb();
// 		// 				UDSlicePermMove[i][j] = (short)d.getUDSlicePerm();
// 		// 			}
// 		// 			for (int j = 0; j < 16; j += 2)
// 		// 			{
// 		// 				d.edge = cubieTables.conjugate(c.edge, cubieTables.symInv[j]);
// 		// 				UDSliceConj[i][j >> 1] = (short)d.getUDSliceComb();
// 		// 			}
// 		// 		}
// 		// 		for (int i = 0; i < N_SLICE; i++)
// 		// 		{
// 		// 			for (int j = 0; j < N_MOVES; j += 3)
// 		// 			{
// 		// 				int udslice_comb = UDSliceMove[i][j];
// 		// 				int udslice_perm = UDSlicePermMove[i][j];
// 		// 				for (int k = 1; k < 3; k++)
// 		// 				{
// 		// 					int cx_comb = UDSliceMove[udslice_comb][j];
// 		// 					int cx_perm = UDSlicePermMove[udslice_comb][j];
// 		// 					
// 		// 					UDSliceMove[i][j + k] = (short)cx_comb;
// 		// 					UDSlicePermMove[i][j + k] = (short)permMult[udslice_perm][cx_perm];
// 		// 				}
// 		// 			}
// 		// 		}
// 	}
	void CoordCubeTables::initCPermMove()
	{
		CubieCube c;
		CubieCube d;
		for (int i = 0; i < N_PERM_SYM; i++) {
			c.setCPerm(cuTbl.epermS2R[i]);
			for (int j = 0; j < N_MOVES; j++) {
				d.corner = c.corner * cuTbl.moveCube[j].corner;
				CPermMove[i][j] = d.getCPermSym();
			}
		}
	}

	void CoordCubeTables::initEPermMove()
	{
		CubieCube c;
		CubieCube d;
		for (int i = 0; i < N_PERM_SYM; i++) {
			c.setEPerm(cuTbl.epermS2R[i]);
			for (int j = 0; j < N_MOVES2; j++) {
				d.edge = c.edge * cuTbl.moveCube[cuTbl.ud2std[j]].edge;
				EPermMove[i][j] = d.getEPermSym();
			}
		}
	}

	void CoordCubeTables::initMPermMoveConj()
	{
		CubieCube c;
		CubieCube d;
		for (int i = 0; i < N_MPERM; i++) {
			c.setMPerm(i);
			for (int j = 0; j < N_MOVES2; j++) {
				d.edge = c.edge * cuTbl.moveCube[cuTbl.ud2std[j]].edge;
				MPermMove[i][j] = d.getMPerm();
			}

			for (int s = 0; s < 16; ++s)
			{
				d.edge = cuTbl.conjugate(c.edge, cuTbl.symInv[s]);
				MPermConj[i][s] = (char)d.getMPerm();

				d.edge = cuTbl.conjugate(c.edge, s);
				MPermConj2[i][s] = (char)d.getMPerm();
			}
		}
	}

	void CoordCubeTables::initCCombMoveConj()
	{
		CubieCube c;
		CubieCube d;

		for (int i = 0; i < N_COMB; i++)
		{
			c.setCComb(i);
			for (int j = 0; j < N_MOVES; j++) {
				d.corner = c.corner * cuTbl.moveCube[j].corner;
				CCombMove[i][j] = (char)d.getCComb();
			}

			for (int j = 0; j < 16; j++)
			{
				d.corner = cuTbl.conjugate(c.corner, cuTbl.symInv[j]);
				CCombConj[i][j] = (char)d.getCComb();
			}
		}
	}

// 	void CoordTables::initTwistMoveConj()
// 	{
// 		CubieCube c;
// 		CubieCube d;
// 		for (int i = 0; i < N_TWIST; i++) {
// 			c.setTwist(i);
// 			for (int j = 0; j < N_MOVES; j += 3) {
// 				d.corner = c.corner * cubieTables.moveCube[j].corner;
// 				TwistMoveF[i][j] = (char)d.getTwist();
// 			}
// 			for (int j = 0; j < 16; j++) {
// 				d.corner = cubieTables.conjugate(c.corner, cubieTables.symInv[j]);
// 				TwistConj[i][j] = (char)d.getTwist();
// 			}
// 		}
// 		for (int i = 0; i < N_TWIST; i++) {
// 			for (int j = 0; j < N_MOVES; j += 3) {
// 				int twist = TwistMoveF[i][j];
// 				for (int k = 1; k < 3; k++) {
// 					twist = TwistMoveF[twist][j];
// 					TwistMoveF[i][j + k] = (char)twist;
// 				}
// 			}
// 		}
// 	}
// 
// 	void CoordTables::initTwistFlipPrun()
// 	{
// 		int depth = 0;
// 		int done = 1;
// 		bool inv;
// 		int select;
// 		int check;
// 		const int N_SIZE = N_FLIP * N_TWIST_SYM;
// 
// 		for (int i = 0; i < (int)GET_ARRAY_SIZE(TwistFlipPrun); i++)
// 		{
// 			TwistFlipPrun[i] = -1;
// 		}
// 		setPruning(TwistFlipPrun, 0, 0);
// 
// 		while (done < N_SIZE) {
// 			inv = depth > 6;
// 			select = inv ? 0xf : depth;
// 			check = inv ? depth : 0xf;
// 			depth++;
// 			int val = 0;
// 			for (int i = 0; i < N_SIZE; i++, val >>= 4)
// 			{
// 				if ((i & 7) == 0)
// 				{
// 					val = TwistFlipPrun[i >> 3];
// 					if (!inv && val == -1)
// 					{
// 						i += 7;
// 						continue;
// 					}
// 				}
// 				if ((val & 0xf) != select)
// 				{
// 					continue;
// 				}
// 				int twist = i >> 11;
// 				int flip = cubieTables.flipR2S[i & 0x7ff];
// 				int fsym = flip & 7;
// 				flip >>= 3;
// 				for (int m = 0; m < N_MOVES; m++)
// 				{
// 					int twistx = TwistMove[twist][m];
// 					int tsymx = twistx & 7;
// 					twistx >>= 3;
// 					int flipx = FlipMove[flip][cubieTables.sym8Move[m][fsym]];
// 					int fsymx = cubieTables.sym8MultInv[cubieTables.sym8Mult[fsym][flipx & 7]][tsymx];
// 					flipx >>= 3;
// 					int idx = twistx << 11 | cubieTables.flipS2RF[flipx][fsymx];
// 					if (getPruning(TwistFlipPrun, idx) != check)
// 					{
// 						continue;
// 					}
// 					done++;
// 					if (inv) {
// 						setPruning(TwistFlipPrun, i, depth);
// 						break;
// 					}
// 					setPruning(TwistFlipPrun, idx, depth);
// 					char sym = cubieTables.symStateTwist[twistx];
// 					if (sym == 1) {
// 						continue;
// 					}
// 					for (int k = 0; k < 8; k++)
// 					{
// 						if ((sym & 1 << k) == 0)
// 						{
// 							continue;
// 						}
// 						int idxx = twistx << 11 | cubieTables.flipS2RF[flipx][cubieTables.sym8MultInv[fsymx][k]];
// 						if (getPruning(TwistFlipPrun, idxx) == 0xf)
// 						{
// 							setPruning(TwistFlipPrun, idxx, depth);
// 							done++;
// 						}
// 					}
// 				}
// 			}
// 			// System.out.println(String.format("%2d%10d", depth, done));
// 		}
// 	}





	void CoordCubeTables::initSliceTwistPrun()
	{
		// 初始为 ffff，表示未定义
		for (int i = 0; i < sliceTwistPrun.nbytes(); ++i)
		{
			sliceTwistPrun[i] = -1;
		}
		
		
		sliceTwistPrun.set(0, 0, 0);
		int depth = 0;
		int done = 1;
		
		
		while (done < sliceTwistPrun.numel())
		{
			bool inv = done > sliceTwistPrun.numel() * 7 / 10;  // 反过来计算, 节省时间
			int select = inv ? 0xf : depth;
			int check = inv ? depth : 0xf;
		
		
			for (int i_raw = 0; i_raw < N_SLICE; ++i_raw)
			{
				for (int i_sym = 0; i_sym < N_TWIST_SYM; ++i_sym)
				{
					int prun = sliceTwistPrun.get(i_raw, i_sym);
		
							
					if (prun != select)
						continue;
		
					// prun == depth（未定义 for inv)
		
							
					for (int m = 0; m < N_MOVES; ++m)
					{
						sym3_t ss = twistMove[i_sym][m];
						int symx = ss.get_count();
						int s = ss.get_s();
						int rawx = sliceConj[sliceMove[i_raw][m]][s];
									
		
						// 新目标不满足未定义/(等于depth for inv)
						if (sliceTwistPrun.get(rawx, symx) != check)
							continue;
									
						// 新目标未定义/(等于depth for inv)
										
						done++;
		
						if (inv)
						{
							sliceTwistPrun.set(i_raw, i_sym, (char)(depth + 1));
							break;
						}
						// not inv
										
						sliceTwistPrun.set(rawx, symx, (char)(depth + 1));
									
						// conj后twist相同，但 slice 不同
						for (int j = 1; j < 8; ++j)
						{
							// 不相同则继续
							if ((cuTbl.symStateTwist[symx] & (1 << j)) == 0)
								continue;

								
		
							// 以下相同
							int rawxx = sliceConj[rawx][j];
		
							if (sliceTwistPrun.get(rawxx, symx) == check)
							{
								sliceTwistPrun.set(rawxx, symx, (char)(depth + 1));
								done++;
							}
						}
									
					}// for m					
				}// for i_sym
			}// for i_raw
		
			++depth;
		
			assert(depth < 15);
		}// for while
		
		assert(done == sliceTwistPrun.numel());
		
		max_sliceTwistPrun = sliceTwistPrun.calc_max();
	}


	void CoordCubeTables::initSliceFlipPrun()
	{
		// 初始为 ffff，表示未定义
		for (int i = 0; i < sliceFlipPrun.nbytes(); ++i)
		{
			sliceFlipPrun[i] = -1;
		}


		sliceFlipPrun.set(0, 0, 0);
		int depth = 0;
		int done = 1;


		while (done < sliceFlipPrun.numel())
		{
			bool inv = done > sliceFlipPrun.numel() * 7 / 10;  // 反过来计算, 节省时间
			int select = inv ? 0xf : depth;
			int check = inv ? depth : 0xf;


			for (int i_raw = 0; i_raw < N_SLICE; ++i_raw)
			{
				for (int i_sym = 0; i_sym < N_FLIP_SYM; ++i_sym)
				{
					int prun = sliceFlipPrun.get(i_raw, i_sym);


					if (prun != select)
						continue;

					// prun == depth（未定义 for inv)


					for (int m = 0; m < N_MOVES; ++m)
					{
						sym3_t ss = flipMove[i_sym][m];
						int symx = ss.get_count();
						int s = ss.get_s();
						int rawx = sliceConj[sliceMove[i_raw][m]][s];


						// 新目标不满足未定义/(等于depth for inv)
						if (sliceFlipPrun.get(rawx, symx) != check)
							continue;

						// 新目标未定义/(等于depth for inv)

						done++;

						if (inv)
						{
							sliceFlipPrun.set(i_raw, i_sym, (char)(depth + 1));
							break;
						}
						// not inv

						sliceFlipPrun.set(rawx, symx, (char)(depth + 1));

						// conj后flip相同，但 slice 不同
						for (int j = 1; j < 8; ++j)
						{
							// 不相同则继续
							if ((cuTbl.symStateFlip[symx] & (1 << j)) == 0)
								continue;

							// 以下相同
							int rawxx = sliceConj[rawx][j];

							if (sliceFlipPrun.get(rawxx, symx) == check)
							{
								sliceFlipPrun.set(rawxx, symx, (char)(depth + 1));
								done++;
							}
						}

					}// for m					
				}// for i_sym
			}// for i_raw

			++depth;

			assert(depth < 15);
		}// for while

		assert(done == sliceFlipPrun.numel());

		max_sliceFlipPrun = sliceFlipPrun.calc_max();
		
	}

	void CoordCubeTables::initTwistFlipPrun()
	{
		// 初始为 ffff，表示未定义
		for (int i = 0; i < twistFlipPrun.nbytes(); ++i)
		{
			twistFlipPrun[i] = -1;
		}


		twistFlipPrun.set(0, 0, 0);
		int depth = 0;
		int done = 1;


		while (done < twistFlipPrun.numel())
		{
			bool inv = done > twistFlipPrun.numel() * 7 / 10;  // 反过来计算, 节省时间
			int select = inv ? 0xf : depth;
			int check = inv ? depth : 0xf;


			for (int i_raw = 0; i_raw < N_FLIP; ++i_raw)
			{
				sym3_t flip_ss = cuTbl.flipR2S[i_raw];
				int flip = flip_ss.get_count();
				int fsym = flip_ss.get_s();

				for (int i_sym = 0; i_sym < N_TWIST_SYM; ++i_sym)
				{
					int prun = twistFlipPrun.get(i_sym, i_raw);


					if (prun != select)
						continue;

					// prun == depth（未定义 for inv)


					for (int m = 0; m < N_MOVES; ++m)
					{
						sym3_t ss = twistMove[i_sym][m];
						int twistx = ss.get_count();
						int tsymx = ss.get_s();


						sym3_t flipx_ss = flipMove[flip][cuTbl.sym8Move[fsym][m]];
						int flipx = flipx_ss.get_count();
						int fsymx = cuTbl.sym8MultInv[cuTbl.sym8Mult[flipx_ss.get_s()][fsym]][tsymx];

						int rawx = cuTbl.flipS2RF[sym3_t(flipx, fsymx).val];


						// 新目标不满足未定义/(等于depth for inv)
						if (twistFlipPrun.get(twistx, rawx) != check)
							continue;

						// 新目标未定义/(等于depth for inv)

						done++;

						if (inv)
						{
							twistFlipPrun.set(i_sym, i_raw, (char)(depth + 1));
							break;
						}
						// not inv

						twistFlipPrun.set(twistx, rawx, (char)(depth + 1));

						// conj后twist相同，但 slice 不同
						for (int j = 1; j < 8; ++j)
						{
							// 不相同则继续
							if ((cuTbl.symStateTwist[twistx] & (1 << j)) == 0)
								continue;



							// 以下相同
							int rawxx = cuTbl.flipS2RF[sym3_t(flipx, cuTbl.sym8MultInv[fsymx][j]).val];

							if (twistFlipPrun.get(twistx, rawxx) == check)
							{
								twistFlipPrun.set(twistx, rawxx, (char)(depth + 1));
								done++;
							}
						}

					}// for m					
				}// for i_sym
			}// for i_raw

			++depth;

			assert(depth < 15);
		}// for while

		assert(done == twistFlipPrun.numel());

		max_twistFlipPrun = twistFlipPrun.calc_max();
	}

	void CoordCubeTables::initMCPermPrun()
	{
		// 初始为 ffff，表示未定义
		for (int i = 0; i < MCPermPrun.nbytes(); ++i)
		{
			MCPermPrun[i] = -1;
		}


		MCPermPrun.set(0, 0, 0);
		int depth = 0;
		int done = 1;


		while (done < MCPermPrun.numel())
		{
			bool inv = done > MCPermPrun.numel() * 7 / 10;  // 反过来计算, 节省时间
			int select = inv ? 0xf : depth;
			int check = inv ? depth : 0xf;


			for (int i_raw = 0; i_raw < N_MPERM; ++i_raw)
			{
				for (int i_sym = 0; i_sym < N_PERM_SYM; ++i_sym)
				{
					int prun = MCPermPrun.get(i_raw, i_sym);


					if (prun != select)
						continue;

					// prun == depth（未定义 for inv)


					for (int m = 0; m < N_MOVES2; ++m)
					{
						sym4_t ss = CPermMove[i_sym][cuTbl.ud2std[m]];
						int symx = ss.get_count();
						int s = ss.get_s();
						int rawx = MPermConj[MPermMove[i_raw][m]][s];


						// 新目标不满足未定义/(等于depth for inv)
						if (MCPermPrun.get(rawx, symx) != check)
							continue;

						// 新目标未定义/(等于depth for inv)

						done++;

						if (inv)
						{
							MCPermPrun.set(i_raw, i_sym, (char)(depth + 1));
							break;
						}
						// not inv

						MCPermPrun.set(rawx, symx, (char)(depth + 1));

						// conj后 sym 相同，但 raw 不同
						for (int j = 1; j < 16; ++j)
						{
							// 不相同则继续
							if ((cuTbl.symStatePerm[symx] & (1 << j)) == 0)
								continue;

							// 以下相同
							int jc = cuTbl.s_e2c[j]; // j ^ cubieTables.e2c[j];
							int rawxx = MPermConj[rawx][jc];

							if (MCPermPrun.get(rawxx, symx) == check)
							{
								MCPermPrun.set(rawxx, symx, (char)(depth + 1));
								done++;
							}
						}

					}// for m					
				}// for i_sym
			}// for i_raw

			++depth;

			assert(depth < 15);
		}// for while

		assert(done == MCPermPrun.numel());

		max_MCPermPrun = MCPermPrun.calc_max();
	}

	void CoordCubeTables::initMEPermPrun()
	{
		// 初始为 ffff，表示未定义
		for (int i = 0; i < MEPermPrun.nbytes(); ++i)
		{
			MEPermPrun[i] = -1;
		}


		MEPermPrun.set(0, 0, 0);
		int depth = 0;
		int done = 1;


		while (done < MEPermPrun.numel())
		{
			bool inv = done > MEPermPrun.numel() * 7 / 10;  // 反过来计算, 节省时间
			int select = inv ? 0xf : depth;
			int check = inv ? depth : 0xf;


			for (int i_raw = 0; i_raw < N_MPERM; ++i_raw)
			{
				for (int i_sym = 0; i_sym < N_PERM_SYM; ++i_sym)
				{
					int prun = MEPermPrun.get(i_raw, i_sym);


					if (prun != select)
						continue;

					// prun == depth（未定义 for inv)


					for (int m = 0; m < N_MOVES2; ++m)
					{
						sym4_t ss = EPermMove[i_sym][m];
						int symx = ss.get_count();
						int s = ss.get_s();
						int rawx = MPermConj[MPermMove[i_raw][m]][s];


						// 新目标不满足未定义/(等于depth for inv)
						if (MEPermPrun.get(rawx, symx) != check)
							continue;

						// 新目标未定义/(等于depth for inv)

						done++;

						if (inv)
						{
							MEPermPrun.set(i_raw, i_sym, (char)(depth + 1));
							break;
						}
						// not inv

						MEPermPrun.set(rawx, symx, (char)(depth + 1));

						// conj后 sym 相同，但 raw 不同
						for (int j = 1; j < 16; ++j)
						{
							// 不相同则继续
							if ((cuTbl.symStatePerm[symx] & (1 << j)) == 0)
								continue;

							// 以下相同
							int rawxx = MPermConj[rawx][j];

							if (MEPermPrun.get(rawxx, symx) == check)
							{
								MEPermPrun.set(rawxx, symx, (char)(depth + 1));
								done++;
							}
						}

					}// for m					
				}// for i_sym
			}// for i_raw

			++depth;

			assert(depth < 15);
		}// for while

		assert(done == MEPermPrun.numel());

		max_MEPermPrun = MEPermPrun.calc_max();
	}

	void CoordCubeTables::initCCombEPermPrun()
	{
		// 初始为 ffff，表示未定义
		for (int i = 0; i < CCombEPermPrun.nbytes(); ++i)
		{
			CCombEPermPrun[i] = -1;
		}


		CCombEPermPrun.set(0, 0, 0);
		int depth = 0;
		int done = 1;


		while (done < CCombEPermPrun.numel())
		{
			bool inv = done > CCombEPermPrun.numel() * 7 / 10;  // 反过来计算, 节省时间
			int select = inv ? 0xf : depth;
			int check = inv ? depth : 0xf;


			for (int i_raw = 0; i_raw < N_COMB; ++i_raw)
			{
				for (int i_sym = 0; i_sym < N_PERM_SYM; ++i_sym)
				{
					int prun = CCombEPermPrun.get(i_raw, i_sym);


					if (prun != select)
						continue;

					// prun == depth（未定义 for inv)


					for (int m = 0; m < N_MOVES2; ++m)
					{
						sym4_t ss = EPermMove[i_sym][m];
						int symx = ss.get_count();
						int s = ss.get_s();
						int rawx = CCombConj[CCombMove[i_raw][cuTbl.ud2std[m]]][s];


						// 新目标不满足未定义/(等于depth for inv)
						if (CCombEPermPrun.get(rawx, symx) != check)
							continue;

						// 新目标未定义/(等于depth for inv)

						done++;

						if (inv)
						{
							CCombEPermPrun.set(i_raw, i_sym, (char)(depth + 1));
							break;
						}
						// not inv

						CCombEPermPrun.set(rawx, symx, (char)(depth + 1));

						// conj后 sym 相同，但 raw 不同
						for (int j = 1; j < 16; ++j)
						{
							// 不相同则继续
							if ((cuTbl.symStatePerm[symx] & (1 << j)) == 0)
								continue;

							// 以下相同
							int rawxx = CCombConj[rawx][j];

							if (CCombEPermPrun.get(rawxx, symx) == check)
							{
								CCombEPermPrun.set(rawxx, symx, (char)(depth + 1));
								done++;
							}
						}

					}// for m					
				}// for i_sym
			}// for i_raw

			++depth;

			assert(depth < 15);
		}// for while

		assert(done == CCombEPermPrun.numel());

		max_CCombEPermPrun = CCombEPermPrun.calc_max();
	}

	void CoordCubeTables::init()
	{
		if (inited)
			return;

		inited = true;

		initPermMult();

		initSliceMoveConj();
		initTwistMove();
		initFlipMove();

		initCPermMove();
		initEPermMove();
		initMPermMoveConj();
		initCCombMoveConj();


		initSliceTwistPrun();
		initSliceFlipPrun();
		initTwistFlipPrun();
		
		initMEPermPrun();
		initMCPermPrun();
		initCCombEPermPrun();

// 		clock_t t1a = clock();
// 		
// 		clock_t t1b = clock();
// 
// 		int dt = t1b - t1a;
// 		cout << "test: " << dt << "ms\n";


		


	}











}