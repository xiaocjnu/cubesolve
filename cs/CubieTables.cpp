
#include "CubieCube.h"

using namespace std;

namespace cs
{

	bool CubieTables::inited = false;


	CubieTables::CubieTables()
	{
		if (inited)
			throw runtime_error("CubieTables：只允许创建单个实例.");

		inited = true;


		init_std2ud_ckmv();
		initMove();
		initSym();
		initFlipSym2Raw();
		initTwistSym2Raw();
		initPermSym2Raw();
	}

	corner_t CubieTables::conjugate(const corner_t & a, int i)
	{
		return cubeSym[symInv[i]].corner * a * cubeSym[i].corner;
	}

	edge_t CubieTables::conjugate(const edge_t & a, int i)
	{
		return cubeSym[symInv[i]].edge * a * cubeSym[i].edge;
	}

	CubieCube CubieTables::conjugate(const CubieCube & a, const CubieCube & S)
	{
		return S.inv() * a * S;
	}

	CubieCube CubieTables::conjugate(const CubieCube & a, int i)
	{
		
		return cubeSym[symInv[i]] * a * cubeSym[i];
	}

	int64 CubieTables::selfSymmetry(const CubieCube & a)
	{
		CubieCube c = a;
		CubieCube d;

		long sym = 0LL;

		for (int i = 0; i < 48; i++)
		{
			d = conjugate(c, symInv[i % 16]);

			if (d == a)
			{
				sym |= 1LL << i;
			}
			if (i % 16 == 15)
			{
				c = conjugate(c, s_urf);
			}
		}
		c = c.inv();

		for (int i = 0; i < 48; i++) {
			d = conjugate(c, symInv[i % 16]);

			if (d == a)
			{
				sym |= 1LL << 48;
				break;

			}
			if (i % 16 == 15) {
				c = conjugate(c, s_urf);
			}
		}
		return sym;
	}

	void CubieTables::init_std2ud_ckmv()
	{
		// std2ud
		for (int i = 0; i < 18; ++i)
		{
			std2ud[i] = -1;
		}

		for (int i = 0; i < 10; i++)
		{
			std2ud[ud2std[i]] = i;
		}

		// ckmv2
		for (int i = 0; i < 10; i++) {
			int ix = ud2std[i];
			for (int j = 0; j < 10; j++) {
				int jx = ud2std[j];
				ckmv2[i][j] = (ix / 3 == jx / 3) || ((ix / 3 % 3 == jx / 3 % 3) && (ix >= jx));
			}
			ckmv2[10][i] = false;
		}
		
	}

	void CubieTables::initMove()
	{
		CubieCube eye = CubieCube::eye();

		// moveCube
		for (int i = 0; i < 18; ++i)
		{
			moveCube[i] = eye.move(i);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void CubieTables::initSym()
	{
		// cubSym
		CubieCube _sym[4] = { CubieCube::eye(), CubieTables::s_u4, CubieTables::s_u4 * CubieTables::s_u4, 
			CubieTables::s_u4 * CubieTables::s_u4 * CubieTables::s_u4 };

		cubeSym[0] = CubieCube::eye();
		cubeSym[1] = s_u4;
		cubeSym[2] = s_u4 * s_u4;
		cubeSym[3] = cubeSym[2] * s_u4;

		cubeSym[4] = s_lr2;
		cubeSym[5] = s_lr2 * cubeSym[1];
		cubeSym[6] = s_lr2 * cubeSym[2];
		cubeSym[7] = s_lr2 * cubeSym[3];

		cubeSym[8] = s_f2;
		cubeSym[9] = s_f2 * cubeSym[1];
		cubeSym[10] = s_f2 * cubeSym[2];
		cubeSym[11] = s_f2 * cubeSym[3];

		cubeSym[12] = s_f2 * s_lr2;
		cubeSym[13] = cubeSym[12] * cubeSym[1];
		cubeSym[14] = cubeSym[12] * cubeSym[2];
		cubeSym[15] = cubeSym[12] * cubeSym[3];


		// symInv, symMult
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				CubieCube cc = cubeSym[i] * cubeSym[j];

				for (int k = 0; k < 16; k++)
				{
					if (cc == cubeSym[k])
					{
						symMult[i][j] = k;
						if (k == 0)
						{
							symInv[i] = j;
						}
						break;
					}
				}

#ifdef _DEBUG
				corner_t corn = cubeSym[i].corner * cubeSym[j].corner;

				for (int k = 0; k < 16; k++) {
					if (corn == cubeSym[k].corner)
					{
						assert(k == symMult[i][j]);
					}
				}
#endif


			}

		}

		// symMove
		for (int j = 0; j < 18; j++)
		{
			for (int s = 0; s < 16; s++)
			{
				CubieCube cc = conjugate(moveCube[j], symInv[s]);
				for (int m = 0; m < 18; m++) {
					if (cc == moveCube[m])
					{
						symMove[s][j] = m;
						break;
					}
				}


#ifdef _DEBUG
				corner_t corn = conjugate(moveCube[j].corner, symInv[s]);
				for (int m = 0; m < 18; m++) {
					if (corn == moveCube[m].corner)
					{
						assert(symMove[s][j] == m);
					}
				}
#endif
			}
		}

		// SymMoveUD, symMultInv
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 10; j++) {
				symMoveUD[i][j] = std2ud[symMove[i][ud2std[j]]];
			}

