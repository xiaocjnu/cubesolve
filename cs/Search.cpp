#include "Search.h"
#include "CubieCube.h"
#include "FaceCubeBase.h"
#include "AxisCube.h"
#include <sstream>

using namespace std;

namespace cs
{

	const int MAX_DEPTH2 = 12;
	const int MAX_DEPTH2_5AXIS = 14;


	static CoordCubeTables& _coTbl = CoordCube::tables;
	static CubieTables& _cuTbl = CubieCube::tables;

	Search::Search()
	{
	}


	void Search::init()
	{
		AxisCube::init();
		CoordCube::init();
	}

	std::string Search::solutionToString() const
	{
		std::stringstream sb;


		if (m_error < 0)
		{
			sb << "Error " << -m_error;
			return sb.str();
		}

		for (int s = 0; s < m_sol; s++) {
			if (use_separator && s == m_depth1) {
				sb << ".  ";
			}
			sb << move2str[m_moves[s]] << " ";
		}

		if (append_length) {
			sb << "(" << m_sol << "f)";
		}

		return sb.str();
	}



	string Search::solve(string facelets)
	{
		ColorCube fc(facelets);
		CubieCube cc(fc);

		solve(cc);
		return solutionToString();
	}

	string Search::solve(string facelets_src, string facelets_dst)
	{
		ColorCube fc_cc = ColorCube(facelets_src);
		ColorCube fc_dd = ColorCube(facelets_dst);
		CubieCube cc(fc_cc);
		CubieCube dd(fc_dd);

		this->solve(cc, dd);
		return solutionToString();
	}


	vector<Move> Search::solve(CubieCube cc)
	{
		int maxDepth = use_5axis ? 25 : 22;

		return solve(cc, maxDepth);
	}

	vector<Move> Search::solve(CubieCube src, CubieCube dst)
	{
		return solve(dst.inv() * src);
	}



	vector<Move> Search::solve(CubieCube cc, int maxDepth, int probeMax, int probeMin)
	{

		m_error =cc.verify();
		if (m_error != 0)
		{
			m_moves.clear();
			return m_moves;
		}

		m_cc = cc;
		m_moves.resize(maxDepth);
		m_sol = maxDepth;
		m_probe = 0;
		m_probeMax = probeMax;
		m_probeMin = min(probeMin, probeMax);
		//m_solution = "";
		m_isRec = false;

		init();
		search();
		
		return m_moves;
	}


	string Search::next(int probeMax, int probeMin)
	{
		m_probe = 0;
		m_probeMax = probeMax;
		m_probeMin = min(probeMin, probeMax);
		m_moves.resize(31);
		//this->m_solution = "";
		m_isRec = true;

		search();

		return solutionToString();
		//		return (verbose & OPTIMAL_SOLUTION) == 0 ? search() : searchopt();
	}

	void Search::search()
	{

		phase1_fcnt = 0;
		phase2_fcnt = 0;
		phase1_steps = 0;
		phase2_steps = 0;
		m_found = false;

		m_selfSym = _cuTbl.selfSymmetry(m_cc);
		m_selfSym = m_selfSym & 0xffffffffffffL;	// 48bit

		CoordCube node(m_cc);

		for (m_length1 = m_isRec ? m_length1 : 0; m_length1 <= m_sol; m_length1++)
		{
			m_maxDep2 = min(use_5axis ? MAX_DEPTH2_5AXIS : MAX_DEPTH2, m_sol - m_length1 - (m_isRec ? 1 : 0));

			int prun = node.calcPrunPhase1();
			int ssym = (int)(0xffff & m_selfSym);	// 只考虑16种

			m_depth1 = m_length1;

			if (prun <= m_depth1
				&& phase1(node, ssym, m_depth1, -1) == 0)
			{
				phase1_steps = m_depth1;
				phase2_steps = m_sol - m_depth1;

				if (m_found)
				{
					m_error = 0;
					m_moves.resize(m_sol);
				}
				else
				{
					m_moves.clear();
					m_error = -8;
				}
				return;
			}
		}

		assert(!m_found);

		m_moves.clear();
		m_error = -7;

// 		if (m_found)
// 		{
// 			m_error = 0;
// 			m_moves.resize(m_sol);
// 		}
// 		else
// 		{
// 			m_moves.clear();
// 			m_error = -7;
// 		}
	}

