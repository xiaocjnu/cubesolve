#include "util.h"
#include "CoordCubeRaw.h"
#include "CubieCube.h"
#include <algorithm>

using namespace std;

namespace cs
{
	bool CoordCubeRawTables::inited = false;
	CubieTables& CoordCubeRawTables::cubieTables = CubieCube::tables;

	CoordCubeRawTables CoordCubeRaw::tables;



	CoordCubeRaw::CoordCubeRaw()
	{
	}

	CoordCubeRaw::CoordCubeRaw(const CubieCube & cc)
	{
		twist = cc.getTwist();
		flip = cc.getFlip();
		slice = cc.getUDSliceComb();

		cperm = cc.getCPerm();
		eperm = cc.getEPerm();
		slice2 = cc.getUDSlicePerm();
	}

	//////////////////////////////////////////////////////////////////////////
	int CoordCubeRaw::getPrunPhase1() const
	{

		int h1 = tables.sliceTwistPrun.get(slice, twist);
		int h2 = tables.sliceFlipPrun.get(slice, flip);
		return max(h1, h2);
	}

	int CoordCubeRaw::getPrunPhase2() const
	{
		int h1 = tables.sliceCPermPrun.get(slice2, cperm);
		int h2 = tables.sliceEPermPrun.get(slice2, eperm);
		return max(h1, h2);
	}

	CoordCubeRaw CoordCubeRaw::movePhase1(int i) const
	{
		CoordCubeRaw dd = *this;

		dd.twist = tables.twistMove[twist][i];
		dd.flip = tables.flipMove[flip][i];
		dd.slice = tables.sliceMove[slice][i];


		return dd;
	}

	CoordCubeRaw CoordCubeRaw::movePhase2(int i) const
	{
		CoordCubeRaw dd = *this;

		dd.cperm = tables.CPermMove[cperm][i];
		dd.eperm = tables.EPermMove[eperm][i];
		dd.slice2 = tables.slice2Move[slice2][i];

		return dd;
	}

	void CoordCubeRaw::init()
	{
		tables.init();
	}

	void CoordCubeRawTables::initPermMult()
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

	void CoordCubeRawTables::initSliceMove()
	{
		CubieCube cc;
		CubieCube dd;

		for (int i = 0; i < N_SLICE1; ++i)
		{
			cc.setUDSlice(i, 0);
			for (int j = 0; j < N_MOVES; ++j)
			{
				dd.edge = cc.edge * cubieTables.moveCube[j].edge;
				sliceMove[i][j] = dd.getUDSliceComb();
			}
		}
	}

	void CoordCubeRawTables::initTwistMove()
	{
		CubieCube cc;
		CubieCube dd;

		for (int i = 0; i < N_TWIST; ++i)
		{
			cc.setTwist(i);
			for (int j = 0; j < N_MOVES; ++j)
			{
				dd.corner = cc.corner * cubieTables.moveCube[j].corner;
				twistMove[i][j] = dd.getTwist();
			}
		}

	}

	void CoordCubeRawTables::initFlipMove()
	{
		CubieCube cc;
		CubieCube dd;

		for (int i = 0; i < N_FLIP; ++i)
		{
			cc.setFlip(i);
			for (int j = 0; j < N_MOVES; ++j)
			{
				dd.edge = cc.edge * cubieTables.moveCube[j].edge;
				flipMove[i][j] = dd.getFlip();
			}
		}
	}

	void CoordCubeRawTables::initSlice2Move()
	{
		CubieCube cc;
		CubieCube dd;

		for (int i = 0; i < N_SLICE2; ++i)
		{
			cc.setUDSlice(0, i);

			for (int j = 0; j < N_MOVES2; ++j)
			{
				dd.edge = cc.edge * cubieTables.moveCube[cubieTables.ud2std[j]].edge;
				slice2Move[i][j] = dd.getUDSlicePerm();
			}
		}
	}

	void CoordCubeRawTables::initCPermMove()
	{
		CubieCube cc;
		CubieCube dd;

		for (int i = 0; i < N_PERM; ++i)
		{
			cc.setCPerm(i);
			for (int j = 0; j < N_MOVES2; ++j)
			{
				dd.corner = cc.corner * cubieTables.moveCube[cubieTables.ud2std[j]].corner;
				CPermMove[i][j] = dd.getCPerm();
			}
		}
	}

	void CoordCubeRawTables::initEPermMove()
	{
		CubieCube cc;
		CubieCube dd;

		for (int i = 0; i < N_PERM; ++i)
		{
			cc.setEPerm(i);
			for (int j = 0; j < N_MOVES2; ++j)
			{
				dd.edge = cc.edge * cubieTables.moveCube[cubieTables.ud2std[j]].edge;
				EPermMove[i][j] = dd.getEPerm();
			}
		}
	}

	void CoordCubeRawTables::initSliceTwistPrun()
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
			for (int i_slice = 0; i_slice < N_SLICE1; ++i_slice)
			{
				for (int i_twist = 0; i_twist < N_TWIST; ++i_twist)
				{
					int prun = sliceTwistPrun.get(i_slice, i_twist);

					if (prun == depth)
					{
						for (int j = 0; j < N_MOVES; ++j)
						{
							int new_slice = sliceMove[i_slice][j];
							int new_twist = twistMove[i_twist][j];

							if (sliceTwistPrun.get(new_slice, new_twist) == 0x0f)
							{
								sliceTwistPrun.set(new_slice, new_twist, (char)(depth + 1));
								done++;
							}
						}
					}// if
				}// for
			}// for

