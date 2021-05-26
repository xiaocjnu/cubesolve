
#include "CoordCube5.h"
#include <time.h>
#include <iostream>

using namespace std;

namespace cs
{
	bool CoordCubeTables5::inited = false;


	
	CoordCubeTables5 CoordCube5::tables5;

	static CubieTables& _cuTbl = CubieCube::tables;
	CoordCubeTables& _tables6 = CoordCube::tables;

	CoordCube5::CoordCube5()
	{
	}

	CoordCube5::CoordCube5(const CubieCube & cc)
	{

		twist = cc.getTwistSym();
		flip = cc.getFlipSym();

		slice = cc.getUDSliceComb();

		cperm = cc.getCPermSym();
		eperm = cc.getEPermSym();
		mperm = cc.getMPerm();
		ccomb = cc.getCComb();
	}

	int CoordCube5::calcPrunPhase1()
	{


		int h1 = tables5.sliceTwistPrun.get(_tables6.sliceConj[slice][twist.get_s()], twist.get_count());
		int h2 = tables5.sliceFlipPrun.get(_tables6.sliceConj[slice][flip.get_s()], flip.get_count());

		int flipRaw = _cuTbl.flipS2RF[sym3_t(flip.get_count(), _cuTbl.sym8MultInv[flip.get_s()][twist.get_s()]).val];
		int h3 = tables5.twistFlipPrun.get(twist.get_count(), flipRaw);
		int h = max(max(h1, h2),h3);
		prun = h;
		return h;
	}

	int CoordCube5::calcPrunPhase2()
	{
		int h1 = tables5.MCPermPrun.get(_tables6.MPermConj[mperm][cperm.get_s()], cperm.get_count());
		int h2 = tables5.MEPermPrun.get(_tables6.MPermConj[mperm][eperm.get_s()], eperm.get_count());
		int h3 = tables5.CCombEPermPrun.get(_tables6.CCombConj[ccomb][eperm.get_s()], eperm.get_count());
		
		int h = max(max(h1, h2), h3);
		prun = h;
		return h;
	}

	CoordCube5 CoordCube5::movePhase1(int i) const
	{
		CoordCube5 dd = *this;

		dd.twist = _tables6.twistMove[twist.get_count()][_cuTbl.sym8Move[twist.get_s()][i]];
		dd.twist.set_s(_cuTbl.sym8Mult[dd.twist.get_s()][twist.get_s()]);

		dd.flip = _tables6.flipMove[flip.get_count()][_cuTbl.sym8Move[flip.get_s()][i]];
		dd.flip.set_s(_cuTbl.sym8Mult[dd.flip.get_s()][flip .get_s()]);

		dd.slice = _tables6.sliceMove[slice][i];

		return dd;
	}

	CoordCube5 CoordCube5::movePhase2(int i) const
	{
		CoordCube5 dd = *this;


		int n = _cuTbl.symMove[cperm.get_s()][_cuTbl.ud2std[i]];
		dd.cperm = _tables6.CPermMove[cperm.get_count()][n];
		dd.cperm.set_s(_cuTbl.symMult[dd.cperm.get_s()][cperm.get_s()]);


		dd.eperm = _tables6.EPermMove[eperm.get_count()][_cuTbl.symMoveUD[eperm.get_s()][i]];
		dd.eperm.set_s(_cuTbl.symMult[dd.eperm.get_s()][eperm.get_s()]);

		dd.mperm = _tables6.MPermMove[mperm][i];
		dd.ccomb = _tables6.CCombMove[ccomb][_cuTbl.ud2std[i]];

		return dd;
	}

	void CoordCube5::init()
	{
		tables5.init();
	}

	//////////////////////////////////////////////////////////////////////////


	CoordCubeTables5::CoordCubeTables5()
	{
	}


	void CoordCubeTables5::setPruning(char *table, int index, char value)
	{
		int s = ((index & 1) << 2);
		char mask = 0xf << s;
		table[index >> 1] = (~mask & table[index >> 1]) | (mask & (value << s));
	}