	/**
	* @return
	*      0: Found or Probe limit exceeded
	*      1: Try Next Power
	*      2: Try Next Axis
	*/
	int Search::phase1(CoordCube node, int ssym, int maxl, int lm)
	{
		++phase1_fcnt;


		if (node.prun == 0 && maxl < 5)
		{
			if (maxl == 0)
			{
				int ret = initPhase2();
				return ret;
			}
			else {
				return 1;
			}
		}


		// 对称忽略
		int skipMoves = 0;
		int i = 1;
		for (long s = ssym; (s >>= 1) != 0; i++) {
			if ((s & 1) == 1) {
				skipMoves |= _cuTbl.firstMoveSym[i];
			}
		}


		for (int axis = 0; axis < (use_5axis ? 15 : 18); axis += 3)
		{
			// 轴相等或轴相对逆序忽略， 相对轴的旋转必须是顺序
			if (axis == lm || axis == lm - 9
				|| (m_isRec && axis < m_moves[m_depth1 - maxl] - 2))
			{
				continue;
			}
			for (int power = 0; power < 3; power++) {
				int m = axis + power;


				// 对称忽略
				if (m_isRec && m != m_moves[m_depth1 - maxl]
					|| ssym != 1 && (skipMoves & 1 << m) != 0) {
					continue;
				}

				CoordCube nodex = node.movePhase1(m);
				int prun = nodex.calcPrunPhase1();



				// prun 必须满足 prun <= maxl - 1
				// 同一个 axis 的 prun 最多相差 1, 此时 prun-1 也不能满足，因此跳到下一个 axis
				if (prun > maxl) {
					break;
				}
				else if (prun == maxl)
				{
					continue;
				}

				m_moves[m_depth1 - maxl] = (Move)m;
				int ret = phase1(nodex, ssym & _cuTbl.moveCubeSym[m], maxl - 1, axis);
				if (ret == 0) {
					return 0;
				}
				else if (ret == 2) {
					break;
				}
			}
		}
		return 1;
	}

	// 	string Search2::searchopt()
	// 	{
	// 		int maxprun1 = 0;
	// 		int maxprun2 = 0;
	// 		for (int i = 0; i < 6; i++) {
	// 			node0[i][0].calcPruning(false);
	// 			if (i < 3) {
	// 				maxprun1 = Math.max(maxprun1, node0[i][0].prun);
	// 			}
	// 			else {
	// 				maxprun2 = Math.max(maxprun2, node0[i][0].prun);
	// 			}
	// 		}
	// 		urfIdx = maxprun2 > maxprun1 ? 3 : 0;
	// 		preIdx = 0;
	// 		for (length1 = isRec ? length1 : 0; length1 < sol; length1++) {
	// 			CoordCube ud = node0[0 + urfIdx][0];
	// 			CoordCube rl = node0[1 + urfIdx][0];
	// 			CoordCube fb = node0[2 + urfIdx][0];
	// 
	// 			if (ud.prun <= length1 && rl.prun <= length1 && fb.prun <= length1
	// 				&& phase1opt(ud, rl, fb, selfSym, length1, -1) == 0) {
	// 				return solution == null ? "Error 8" : solution;
	// 			}
	// 		}
	// 		return solution == null ? "Error 7" : solution;
	// 	}

	/**
	* @return
	*      0: Found or Probe limit exceeded
	*      1: Try Next Power
	*      2: Try Next Axis
	*/
	// 	int Search2::phase1opt(CoordCube ud, CoordCube rl, CoordCube fb, long ssym, int maxl, int lm)
	// 	{
	// 		if (ud.prun == 0 && rl.prun == 0 && fb.prun == 0 && maxl < 5) {
	// 			maxDep2 = maxl + 1;
	// 			depth1 = length1 - maxl;
	// 			return initPhase2() == 0 ? 0 : 1;
	// 		}
	// 
	// 		int skipMoves = 0;
	// 		int i = 1;
	// 		for (long s = ssym; (s >>= 1) != 0; i++) {
	// 			if ((s & 1) == 1) {
	// 				skipMoves |= CubieCube.firstMoveSym[i];
	// 			}
	// 		}
	// 
	// 		for (int axis = 0; axis < 18; axis += 3) {
	// 			if (axis == lm || axis == lm - 9 || (isRec && axis < move[length1 - maxl] - 2)) {
	// 				continue;
	// 			}
	// 			for (int power = 0; power < 3; power++) {
	// 				int m = axis + power;
	// 
	// 				if (isRec && m != move[length1 - maxl]
	// 					|| ssym != 1 && (skipMoves & 1 << m) != 0) {
	// 					continue;
	// 				}
	// 
	// 				// UD Axis
	// 				int prun_ud = nodeUD[maxl].doMovePrun(ud, m, false);
	// 				if (prun_ud > maxl) {
	// 					break;
	// 				}
	// 				else if (prun_ud == maxl) {
	// 					continue;
	// 				}
	// 
	// 				// RL Axis
	// 				m = CubieCube.urfMove[2][m];
	// 
	// 				int prun_rl = nodeRL[maxl].doMovePrun(rl, m, false);
	// 				if (prun_rl > maxl) {
	// 					break;
	// 				}
	// 				else if (prun_rl == maxl) {
	// 					continue;
	// 				}
	// 
	// 				// FB Axis
	// 				m = CubieCube.urfMove[2][m];
	// 
	// 				int prun_fb = nodeFB[maxl].doMovePrun(fb, m, false);
	// 				if (prun_ud == prun_rl && prun_rl == prun_fb && prun_fb != 0) {
	// 					prun_fb++;
	// 				}
	// 
	// 				if (prun_fb > maxl) {
	// 					break;
	// 				}
	// 				else if (prun_fb == maxl) {
	// 					continue;
	// 				}
	// 
	// 				m = CubieCube.urfMove[2][m];
	// 
	// 				move[length1 - maxl] = m;
	// 				int ret = phase1opt(nodeUD[maxl], nodeRL[maxl], nodeFB[maxl], ssym & CubieCube.moveCubeSym[m], maxl - 1, axis);
	// 				if (ret == 0) {
	// 					return 0;
	// 				}
	// 				else if (ret == 2) {
	// 					break;
	// 				}
	// 			}
	// 		}
	// 		return 1;
	// 	}