			++depth;

			assert(depth < 15);
		}// for while


		max_sliceTwistPrun = 0;
		for (int i_slice = 0; i_slice < N_SLICE1; ++i_slice)
		{
			for (int i_twist = 0; i_twist < N_TWIST; ++i_twist)
			{
				if (sliceTwistPrun.get(i_slice,i_twist) > max_sliceTwistPrun)
					max_sliceTwistPrun = sliceTwistPrun.get(i_slice, i_twist);
			}
		}

	}

	void CoordCubeRawTables::initSliceFlipPrun()
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
			for (int i_slice = 0; i_slice < N_SLICE1; ++i_slice)
			{
				for (int i_flip = 0; i_flip < N_FLIP; ++i_flip)
				{
					int prun = sliceFlipPrun.get(i_slice, i_flip);

					if (prun == depth)
					{
						for (int j = 0; j < N_MOVES; ++j)
						{
							int new_slice = sliceMove[i_slice][j];
							int new_flip = flipMove[i_flip][j];

							if (sliceFlipPrun.get(new_slice, new_flip) == 0x0f)
							{
								sliceFlipPrun.set(new_slice, new_flip, (char)(depth + 1));
								done++;
							}
						}
					}// if
				}// for
			}// for

			++depth;

			assert(depth < 15);
		}// for while

		max_sliceFlipPrun = 0;
		for (int i_slice = 0; i_slice < N_SLICE1; ++i_slice)
		{
			for (int i_flip = 0; i_flip < N_FLIP; ++i_flip)
			{
				if (sliceFlipPrun.get(i_slice, i_flip) > max_sliceFlipPrun)
					max_sliceFlipPrun = sliceFlipPrun.get(i_slice, i_flip);
			}
		}
	}

	void CoordCubeRawTables::initSliceCPermPrun()
	{
		// 初始为 ffff，表示未定义
		for (int i = 0; i < sliceCPermPrun.nbytes(); ++i)
		{
			sliceCPermPrun[i] = -1;
		}


		sliceCPermPrun.set(0, 0, 0);
		int depth = 0;
		int done = 1;

		while (done < sliceCPermPrun.numel())
		{
			for (int i_slice = 0; i_slice < N_SLICE2; ++i_slice)
			{
				for (int i_perm = 0; i_perm < N_PERM; ++i_perm)
				{
					int prun = sliceCPermPrun.get(i_slice, i_perm);

					if (prun == depth)
					{
						for (int j = 0; j < N_MOVES2; ++j)
						{
							int new_slice = slice2Move[i_slice][j];
							int new_perm = CPermMove[i_perm][j];

							if (sliceCPermPrun.get(new_slice, new_perm) == 0x0f)
							{
								sliceCPermPrun.set(new_slice, new_perm, (char)(depth + 1));
								done++;
							}
						}
					}// if
				}// for
			}// for

			++depth;

			assert(depth < 15);
		}// for while

		max_sliceCPermPrun = 0;
		for (int i_slice = 0; i_slice < N_SLICE2; ++i_slice)
		{
			for (int i_perm = 0; i_perm < N_PERM; ++i_perm)
			{
				if (sliceCPermPrun.get(i_slice, i_perm) > max_sliceCPermPrun)
					max_sliceCPermPrun = sliceCPermPrun.get(i_slice, i_perm);
			}
		}

	}

	void CoordCubeRawTables::initSliceEPermPrun()
	{
		// 初始为 ffff，表示未定义
		for (int i = 0; i < sliceEPermPrun.nbytes(); ++i)
		{
			sliceEPermPrun[i] = -1;
		}


		sliceEPermPrun.set(0, 0, 0);
		int depth = 0;
		int done = 1;

		while (done < sliceEPermPrun.numel())
		{
			for (int i_slice = 0; i_slice < N_SLICE2; ++i_slice)
			{
				for (int i_perm = 0; i_perm < N_PERM; ++i_perm)
				{
					int prun = sliceEPermPrun.get(i_slice, i_perm);

					if (prun == depth)
					{
						for (int j = 0; j < N_MOVES2; ++j)
						{
							int new_slice = slice2Move[i_slice][j];
							int new_perm = EPermMove[i_perm][j];

							if (sliceEPermPrun.get(new_slice, new_perm) == 0x0f)
							{
								sliceEPermPrun.set(new_slice, new_perm, (char)(depth + 1));
								done++;
							}
						}
					}// if
				}// for
			}// for

			++depth;

			assert(depth < 15);
		}// for while


		max_sliceEPermPrun = 0;
		for (int i_slice = 0; i_slice < N_SLICE2; ++i_slice)
		{
			for (int i_perm = 0; i_perm < N_PERM; ++i_perm)
			{
				if (sliceEPermPrun.get(i_slice, i_perm) > max_sliceEPermPrun)
					max_sliceEPermPrun = sliceEPermPrun.get(i_slice, i_perm);
			}
		}
	}


	void CoordCubeRawTables::init()
	{

		if (inited)
			return;

		inited = true;


		initPermMult();

		initSliceMove();
		initTwistMove();
		initFlipMove();

		initSlice2Move();
		initCPermMove();
		initEPermMove();

		initSliceTwistPrun();
		initSliceFlipPrun();
		initSliceCPermPrun();

		initSliceEPermPrun();
	}



}