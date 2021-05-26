#include "HumanSolver.h"
#include "CubieCube.h"
#include "FaceCubeBase.h"
#include "AxisCube.h"
#include "Search.h"
#include <random>
#include <vector>

using namespace std;

namespace cs
{
	void HumanSolver::setCube(const CubieCube & cc)
	{
		myCubieCubie = cc;
	}

	bool HumanSolver::isDCrossSolved(const CubieCube & cc)
	{
		bool is_solved = true;

		const edge_t& edge = cc.edge;
		for (int i = 4; i < 8; ++i)
		{
			if (!(edge.perm[i] == i && edge.ori[i] == 0))
			{
				is_solved = false;
				break;
			}
		}

		return is_solved;

	}
	bool HumanSolver::isOnlyDCornerSolved(const CubieCube & cc)
	{
		bool is_solved = true;
		const corner_t& corner = cc.corner;
		for (int i = 4; i < 8; ++i)
		{
			if (!(corner.perm[i] == i && corner.ori[i] == 0))
			{
				is_solved = false;
				break;
			}
		}

		return is_solved;
	}
	bool HumanSolver::isDLayerSolved(const CubieCube & cc)
	{
		return isDCrossSolved(cc) && isOnlyDCornerSolved(cc);
	}

	bool HumanSolver::isF2LSolved(const CubieCube & cc)
	{
		bool is_solved = isDLayerSolved(cc);

		if (!is_solved)
			return false;

		const edge_t& edge = cc.edge;
		for (int i = 8; i < 12; ++i)
		{
			if (!(edge.perm[i] == i && edge.ori[i] == 0))
			{
				is_solved = false;
				break;
			}
		}

		return is_solved;
	}

	int HumanSolver::numPairF2LIgnoreDCross(const CubieCube & cc)
	{
		const edge_t& edge = cc.edge;
		const corner_t& corner = cc.corner;
		int cnt = 0;


		for (int i = 0; i < 4; ++i)
		{
			int ie = 8 + i;
			int ic = 4 + i;

			if (edge.perm[ie] == ie && edge.ori[ie] == 0 &&
				corner.perm[ic] == ic && corner.ori[ic] == 0)
			{
				++cnt;
			}
		}


		return cnt;
	}

	int HumanSolver::numPairF2LSolved(const CubieCube & cc)
	{
		bool is_solved = isDCrossSolved(cc);

		if (!is_solved)
			return 0;

		return numPairF2LIgnoreDCross(cc);
	}

	bool HumanSolver::isOnlyUCrossSolved(const CubieCube & cc)
	{
		bool is_solved = true;

		const edge_t& edge = cc.edge;
		for (int i = 0; i < 4; ++i)
		{
			if (!(edge.ori[i] == 0))
			{
				is_solved = false;
				break;
			}
		}

		return is_solved;
	}

	bool HumanSolver::isUCrossSolved(const CubieCube & cc)
	{
		return isF2LSolved(cc) && isOnlyUCrossSolved(cc);
	}

	bool HumanSolver::isOnlyUCornerOriSolved(const CubieCube & cc)
	{
		bool is_solved = true;

		const corner_t& corner = cc.corner;
		for (int i = 0; i < 4; ++i)
		{
			if (!(corner.ori[i] == 0))
			{
				is_solved = false;
				break;
			}
		}

		return is_solved;
	}

	bool HumanSolver::isOnlyUEdgeOriSolved(const CubieCube & cc)
	{
		bool is_solved = true;

		const edge_t& edge = cc.edge;
		for (int i = 0; i < 4; ++i)
		{
			if (!(edge.ori[i] == 0))
			{
				is_solved = false;
				break;
			}
		}

		return is_solved;
	}

	bool HumanSolver::isOLLSolved(const CubieCube & cc)
	{
		return isUCrossSolved(cc) && isOnlyUCornerOriSolved(cc);

	}

	bool HumanSolver::isOnlyUCornerPermSolved(const CubieCube & cc)
	{
		bool is_solved = true;


		const corner_t& corner = cc.corner;
		for (int i = 0; i < 4; ++i)
		{
			if (!(corner.perm[i] == i))
			{
				is_solved = false;
				break;
			}
		}

		return is_solved;
	}

	bool HumanSolver::isUCornerSolved(const CubieCube & cc)
	{
		return isOLLSolved(cc) && isOnlyUCornerPermSolved(cc);
	}

	bool HumanSolver::isOnlyUEdgePermSolved(const CubieCube & cc)
	{
		bool is_solved = true;

		const edge_t& edge = cc.edge;
		for (int i = 0; i < 4; ++i)
		{
			if (!(edge.perm[i] == i))
			{
				is_solved = false;
				break;
			}
		}

		return is_solved;
	}

	bool HumanSolver::isSolved(const CubieCube & cc)
	{
		return cc.iseye();
	}

	bool HumanSolver::isDCrossSolved(const AxisCubieCube & cc)
	{
		vector<Move> ff = cc.axis.inv().getGenFormula();
		CubieCube ss = CubieCube::eye().move(ff);
		return isDCrossSolved(ss * cc.cubie);
	}

	bool HumanSolver::isDLayerSolved(const AxisCubieCube & cc)
	{
		vector<Move> ff = cc.axis.inv().getGenFormula();
		CubieCube ss = CubieCube::eye().move(ff);
		return isDLayerSolved(ss * cc.cubie);
	}

	bool HumanSolver::isF2LSolved(const AxisCubieCube & cc)
	{
		vector<Move> ff = cc.axis.inv().getGenFormula();
		CubieCube ss = CubieCube::eye().move(ff);
		return isF2LSolved(ss * cc.cubie);
	}

	int HumanSolver::numPairF2LSolved(const AxisCubieCube & cc)
	{
		vector<Move> ff = cc.axis.inv().getGenFormula();
		CubieCube ss = CubieCube::eye().move(ff);
		return numPairF2LSolved(ss * cc.cubie);
	}

	bool HumanSolver::isUCrossSolved(const AxisCubieCube & cc)
	{
		vector<Move> ff = cc.axis.inv().getGenFormula();
		CubieCube ss = CubieCube::eye().move(ff);
		return isUCrossSolved(ss * cc.cubie);
	}

	bool HumanSolver::isOLLSolved(const AxisCubieCube & cc)
	{
		vector<Move> ff = cc.axis.inv().getGenFormula();
		CubieCube ss = CubieCube::eye().move(ff);
		return isOLLSolved(ss * cc.cubie);
	}

	bool HumanSolver::isSolved(const AxisCubieCube & cc)
	{
		return cc.isSolved();
	}

	bool HumanSolver::isUCornerSolved(const AxisCubieCube & cc)
	{
		vector<Move> ff = cc.axis.inv().getGenFormula();
		CubieCube ss = CubieCube::eye().move(ff);
		return isUCornerSolved(ss * cc.cubie);
	}

	void HumanSolver::formula_append(std::vector<Move>& moves, Move m)
	{
		moves.push_back(m);
	}

	void HumanSolver::formula_append(std::vector<Move>& moves, const std::vector<Move>& mm)
	{
		for (Move m : mm)
		{
			formula_append(moves, m);
		}
	}

	void HumanSolver::formula_append_merge(std::vector<Move>& moves, Move m)
	{

#ifdef _DEBUG
		std::vector<Move> old_moves = moves;
		formula_append(old_moves, m);
#endif

		if (moves.empty())
		{
			moves.push_back(m);
			return;
		}


		int last_m = moves[moves.size() - 1];

		int cur_ax = m / 3;
		int cur_po = 1 + m % 3;

		int last_ax = last_m / 3;
		int last_po = 1 + last_m % 3;


		if (last_ax == cur_ax)
		{
			int new_po = last_po + cur_po;
			new_po = new_po >= 4 ? new_po - 4 : new_po;

			if (new_po == 0)
				moves.pop_back();
			else
				moves[moves.size() - 1] = Move(last_ax * 3 + new_po - 1);
		}
		else
		{
			moves.push_back(m);
		}


#ifdef _DEBUG
		assert(AxisCubieCube::eye().move(old_moves) == AxisCubieCube::eye().move(moves));
#endif
	}

	void HumanSolver::formula_append_merge(std::vector<Move>& moves, const std::vector<Move>& mm)
	{
		for (Move m : mm)
		{
			formula_append_merge(moves, m);
		}
	}

	void HumanSolver::formula_append_merge_rot(std::vector<Move>& moves, Move m)
	{
		if (m <= Rot_x1 && m <= Rot_z3)
			formula_append_merge(moves, m);
		else
			formula_append(moves, m);
	}

	void HumanSolver::formula_append_merge_rot(std::vector<Move>& moves, const std::vector<Move>& mm)
	{
		for (Move m : mm)
		{
			formula_append_merge_rot(moves, m);
		}
	}




	std::vector<Move> HumanSolver::mergeRepetedFormula(const std::vector<Move>& moves)
	{
		std::vector<Move> moves2;
		formula_append_merge(moves2, moves);
		return moves2;
	}

	std::vector<Move> HumanSolver::mergeRepetedRotateFormula(const std::vector<Move>& moves)
	{
		std::vector<Move> moves2;
		formula_append_merge_rot(moves2, moves);
		return moves2;
	}

	//////////////////////////////////////////////////////////////////////////
	bool HumanSolverLBLSearch::inited = false;

	int HumanSolverLBLSearch::moveConjY2[18];

	int HumanSolverLBLSearch::DCrossMove[528][18];
	int HumanSolverLBLSearch::DCrossPrun[528];

	int HumanSolverLBLSearch::DLayerMove[504][18];
	int HumanSolverLBLSearch::DLayerPrun[504];

	void HumanSolverLBLSearch::init()
	{
		if (inited)
			return;

		init_moveConjY2();

		// DCrossMove and Prun
		init_MoveTable(528, (int*)DCrossMove, toDCrossIdx, fromDCrossIdx);
		init_PrunTable(528, DCrossPrun, toDCrossIdx, fromDCrossIdx);


		// DLayerMove and Prun
		init_MoveTable(504, (int*)DLayerMove, toDLayerIdx, fromDLayerIdx);
		init_PrunTable(504, DLayerPrun, toDLayerIdx, fromDLayerIdx);

		inited = true;
	}

	int HumanSolverLBLSearch::toDCrossIdx(const CubieCube & cc)
	{
		return toTwoEdgeIdx(4, cc);
	}

	CubieCube HumanSolverLBLSearch::fromDCrossIdx(int idx)
	{
		return fromTwoEdgeIdx(4, idx);
	}

	int HumanSolverLBLSearch::toDLayerIdx(const CubieCube & cc)
	{
		return toTwoCornerIdx(4, cc);
	}

	CubieCube HumanSolverLBLSearch::fromDLayerIdx(int idx)
	{
		return fromTwoCornerIdx(4, idx);
	}

	int HumanSolverLBLSearch::toTwoEdgeIdx(int edgeNo, const CubieCube & cc)
	{
		const edge_t& edge = cc.edge;

		int edgeNo_b = edgeNo + 1;
		if (edgeNo_b >= 12)
			edgeNo_b = edgeNo_b - 1;

		int perm_a = -1;
		int perm_b = -1;
		int ori_a = -1;
		int ori_b = -1;
		for (int i = 0; i < 12; ++i)
		{
			if (edge.perm[i] == edgeNo)
			{
				perm_a = i;
				ori_a = edge.ori[i].get();
			}


			if (edge.perm[i] == edgeNo_b)
			{
				perm_b = i;
				ori_b = edge.ori[i].get();
			}
		}

		assert(perm_a >= 0);
		assert(perm_b >= 0);
		assert(ori_a >= 0);
		assert(ori_b >= 0);


		perm_a = perm_a - edgeNo;
		if (perm_a < 0)
			perm_a = perm_a + 12;

		perm_b = perm_b - edgeNo;
		if (perm_b < 0)
			perm_b = perm_b + 12;

		permutation_Ank<12, 2> perm{ perm_a, perm_b };
		int idx1 = perm.getIndex();

		int idx2 = ori_b * 2 + ori_a;

		int idx = idx1 * 4 + idx2;

		return idx;
	}