	// Extract pruning value
	char CoordCubeTables5::getPruning(char *table, int index)
	{
		int s = (index & 1) << 2;
		return (table[index >> 1] >> s) & 0x0f;;
	}


	void CoordCubeTables5::initSliceTwistPrun()
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
						sym3_t ss = _tables6.twistMove[i_sym][m];
						int symx = ss.get_count();
						int s = ss.get_s();
						int rawx = _tables6.sliceConj[_tables6.sliceMove[i_raw][m]][s];
									
		
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
							if ((_cuTbl.symStateTwist[symx] & (1 << j)) == 0)
								continue;

								
		
							// 以下相同
							int rawxx = _tables6.sliceConj[rawx][j];
		
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


	void CoordCubeTables5::initSliceFlipPrun()
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
						sym3_t ss = _tables6.flipMove[i_sym][m];
						int symx = ss.get_count();
						int s = ss.get_s();
						int rawx = _tables6.sliceConj[_tables6.sliceMove[i_raw][m]][s];


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
							if ((_cuTbl.symStateFlip[symx] & (1 << j)) == 0)
								continue;

							// 以下相同
							int rawxx = _tables6.sliceConj[rawx][j];

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

	void CoordCubeTables5::initTwistFlipPrun()
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
				sym3_t flip_ss = _cuTbl.flipR2S[i_raw];
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
						sym3_t ss = _tables6.twistMove[i_sym][m];
						int twistx = ss.get_count();
						int tsymx = ss.get_s();


						sym3_t flipx_ss = _tables6.flipMove[flip][_cuTbl.sym8Move[fsym][m]];
						int flipx = flipx_ss.get_count();
						int fsymx = _cuTbl.sym8MultInv[_cuTbl.sym8Mult[flipx_ss.get_s()][fsym]][tsymx];

						int rawx = _cuTbl.flipS2RF[sym3_t(flipx, fsymx).val];


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
							if ((_cuTbl.symStateTwist[twistx] & (1 << j)) == 0)
								continue;



							// 以下相同
							int rawxx = _cuTbl.flipS2RF[sym3_t(flipx, _cuTbl.sym8MultInv[fsymx][j]).val];

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

	void CoordCubeTables5::initMCPermPrun()
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
						sym4_t ss = _tables6.CPermMove[i_sym][_cuTbl.ud2std[m]];
						int symx = ss.get_count();
						int s = ss.get_s();
						int rawx = _tables6.MPermConj[_tables6.MPermMove[i_raw][m]][s];


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
							if ((_cuTbl.symStatePerm[symx] & (1 << j)) == 0)
								continue;

							// 以下相同
							int jc = _cuTbl.s_e2c[j]; // j ^ cubieTables.e2c[j];
							int rawxx = _tables6.MPermConj[rawx][jc];

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

	void CoordCubeTables5::initMEPermPrun()
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
						sym4_t ss = _tables6.EPermMove[i_sym][m];
						int symx = ss.get_count();
						int s = ss.get_s();
						int rawx = _tables6.MPermConj[_tables6.MPermMove[i_raw][m]][s];


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
							if ((_cuTbl.symStatePerm[symx] & (1 << j)) == 0)
								continue;

							// 以下相同
							int rawxx = _tables6.MPermConj[rawx][j];

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

	void CoordCubeTables5::initCCombEPermPrun()
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
						sym4_t ss = _tables6.EPermMove[i_sym][m];
						int symx = ss.get_count();
						int s = ss.get_s();
						int rawx = _tables6.CCombConj[_tables6.CCombMove[i_raw][_cuTbl.ud2std[m]]][s];


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
							if ((_cuTbl.symStatePerm[symx] & (1 << j)) == 0)
								continue;

							// 以下相同
							int rawxx = _tables6.CCombConj[rawx][j];

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

	void CoordCubeTables5::init()
	{
		if (inited)
			return;

		inited = true;

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