			for (int j = 0; j < 16; j++) {
				symMultInv[i][j] = symMult[i][symInv[j]];
			}
		}


		// sym8Mult, sym8MultInv, sym8Move
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				sym8Mult[i][j] = symMult[i << 1][j << 1] >> 1;
				sym8MultInv[i][j] = symMult[i << 1][symInv[j << 1]] >> 1;
			}

			for (int j = 0; j < 18; j++)
			{
				sym8Move[i][j] = symMove[2*i][j];
			}
		}


		// moveCubeSym
		for (int i = 0; i < 18; i++) {
			moveCubeSym[i] = selfSymmetry(moveCube[i]);
		}


		// firstMoveSym
		for (int i = 0; i < 18; i++)
		{
			int j = i;
			for (int s = 0; s < 48; s++)
			{
				if (symMove[s % 16][j] < i)
				{
					firstMoveSym[s] |= 1 << i;
				}
				if (s % 16 == 15) {
					j = urfMove[2][j];
				}
			}
		}
	}

	void CubieTables::initFlipSym2Raw()
	{
		CubieCube c;
		CubieCube d;
		int count = 0;

		// 初始 flipR2S 为 0
		for (int i = 0; i < 2048; i++)
			flipR2S[i].val = 0;

		for (int i = 0; i < 2048; i++)
		{
			if (flipR2S[i].val != 0)
			{
				continue;
			}
			c.setFlip(i);
			for (int s = 0; s < 16; s += 2)
			{
				d.edge = conjugate(c.edge, s);
				int idx = d.getFlip();
				if (idx == i)
				{
					symStateFlip[count] |= 1 << (s >> 1);
				}
				flipR2S[idx].set(count, s >> 1);

 				flipS2RF[sym3_t(count, s >> 1).val] = (short)idx;

			}
			flipS2R[count++] = (short)i;
		}
		assert(count == 336);
	}

	void CubieTables::initTwistSym2Raw()
	{
		CubieCube c;
		CubieCube d;
		int count = 0;
		
		for (int i = 0; i < 2187; i++)
			twistR2S[i].val = 0;


		for (int i = 0; i < 2187; i++) {
			if (twistR2S[i].val != 0) {
				continue;
			}
			c.setTwist(i);
			for (int s = 0; s < 16; s += 2) {
				d.corner = conjugate(c.corner, s);				


				int idx = d.getTwist();
				if (idx == i) {
					symStateTwist[count] |= 1 << (s >> 1);
				}
				twistR2S[idx].set(count, s >> 1);

				twistS2RF[sym3_t(count, s >> 1).val] = (short)idx;
	
			}
			twistS2R[count++] = (short)i;
		}
		assert(count == 324);
	}

	void CubieTables::initPermSym2Raw()
	{
		CubieCube c;
		CubieCube d;

		int count = 0;

		for (int i = 0; i < 40320; i++)
			epermR2S[i].val = 0;

		for (int i = 0; i < 40320; i++) {
			if (epermR2S[i].val != 0) {
				continue;
			}
			c.setEPerm(i);
			for (int s = 0; s < 16; s++)
			{
				d.edge = conjugate(c.edge, s);
				int idx = d.getEPerm();
				if (idx == i) {
					symStatePerm[count] |= 1 << s;
				}
// 				int a = d.getU4Comb();
// 				int b = d.getD4Comb() >> 9;
// 				int m = 494 - (a & 0x1ff) + (a >> 9) * 70 + b * 1680;

				int m = 494 - d.getU4Comb() + d.getU4Perm() * 70 + d.getD4Perm() * 1680;

				epermR2S[idx].set(count, s);
				MtoEPerm[m] = epermR2S[idx];
				if (s == 0) {
					perm2comb[count] = (char)(494 - d.getU4Comb());
				}
			}
			epermS2R[count++] = (short)i;
		}
		assert(count == 2768);
	}

}