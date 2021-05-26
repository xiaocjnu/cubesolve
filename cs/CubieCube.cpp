#include "CubieCube.h"
#include "FaceCubeBase.h"
#include "CubieTables.h"
#include <sstream>
#include <assert.h>


using namespace std;

namespace cs
{
	const CubieCube CubieCube::move_table[9] =
	{
		CubieCube(15120,0,119750400,0),		// U
		CubieCube(21021,1494,323403417,0),	// R
		CubieCube(8064,1236,29441808,550),	// F
		CubieCube(9,0,5880,0),				// D
		CubieCube(1230,412,2949660,0),		// L
		CubieCube(224,137,328552,137),		// B
		CubieCube(23182,1906,340098425,680),// x
		CubieCube(15138,0,119765538,7),		// y
		CubieCube(8427,1373,110964579,2047)	// z
		
	};

	const int CubieCube::middle_table[9][3] =
	{
		// move1, move2, rot
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


	const int CubieCube::lower_table[18][2] =
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


	CubieTables CubieCube::tables;


	CubieCube::CubieCube()
	{
		corner = corner_t::eye();
		edge = edge_t::eye();
	}


	CubieCube::CubieCube(int cperm, int twist, int eperm, int flip)
	{
		corner.set_perm(cperm);
		corner.set_ori(twist);
		edge.set_perm(eperm);
		edge.set_ori(flip);
	}

	CubieCube::CubieCube(const initializer_list<initializer_list<int>>& lt)
	{
		int n = 0;
		
		auto it = lt.begin();
		corner.perm = (*it++);
		corner.ori = (*it++);
		edge.perm = (*it++);
		edge.ori = (*it++);

		assert(it == lt.end());
	}

	CubieCube::CubieCube(const ColorCube & fc)
	{
		int ori;
		Color col1, col2;


		for (int i = 0; i < 8; i++)
			corner.perm[i] = 0;// invalidate corners
		for (int i = 0; i < 12; i++)
			edge.perm[i] = 0;		// and edges

		for (int i = 0; i < 8; i++) {
			// get the colors of the cubie at corner i, starting with U/D
			for (ori = 0; ori < 3; ori++)
				if (fc[cornerFacelet[i][ori]] == U || fc[cornerFacelet[i][ori]] == D)
					break;

			col1 = (Color)fc[cornerFacelet[i][(ori + 1) % 3]];
			col2 = (Color)fc[cornerFacelet[i][(ori + 2) % 3]];

			for (int j = 0; j < 8; j++) {
				if (col1 == cornerColor[j][1] && col2 == cornerColor[j][2]) {
					// in cornerposition i we have cornercubie j
					corner.perm[i] = j;
					corner.ori[i] = ori % 3;
					break;
				}
			}
		}

		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 12; j++) {
				if (fc[edgeFacelet[i][0]] == edgeColor[j][0]
					&& fc[edgeFacelet[i][1]] == edgeColor[j][1]) {
					edge.perm[i] = j;
					edge.ori[i] = 0;
					break;
				}
				if (fc[edgeFacelet[i][0]] == edgeColor[j][1]
					&& fc[edgeFacelet[i][1]] == edgeColor[j][0]) {
					edge.perm[i] = j;
					edge.ori[i] = 1;
					break;
				}
			}
		}
	}

	CubieCube::~CubieCube()
	{
	}


	CubieCube CubieCube::randomScramble(int m)
	{
		CubieCube cc;

		for (int i = 0; i < m; ++i)
		{
			int mv = randInt(0, 17);
			cc = cc.move(mv);
		}

		return cc;
	}

	CubieCube CubieCube::randomCube()
	{
		CubieCube cc;
		while (true)
		{
			int cperm = randInt(0, 40320 - 1);
			int twist = randInt(0, 2187 - 1);
			int eperm = randInt(0, 479001600 - 1);
			int flip = randInt(0, 2048 - 1);


			cc = CubieCube(cperm, twist, eperm, flip);

			if (cc.verify() == 0)
			{
				break;
			}
		}


		return cc;
	}

	CubieCube CubieCube::mul(const CubieCube & b) const
	{
		CubieCube c;
		c.corner = corner * b.corner;
		c.edge = edge * b.edge;
		return c;
	}



	CubieCube CubieCube::inv() const
	{
		CubieCube b;
		b.corner = corner.inv();
		b.edge = edge.inv();
		return b;
	}

	const CubieCube& CubieCube::operator=(const CubieCube & src)
	{
		this->corner = src.corner;
		this->edge = src.edge;
		return *this;
	}

	int CubieCube::getFlip() const
	{
		return edge.ori.getIndex();
	}

	void CubieCube::setFlip(int idx)
	{
		edge.ori.setIndex(idx);
	}

	sym3_t CubieCube::getFlipSym() const
	{
		return tables.flipR2S[getFlip()];
	}

	void CubieCube::setFlipSym(sym3_t s)
	{
		int raw0 = tables.flipS2R[s.get_count()];
		setFlip(raw0);
		edge = tables.conjugate(edge, s.get_s() * 2);
	}

	int CubieCube::getTwist() const
	{
		return corner.ori.getIndex();
	}

	void CubieCube::setTwist(int idx)
	{
		corner.ori.setIndex(idx);
	}

	sym3_t CubieCube::getTwistSym() const
	{
		return tables.twistR2S[getTwist()];
	}

	void CubieCube::setTwistSym(sym3_t s)
	{
		int raw0 = tables.twistS2R[s.get_count()];
		setTwist(raw0);
		corner = tables.conjugate(corner, s.get_s() * 2);
	}

	int CubieCube::getUDSliceComb() const
	{
		combination<12, 4> comb;

		for (int i = 0; i<12; ++i)
		{
			comb[i] = (edge.perm[i] >= 8);  // 中间层
		}

		return comb.getIndex();
	}

	int CubieCube::getUDSlicePerm() const
	{
		return getMPerm();
	}

	void CubieCube::setUDSlice(int c, int p)
	{
		permutation<4> perm(p);
		combination<12, 4> comb(c);

		int slice = 0;
		int other = 0;

		for (int i = 0; i < 12; ++i)
		{
			if (comb[i])  // 是 UDSlice
			{
				edge.perm[i] = perm[slice++] + 8;
			}
			else // others
			{
				edge.perm[i] = other++;
			}
		}

		assert(slice == 4);
		assert(other == 8);

	}