	CubieCube HumanSolverLBLSearch::fromTwoEdgeIdx(int edgeNo, int idx)
	{
		int edgeNo_b = edgeNo + 1;
		if (edgeNo_b >= 12)
			edgeNo_b = edgeNo_b - 1;

		int idx1 = idx / 4;
		int idx2 = idx % 4;


		permutation_Ank<12, 2> perm(idx1);

		int ori_a = idx2 % 2;
		int ori_b = idx2 / 2;

		int perm_a = perm[0];
		int perm_b = perm[1];


		perm_a = perm_a + edgeNo;
		if (perm_a >= 12)
			perm_a = perm_a - 12;

		perm_b = perm_b + edgeNo;
		if (perm_b >= 12)
			perm_b = perm_b - 12;


		CubieCube cc;
		edge_t& edge = cc.edge;

		edge.perm[perm_a] = edgeNo;
		edge.ori[perm_a] = ori_a;
		edge.perm[perm_b] = edgeNo_b;
		edge.ori[perm_b] = ori_b;

		int j = 0;
		for (int i = 0; i < 12; ++i)
		{
			if (i == perm_a || i == perm_b)
				continue;

			while (j == edgeNo || j == edgeNo_b)
			{
				++j;
			}

			edge.perm[i] = j;

			++j;
		}

		return cc;
	}

	int HumanSolverLBLSearch::toTwoCornerIdx(int cornerNo, const CubieCube & cc)
	{
		const corner_t& corner = cc.corner;

		int cornerNo_b = cornerNo + 1;
		if (cornerNo_b >= 8)
			cornerNo_b = cornerNo_b - 1;

		int perm_a = -1;
		int perm_b = -1;
		int ori_a = -1;
		int ori_b = -1;
		for (int i = 0; i < 8; ++i)
		{
			if (corner.perm[i] == cornerNo)
			{
				perm_a = i;
				ori_a = corner.ori[i].get();
			}


			if (corner.perm[i] == cornerNo_b)
			{
				perm_b = i;
				ori_b = corner.ori[i].get();
			}
		}

		assert(perm_a >= 0);
		assert(perm_b >= 0);
		assert(ori_a >= 0);
		assert(ori_b >= 0);


		perm_a = perm_a - cornerNo;
		if (perm_a < 0)
			perm_a = perm_a + 8;

		perm_b = perm_b - cornerNo;
		if (perm_b < 0)
			perm_b = perm_b + 8;

		permutation_Ank<8, 2> perm{ perm_a, perm_b };
		int idx1 = perm.getIndex();

		int idx2 = ori_b * 3 + ori_a;

		int idx = idx1 * 9 + idx2;

		return idx;
	}

	CubieCube HumanSolverLBLSearch::fromTwoCornerIdx(int cornerNo, int idx)
	{
		int cornerNo_b = cornerNo + 1;
		if (cornerNo_b >= 8)
			cornerNo_b = cornerNo_b - 1;

		int idx1 = idx / 9;
		int idx2 = idx % 9;


		permutation_Ank<8, 2> perm(idx1);

		int ori_a = idx2 % 3;
		int ori_b = idx2 / 3;

		int perm_a = perm[0];
		int perm_b = perm[1];


		perm_a = perm_a + cornerNo;
		if (perm_a >= 8)
			perm_a = perm_a - 8;

		perm_b = perm_b + cornerNo;
		if (perm_b >= 8)
			perm_b = perm_b - 8;


		CubieCube cc;
		corner_t& corner = cc.corner;

		corner.perm[perm_a] = cornerNo;
		corner.ori[perm_a] = ori_a;
		corner.perm[perm_b] = cornerNo_b;
		corner.ori[perm_b] = ori_b;

		int j = 0;
		for (int i = 0; i < 8; ++i)
		{
			if (i == perm_a || i == perm_b)
				continue;

			while (j == cornerNo || j == cornerNo_b)
			{
				++j;
			}

			corner.perm[i] = j;

			++j;
		}

		return cc;
	}

	// 	void HumanSolverLBLSearch::init_DCrossMove()
	// 	{
	// 		for (int i = 0; i < 528; ++i)
	// 		{
	// 			CubieCube cc = fromDCrossIdx(i);
	// 			for (int j = 0; j < 18; ++j)
	// 			{
	// 				CubieCube dd = cc.move(j);
	// 				int idx = toDCrossIdx(dd);
	// 				assert(0 <= idx && idx < 528);
	// 				DCrossMove[i][j] = idx;
	// 			}
	// 		}
	// 	}
	// 
	// 	void HumanSolverLBLSearch::init_DCrossPrun()
	// 	{
	// 		for (int i = 0; i < 528; ++i)
	// 		{
	// 			DCrossPrun[i] = -1;
	// 		}
	// 
	// 		DCrossPrun[0] = 0;
	// 		int cnt = 1;
	// 		int depth = 0;
	// 
	// 
	// 		while (cnt < 528)
	// 		{
	// 			for (int i = 0; i < 528; ++i)
	// 			{
	// 				if (DCrossPrun[i] != depth)
	// 					continue;
	// 
	// 				CubieCube cc = fromDCrossIdx(i);
	// 
	// 				for (int m = 0; m < 18; ++m)
	// 				{
	// 					CubieCube dd = cc.move(m);
	// 					int j = toDCrossIdx(dd);
	// 
	// 					if (DCrossPrun[j] == -1)
	// 					{
	// 						DCrossPrun[j] = depth + 1;
	// 						++cnt;
	// 					}
	// 				}
	// 			}
	// 
	// 			++depth;
	// 		}
	// 
	// 		assert(cnt == 528);
	// 
	// 	}

	void HumanSolverLBLSearch::init_moveConjY2()
	{
		CubieCube y2 = CubieCube::eye().move(Rot_y2);


		vector<CubieCube> mm(18);
		for (int i = 0; i < 18; ++i)
		{
			mm[i] = CubieCube::eye().move(i);
		}

		for (int i = 0; i < 18; ++i)
		{
			CubieCube cc = mm[i];

			CubieCube dd = y2 * cc * y2;

			for (int j = 0; j < 18; ++j)
			{
				if (mm[j] == dd)
				{
					moveConjY2[i] = j;
				}
			}
		}
	}

	void HumanSolverLBLSearch::init_MoveTable(int nSize, int * moveTable, int(*fcnToIdx)(const CubieCube &), CubieCube(*fcnFromIdx)(int))
	{
		for (int i = 0; i < nSize; ++i)
		{
			CubieCube cc = fcnFromIdx(i);
			for (int j = 0; j < 18; ++j)
			{
				CubieCube dd = cc.move(j);
				int idx = fcnToIdx(dd);
				assert(0 <= idx && idx < nSize);
				moveTable[i * 18 + j] = idx;
			}
		}
	}

	void HumanSolverLBLSearch::init_PrunTable(int nSize, int * prunTable, int(*fcnToIdx)(const CubieCube &), CubieCube(*fcnFromIdx)(int))
	{
		for (int i = 0; i < nSize; ++i)
		{
			prunTable[i] = -1;
		}

		prunTable[0] = 0;
		int cnt = 1;
		int depth = 0;


		while (cnt < nSize)
		{
			for (int i = 0; i < nSize; ++i)
			{
				if (prunTable[i] != depth)
					continue;

				CubieCube cc = fcnFromIdx(i);

				for (int m = 0; m < 18; ++m)
				{
					CubieCube dd = cc.move(m);
					int j = fcnToIdx(dd);

					if (prunTable[j] == -1)
					{
						prunTable[j] = depth + 1;
						++cnt;
					}
				}
			}

			++depth;
		}

		assert(cnt == nSize);
	}



	bool HumanSolverLBLSearch::_search(const CubieCube & cc, const std::vector<PrunInfo>& prunInfos,
		int max_depth, std::vector<Move>& moves, int& num_try)
	{
		CubieCube y2 = CubieCube::eye().move(Rot_y2);

		int numPrun = (int)prunInfos.size();

		// 判断是否已经有解
		vector<int> idx(numPrun);
		int h = -1;
		for (int i = 0; i < numPrun; ++i)
		{
			int idx1;
			if (prunInfos[i].is_conj)
				idx1 = prunInfos[i].fcnToIdx(y2 * cc * y2);
			else
				idx1 = prunInfos[i].fcnToIdx(cc);

			idx[i] = idx1;
			int h1 = prunInfos[i].prunTable[idx1];
			h = max(h, h1);
		}

		if (h == 0)
		{
			moves.clear();
			return true;
		}

		vector<vector<int>> cubeIdx;
		cubeIdx.push_back(idx);

		vector<int> curIdx(numPrun);
		vector<int> nextIdx(numPrun);

		bool is_found = false;

		num_try = 0;
		int depth = 1;

		for (; !is_found && depth <= max_depth; ++depth)
		{
			moves.resize(depth);
			cubeIdx.resize(depth);

			for (int j = 0; j < (int)moves.size(); ++j)
			{
				moves[j] = InvalidMove;
			}

			int k = 0;
			while (!is_found && k < depth)
			{
				curIdx = cubeIdx[k];

				int last_ax = -1;

				if (k > 0)
					last_ax = moves[k - 1] / 3;


				bool next_k = false;

				for (int mv = moves[k] + 1; !is_found && mv < 18; ++mv)
				{
					int ax = mv / 3;

					if (ax == last_ax)
						continue;

					if (ax == last_ax - 3)
						continue;

					++num_try;


					// move and h
					int h = -1;
					for (int i = 0; i < numPrun; ++i)
					{
						int m = mv;
						if (prunInfos[i].is_conj)
							m = moveConjY2[mv];


						int nextIdx1 = prunInfos[i].moveTable[curIdx[i] * 18 + m];
						nextIdx[i] = nextIdx1;
						int h1 = prunInfos[i].prunTable[nextIdx1];

						h = max(h, h1);
					}

					if (h == 0)	// 已找到解
					{
						moves[k] = (Move)mv;
						moves.resize(k + 1);
						is_found = true;
						break;
					}

					if (h < depth - k)	// 深度优先
					{
						moves[k] = (Move)mv;
						cubeIdx[k + 1] = nextIdx;
						++k;
						next_k = true;
						break;
					}

				}// for mv

				if (!is_found && !next_k)
				{
					if (k == 0)
						break;
					else
					{
						moves[k] = InvalidMove;
						--k;
					}

				}

			}// for k
		}// for depth


		if (!is_found)
			moves.clear();

		return is_found;
	}

	// 	std::vector<Move> HumanSolverLBLSearch::solveDCross(const CubieCube & cc)
	// 	{
	// 		std::vector<Move> moves;
	// 
	// 
	// 		CubieCube y2 = CubieCube::eye().move(Rot_y2);
	// 
	// 		int idx1 = toDCrossIdx(cc);
	// 		int idx2 = toDCrossIdx(y2 * cc * y2);
	// 
	// 		if (idx1 == 0 && idx2 == 0) 
	// 		{
	// 			myIsSolved = true;
	// 			myDCrossMoves = moves;
	// 			myDCross = cc;
	// 			return moves;
	// 		}
	// 
	// 		vector<int> cubeIdx1;	// for DR,DF, 有 cubeIdx[k+1] = cubeIdx[k].move(moves[k])
	// 		vector<int> cubeIdx2;	// for DL,DB
	// 		cubeIdx1.push_back(idx1);
	// 		cubeIdx2.push_back(idx2);
	// 
	// 
	// 		// 猜测 10 步内必定有解
	// 
	// 		bool is_found = false;
	// 
	// 		int try_cnt = 0;
	// 		int depth = 1;
	// 		for (; !is_found && depth <= 10; ++depth)
	// 		{
	// 			moves.resize(depth);
	// 			cubeIdx1.resize(depth);
	// 			cubeIdx2.resize(depth);
	// 
	// 
	// 			for (int j = 0; j < (int)moves.size(); ++j)
	// 			{
	// 				moves[j] = InvalidMove;
	// 			}
	// 
	// 			int k = 0;
	// 			while (!is_found && k < depth)
	// 			{
	// 				int curIdx1 = cubeIdx1[k];
	// 				int curIdx2 = cubeIdx2[k];
	// 
	// 				int last_ax = -1;
	// 
	// 				if (k > 0)
	// 					last_ax = moves[k-1] / 3;
	// 
	// 
	// 				bool next_k = false;
	// 
	// 				for (int mv = moves[k]+1; !is_found && mv < 18; ++mv)
	// 				{
	// 					int ax = mv / 3;
	// 
	// 					if (ax == last_ax)
	// 						continue;
	// 
	// 					++try_cnt;
	// 
	// 					int nextIdx1 = DCrossMove[curIdx1][mv];
	// 					int nextIdx2 = DCrossMove[curIdx2][moveConjY2[mv]];
	// 
	// 					int h1 = DCrossPrun[nextIdx1];
	// 					int h2 = DCrossPrun[nextIdx2];
	// 
	// 					int h = max(h1, h2);
	// 
	// 					if (h == 0)	// 已找到解
	// 					{
	// 						moves[k] = (Move)mv;
	// 						moves.resize(k+1);
	// 						is_found = true;
	// 						break;
	// 					}
	// 
	// 					if (h < depth - k)	// 深度优先
	// 					{
	// 						moves[k] = (Move)mv;
	// 						cubeIdx1[k + 1] = nextIdx1;
	// 						cubeIdx2[k + 1] = nextIdx2;
	// 
	// 						++k;
	// 
	// 						next_k = true;
	// 						break;
	// 					}
	// 
	// 				}// for mv
	// 
	// 				if (!is_found && !next_k)
	// 				{
	// 					if (k == 0)
	// 						break;
	// 					else
	// 					{
	// 						moves[k] = InvalidMove;
	// 						--k;
	// 					}
	// 						
	// 				}
	// 
	// 			}// for k
	// 		}// for depth
	// 
	// 		if (!is_found)
	// 		{
	// 			myIsSolved = false;
	// 			myDCrossMoves.clear();
	// 			myDCross = cc;
	// 			return myDCrossMoves;
	// 		}
	// 
	// 		myIsSolved = true;
	// 		myDCrossMoves = moves;
	// 		myDCross = cc.move(moves);
	// 		return moves;
	// 	}