	/**
	* @return
	*      0: Found or Probe limit exceeded
	*      1: Try Next Power
	*      2: Try Next Axis
	*/
	int Search::initPhase2()
	{

		m_isRec = false;
		if (m_probe >= (!m_found ? m_probeMax : m_probeMin))
		{
			return 0;
		}
		++m_probe;

		CubieCube dd = m_cc;
		for (int i = 0; i < m_depth1; i++)
		{
			int m = m_moves[i];
			dd = dd.move(m);
		}

		CoordCube node(dd);
		int prun = node.calcPrunPhase2();

		// prun 必须满足 prun <= m_maxDep2
		if (prun >= m_maxDep2 + 1) {
			return prun > m_maxDep2 + 1 ? 2 : 1;
		}

		int lm = 10;
		if (m_depth1 >= 2 && m_moves[m_depth1 - 1] / 3 % 3 == m_moves[m_depth1 - 2] / 3 % 3) {
			lm = _cuTbl.std2ud[max(m_moves[m_depth1 - 1], m_moves[m_depth1 - 2]) / 3 * 3 + 1];
		}
		else if (m_depth1 >= 1) {
			lm = _cuTbl.std2ud[m_moves[m_depth1 - 1] / 3 * 3 + 1];
			if (m_moves[m_depth1 - 1] > Fx3) {
				lm = -lm;
			}
		}

		int depth2;
		for (depth2 = m_maxDep2/* - 1*/; depth2 >= prun; depth2--) {
			int ret = phase2(node, depth2, m_depth1, lm);
			if (ret < 0) {
				break;
			}


			depth2 = depth2 - ret;
			m_sol = m_depth1 + depth2;
			m_found = true;
		}

		if (depth2 != m_maxDep2/* - 1*/) { //At least one solution has been found.
			m_maxDep2 = min(MAX_DEPTH2, m_sol - m_length1);
			return m_probe >= m_probeMin ? 0 : 1;
		}
		else {
			return 1;
		}
	}

	//-1: no solution found
	// X: solution with X moves shorter than expectation. Hence, the length of the solution is  depth - X
	int Search::phase2(CoordCube node, int maxl, int depth, int lm)
	{
		++phase2_fcnt;

		if (node.prun == 0) {
			return maxl;
		}
		for (int m = 0; m < (use_5axis ? 9 : 10); m++)
		{

			// 同轴忽略，相对立的面智能升序搜索，不能逆序
			// lm < 0 只用于第一次，当phase1 最后一步是 URF 时，允许phase2 第一步是相对立的逆序(DL)B
			if (lm < 0 ? (m == -lm) : _cuTbl.ckmv2[lm][m])
			{
				continue;
			}

			CoordCube nodex = node.movePhase2(m);
			int prun = nodex.calcPrunPhase2();


			// prun 必须满足 prun <= max  - 1
			if (prun >= maxl)
				continue;

			int ret = phase2(nodex, maxl - 1, depth + 1, (lm < 0 && m + lm == -5) ? -lm : m);
			if (ret >= 0) {
				m_moves[depth] = (Move)_cuTbl.ud2std[m];
				return ret;
			}
		}
		return -1;
	}

}