// 	int CubieCube::getUDSlicePerm()
// 	{
// 		combination<12, 4> comb;
// 		permutation<4> perm;
// 
// 		int n = 0;
// 		for (int i = 0; i < 12; ++i)
// 		{
// 			if (edge.perm[i] >= 8)  // 中间层
// 			{
// 				comb[i] = true;
// 				perm[n++] = edge.perm[i] - 8;
// 			}
// 			else
// 			{
// 				comb[i] = false;
// 			}
// 		}
// 
// 		assert(n == 4);
// 
// 		int idx1 = comb.getIndex();
// 		int idx2 = perm.getIndex();
// 
// 		return 24 * idx1 + idx2;
// 
// 	}
// 
// 	void CubieCube::setUDSlicePerm(int idx)
// 	{
// 		int idx2 = idx % 24;
// 		int idx1 = idx / 24;
// 
// 		combination<12, 4> comb(idx1);
// 		array<int, 4> number = comb.toNumber();
// 		permutation<4> perm(idx2);
// 
// 		array<int, 4> slice;
// 
// 		for (int i = 0; i < 4; ++i)
// 		{
// 			slice[i] = perm[i] + 8;
// 		}
// 
// 		int n = 0;
// 		int other = 0;
// 		for (int i = 0; i < 12; ++i)
// 		{
// 			if (comb[i])  // UDSlice
// 			{
// 				edge.perm[i] = slice[n++];
// 			}
// 			else
// 			{
// 				edge.perm[i] = other++;
// 			}
// 		}
// 
// 		assert(n == 4);
// 		assert(other == 8);
// 	}

	int CubieCube::getU4Comb()
	{
		combination<12, 4> comb;

		for (int i = 0; i < 12; ++i)
		{
			comb[i] = edge.perm[i] < 4;
		}

		return comb.getIndex();
	}

	int CubieCube::getD4Comb()
	{
		combination<12, 4> comb;

		for (int i = 0; i < 12; ++i)
		{
			comb[i] = edge.perm[i] >= 4 && edge.perm[i] < 8;
		}

		return comb.getIndex();
	}

	int CubieCube::getU4Perm()
	{
		permutation<4> perm;

		int n = 0;
		for (int i = 0; i < 12; ++i)
		{
			if (edge.perm[i] < 4)
			{
				perm[n++] = edge.perm[i];
			}
		}

		return perm.getIndex();
	}

	int CubieCube::getD4Perm()
	{
		permutation<4> perm;

		int n = 0;
		for (int i = 0; i < 12; ++i)
		{
			if (edge.perm[i] >= 4 && edge.perm[i] < 8)
			{
				perm[n++] = edge.perm[i] - 4;
			}
		}

		return perm.getIndex();
	}



	int CubieCube::getCPerm() const
	{
		return corner.perm.getIndex();
	}

	void CubieCube::setCPerm(int idx)
	{
		corner.perm.setIndex(idx);
	}

	sym4_t CubieCube::getCPermSym() const
	{
		sym4_t idx = tables.epermR2S[getCPerm()];

		int es = idx.get_s();
		int cs = tables.s_e2c[es];
		idx.set_s(cs);
		return idx;
	}

	void CubieCube::setCPermSym(sym4_t s)
	{
		sym4_t se = s;
		se.set_s(tables.s_c2e[s.get_s()]);

		CubieCube cc;
		cc.setEPermSym(se);
		int raw = cc.getEPerm();
		setCPerm(raw);

	}

	int CubieCube::getEPerm() const
	{
		// UD Edge
		permutation<8> perm;
		copy(edge.perm.begin(), edge.perm.begin() + 8, perm.begin());
		
		if (!perm.verify())
			return -1;

		return perm.getIndex();
	}

	void CubieCube::setEPerm(int idx)
	{
		// UD Edge
		permutation<8> perm(idx);
		copy(perm.begin(), perm.end(), edge.perm.begin());
	}

	sym4_t CubieCube::getEPermSym() const
	{
		int raw = getEPerm();

		if (raw >= 0)
			return tables.epermR2S[getEPerm()];
		else
			return sym4_t(-1, -1);
	}

	void CubieCube::setEPermSym(sym4_t s)
	{
		int raw0 = tables.epermS2R[s.get_count()];
		setEPerm(raw0);
		edge = tables.conjugate(edge, s.get_s());
	}

	int CubieCube::getMPerm() const
	{
		permutation<4> perm;

		int n = 0;
		for (int i = 0; i < 12; ++i)
		{
			if (edge.perm[i] >= 8)
			{
				perm[n++] = edge.perm[i] - 8;
			}
		}

		return perm.getIndex();

	}

	void CubieCube::setMPerm(int idx)
	{
		permutation<4> perm(idx);

		int n = 0;
		for (int i = 0; i < 4; ++i)
		{
			edge.perm[i + 8] = perm[i] + 8;
		}
	}

	int CubieCube::getCComb() const
	{
		combination<8, 4> comb;

		for (int i = 0; i < 8; ++i)
		{
			comb[i] = corner.perm[i] < 4;
		}

		return 69 - comb.getIndex();
	}

	void CubieCube::setCComb(int idx)
	{
		combination<8, 4> comb(69 - idx);

		int ud = 0;
		int other = 4;


		for (int i = 0; i < 8; ++i)
		{
			if (comb[i])
				corner.perm[i] = ud++;
			else
				corner.perm[i] = other++;
		}

		assert(ud == 4);
		assert(other == 8);
	}

	string CubieCube::toString() const
	{
		stringstream ss;


		ss << "{{";
		for (int i = 0; i < 7; i++)
		{
			ss << corner.perm[i] << ",";
		}
		ss << corner.perm[7] << "},\n{";

		for (int i = 0; i < 7; i++)
		{
			ss << corner.ori[i].get() << ",";
		}
		ss << corner.ori[7].get() << "},\n{";


		for (int i = 0; i < 11; i++)
		{
			ss << edge.perm[i] << ",";
		}

		ss << edge.perm[11] << "},\n{";

		for (int i = 0; i < 11; i++)
		{
			ss << edge.ori[i].get() << ",";
		}

		ss << edge.ori[11].get() << "}}\n";

		return ss.str();

	}

	string CubieCube::toString2() const
	{
		stringstream ss;

		for (int i = 0; i < 8; i++)
		{
			ss << "|" << corner.perm[i] << " " << corner.ori[i].get();
		}
		ss << "\n";


		for (int i = 0; i < 12; i++)
		{
			ss << "|" << edge.perm[i] << " " << edge.ori[i].get();
		}

		ss << "\n";

		return ss.str();
	}

	string CubieCube::idxtoString() const
	{
		stringstream ss;
		ss << corner.perm.getIndex() << ","
			<< corner.ori.getIndex() << ","
			<< edge.perm.getIndex() << ","
			<< edge.ori.getIndex();
		return ss.str();
	}

	FaceCube CubieCube::toFaceCube() const
	{
		FaceCube fc;

		// 角块
		for (int i = 0; i < 8; ++i)
		{
			int p = corner.perm[i];
			int o = corner.ori[i].get();

			for (int j = 0; j < 3; ++j)
			{
				int oo = (j - o) % 3;
				oo = oo < 0 ? oo + 3 : oo;
				fc[cornerFacelet[i][j]] = cornerFacelet[p][oo];
			}
		}
		
		// 棱块
		for (int i = 0; i < 12; ++i)
		{
			int p = edge.perm[i];
			int o = edge.ori[i].get();

			for (int j = 0; j < 2; ++j)
			{
				int oo = (j - o) % 2;
				oo = oo < 0 ? oo + 2 : oo;
				fc[edgeFacelet[i][j]] = edgeFacelet[p][oo];
			}
		}

		return fc;
	}

	ColorCube CubieCube::toColorCube() const
	{
		FaceCube fc = toFaceCube();
		return fc.toColorCube();
	}


	CubieCube CubieCube::_move(Move mv) const
	{
		CubieCube ret = (*this);

		int ax = mv / 3;
		int po = 1 + (mv % 3);


		for (int k = 0; k < po; ++k)
		{
			ret = ret * move_table[ax];
		}

		return ret;
	}


	CubieCube CubieCube::move(Move mv) const
	{
		mv = (Move)(mv & 0xff);

		CubieCube dd = *this;
		if (Ux1 <= mv && mv <= Rot_z3) // 基本公式和整体转动
		{
			dd = dd._move(mv);
		}
		else if (Ex1 <= mv && mv <= Sx3)	// 中间层
		{
			int idx = mv - Ex1;
			dd = dd.move(middle_table[idx][0]);
			dd = dd.move(middle_table[idx][1]);
			dd = dd.move(middle_table[idx][2]);
		}
		else if (LowerUx1 <= mv && mv <= LowerBx3)	// 转两层
		{
			int idx = mv - LowerUx1;
			dd = dd.move(lower_table[idx][0]);
			dd = dd.move(lower_table[idx][1]);
		}

		return dd;
	}

	CubieCube CubieCube::move(vector<Move> mv) const
	{
		CubieCube dd = *this;

		for (size_t i = 0; i < mv.size(); ++i)
		{
			dd = dd.move(mv[i]);
		}

		return dd;
	}


	/**
	* Check a cubiecube for solvability. Return the error code.
	* 0: Cube is solvable
	* -2: Not all 12 edges exist exactly once
	* -3: Flip error: One edge has to be flipped
	* -4: Not all corners exist exactly once
	* -5: Twist error: One corner has to be twisted
	* -6: Parity error: Two corners or two edges have to be exchanged
	*/
	int CubieCube::verify() const
	{
		int sum = 0;
		int edgeMask = 0;
		for (int e = 0; e < 12; e++) {
			edgeMask |= 1 << (edge.perm[e]);
			sum ^= edge.ori[e].get();
		}
		if (edgeMask != 0xfff) {
			return -2; // missing edges
		}
		if (sum != 0) {
			return -3; // Flip error
		}

		int cornMask = 0;
		sum = 0;
		for (int c = 0; c < 8; c++) {
			cornMask |= 1 << corner.perm[c];
			sum += corner.ori[c].get();
		}
		if (cornMask != 0xff) {
			return -4;// missing corners
		}
		if (sum % 3 != 0) {
			return -5;// twisted corner
		}
		if ((getNParity(edge.perm.getIndex(), 12) ^ getNParity(getCPerm(), 8)) != 0)
		{
			return -6;// parity error
		}
		return 0;// cube ok
	}

}