	std::vector<Move> HumanSolverLBLSearch::solve(const CubieCube & cc)
	{
		mySrcCube = cc;

		vector<Move> moves1 = solveDCross();

		assert(HumanSolver::isDCrossSolved(myDCrossCube));

		vector<Move> moves2 = solveDLayer();

		assert(HumanSolver::isDLayerSolved(myDLayerCube));

		vector<Move> moves;
		moves.insert(moves.end(), moves1.begin(), moves1.end());
		moves.insert(moves.end(), moves2.begin(), moves2.end());

		return moves;
	}

	std::vector<Move> HumanSolverLBLSearch::solveDCross()
	{
		const CubieCube & cc = mySrcCube;

		std::vector<Move> moves;

		vector<PrunInfo> prunInfos(2);

		prunInfos[0].fcnToIdx = toDCrossIdx;
		prunInfos[0].fcnFromIdx = fromDCrossIdx;
		prunInfos[0].moveTable = (int*)DCrossMove;
		prunInfos[0].prunTable = DCrossPrun;
		prunInfos[0].is_conj = false;


		prunInfos[1].fcnToIdx = toDCrossIdx;
		prunInfos[1].fcnFromIdx = fromDCrossIdx;
		prunInfos[1].moveTable = (int*)DCrossMove;
		prunInfos[1].prunTable = DCrossPrun;
		prunInfos[1].is_conj = true;


		bool is_found = _search(cc, prunInfos, 10, moves, myDCrossNumTry);

		myDCrossMoves = moves;
		myDCrossCube = cc.move(moves);
		return moves;
	}

	std::vector<Move> HumanSolverLBLSearch::solveDLayer()
	{
		const CubieCube & cc = myDCrossCube;

		std::vector<Move> moves;

		vector<PrunInfo> prunInfos(3);

		prunInfos[0].fcnToIdx = toDCrossIdx;
		prunInfos[0].fcnFromIdx = fromDCrossIdx;
		prunInfos[0].moveTable = (int*)DCrossMove;
		prunInfos[0].prunTable = DCrossPrun;
		prunInfos[0].is_conj = false;

		prunInfos[1].fcnToIdx = toDCrossIdx;
		prunInfos[1].fcnFromIdx = fromDCrossIdx;
		prunInfos[1].moveTable = (int*)DCrossMove;
		prunInfos[1].prunTable = DCrossPrun;
		prunInfos[1].is_conj = true;

		prunInfos[2].fcnToIdx = toDLayerIdx;
		prunInfos[2].fcnFromIdx = fromDLayerIdx;
		prunInfos[2].moveTable = (int*)DLayerMove;
		prunInfos[2].prunTable = DLayerPrun;
		prunInfos[2].is_conj = false;

		int ntry1;
		bool is_found = _search(cc, prunInfos, 10, moves, ntry1);

		prunInfos.resize(4);
		prunInfos[3].fcnToIdx = toDLayerIdx;
		prunInfos[3].fcnFromIdx = fromDLayerIdx;
		prunInfos[3].moveTable = (int*)DLayerMove;
		prunInfos[3].prunTable = DLayerPrun;
		prunInfos[3].is_conj = true;

		int ntry2;
		is_found = _search(cc, prunInfos, 10, moves, ntry2);


		myDLayerMoves = moves;
		myDLayerCube = cc.move(moves);
		return moves;
	}

	// 	bool HumanSolverLBLSearch::isDCrossSolved(const CubieCube & cc)
	// 	{
	// 		int idx1 = toDCrossIdx(cc);
	// 		CubieCube y2 = CubieCube::eye().move(Rot_y2);
	// 		CubieCube dd = y2 * cc * y2;
	// 		int idx2 = toDCrossIdx(dd);
	// 
	// 		return (idx1 == 0) && (idx2 == 0);
	// 	}
	// 	bool HumanSolverLBLSearch::isDLayerSolved(const CubieCube & cc)
	// 	{
	// 		return false;
	// 	}


	//////////////////////////////////////////////////////////////////////////
	bool HumanSolverLBL::inited = false;
	Move HumanSolverLBL::conjMove[4] = { InvalidMove, Rot_y1, Rot_y2, Rot_y3 };
	int HumanSolverLBL::conjInv[4] = { 0, 3, 2, 1 };
	AxisCubieCube HumanSolverLBL::conjCube[4];
	int HumanSolverLBL::moveConj[54][4];


	void HumanSolverLBL::init()
	{
		if (inited)
			return;

		_init_moveConj();
	}

	std::vector<Move> HumanSolverLBL::solve(const CubieCube & cc)
	{
		return solve(AxisCubieCube(AxisCube::eye(), cc));
	}

	std::vector<Move> HumanSolverLBL::solve(const AxisCubieCube & cc)
	{
		init();

		AxisCube axis = cc.axis.inv();
		vector<Move> ff = axis.getGenFormula();
		AxisCubieCube ss = AxisCubieCube::eye().move(ff);
		mySrcCube = ss * cc;

		myError = mySrcCube.cubie.verify();
		if (myError != 0)
		{
			myIsSolved = false;
			myMoves.clear();
			return myMoves;
		}

		if (cc.isSolved())
		{
			myIsSolved = true;
			myMoves.clear();
			return myMoves;
		}

		vector<Move> moves1 = solveDCross();
		vector<Move> moves2 = solveDLayer();
		vector<Move> moves3 = solveMLayer();
		vector<Move> moves4 = solveUCross();
		vector<Move> moves5 = solveUFace();
		vector<Move> moves6 = solveUCorner();
		vector<Move> moves7 = solveFinal();


		vector<Move> moves;
		moves.insert(moves.end(), moves1.begin(), moves1.end());
		moves.insert(moves.end(), moves2.begin(), moves2.end());
		moves.insert(moves.end(), moves3.begin(), moves3.end());
		moves.insert(moves.end(), moves4.begin(), moves4.end());
		moves.insert(moves.end(), moves5.begin(), moves5.end());
		moves.insert(moves.end(), moves6.begin(), moves6.end());
		moves.insert(moves.end(), moves7.begin(), moves7.end());


		moves = HumanSolver::mergeRepetedRotateFormula(moves);

#ifdef _DEBUG
		AxisCubieCube dd = cc.move(moves);
		assert(dd.isSolved());

#endif // _DEBUG



		myMoves = moves;
		myIsSolved = true;
		return moves;
	}

	std::vector<Move> HumanSolverLBL::solve(const AxisCube & axis, const CubieCube & cc)
	{
		return solve(AxisCubieCube(axis, cc));
	}

	std::vector<Move> HumanSolverLBL::solveDCross()
	{
		HumanSolverLBLSearch search;
		search.setCube(mySrcCube.getAbsCubie());
		myDCrossMoves = search.solveDCross();
		myDCrossCube = mySrcCube.move(myDCrossMoves);

		return myDCrossMoves;
	}

	std::vector<Move> HumanSolverLBL::solveDLayer()
	{
		AxisCubieCube cc = myDCrossCube;

		assert(cc.axis.iseye());
		assert(HumanSolver::isDCrossSolved(cc.cubie));

 		vector<Move> moves;
		for (int i = 0; i < 4; ++i)
		{
			vector<Move> mv1 = _solveDLayerDFR(conjCube[i].inv() * cc * conjCube[i]);
			
			vector<Move> mv2;
			if (i > 0)
				mv2.push_back(conjMove[i]);

			mv2.insert(mv2.end(), mv1.begin(), mv1.end());

			if (i > 0)
				mv2.push_back(conjMove[conjInv[i]]);


			cc = cc.move(mv2);
			moves.insert(moves.end(), mv2.begin(), mv2.end());
		}


		assert(cc.axis.iseye());
		assert(HumanSolver::isDLayerSolved(cc.getAbsCubie()));
		assert(cc == myDCrossCube.move(moves));

		myDLayerCube = cc;
		myDLayerMoves = moves;

		return moves;
	}

	std::vector<Move> HumanSolverLBL::solveMLayer()
	{
		AxisCubieCube cc = myDLayerCube;


		assert(cc.axis.iseye());
		assert(HumanSolver::isDLayerSolved(cc.cubie));

		vector<Move> moves;
		for (int i = 0; i < 4; ++i)
		{
			vector<Move> mv1 = _solveMLayerFR(conjCube[i].inv() * cc * conjCube[i]);

			vector<Move> mv2;
			if (i > 0)
				mv2.push_back(conjMove[i]);

			mv2.insert(mv2.end(), mv1.begin(), mv1.end());

			if (i > 0)
				mv2.push_back(conjMove[conjInv[i]]);


			cc = cc.move(mv2);
			moves.insert(moves.end(), mv2.begin(), mv2.end());
		}


		assert(cc.axis.iseye());
		assert(HumanSolver::isF2LSolved(cc.getAbsCubie()));
		assert(cc == myDLayerCube.move(moves));

		myMLayerCube = cc;
		myMLayerMoves = moves;

		return moves;
	}

	std::vector<Move> HumanSolverLBL::solveUCross()
	{
		AxisCubieCube cc = myMLayerCube;

		assert(cc.axis.iseye());
		assert(HumanSolver::isF2LSolved(cc.cubie));


		std::vector<Move> moves;

		vector<Move> mv1 = { Fx1, Rx1, Ux1, Rx3, Ux3, Fx3 };

		int cnt = 0;

		while (!HumanSolver::isUCrossSolved(cc.cubie))
		{
			++cnt;

			assert(cnt < 100);	// 防止死循环


			int num = 0;

			edge_t& edge = cc.cubie.edge;
			
			for (int i = 0; i < 4; ++i)
			{
				if (edge.ori[i] == 0)
					++num;
			}


			if (num == 0)
			{
				
				moves.insert(moves.end(), mv1.begin(), mv1.end());
				cc = cc.move(mv1);
			}
			else if (num == 2)
			{
				// 根据情况做公式前转动U
				int x = 0;

				if (edge.ori[0] == 0 && edge.ori[2] == 0)	// 一字，侧边U色在前
					x = 0;
				else if (edge.ori[1] == 0 && edge.ori[1] == 0)	// 一字，侧边U色不在前
					x = 1;
				else if (edge.ori[2] == 0 && edge.ori[3] == 0)  // 对角,不需转动
					x = 0;
				else if (edge.ori[1] == 0 && edge.ori[2] == 0)
					x = 1;
				else if (edge.ori[0] == 0 && edge.ori[1] == 0)
					x = 2;
				else if (edge.ori[3] == 0 && edge.ori[0] == 0)
					x = 3;

				if (x > 0)
				{
					Move m = Move((int)Ux1 + (x - 1));
					moves.push_back(m);
					cc = cc.move(m);
				}
					


				// 做公式
				moves.insert(moves.end(), mv1.begin(), mv1.end());
				cc = cc.move(mv1);

			}
			else
			{
				throw "invalid cube.";
			}
		}

		assert(cc.axis.iseye());
		assert(HumanSolver::isUCrossSolved(cc.getAbsCubie()));
		assert(cc == myMLayerCube.move(moves));

		myUCrossCube = cc;
		myUCrossMoves = moves;


		return moves;

	}

	std::vector<Move> HumanSolverLBL::solveUFace()
	{
		AxisCubieCube cc = myUCrossCube;

		assert(cc.axis.iseye());
		assert(HumanSolver::isUCrossSolved(cc.cubie));


		std::vector<Move> moves;

		vector<Move> mv1 = {Rx1, Ux2, Rx3, Ux3, Rx1, Ux3, Rx3 };
		vector<Move> mv2 = { Rx1, Ux1, Rx3, Ux1, Rx1, Ux2, Rx3 };


		

		int cnt = 0;
		while (!HumanSolver::isOLLSolved(cc.cubie))
		{
			assert(HumanSolver::isUCrossSolved(cc.cubie));

			++cnt;
			assert(cnt < 100);	// 防止死循环


			int num = 0;

			corner_t& corner = cc.cubie.corner;
			vector<int> ori(4);

			for (int i = 0; i < 4; ++i)
			{
				ori[i] = corner.ori[i].get();
				if (ori[i] != 0)
					++num;
			}

			if (num == 3)	// 小鱼
			{

				// 根据情况做公式前转动U
				int x = 0;

				if (ori[0] == 1 || ori[1] == 1) // 三个方向是1
				{
					int x = 0;
					if (ori[0] == 0)
						x = 3;
					else if (ori[1] == 0)
						x = 2;
					else if (ori[2] == 0)
						x = 1;
					else // ori[3] == 3
						x = 0;

					if (x > 0)
					{
						Move m = Move((int)Ux1 + (x - 1));
						moves.push_back(m);
						cc = cc.move(m);
					}

					// 做公式 1
					moves.insert(moves.end(), mv1.begin(), mv1.end());
					cc = cc.move(mv1);

					assert(HumanSolver::isOLLSolved(cc.getAbsCubie()));

				}
				else if (ori[0] == 2 || ori[1] == 2) // 三个方向是 2
				{
					int x = 0;
					if (ori[0] == 0)
						x = 1;
					else if (ori[1] == 0)
						x = 0;
					else if (ori[2] == 0)
						x = 3;
					else // ori[3] == 3
						x = 2;

					if (x > 0)
					{
						Move m = Move((int)Ux1 + (x - 1));
						moves.push_back(m);
						cc = cc.move(m);
					}

					// 做公式 2
					moves.insert(moves.end(), mv2.begin(), mv2.end());
					cc = cc.move(mv2);

					assert(HumanSolver::isOLLSolved(cc.getAbsCubie()));
				}
				else
				{
					throw "invalid cube";
				}
			}
			else if (num == 2) // 不是小鱼，需转成小鱼
			{
				// 判断是否需要先转动顶层
				CubieCube dd = cc.cubie;

				int x = 0;
				while (dd.corner.ori[3].get() != 2)
				{
					++x;
					dd = dd.move(Ux1);
				}
				

				if (x > 0)
				{
					Move m = Move((int)Ux1 + (x-1));
					moves.push_back(m);
					cc = cc.move(m);
				}

				// 公式1
				moves.insert(moves.end(), mv1.begin(), mv1.end());
				cc = cc.move(mv1);
				
			}
			else if (num == 4)
			{
				// 判断是否需要先转动顶层
				CubieCube dd = cc.cubie;

				int x = 0;
				while (dd.corner.ori[0].get() != 2)
				{
					++x;
					dd = dd.move(Ux1);
				}


				if (x > 0)
				{
					Move m = Move((int)Ux1 + (x - 1));
					moves.push_back(m);
					cc = cc.move(m);
				}

				// 公式1
				moves.insert(moves.end(), mv1.begin(), mv1.end());
				cc = cc.move(mv1);
			}
			else
			{
				throw "invalid cube";
			}// if
		}// while

		assert(cc.axis.iseye());
		assert(HumanSolver::isOLLSolved(cc.getAbsCubie()));
		assert(cc == myUCrossCube.move(moves));

		myUFaceCube = cc;
		myUFaceMoves = moves;


		return moves;
	}

	std::vector<Move> HumanSolverLBL::solveUCorner()
	{
		AxisCubieCube cc = myUFaceCube;

		assert(cc.axis.iseye());
		assert(HumanSolver::isOLLSolved(cc.cubie));


		std::vector<Move> moves;

		vector<Move> mv1 = { Rx3, Ux1, Lx3, Ux2, Rx1, Ux3, Rx3, Ux2, Lx1, Rx1 };

		corner_t& corner = cc.cubie.corner;

		int cnt = 0;
		while (!HumanSolver::isUCornerSolved(cc.cubie))
		{
			assert(HumanSolver::isOLLSolved(cc.cubie));

			++cnt;
			assert(cnt < 100);	// 防止死循环


			int num = 0;	// 同一面颜色相同的数量
			for (int i = 0; i < 4; ++i)
			{
				int j = i == 3 ? 0 : i + 1;

				if (corner.perm[i] + 1 == corner.perm[j] || (corner.perm[i] == 3 && corner.perm[j] == 0))
					++num;
			}


			if (num == 4)	// 四个面颜色已经相同，只需转动顶层
			{
				int x = 0;

				CubieCube dd = cc.cubie;

				while (dd.corner.perm[0] != 0)
				{
					++x;
					dd = dd.move(Ux1);
				}

				if (x > 0)
				{
					Move m = Move(Ux1 + (x-1));
					moves.push_back(m);
					cc = cc.move(m);
				}
			}
			else if (num == 0)
			{
				// 公式1
				moves.insert(moves.end(), mv1.begin(), mv1.end());
				cc = cc.move(mv1);
			}
			else if (num == 1)
			{
				// 转动顶层是相同颜色在前面
				int x = 0;

				CubieCube dd = cc.cubie;

				while (!(dd.corner.perm[0] + 1 == dd.corner.perm[1] ||
					(dd.corner.perm[0] == 3 && dd.corner.perm[1] == 0)))
				{
					++x;
					dd = dd.move(Ux1);
				}

				if (x > 0)
				{
					Move m = Move(Ux1 + (x - 1));
					moves.push_back(m);
					cc = cc.move(m);
				}

				// 公式1
				moves.insert(moves.end(), mv1.begin(), mv1.end());
				cc = cc.move(mv1);
			}
			else
			{
				throw "invalid cube";
			}


			
		}// while

		assert(cc.axis.iseye());
		assert(HumanSolver::isUCornerSolved(cc.getAbsCubie()));
		assert(cc == myUFaceCube.move(moves));

		myUCornerCube = cc;
		myUCornerMoves = moves;


		return moves;
	}

	std::vector<Move> HumanSolverLBL::solveFinal()
	{
		AxisCubieCube cc = myUCornerCube;

		assert(cc.axis.iseye());
		assert(HumanSolver::isUCornerSolved(cc.cubie));


		std::vector<Move> moves;

		vector<Move> mv1 = { Mx2, Ux1, Mx1, Ux2, Mx3, Ux1, Mx2 };

		edge_t& edge = cc.cubie.edge;

		int cnt = 0;
		while (!HumanSolver::isSolved(cc.cubie))
		{

			assert(HumanSolver::isUCornerSolved(cc.cubie));

			++cnt;
			assert(cnt < 100);	// 防止死循环


			int num = 0;
			for (int i = 0; i < 4; ++i)
			{
				int j = i == 3 ? 0 : i + 1;

				if (edge.perm[i] == i)
					++num;
			}


			if (num == 1)	// 只有一个棱块已归位，做三棱换
			{
				vector<Move> mv2;
				if (edge.perm[0] == 0)
				{
					mv2.push_back(Rot_y3);
					mv2.insert(mv2.end(), mv1.begin(), mv1.end());
					mv2.push_back(Rot_y1);
				}
				else if (edge.perm[1] == 1)
				{
					mv2.push_back(Rot_y2);
					mv2.insert(mv2.end(), mv1.begin(), mv1.end());
					mv2.push_back(Rot_y2);
				}
				else if (edge.perm[2] == 2)
				{
					mv2.push_back(Rot_y1);
					mv2.insert(mv2.end(), mv1.begin(), mv1.end());
					mv2.push_back(Rot_y3);
				}
				else if (edge.perm[3] == 3)
				{
					mv2 = mv1;
				}
				else
				{
					throw "logic error";
				}

				moves.insert(moves.end(), mv2.begin(), mv2.end());
				cc = cc.move(mv2);
			}
			else if (num == 0)	// 没有一个棱块已归位
			{
				moves.insert(moves.end(), mv1.begin(), mv1.end());
				cc = cc.move(mv1);
			}
			else
			{
				throw "invalid cube";
			}



		}// while

		assert(cc.axis.iseye());
		assert(HumanSolver::isSolved(cc.getAbsCubie()));
		assert(cc == myUCornerCube.move(moves));

		myFinalCube = cc;
		myFinalMoves = moves;


		return moves;
	}

	std::vector<Move> HumanSolverLBL::_solveDLayerDFR(const AxisCubieCube& cc)
	{
		assert(cc.axis.iseye());

		AxisCubieCube dd = cc;
		const corner_t& corner = dd.cubie.corner;

		std::vector<Move> moves;
		bool done = false;
		int cnt = 0;
		while (!done)
		{
			++cnt;


			assert(cnt < 100);	// 防止死循环

			corner_t cornerInv = corner.inv();
			int perm = cornerInv.perm[DFR];
			int ori = corner.ori[perm].get();

			if (perm == DFR && ori == 0)	// 已经归位
				break;


			if (perm < 4)  // DFR 在顶层
			{
				// 第一步，把角块DFR 移植位置 0
				if (perm == 1)
					moves.push_back(Ux3);
				else if (perm == 2)
					moves.push_back(Ux2);
				else if (perm == 3)
					moves.push_back(Ux1);


				// 根据D色面方向移到底层
				if (ori == 0)	// D色面在顶
				{
					// R U2 R' U'
					moves.push_back(Rx1);
					moves.push_back(Ux2);
					moves.push_back(Rx3);
					moves.push_back(Ux3);

					// R U R'
					moves.push_back(Rx1);
					moves.push_back(Ux1);
					moves.push_back(Rx3);
				}
				else if (ori == 1) // D色面在右
				{
					// R U R'
					moves.push_back(Rx1);
					moves.push_back(Ux1);
					moves.push_back(Rx3);
				}
				else // D色面在前
				{
					// U R U' R'
					moves.push_back(Ux1);
					moves.push_back(Rx1);
					moves.push_back(Ux3);
					moves.push_back(Rx3);
				}

				done = true;
			}
			else // // DFR 在底层，把它移到顶层
			{
				// 把目标转到4的位置
				vector<Move> mv3;

				if (perm == 4)
				{
					// R U R'
					mv3.push_back(Rx1);
					mv3.push_back(Ux1);
					mv3.push_back(Rx3);
				}
				else if (perm == 5)
				{
					mv3.push_back(Rot_y3);

					// R U R'
					mv3.push_back(Rx1);
					mv3.push_back(Ux1);
					mv3.push_back(Rx3);

					mv3.push_back(Rot_y1);
				}
				else if (perm == 6)
				{
					mv3.push_back(Rot_y2);

					// R U R'
					mv3.push_back(Rx1);
					mv3.push_back(Ux1);
					mv3.push_back(Rx3);

					mv3.push_back(Rot_y2);
				}
				else // 7
				{
					mv3.push_back(Rot_y1);

					// R U R'
					mv3.push_back(Rx1);
					mv3.push_back(Ux1);
					mv3.push_back(Rx3);

					mv3.push_back(Rot_y3);
				}


				
				moves.insert(moves.end(), mv3.begin(), mv3.end());
				dd = dd.move(mv3);
			}// if

			assert(dd.axis.iseye());

		}// while


#ifdef _DEBUG
		AxisCubieCube ff = cc.move(moves);

		assert(ff.axis.iseye());
		assert(ff.cubie.corner.perm[DFR] == DFR);
		assert(ff.cubie.corner.ori[DFR].get() == 0);
#endif

		return moves;
	}

	std::vector<Move> HumanSolverLBL::_solveMLayerFR(const AxisCubieCube & cc)
	{
		assert(cc.axis.iseye());

		AxisCubieCube dd = cc;
		const edge_t& edge = dd.cubie.edge;

		vector<Move> mv1 = { Rx3, Fx3, Rx1, Ux1, Rx1, Ux3, Rx3, Fx1 };
		vector<Move> mv2 = { Ux1, Rx1, Ux3, Rx3, Ux3, Fx3, Ux1, Fx1 };

		std::vector<Move> moves;
		bool done = false;
		int cnt = 0;
		while (!done)
		{
			++cnt;


			assert(cnt < 100);	// 防止死循环

			edge_t edgeInv = edge.inv();
			int perm = edgeInv.perm[FR];
			int ori = edge.ori[perm].get();

			if (perm == FR && ori == 0)	// 已经归位
				break;


			if (perm < 4)  // FR 在顶层
			{
				if (ori == 0)	// F 色在顶面
				{
					// 第一步，把角块FR 移植位置 0
					if (perm == 1)
						moves.push_back(Ux3);
					else if (perm == 2)
						moves.push_back(Ux2);
					else if (perm == 3)
						moves.push_back(Ux1);

					moves.insert(moves.end(), mv1.begin(), mv1.end());
					
				}
				else // F 色在侧面
				{
					// 第一步，把角块FR 移植位置 1
					if (perm == 0)
						moves.push_back(Ux1);
					else if (perm == 2)
						moves.push_back(Ux3);
					else if (perm == 3)
						moves.push_back(Ux2);

					
					moves.insert(moves.end(), mv2.begin(), mv2.end());
				}
				done = true;
			}
			else // FR 不在顶层
			{
				// 把目标转到4的位置
				vector<Move> mv3;

				if (perm == 8)
				{
					mv3.insert(mv3.end(), mv2.begin(), mv2.end());
				}
				else if (perm == 9)
				{
					mv3.push_back(Rot_y3);
					mv3.insert(mv3.end(), mv2.begin(), mv2.end());
					mv3.push_back(Rot_y1);
				}
				else if (perm == 10)
				{
					mv3.push_back(Rot_y2);
					mv3.insert(mv3.end(), mv2.begin(), mv2.end());
					mv3.push_back(Rot_y2);
				}
				else // 11
				{
					mv3.push_back(Rot_y1);
					mv3.insert(mv3.end(), mv2.begin(), mv2.end());
					mv3.push_back(Rot_y3);
				}



				moves.insert(moves.end(), mv3.begin(), mv3.end());
				dd = dd.move(mv3);
			}// if

			assert(dd.axis.iseye());

		}// while


#ifdef _DEBUG
		AxisCubieCube ff = cc.move(moves);

		assert(ff.axis.iseye());
		assert(ff.cubie.edge.perm[FR] == FR);
		assert(ff.cubie.edge.ori[FR].get() == 0);
#endif

		return moves;
	}

	void HumanSolverLBL::_init_moveConj()
	{
		AxisCubieCube eye = AxisCubieCube::eye();


		AxisCubieCube mv[54];
		for (int i = 0; i < 54; ++i)
		{
			mv[i] = eye.move(i);
		}

		conjCube[0] = eye;
		conjCube[1] = eye.move(Rot_y1);
		conjCube[2] = eye.move(Rot_y2);
		conjCube[3] = eye.move(Rot_y3);

		for (int i = 0; i < 18; ++i)
		{
			AxisCubieCube cc = mv[i];

			for (int j = 0; j < 4; ++j)
			{
				AxisCubieCube dd = conjCube[j].inv() * cc * conjCube[j];

				for (int k = 0; k < 18; ++k)
				{
					if (mv[k] == dd)
					{
						moveConj[i][j] = k;
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	std::vector<std::tuple<int, int, std::vector<Move>>> HumanSolverCFOP::F2L_table =
	{
		std::tuple<int, int, std::vector<Move>>{4 * 3 + 0, 8 * 2 + 1, {Rx1, Ux3, Rx3, Ux1, Rot_y3, Rx3, Ux2, Rx1, Ux2, Rx3, Ux1, Rx1}},	// 01
		std::tuple<int, int, std::vector<Move>>{4 * 3 + 0, 1 * 2 + 1, {Ux1, Rx1, Ux3, Rx3, Ux3, Rot_y3, Rx3, Ux1, Rx1}}, // 02
		std::tuple<int, int, std::vector<Move>>{4 * 3 + 0, 0 * 2 + 0, {Rx3, Fx3, Rx1, Ux1, Rx1, Ux3, Rx3, Fx1}}, // 03

		std::tuple<int, int, std::vector<Move>>{4 * 3 + 1, 8 * 2 + 0, {Rx1, Ux1, Rx3, Ux3, Rx1, Ux2, Rx3, Ux3, Rx1, Ux1, Rx3}}, // 04
		std::tuple<int, int, std::vector<Move>>{4 * 3 + 1, 8 * 2 + 1, {Rx1, Fx1, Ux1, Rx1, Ux3, Rx3, Fx3, Ux3, Rx3}}, // 05
		std::tuple<int, int, std::vector<Move>>{4 * 3 + 1, 1 * 2 + 1, {Rot_y3, Rx3, Ux3, Rx1, Ux1, Rx3, Ux3, Rx1}}, // 06
		std::tuple<int, int, std::vector<Move>>{4 * 3 + 1, 0 * 2 + 0, {Rx1, Ux3, Rx3, Ux1, Rx1, Ux3, Rx3}}, // 07

		std::tuple<int, int, std::vector<Move>>{4 * 3 + 2, 8 * 2 + 0, {Rx1, Ux3, Rx3, Ux1, Rx1, Ux2, Rx3, Ux1, Rx1, Ux3, Rx3}}, // 08
		std::tuple<int, int, std::vector<Move>>{4 * 3 + 2, 8 * 2 + 1, {Rx1, Ux1, Fx1, Rx1, Ux1, Rx3, Ux3, Fx3, Rx3}}, // 09
		std::tuple<int, int, std::vector<Move>>{4 * 3 + 2, 1 * 2 + 1, {Rot_y3, Rx3, Ux1, Rx1, Ux3, Rx3, Ux1, Rx1}}, // 10
		std::tuple<int, int, std::vector<Move>>{4 * 3 + 2, 0 * 2 + 0, {Rx1, Ux1, Rx3, Ux3, Rx1, Ux1, Rx3}}, // 11

		std::tuple<int, int, std::vector<Move>>{0 * 3 + 0, 8 * 2 + 0, {Rx1, Ux1, Rx3, Ux3, Rx1, Ux1, Rx3, Ux3, Rx1, Ux1, Rx3}}, // 12
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 0, 8 * 2 + 1, {Rx1, Ux3, Rx3, Ux1, Rot_y3, Rx3, Ux1, Rx1}}, // 13
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 0, 1 * 2 + 1, {Rot_y3, Rx3, Ux2, Rx1, Ux1, Rx3, Ux3, Rx1}}, // 14
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 0, 2 * 2 + 1, {Rot_y3, Ux3, Rx3, Ux2, Rx1, Ux3, Rx3, Ux1, Rx1}}, // 15
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 0, 3 * 2 + 1, {Rot_y3, Rx3, Ux1, Rx1, Ux2, Rx3, Ux3, Rx1}}, // 16
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 0, 0 * 2 + 1, {Fx1, Ux1, Rx1, Ux3, Rx3, Fx3, Rx1, Ux3, Rx3}}, // 17
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 0, 1 * 2 + 0, {Ux1, Rx1, Ux3, Rx3, Ux3, Rx1, Ux3, Rx3, Ux1, Rx1, Ux3, Rx3}}, // 18
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 0, 2 * 2 + 0, {Rx1, Ux3, Rx3, Ux2, Rx1, Ux1, Rx3}}, // 19
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 0, 3 * 2 + 0, {Ux1, Rx1, Ux2, Rx3, Ux1, Rx1, Ux3, Rx3}}, // 20
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 0, 0 * 2 + 0, {Rx1, Ux2, Rx3, Ux3, Rx1, Ux1, Rx3}}, // 21


		std::tuple<int, int, std::vector<Move>>{0 * 3 + 2, 8 * 2 + 0, {Ux3, Rx1, Ux3, Rx3, Ux2, Rx1, Ux3, Rx3}}, // 22
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 2, 8 * 2 + 1, {Ux3, Rx1, Ux1, Rx3, Rot_y3, Ux1, Rx3, Ux3, Rx1}}, // 23
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 2, 1 * 2 + 1, {Rot_y3, Ux1, Rx3, Ux1, Rx1, Ux3, Rx3, Ux3, Rx1}}, // 24
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 2, 2 * 2 + 1, {Rot_y3, Rx3, Ux3, Rx1}}, // 25
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 2, 3 * 2 + 1, {Rot_y3, Ux1, Rx3, Ux3, Rx1, Ux3, Rx3, Ux3, Rx1}}, // 26
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 2, 0 * 2 + 1, {Rot_y3, Rx1, Ux2, Rx2, Ux3, Rx2, Ux3, Rx3}}, // 27
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 2, 1 * 2 + 0, {LowerLx1, Ux1, LowerRx1, Ux3, LowerRx3, Ux3, LowerLx3}}, // 28
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 2, 2 * 2 + 0, {Ux3, Rx1, Ux2, Rx3, Ux2, Rx1, Ux3, Rx3}}, // 29
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 2, 3 * 2 + 0, {Ux3, Rx1, Ux1, Rx3, Ux3, Rx1, Ux2, Rx3}}, // 30
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 2, 0 * 2 + 0, {Ux1, Rx1, Ux3, Rx3}}, // 31

		std::tuple<int, int, std::vector<Move>>{0 * 3 + 1, 8 * 2 + 0, {Ux3, Rx1, Ux2, Rx3, Ux1, Rx1, Ux1, Rx3}}, // 32
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 1, 8 * 2 + 1, {Rot_y3, Ux1, Rx3, Ux3, Rx1, LowerDx3, Rx1, Ux1, Rx3}}, // 33
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 1, 1 * 2 + 1, {Rot_y3, Ux3, Rx3, Ux1, Rx1}}, // 34
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 1, 2 * 2 + 1, {Rot_y3, Ux1, Rx3, Ux3, Rx1, Ux2, Rx3, Ux1, Rx1}}, // 35
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 1, 3 * 2 + 1, {Rot_y3, Ux1, Rx3, Ux2, Rx1, Ux2, Rx3, Ux1, Rx1}}, // 36
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 1, 0 * 2 + 1, {Rx1, Ux3, Rx3, Ux2, Rot_y3, Rx3, Ux3, Rx1}}, // 37
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 1, 1 * 2 + 0, {Rx1, Ux3, Rx3, Ux1, Rx1, Ux3, Rx3, Ux2, Rx1, Ux3, Rx3}}, // 38
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 1, 2 * 2 + 0, {Ux3, Rx1, Ux1, Rx3, Ux1, Rx1, Ux1, Rx3}}, // 39
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 1, 3 * 2 + 0, {Rx1, Ux1, Rx3}}, // 40
		std::tuple<int, int, std::vector<Move>>{0 * 3 + 1, 0 * 2 + 0, {Ux3, Rx1, Ux3, Rx3, Ux1, Rx1, Ux1, Rx3}} // 41

	};

	// {x + x * 3 + x * 9 + x * 27, x + x * 2 + x * 4 + x * 8,
	std::vector<std::tuple<int, int, std::vector<Move>>> HumanSolverCFOP::OLL_table =
	{
		std::tuple<int, int, std::vector<Move>>{1 + 2 * 3 + 1 * 9 + 2 * 27, 1 + 1 * 2 + 1 * 4 + 1 * 8, {Rx1, Ux2, Rx2, Fx1, Rx1, Fx3, Ux2, Rx3, Fx1, Rx1, Fx3}}, // 01
		std::tuple<int, int, std::vector<Move>>{2 + 2 * 3 + 1 * 9 + 1 * 27, 1 + 1 * 2 + 1 * 4 + 1 * 8, {Fx1, Rx1, Ux1, Rx3, Ux3, Fx3, LowerFx1, Rx1, Ux1, Rx3, Ux3, LowerFx3}}, // 02
		std::tuple<int, int, std::vector<Move>>{0 + 2 * 3 + 2 * 9 + 2 * 27, 1 + 1 * 2 + 1 * 4 + 1 * 8, {LowerFx1, Rx1, Ux1, Rx3, Ux3, LowerFx3, Ux3, Fx1, Rx1, Ux1, Rx3, Ux3, Fx3}}, // 03
		std::tuple<int, int, std::vector<Move>>{1 + 1 * 3 + 1 * 9 + 0 * 27, 1 + 1 * 2 + 1 * 4 + 1 * 8, {LowerFx1, Rx1, Ux1, Rx3, Ux3, LowerFx3, Ux1, Fx1, Rx1, Ux1, Rx3, Ux3, Fx3}}, // 04
		std::tuple<int, int, std::vector<Move>>{0 + 2 * 3 + 2 * 9 + 2 * 27, 0 + 0 * 2 + 1 * 4 + 1 * 8, {LowerRx3, Ux2, Rx1, Ux1, Rx3, Ux1, LowerRx1}}, // 05
		std::tuple<int, int, std::vector<Move>>{1 + 1 * 3 + 1 * 9 + 0 * 27, 0 + 1 * 2 + 1 * 4 + 0 * 8, {LowerRx1, Ux2, Rx3, Ux3, Rx1, Ux3, LowerRx3}}, // 06
		std::tuple<int, int, std::vector<Move>>{2 + 0 * 3 + 2 * 9 + 2 * 27, 1 + 1 * 2 + 0 * 4 + 0 * 8, {LowerRx1, Ux1, Rx3, Ux1, Rx1, Ux2, LowerRx3}}, // 07
		std::tuple<int, int, std::vector<Move>>{1 + 1 * 3 + 0 * 9 + 1 * 27, 1 + 0 * 2 + 0 * 4 + 1 * 8, {LowerRx3, Ux3, Rx1, Ux3, Rx3, Ux2, LowerRx1}}, // 08
		std::tuple<int, int, std::vector<Move>>{0 + 1 * 3 + 1 * 9 + 1 * 27, 1 + 1 * 2 + 0 * 4 + 0 * 8, {Rx1, Ux1, Rx3, Ux3, Rx3, Fx1, Rx2, Ux1, Rx3, Ux3, Fx3}}, // 09
		std::tuple<int, int, std::vector<Move>>{2 + 2 * 3 + 2 * 9 + 0 * 27, 1 + 0 * 2 + 0 * 4 + 1 * 8, {Rx1, Ux1, Rx3, Ux1, Rx3, Fx1, Rx1, Fx3, Rx1, Ux2, Rx3}}, // 10
		std::tuple<int, int, std::vector<Move>>{2 + 0 * 3 + 2 * 9 + 2 * 27, 0 + 0 * 2 + 1 * 4 + 1 * 8, {LowerRx3, Rx2, Ux1, Rx3, Ux1, Rx1, Ux2, Rx3, Ux1, Mx3}}, // 11
		std::tuple<int, int, std::vector<Move>>{1 + 1 * 3 + 0 * 9 + 1 * 27, 0 + 1 * 2 + 1 * 4 + 0 * 8, {Mx3, Rx3, Ux3, Rx1, Ux3, Rx3, Ux2, Rx1, Ux3, Mx1}}, // 12
		std::tuple<int, int, std::vector<Move>>{2 + 2 * 3 + 2 * 9 + 0 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {LowerFx1, Rx1, Ux1, Rx2, Ux3, Rx3, Ux1, Rx1, Ux3, LowerFx3}}, // 13
		std::tuple<int, int, std::vector<Move>>{0 + 1 * 3 + 1 * 9 + 1 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {Rx3, Fx1, Rx1, Ux1, Rx3, Fx3, Rx1, Fx1, Ux3, Fx3}}, // 14
		std::tuple<int, int, std::vector<Move>>{0 + 2 * 3 + 2 * 9 + 2 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {LowerRx3, Ux3, LowerRx1, Rx3, Ux3, Rx1, Ux1, LowerRx3, Ux1, LowerRx1}}, // 15
		std::tuple<int, int, std::vector<Move>>{1 + 1 * 3 + 1 * 9 + 0 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {LowerRx1, Ux1, LowerRx3, Rx1, Ux1, Rx3, Ux3, LowerRx1, Ux3, LowerRx3}}, // 16
		std::tuple<int, int, std::vector<Move>>{0 + 2 * 3 + 0 * 9 + 1 * 27, 1 + 1 * 2 + 1 * 4 + 1 * 8, {LowerLx1, Ux3, LowerLx3, LowerFx1, Rx2, Bx1, Rx3, Ux1, Rx3, Ux3, LowerFx3}}, // 17
		std::tuple<int, int, std::vector<Move>>{2 + 1 * 3 + 0 * 9 + 0 * 27, 1 + 1 * 2 + 1 * 4 + 1 * 8, {LowerRx1, Ux1, Rx3, Ux1, Rx1, Ux2, LowerRx3, LowerRx3, Ux3, Rx1, Ux3, Rx3, Ux2, LowerRx1}}, // 18
		std::tuple<int, int, std::vector<Move>>{1 + 2 * 3 + 0 * 9 + 0 * 27, 1 + 1 * 2 + 1 * 4 + 1 * 8, {LowerRx3, Rx1, Ux1, Rx1, Ux1, Rx3, Ux3, LowerRx1, Rx2, Fx1, Rx1, Fx3}}, // 19
		std::tuple<int, int, std::vector<Move>>{0 + 0 * 3 + 0 * 9 + 0 * 27, 1 + 1 * 2 + 1 * 4 + 1 * 8, {LowerRx1, Ux1, Rx3, Ux3, Mx2, Ux1, Rx1, Ux3, Rx3, Ux3, Mx3}}, // 20
		std::tuple<int, int, std::vector<Move>>{2 + 1 * 3 + 2 * 9 + 1 * 27, 0 + 0 * 2 + 0 * 4 + 0 * 8, {Rx1, Ux2, Rx3, Ux3, Rx1, Ux1, Rx3, Ux3, Rx1, Ux3, Rx3}}, // 21
		std::tuple<int, int, std::vector<Move>>{2 + 2 * 3 + 1 * 9 + 1 * 27, 0 + 0 * 2 + 0 * 4 + 0 * 8, {Rx1, Ux2, Rx2, Ux3, Rx2, Ux3, Rx2, Ux3, Ux3, Rx1}}, // 22
		std::tuple<int, int, std::vector<Move>>{0 + 0 * 3 + 2 * 9 + 1 * 27, 0 + 0 * 2 + 0 * 4 + 0 * 8, {Rx2, Dx3, Rx1, Ux2, Rx3, Dx1, Rx1, Ux2, Rx1}}, // 23
		std::tuple<int, int, std::vector<Move>>{0 + 1 * 3 + 2 * 9 + 0 * 27, 0 + 0 * 2 + 0 * 4 + 0 * 8, {LowerRx1, Ux1, Rx3, Ux3, LowerRx3, Fx1, Rx1, Fx3}}, // 24
		std::tuple<int, int, std::vector<Move>>{2 + 0 * 3 + 1 * 9 + 0 * 27, 0 + 0 * 2 + 0 * 4 + 0 * 8, {Fx3, LowerRx1, Ux1, Rx3, Ux3, LowerRx3, Fx1, Rx1}}, // 25
		std::tuple<int, int, std::vector<Move>>{1 + 1 * 3 + 1 * 9 + 0 * 27, 0 + 0 * 2 + 0 * 4 + 0 * 8, {Rx1, Ux2, Rx3, Ux3, Rx1, Ux3, Rx3}}, // 26
		std::tuple<int, int, std::vector<Move>>{2 + 0 * 3 + 2 * 9 + 2 * 27, 0 + 0 * 2 + 0 * 4 + 0 * 8, {Rx1, Ux1, Rx3, Ux1, Rx1, Ux2, Rx3}}, // 27
		std::tuple<int, int, std::vector<Move>>{0 + 0 * 3 + 0 * 9 + 0 * 27, 1 + 1 * 2 + 0 * 4 + 0 * 8, {LowerRx1, Ux1, Rx3, Ux3, LowerRx3, Rx1, Ux1, Rx1, Ux3, Rx3}}, // 28
		std::tuple<int, int, std::vector<Move>>{0 + 1 * 3 + 2 * 9 + 0 * 27, 1 + 1 * 2 + 0 * 4 + 0 * 8, {Rx1, Ux1, Rx3, Ux3, Rx1, Ux3, Rx3, Fx3, Ux3, Fx1, Rx1, Ux1, Rx3}}, // 29
		std::tuple<int, int, std::vector<Move>>{1 + 2 * 3 + 0 * 9 + 0 * 27, 0 + 0 * 2 + 1 * 4 + 1 * 8, {LowerFx1, Rx1, Ux1, Rx2, Ux3, Rx3, Ux1, Rx2, Ux3, Rx3, LowerFx3}}, // 30
		std::tuple<int, int, std::vector<Move>>{2 + 0 * 3 + 0 * 9 + 1 * 27, 1 + 0 * 2 + 0 * 4 + 1 * 8, {LowerRx3, Fx3, Ux1, Fx1, LowerRx1, Ux3, LowerRx3, Ux3, LowerRx1}}, // 31
		std::tuple<int, int, std::vector<Move>>{0 + 1 * 3 + 2 * 9 + 0 * 27, 0 + 0 * 2 + 1 * 4 + 1 * 8, {Rx1, Ux1, Bx3, Ux3, Rx3, Ux1, Rx1, Bx1, Rx3}}, // 32
		std::tuple<int, int, std::vector<Move>>{0 + 1 * 3 + 2 * 9 + 0 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {Rx1, Ux1, Rx3, Ux3, Rx3, Fx1, Rx1, Fx3}}, // 33
		std::tuple<int, int, std::vector<Move>>{0 + 0 * 3 + 1 * 9 + 2 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {Rx1, Ux1, Rx2, Ux3, Rx3, Fx1, Rx1, Ux1, Rx1, Ux3, Fx3}}, // 34
		std::tuple<int, int, std::vector<Move>>{0 + 1 * 3 + 0 * 9 + 2 * 27, 0 + 0 * 2 + 1 * 4 + 1 * 8, {Rx1, Ux2, Rx2, Fx1, Rx1, Fx3, Rx1, Ux2, Rx3}}, // 35
		std::tuple<int, int, std::vector<Move>>{0 + 1 * 3 + 0 * 9 + 2 * 27, 1 + 0 * 2 + 0 * 4 + 1 * 8, {Rx3, Ux3, Rx1, Ux3, Rx3, Ux1, Rx1, Ux1, LowerLx1, Ux3, Rx3, Ux1}}, // 36
		std::tuple<int, int, std::vector<Move>>{0 + 1 * 3 + 0 * 9 + 2 * 27, 1 + 1 * 2 + 0 * 4 + 0 * 8, {Fx1, Rx1, Ux3, Rx3, Ux3, Rx1, Ux1, Rx3, Fx3}}, // 37
		std::tuple<int, int, std::vector<Move>>{1 + 0 * 3 + 2 * 9 + 0 * 27, 1 + 1 * 2 + 0 * 4 + 0 * 8, {Rx1, Ux1, Rx3, Ux1, Rx1, Ux3, Rx3, Ux3, Rx3, Fx1, Rx1, Fx3}}, // 38
		std::tuple<int, int, std::vector<Move>>{2 + 0 * 3 + 1 * 9 + 0 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {Rx1, Ux1, Rx3, Fx3, Ux3, Fx1, Ux1, Rx1, Ux2, Rx3}}, // 39
		std::tuple<int, int, std::vector<Move>>{0 + 2 * 3 + 0 * 9 + 1 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {Rx3, Fx1, Rx1, Ux1, Rx3, Ux3, Fx3, Ux1, Rx1}}, // 40
		std::tuple<int, int, std::vector<Move>>{0 + 0 * 3 + 2 * 9 + 1 * 27, 1 + 1 * 2 + 0 * 4 + 0 * 8, {Rx1, Ux1, Rx3, Ux1, Rx1, Ux2, Rx3, Fx1, Rx1, Ux1, Rx3, Ux3, Fx3}}, // 41
		std::tuple<int, int, std::vector<Move>>{2 + 1 * 3 + 0 * 9 + 0 * 27, 1 + 0 * 2 + 0 * 4 + 1 * 8, {Rx3, Ux3, Rx1, Ux3, Rx3, Ux2, Rx1, Fx1, Rx1, Ux1, Rx3, Ux3, Fx3}}, // 42
		std::tuple<int, int, std::vector<Move>>{1 + 0 * 3 + 0 * 9 + 2 * 27, 1 + 0 * 2 + 0 * 4 + 1 * 8, {Bx3, Ux3, Rx3, Ux1, Rx1, Bx1}}, // 43
		std::tuple<int, int, std::vector<Move>>{0 + 2 * 3 + 1 * 9 + 0 * 27, 0 + 0 * 2 + 1 * 4 + 1 * 8, {LowerFx1, Rx1, Ux1, Rx3, Ux3, LowerFx3}}, // 44
		std::tuple<int, int, std::vector<Move>>{0 + 2 * 3 + 1 * 9 + 0 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {Fx1, Rx1, Ux1, Rx3, Ux3, Fx3}}, // 45
		std::tuple<int, int, std::vector<Move>>{1 + 0 * 3 + 0 * 9 + 2 * 27, 1 + 0 * 2 + 1 * 4 + 0 * 8, {Rx3, Ux3, Rx3, Fx1, Rx1, Fx3, Ux1, Rx1}}, // 46
		std::tuple<int, int, std::vector<Move>>{1 + 1 * 3 + 2 * 9 + 2 * 27, 0 + 1 * 2 + 1 * 4 + 0 * 8, {LowerBx3, Ux3, Rx3, Ux1, Rx1, Ux3, Rx3, Ux1, Rx1, LowerBx1}}, // 47
		std::tuple<int, int, std::vector<Move>>{2 + 2 * 3 + 1 * 9 + 1 * 27, 1 + 1 * 2 + 0 * 4 + 0 * 8, {Fx1, Rx1, Ux1, Rx3, Ux3, Rx1, Ux1, Rx3, Ux3, Fx3}}, // 48
		std::tuple<int, int, std::vector<Move>>{1 + 1 * 3 + 2 * 9 + 2 * 27, 1 + 0 * 2 + 0 * 4 + 1 * 8, {Rx1, Bx3, Rx2, Fx1, Rx2, Bx1, Rx2, Fx3, Rx1}}, // 49
		std::tuple<int, int, std::vector<Move>>{2 + 2 * 3 + 1 * 9 + 1 * 27, 0 + 0 * 2 + 1 * 4 + 1 * 8, {LowerRx3, Ux1, LowerRx2, Ux3, LowerRx2, Ux3, LowerRx2, Ux1, LowerRx3}}, // 50
		std::tuple<int, int, std::vector<Move>>{2 + 2 * 3 + 1 * 9 + 1 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {LowerFx1, Rx1, Ux1, Rx3, Ux3, Rx1, Ux1, Rx3, Ux3, LowerFx3}}, // 51
		std::tuple<int, int, std::vector<Move>>{2 + 2 * 3 + 1 * 9 + 1 * 27, 1 + 0 * 2 + 1 * 4 + 0 * 8, {Rx3, Fx3, Ux3, Fx1, Ux3, Rx1, Ux1, Rx3, Ux1, Rx1}}, // 52
		std::tuple<int, int, std::vector<Move>>{2 + 1 * 3 + 2 * 9 + 1 * 27, 1 + 0 * 2 + 0 * 4 + 1 * 8, {LowerRx3, Ux2, Rx1, Ux1, Rx3, Ux3, Rx1, Ux1, Rx3, Ux1, LowerRx1}}, // 53
		std::tuple<int, int, std::vector<Move>>{2 + 1 * 3 + 2 * 9 + 1 * 27, 1 + 1 * 2 + 0 * 4 + 0 * 8, {LowerRx1, Ux2, Rx3, Ux3, Rx1, Ux1, Rx3, Ux3, Rx1, Ux3, LowerRx3}}, // 54
		std::tuple<int, int, std::vector<Move>>{2 + 1 * 3 + 2 * 9 + 1 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {LowerRx1, Ux2, Rx3, Ux3, LowerRx3, Rx2, Ux1, Rx3, Ux3, LowerRx1, Ux3, LowerRx3}}, // 55
		std::tuple<int, int, std::vector<Move>>{1 + 2 * 3 + 1 * 9 + 2 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {LowerRx1, Ux1, LowerRx3, Ux1, Rx1, Ux3, Rx3, Ux1, Rx1, Ux3, Rx3, LowerRx1, Ux3, LowerRx3}}, // 56
		std::tuple<int, int, std::vector<Move>>{0 + 0 * 3 + 0 * 9 + 0 * 27, 0 + 1 * 2 + 0 * 4 + 1 * 8, {Rx1, Ux1, Rx3, Ux3, Mx3, Ux1, Rx1, Ux3, LowerRx3}} // 57
	};


	std::vector<std::tuple<int, int, std::vector<Move>>> HumanSolverCFOP::PLL_table =
	{
		std::tuple<int, int, std::vector<Move>>{permutation<4>{0,1,2,3}.getIndex(), permutation<4>{2,0,1,3}.getIndex(), {Mx2, Ux1, Mx1, Ux2, Mx3, Ux1, Mx2}}, // 01
		std::tuple<int, int, std::vector<Move>>{permutation<4>{0,1,2,3}.getIndex(), permutation<4>{1,2,0,3}.getIndex(), { Mx2, Ux3, Mx1, Ux2, Mx3, Ux3, Mx2 }}, // 02
		std::tuple<int, int, std::vector<Move>>{permutation<4>{0,1,2,3}.getIndex(), permutation<4>{2,3,0,1}.getIndex(), { Mx2, Ux1, Mx2, Ux2, Mx2, Ux1, Mx2 }}, // 03
		std::tuple<int, int, std::vector<Move>>{permutation<4>{0,1,2,3}.getIndex(), permutation<4>{3,2,1,0}.getIndex(), {Mx3, Ux1, Mx2, Ux1, Mx2, Ux1, Mx3, Ux2, Mx2, Ux3}}, // 04
		std::tuple<int, int, std::vector<Move>>{permutation<4>{1,3,2,0}.getIndex(), permutation<4>{0,1,2,3}.getIndex(), {Rot_x3, Rx2, Dx2, Rx3, Ux3, Rx1, Dx2, Rx3, Ux1, Rx3}}, // 05
		std::tuple<int, int, std::vector<Move>>{permutation<4>{3,0,2,1}.getIndex(), permutation<4>{0,1,2,3}.getIndex(), {Rot_x3, Rx1, Ux3, Rx1, Dx2, Rx3, Ux1, Rx1, Dx2, Rx2}}, // 06
		std::tuple<int, int, std::vector<Move>>{permutation<4>{3,2,1,0}.getIndex(), permutation<4>{0,1,2,3}.getIndex(), {Rot_x3, Rx1, Ux3, Rx3, Dx1, Rx1, Ux1, Rx3, Dx3, Rx1, Ux1, Rx3, Dx1, Rx1, Ux3, Rx3, Dx3}}, // 07
		std::tuple<int, int, std::vector<Move>>{permutation<4>{3,1,2,0}.getIndex(), permutation<4>{2,1,0,3}.getIndex(), {Rx1, Ux1, Rx3, Ux3, Rx3, Fx1, Rx2, Ux3, Rx3, Ux3, Rx1, Ux1, Rx3, Fx3}}, // 08
		std::tuple<int, int, std::vector<Move>>{permutation<4>{3,1,2,0}.getIndex(), permutation<4>{0,3,2,1}.getIndex(), {Rx3, Ux3, Fx3, Rx1, Ux1, Rx3, Ux3, Rx3, Fx1, Rx2, Ux3, Rx3, Ux3, Rx1, Ux1, Rx3, Ux1, Rx1}}, // 09
		std::tuple<int, int, std::vector<Move>>{permutation<4>{2,1,0,3}.getIndex(), permutation<4>{3,1,2,0}.getIndex(), {Rx3, Ux1, Rx3, LowerDx3, Rx3, Fx3, Rx2, Ux3, Rx3, Ux1, Rx3, Fx1, Rx1, Fx1}}, // 10
		std::tuple<int, int, std::vector<Move>>{permutation<4>{2,1,0,3}.getIndex(), permutation<4>{0,1,3,2}.getIndex(), {Fx1, Rx1, Ux3, Rx3, Ux3, Rx1, Ux1, Rx3, Fx3, Rx1, Ux1, Rx3, Ux3, Rx3, Fx1, Rx1, Fx3}}, // 11
		std::tuple<int, int, std::vector<Move>>{permutation<4>{1,0,2,3}.getIndex(), permutation<4>{1,0,2,3}.getIndex(), {Rot_z1, Ux3, Rx1, Dx3, Rx2, Ux1, Rx3, Ux3, Rx2, Ux1, Dx1, Rx3}}, // 12
		std::tuple<int, int, std::vector<Move>>{permutation<4>{3,1,2,0}.getIndex(), permutation<4>{1,0,2,3}.getIndex(), {Rx1, Ux1, Rx3, Fx3, Rx1, Ux1, Rx3, Ux3, Rx3, Fx1, Rx2, Ux3, Rx3, Ux3}}, // 13
		std::tuple<int, int, std::vector<Move>>{permutation<4>{0,1,3,2}.getIndex(), permutation<4>{1,0,2,3}.getIndex(), {Rx3, Ux2, Rx1, Ux2, Rx3, Fx1, Rx1, Ux1, Rx3, Ux3, Rx3, Fx3, Rx2, Ux3}}, // 14
		std::tuple<int, int, std::vector<Move>>{permutation<4>{3,1,2,0}.getIndex(), permutation<4>{0,1,3,2}.getIndex(), {Rx1, Ux3, Rx3, Ux3, Rx1, Ux1, Rx1, Dx1, Rx3, Ux3, Rx1, Dx3, Rx3, Ux2, Rx3, Ux3}}, // 15
		std::tuple<int, int, std::vector<Move>>{permutation<4>{2,0,1,3}.getIndex(), permutation<4>{1,2,0,3}.getIndex(), {Rx2, LowerUx3, Rx1, Ux3, Rx1, Ux1, Rx3, LowerUx1, Rx2, LowerFx1, Rx3, LowerFx3}}, // 16
		std::tuple<int, int, std::vector<Move>>{permutation<4>{0,2,3,1}.getIndex(), permutation<4>{0,3,1,2}.getIndex(), {Rx1, Ux1, Rx3, Rot_y3, Rx2, LowerUx3, Rx1, Ux3, Rx3, Ux1, Rx3, LowerUx1, Rx2}}, // 17
		std::tuple<int, int, std::vector<Move>>{permutation<4>{0,2,3,1}.getIndex(), permutation<4>{3,1,0,2}.getIndex(), {Rx2, LowerUx1, Rx3, Ux1, Rx3, Ux3, Rx1, LowerUx3, Rx2, Fx3, Ux1, Fx1}}, // 18
		std::tuple<int, int, std::vector<Move>>{permutation<4>{2,0,1,3}.getIndex(), permutation<4>{0,2,3,1}.getIndex(), {Rx3, LowerDx3, Fx1, Rx2, LowerUx1, Rx3, Ux1, Rx1, Ux3, Rx1, LowerUx3, Rx2}}, // 19
		std::tuple<int, int, std::vector<Move>>{permutation<4>{2,1,0,3}.getIndex(), permutation<4>{2,1,0,3}.getIndex(), {Rx3, Ux1, Rx1, Ux3, Rx3, Fx3, Ux3, Fx1, Rx1, Ux1, Rx3, Fx1, Rx3, Fx3, Rx1, Ux3, Rx1}}, // 20
		std::tuple<int, int, std::vector<Move>>{permutation<4>{0,3,2,1}.getIndex(), permutation<4>{2,1,0,3}.getIndex(), {Rx1, Ux1, Rx3, Ux1, Rx1, Ux1, Rx3, Fx3, Rx1, Ux1, Rx3, Ux3, Rx3, Fx1, Rx2, Ux3, Rx3, Ux2, Rx1, Ux3, Rx3}} // 21

	};


	void HumanSolverCFOP::init()
	{
	}

	std::vector<Move> HumanSolverCFOP::solve(const CubieCube & cc)
	{
		return solve(AxisCubieCube(AxisCube::eye(), cc));
	}

	std::vector<Move> HumanSolverCFOP::solve(const AxisCubieCube & cc)
	{
		return _solve(cc, 4);
	}

	std::vector<Move> HumanSolverCFOP::solve(const AxisCube & axis, const CubieCube & cc)
	{
		return solve(AxisCubieCube(axis, cc));
	}

	std::vector<Move> HumanSolverCFOP::solveCross(const CubieCube & cc)
	{
		return solveCross(AxisCubieCube(AxisCube::eye(), cc));
	}

	std::vector<Move> HumanSolverCFOP::solveCross(const AxisCubieCube & cc)
	{
		return _solve(cc, 1);
	}

	std::vector<Move> HumanSolverCFOP::solveCross(const AxisCube & axis, const CubieCube & cc)
	{
		return solveCross(AxisCubieCube(axis, cc));
	}

	std::vector<Move> HumanSolverCFOP::solveF2L(const CubieCube & cc)
	{
		return solveF2L(AxisCubieCube(AxisCube::eye(), cc));
	}

	std::vector<Move> HumanSolverCFOP::solveF2L(const AxisCubieCube & cc)
	{
		return _solve(cc, 2);
	}

	std::vector<Move> HumanSolverCFOP::solveF2L(const AxisCube & axis, const CubieCube & cc)
	{
		return solveF2L(AxisCubieCube(axis, cc));
	}

	std::vector<Move> HumanSolverCFOP::solveOLL(const CubieCube & cc)
	{
		return solveOLL(AxisCubieCube(AxisCube::eye(), cc));
	}

	std::vector<Move> HumanSolverCFOP::solveOLL(const AxisCubieCube & cc)
	{
		return  _solve(cc, 3);
	}

	std::vector<Move> HumanSolverCFOP::solveOLL(const AxisCube & axis, const CubieCube & cc)
	{
		return solveOLL(AxisCubieCube(axis, cc));
	}


	std::vector<Move> HumanSolverCFOP::_solveCross()
	{
		HumanSolverLBLSearch search;
		search.setCube(mySrcCube.getAbsCubie());
		myCrossMoves = search.solveDCross();
		myCrossCube = mySrcCube.move(myCrossMoves);

		return myCrossMoves;
	}

	std::vector<Move> HumanSolverCFOP::_solveF2L()
	{
		AxisCubieCube cc = myCrossCube;

		assert(cc.axis.iseye());
		assert(HumanSolver::isDCrossSolved(cc.cubie));

		vector<Move> moves;
		for (int i = 0; i < 4; ++i)
		{
			vector<Move> mv1 = _solveF2L_internal(HumanSolverLBL::conjCube[i].inv() * cc * HumanSolverLBL::conjCube[i]);

			vector<Move> mv2;
			if (i > 0)
				mv2.push_back(HumanSolverLBL::conjMove[i]);

			mv2.insert(mv2.end(), mv1.begin(), mv1.end());

			if (i > 0)
				mv2.push_back(HumanSolverLBL::conjMove[HumanSolverLBL::conjInv[i]]);


			cc = cc.move(mv2);
			moves.insert(moves.end(), mv2.begin(), mv2.end());
		}


		assert(cc.axis.iseye());
		assert(HumanSolver::isF2LSolved(cc.getAbsCubie()));
		assert(cc == myCrossCube.move(moves));

		myF2LCube = cc;
		myF2LMoves = moves;

		return moves;
	}

	std::vector<Move> HumanSolverCFOP::_solveOLL()
	{
		AxisCubieCube cc = myF2LCube;

		assert(cc.axis.iseye());
		assert(HumanSolver::isF2LSolved(cc.cubie));

		vector<Move> moves;

		if (HumanSolver::isOLLSolved(cc))
		{
			myOLLCube = cc;
			myOLLMoves = moves;
			return moves;
		}
			

		Move m1 = InvalidMove;
		int formula_idx = -1;

		

		for (int i = 0; i < 4; ++i)
		{
			AxisCubieCube dd = cc.move(HumanSolverLBL::conjMove[i]);

			const corner_orientation& co = dd.cubie.corner.ori;
			const edge_orientation& eo = dd.cubie.edge.ori;
			int corner_idx = co[0].get() + co[1].get() * 3 + co[2].get() * 9 + co[3].get() * 27;
			int edge_idx = eo[0].get() + eo[1].get() * 2 + eo[2].get() * 4 + eo[3].get() * 8;
			
			for (int j = 0; j < (int)OLL_table.size(); ++j)
			{
				if (corner_idx == get<0>(OLL_table[j]) && edge_idx == get<1>(OLL_table[j]))
				{
					formula_idx = j;
					m1 = HumanSolverLBL::conjMove[i];
					break;
				}
			}
		}

		assert(formula_idx >= 0);

		if (m1 != InvalidMove)
		{
			moves.push_back(m1);
			cc = cc.move(m1);
		}

		const vector<Move>& formula = get<2>(OLL_table[formula_idx]);
		HumanSolver::formula_append(moves, formula);
		cc = cc.move(formula);

		vector<Move> m2 = cc.axis.inv().getGenFormula();
		HumanSolver::formula_append(moves, m2);
		cc = cc.move(m2);


		assert(cc.axis.iseye());
		assert(HumanSolver::isOLLSolved(cc.getAbsCubie()));
		assert(cc == myF2LCube.move(moves));



		myOLLCube = cc;
		myOLLMoves = moves;

		return moves;
	}

	std::vector<Move> HumanSolverCFOP::_solvePLL()
	{
		// 顶层 permutaion eye, y, y2, y' 的表
		static permutation<4> p4y[4] = {
			{0,1,2,3},
			{3,0,1,2},
			{2,3,0,1},
			{1,2,3,0}
		};

		AxisCubieCube cc = myOLLCube;

		assert(cc.axis.iseye());
		assert(HumanSolver::isOLLSolved(cc.cubie));

		vector<Move> moves;


		Move m1[2] = { InvalidMove, InvalidMove };
		int formula_idx = -1;

		Move move_u[4] = { InvalidMove, Ux1, Ux2, Ux3 };



		// 匹配颜色
		for (int iu = 0; iu < 4 && formula_idx == -1; ++iu)
		{
			AxisCubieCube dd = cc.move(move_u[iu]);

			if (dd.isSolved())
			{
				cc = dd;

				if (move_u[iu] != InvalidMove)
					moves.push_back(move_u[iu]);
	
				formula_idx = 9999;
				break;
			}


			for (int iy = 0; iy < 4; ++iy)
			{
				//dd = dd.move(HumanSolverLBL::conjMove[iy]);



				const permutation<8>& cp = dd.cubie.corner.perm;
				const permutation<12>& ep = dd.cubie.edge.perm;

				permutation<4> cpp = permutation<4>{ cp[0], cp[1], cp[2], cp[3] };
				permutation<4> epp = permutation<4>{ ep[0], ep[1], ep[2], ep[3] };

				permutation<4> ccpp = p4y[iy].inv() * cpp * p4y[iy];
				permutation<4> eepp = p4y[iy].inv() * epp * p4y[iy];

				int corner_idx = ccpp.getIndex();
				int edge_idx = eepp.getIndex();

				for (int j = 0; j < (int)PLL_table.size(); ++j)
				{
					if (corner_idx == get<0>(PLL_table[j]) && edge_idx == get<1>(PLL_table[j]))
					{
						formula_idx = j;
						m1[0] = HumanSolverLBL::conjMove[iy];
						m1[1] = move_u[iu];
						break;
					}
				}
			}


			

		}

		assert(formula_idx >= 0);


		if (formula_idx < 9999)
		{
			for (int i = 0; i < 2; ++i)
			{
				if (m1[i] != InvalidMove)
				{
					moves.push_back(m1[i]);
					cc = cc.move(m1[i]);
				}
			}



			// 执行公式
			const vector<Move>& formula = get<2>(PLL_table[formula_idx]);
			HumanSolver::formula_append(moves, formula);
			cc = cc.move(formula);
		}

		


		// 调整axis
		vector<Move> m2 = cc.axis.inv().getGenFormula();
		HumanSolver::formula_append(moves, m2);
		cc = cc.move(m2);


		assert(cc.axis.iseye());
		assert(HumanSolver::isSolved(cc.getAbsCubie()));
		assert(cc.isSolved());
		assert(cc == myOLLCube.move(moves));



		myPLLCube = cc;
		myPLLMoves = moves;

		return moves;
	}

	std::vector<Move> HumanSolverCFOP::_solve(const AxisCubieCube & cc, int progress)
	{
		init();

		AxisCube axis = cc.axis.inv();
		vector<Move> ff = axis.getGenFormula();
		AxisCubieCube ss = AxisCubieCube::eye().move(ff);


		mySrcCube = ss * cc;
		myError = mySrcCube.cubie.verify();
		if (myError != 0)
		{
			myIsSolved = false;
			myMoves.clear();
			return myMoves;
		}

		if (cc.isSolved())
		{
			myIsSolved = true;
			myMoves.clear();
			return myMoves;
		}


		vector<Move>moves1, moves2, moves3, moves4;

		if (progress >= 1)
			moves1 = _solveCross();

		if (progress >= 2)
			moves2 = _solveF2L();

		if (progress >= 3)
			moves3 = _solveOLL();
		
		if (progress >= 4)
			moves4 = _solvePLL();



		vector<Move> moves;

		if (progress >= 1)
			moves.insert(moves.end(), moves1.begin(), moves1.end());

		if (progress >= 2)
			moves.insert(moves.end(), moves2.begin(), moves2.end());

		if (progress >= 3)
			moves.insert(moves.end(), moves3.begin(), moves3.end());

		if (progress >= 4)
			moves.insert(moves.end(), moves4.begin(), moves4.end());

		moves = HumanSolver::mergeRepetedRotateFormula(moves);


		myMoves = moves;

		if (progress >= 4)
			myIsSolved = true;

		return moves;
	}


	std::vector<Move> HumanSolverCFOP::_solveF2L_internal(const AxisCubieCube & cc)
	{
		assert(cc.axis.iseye());

		vector<Move> moves;
		AxisCubieCube dd = cc;
		AxisCubieCube pp = dd.inv();



		// 角块在位置 5,6,7, 移到顶面
		int corner_perm = pp.cubie.corner.perm[DFR];
		if (corner_perm == 5)
		{
			vector<Move> mv1 = { Rot_y3, Rx1, Ux1, Rx3, Rot_y1 };
			moves.insert(moves.end(), mv1.begin(), mv1.end());
			dd = dd.move(mv1);
		}
		else if (corner_perm == 6)
		{
			vector<Move> mv1 = { Rot_y2, Rx1, Ux1, Rx3, Rot_y2 };
			moves.insert(moves.end(), mv1.begin(), mv1.end());
			dd = dd.move(mv1);
		}
		else if (corner_perm == 7)
		{
			vector<Move> mv1 = { Rot_y1, Rx1, Ux1, Rx3, Rot_y3 };
			moves.insert(moves.end(), mv1.begin(), mv1.end());
			dd = dd.move(mv1);
		}

		pp = dd.inv();

		// 棱块在位置 9,10,11 移到顶面, 它下面的角块肯定是还没有复原的，可以移动
		corner_perm = pp.cubie.corner.perm[DFR];
		int edge_perm = pp.cubie.edge.perm[FR];

		if (edge_perm == 9)
		{
			if (corner_perm == 1)	// 移走角块
			{
				moves.push_back(Ux1);
				dd = dd.move(Ux1);
			}

			vector<Move> mv1 = { Rot_y3, Rx1, Ux1, Rx3, Rot_y1 };
			moves.insert(moves.end(), mv1.begin(), mv1.end());
			dd = dd.move(mv1);
		}
		else if (edge_perm == 10)
		{
			if (corner_perm == 2)	// 移走角块
			{
				moves.push_back(Ux1);
				dd = dd.move(Ux1);
			}

			vector<Move> mv1 = { Rot_y2, Rx1, Ux1, Rx3, Rot_y2 };
			moves.insert(moves.end(), mv1.begin(), mv1.end());
			dd = dd.move(mv1);
		}
		else if (edge_perm == 11)
		{
			if (corner_perm == 3)	// 移走角块
			{
				moves.push_back(Ux1);
				dd = dd.move(Ux1);
			}

			vector<Move> mv1 = { Rot_y1, Rx1, Ux1, Rx3, Rot_y3 };
			moves.insert(moves.end(), mv1.begin(), mv1.end());
			dd = dd.move(mv1);
		}

		pp = dd.inv();


		// 角块在顶面，移到位置0
		corner_perm = pp.cubie.corner.perm[DFR];
		assert(corner_perm <= 4);

		if (corner_perm == 1)
		{
			moves.push_back(Ux3);
			dd = dd.move(Ux3);
		}
		else if (corner_perm == 2)
		{
			moves.push_back(Ux2);
			dd = dd.move(Ux2);
		}
		else if (corner_perm == 3)
		{
			moves.push_back(Ux1);
			dd = dd.move(Ux1);
		}

		pp = dd.inv();

		// 角块在4, 棱块在顶面，移到到棱块使侧面颜色相同
		corner_perm = pp.cubie.corner.perm[DFR];
		edge_perm = pp.cubie.edge.perm[FR];
		int edge_ori = dd.cubie.edge.ori[edge_perm].get();

		if (corner_perm == 4 && edge_perm < 4)
		{
			Move m = InvalidMove;
			if (edge_ori == 0)	// 移到位置 0 
			{
				if (edge_perm == 1)
					m = Ux3;
				else if (edge_perm == 2)
					m = Ux2;
				else if (edge_perm == 3)
					m = Ux1;
			}
			else // 移到位置 1
			{
				if (edge_perm == 0)
					m = Ux1;
				else if (edge_perm == 2)
					m = Ux3;
				else if (edge_perm == 3)
					m = Ux2;
			}

			if (m != InvalidMove)
			{
				moves.push_back(m);
				dd = dd.move(m);
			}
		}

		pp = dd.inv();

		// 重新计算 idx
		corner_perm = pp.cubie.corner.perm[DFR];
		int corner_ori = dd.cubie.corner.ori[corner_perm].get();
		int corner_idx = corner_perm * 3 + corner_ori;

		edge_perm = pp.cubie.edge.perm[FR];
		edge_ori = dd.cubie.edge.ori[edge_perm].get();
		int edge_idx = edge_perm * 2 + edge_ori;

		if (corner_idx != 12 || edge_idx != 16)	// 未归位
		{
			// 查找公式
			int formula_idx = -1;
			for (int i = 0; i < (int)F2L_table.size(); ++i)
			{

				if (get<0>(F2L_table[i]) == corner_idx && get<1>(F2L_table[i]) == edge_idx)
				{
					formula_idx = i;

				}
			}

			assert(formula_idx >= 0);

			// 执行公式
			const vector<Move>& formula = get<2>(F2L_table[formula_idx]);

			moves.insert(moves.end(), formula.begin(), formula.end());
			dd = dd.move(formula);

			vector<Move> m2 = dd.axis.inv().getGenFormula();
			HumanSolver::formula_append(moves, m2);
			dd = dd.move(m2);
		}


		// 验证
		assert(dd.axis.iseye());
		assert(dd.cubie.corner.perm[DFR] == DFR);
		assert(dd.cubie.corner.ori[DFR] == 0);

		assert(dd.cubie.edge.perm[FR] == FR);
		assert(dd.cubie.edge.ori[FR] == 0);


		return moves;
	}


}