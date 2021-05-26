// 层先法AI教学

#include <cassert>
#include <vector>
#include <limits>
#include "AiTeachingLBL.h"



namespace AiTeachingLBL
{
	using namespace std;

	// ===========================================================================
	// teacher_base
	// ===========================================================================

	// 公式背包
	std::vector<std::vector<Move>> TeacherBase::formula_packsack =
	{
		{Rx1, Ux1, Rx3, Ux3},	// 0 上左下右
		{Rx1, Ux1, Rx1, Ux1, Rx1, Ux3, Rx3, Ux3, Rx3},	// 1 五顺四逆
		{Rx3, Ux3, Rx3, Ux3, Rx3, Ux1, Rx1, Ux1, Rx1},	// 2 五逆四顺
		{Fx1, Rx1, Ux1, Rx3, Ux3, Fx3},	// 3 十字公式
		{Rx1, Ux1, Rx3, Ux1, Rx1, Ux2, Rx3},	// 4 小鱼公式
		{Rx3, Ux1, Lx3, Ux2, Rx1, Ux3, Rx3, Ux2, Lx1, Rx1},	// 5 换角公式
		{Mx2n, Ux1, Mx1, Ux2, Mx3, Ux1, Mx2n}	// 7 换棱公式
	};

	void TeacherBase::setCurrStep(code_type a)
	{
		if (Step0_Basics <= a && a <= Step7_TopEdgePerm)
		{
			m_currStep = a;
		}
	}

	bool TeacherBase::isFinished(code_type state) const
	{
		if (!(Finished_Flower <= state && state <= Finished_All))
			return false;


		code_type s = checkOnlyFinishedState();

		return s >= state;
	}

	bool TeacherBase::isOnlyFinished(code_type state) const
	{
		if (!(Finished_Flower <= state && state <= Finished_All))
			return false;


		code_type s = checkOnlyFinishedState();

		return s == state;
	}

	code_type TeacherBase::checkOnlyFinishedState() const
	{
		code_type state = Finished_Init;

		const CubieCube& cc = m_currCube;

		if (cc.verify() != 0)	// Invalid
			return Invalid;


		// 检查是否完成了白色小花
		if (!_isFlowerDone())
			return state;

		state = Finished_Flower;


		// 检查是否完成底层十字
		if (!HumanSolver::isDCrossSolved(cc))
			return state;

		state = Finished_BottomCross;

		// 检查是否完成4个底层角块
		if (!HumanSolver::isOnlyDCornerSolved(cc))
			return state;

		state = Finished_BottomCorner;

		// 检查是否完成复原中层4个棱块
		if (!_checkOnlyMiddleEdge())
			return state;

		state = Finished_MiddleEdge;


		// 检查是否完成顶层十字
		if (!HumanSolver::isOnlyUEdgeOriSolved(cc))
			return state;

		state = Finished_TopCross;

		// 检查是否完成顶层4个角块色向
		if (!HumanSolver::isOnlyUCornerOriSolved(cc))
			return state;

		state = Finished_TopCornerOri;


		// 检查是否完成顶面4个角块的位置（或排列）
		if (!HumanSolver::isOnlyUCornerPermSolved(cc))
			return state;

		state = Finished_TopCornerPerm;
		
		
		// 检查是否完成顶面棱块的位置（或排列）
		if (!HumanSolver::isOnlyUEdgePermSolved(cc))
			return state;

		state = Finished_All;

		assert(cc.iseye());




		return state;
	}

	hint_type TeacherBase::step1_getHint()
	{

		hint_type result, invalid_result;
		result.code = Invalid;
		invalid_result.code = Invalid;


		if (m_currStep != Step1_BottomCross)
			return invalid_result;

		code_type state = checkOnlyFinishedState();

		if (state == Invalid)
			return invalid_result;


		if (state >= Finished_BottomCross)		// 已经完成了
		{
			result.code = Hint_Step1_Done;
			return result;
		}


		assert(state >= Finished_Init || state < Finished_BottomCross);


		bool flowerDone = (state == Finished_Flower);

		if (!flowerDone)	// 做小花
		{
			result = _step1_getHint_flower();
		}
		else
		{
			result = _step1_getHint_cross();
		}

		return result;
	}

	hint_type TeacherBase::step2_getHint()
	{
		hint_type result, invalid_result;
		result.code = Invalid;
		invalid_result.code = Invalid;


		if (m_currStep != Step2_BottomCorner)
			return invalid_result;

		code_type state = checkOnlyFinishedState();

		if (state == Invalid)
			return invalid_result;


		if (state >= Finished_BottomCorner)		// 已经完成了
		{
			result.code = Hint_Step2_Done;
			return result;
		}


		if (state < Finished_BottomCross)	// 还没有完成十字
		{
			result.code = Invalid;
			return result;
		}
		

		assert(state == Finished_BottomCross);


		const CubieCube& cc = m_currCube;

		CubieCube ss[4] = {
			CubieCube::eye(),
			CubieCube::eye().move(Rot_y1),
			CubieCube::eye().move(Rot_y2),
			CubieCube::eye().move(Rot_y3)
		};



		// 解目标角块DFR(4)在顶，返回 result.code 是 Hint_Step2_Top* 或 Invalid
		auto _get_hint_upper = [](const CubieCube& cc) -> hint_type
		{
			hint_type result;

			auto cp_inv = cc.corner.perm.inv();
			int pos = cp_inv[4];

			if (!(0 <= pos && pos < 4))	// 目标不在顶面
			{
				result.code = Invalid;
				return result;
			}

			int ori = cc.corner.ori[pos].get();

			// 需要转U
			if (pos == 1)
				result.pre_moves.push_back(Ux3);
			else if (pos == 2)
				result.pre_moves.push_back(Ux2);
			else if (pos == 3)
				result.pre_moves.push_back(Ux1);


			// 判断情况
			int n = 0;


			if (ori == 1)
			{
				result.code = Hint_Step2_Top1_ori1;
				n = 1;
			}
			else if (ori == 0)
			{
				result.code = Hint_Step2_Top2_ori0;
				n = 3;
			}
			else if (ori == 2)
			{
				result.code = Hint_Step2_Top3_ori2;
				n = 5;
			}


			for (int i = 0; i < n; ++i)
			{
				for (Move a : formula_packsack[Formula_RURU])
				{
					result.moves.push_back(a);
				}
				
			}


			return result;

		};


		// 解目标角块DFR(4)在底，返回 result.code 是 Hint_Step2_Top* 或 Invalid
		auto _get_hint_bottom = [](const CubieCube& cc) -> hint_type
		{
			hint_type result;

			auto cp_inv = cc.corner.perm.inv();
			int pos = cp_inv[4];

			if (!(4 <= pos && pos < 8))	// 目标不在底面
			{
				result.code = Invalid;
				return result;
			}


			int ori = cc.corner.ori[pos].get();

			// 需要转体
			if (pos == 5)
				result.rotates.push_back(Rot_y3);
			else if (pos == 6)
				result.rotates.push_back(Rot_y2);
			else if (pos == 7)
				result.rotates.push_back(Rot_y1);


			// 判断情况
			if (ori == 2)
				result.code = Hint_Step2_Bottom1_ori2;
			else if (ori == 1)
				result.code = Hint_Step2_Bottom2_ori1;
			else if (ori == 0)
				result.code = Hint_Step2_Bottom3_ori0;


			for (Move a : formula_packsack[Formula_RURU])
			{
				result.moves.push_back(a);
			}


			return result;

		};


		int found_idx = -1;
		int found_hint_code = std::numeric_limits<int>::max();
		hint_type found_hint;
		int done_cnt = 0;



		for (int i = 0; i < 4; ++i)
		{
			CubieCube dd = ss[i].inv() * cc * ss[i];


			// 查看目标是否归位
			if (dd.corner.perm[4] == 4 && dd.corner.ori[4] == 0)
			{
				++done_cnt;
				continue;
			}


			// 查看顶面是否存在目标
			hint_type hint = _get_hint_upper(dd);


			if (hint.code == Invalid)
				hint = _get_hint_bottom(dd);



			if (hint.code != Invalid && hint.code < found_hint_code)
			{
				found_idx = i;
				found_hint_code = hint.code;
				found_hint = hint;
			}
		}

		assert(done_cnt < 4);
		assert(found_idx != -1);


		assert(Hint_Step2_Top1_ori1 <= found_hint_code && found_hint_code <= Hint_Step2_Bottom3_ori0);

		result.code = found_hint.code;
		result.pre_moves = found_hint.pre_moves;
		result.moves = found_hint.moves;

		int rot_po = 0;


		if (found_hint.rotates.size() == 1)
		{
			rot_po = found_hint.rotates[0] % 3 + 1;
		}

		rot_po += found_idx;
		rot_po = rot_po % 4;

		if (rot_po > 0)	// 需要前置转体
		{
			result.rotates.push_back((Move)(Rot_y1 + rot_po - 1));
		}


		// 目标角块位置
		auto cp_inv = cc.corner.perm.inv();

		if (found_idx == 0)
			result.obj_corner.push_back((Corner)cp_inv[4]);
		else if (found_idx == 1)
			result.obj_corner.push_back((Corner)cp_inv[7]);
		else if (found_idx == 2)
			result.obj_corner.push_back((Corner)cp_inv[6]);
		else if (found_idx == 3)
			result.obj_corner.push_back((Corner)cp_inv[5]);

		return result;
	}

	hint_type TeacherBase::step3_getHint()
	{
		hint_type result, invalid_result;
		result.code = Invalid;
		invalid_result.code = Invalid;


		if (m_currStep != Step3_MiddleEdge)
			return invalid_result;

		code_type state = checkOnlyFinishedState();

		if (state == Invalid)
			return invalid_result;


		if (state >= Finished_MiddleEdge)		// 已经完成了
		{
			result.code = Hint_Step3_Done;
			return result;
		}


		if (state < Finished_BottomCorner)	// 还没有完成底层
		{
			result.code = Invalid;
			return result;
		}


		assert(state == Finished_BottomCorner);


		const CubieCube& cc = m_currCube;

		CubieCube ss[4] = {
			CubieCube::eye(),
			CubieCube::eye().move(Rot_y1),
			CubieCube::eye().move(Rot_y2),
			CubieCube::eye().move(Rot_y3)
		};



		// 解目标棱块 FR(8)，返回 result.code 是 Hint_Step3_* 或 OK
		auto _get_hint = [](const CubieCube& cc) -> hint_type
		{
			hint_type result;


			if (cc.edge.perm[8] == 8 && cc.edge.ori[8] == 0)
			{
				result.code = OK;
				return result;
			}


			int pos = cc.edge.perm.inv()[8];
			int ori = cc.edge.ori[pos].get();

			result.obj_edge.push_back((Edge)pos);

			if (0 <= pos && pos < 4)	// 目标在顶层
			{
				if (ori == 0) // 五逆四顺
				{
					result.code = Hint_Step3_5CCW;
					result.moves = formula_packsack[Formula_5CCW];

					// 移到 UR(0) 位置
					if (pos == 1)
						result.pre_moves.push_back(Ux3);
					else if (pos == 2)
						result.pre_moves.push_back(Ux2);
					else if (pos == 3)
						result.pre_moves.push_back(Ux1);
				}
				else	// 五顺四逆
				{
					result.code = Hint_Step3_5CW;
					result.moves = formula_packsack[Formula_5CW];

					result.rotates.push_back(Rot_y3);

					// 移到 UF(1) 位置
					if (pos == 2)
						result.pre_moves.push_back(Ux3);
					else if (pos == 3)
						result.pre_moves.push_back(Ux2);
					else if (pos == 0)
						result.pre_moves.push_back(Ux1);
				}


			}
			else if (8 <= pos && pos < 12) // 目标在中层
			{
				result.code = Hint_Step3_NotOnTop;

				// 需要转体
				if (pos == 9)
					result.rotates.push_back(Rot_y3);
				else if (pos == 10)
					result.rotates.push_back(Rot_y2);
				else if (pos == 11)
					result.rotates.push_back(Rot_y1);


				result.moves = formula_packsack[Formula_5CCW];


			}
			else
			{
				throw "unreachable";
			}// if


			return result;

		}; // _get_hint


		int found_idx = -1;
		int found_hint_code = std::numeric_limits<int>::max();
		hint_type found_hint;
		int done_cnt = 0;



		for (int i = 0; i < 4; ++i)
		{
			CubieCube dd = ss[i].inv() * cc * ss[i];

			hint_type hint = _get_hint(dd);

			// 查看目标是否归位
			if (hint.code == OK)
			{
				++done_cnt;
				continue;
			}


			if (hint.code < found_hint_code)
			{
				found_idx = i;
				found_hint_code = hint.code;
				found_hint = hint;
			}
		}

		assert(done_cnt < 4);
		assert(found_idx != -1);


		assert(Hint_Step3_5CW <= found_hint_code && found_hint_code <= Hint_Step3_NotOnTop);

		result.code = found_hint.code;
		result.pre_moves = found_hint.pre_moves;
		result.moves = found_hint.moves;

		int rot_po = 0;


		if (found_hint.rotates.size() == 1)
		{
			rot_po = found_hint.rotates[0] % 3 + 1;
		}

		rot_po += found_idx;
		rot_po = rot_po % 4;

		if (rot_po > 0)	// 需要前置转体
		{
			result.rotates.push_back((Move)(Rot_y1 + rot_po - 1));
		}


		// 目标棱块位置
		result.obj_edge.push_back((Edge)ss[found_idx].edge.perm[found_hint.obj_edge[0]]);


		return result;
	}

	hint_type TeacherBase::step4_getHint()
	{
		hint_type result, invalid_result;
		result.code = Invalid;
		invalid_result.code = Invalid;


		if (m_currStep != Step4_TopCross)
			return invalid_result;

		code_type state = checkOnlyFinishedState();

		if (state == Invalid)
			return invalid_result;


		if (state >= Finished_TopCross)		// 已经完成了
		{
			result.code = Hint_Step4_Done;
			return result;
		}


		if (state < Finished_MiddleEdge)	// 还没有完成前两层
		{
			result.code = Invalid;
			return result;
		}


		assert(state == Finished_MiddleEdge);


		const CubieCube& cc = m_currCube;


		// 顶层棱块黄色朝上的数量
		int numYellow = 0;

		for (int i = 0; i < 4; ++i)
		{
			if (cc.edge.ori[i].get() == 0)
			{
				++numYellow;

				result.obj_facelet.push_back(edgeFacelet[i][0]);
			}

			
		}

		result.obj_facelet.push_back(U5);
			


		if (numYellow == 0)	// 单点
		{
			result.code = Hint_Step4_Point;
		}
		else if (numYellow == 2)
		{
			if (cc.edge.ori[0] == cc.edge.ori[2])	// 一字
			{
				result.code = Hint_Step4_Line;

				if (cc.edge.ori[0].get() == 1) // 需要转 U
				{
					result.pre_moves.push_back(Ux1);
				}
			}
			else // 拐角
			{
				result.code = Hint_Step4_Corner;

				// 转 U

				if (cc.edge.ori[0].get() == 1 && cc.edge.ori[1].get() == 0)
					result.pre_moves.push_back(Ux1);
				else if (cc.edge.ori[0].get() == 0 && cc.edge.ori[1].get() == 0)
					result.pre_moves.push_back(Ux2);
				else if (cc.edge.ori[0].get() == 0 && cc.edge.ori[1].get() == 1)
					result.pre_moves.push_back(Ux3);
			}
		}
		else
			throw "unreachable";


		result.moves = formula_packsack[Formula_Cross];


		return result;
	}

	hint_type TeacherBase::step5_getHint()
	{
		hint_type result, invalid_result;
		result.code = Invalid;
		invalid_result.code = Invalid;


		if (m_currStep != Step5_TopCornerOri)
			return invalid_result;

		code_type state = checkOnlyFinishedState();

		if (state == Invalid)
			return invalid_result;


		if (state >= Finished_TopCornerOri)		// 已经完成了
		{
			result.code = Hint_Step5_Done;
			return result;
		}


		if (state < Finished_TopCross)	// 还没有完成顶面十字
		{
			result.code = Invalid;
			return result;
		}


		assert(state == Finished_TopCross);


		const CubieCube& cc = m_currCube;


		// 顶面角块不是黄色朝上
		int numNY = 0;

		for (int i = 0; i < 4; ++i)
		{
			numNY += (cc.corner.ori[i].get() != 0);
		}


		// 高亮所有黄色贴纸
		ColorCube colorCube = cc.toColorCube();

		static Facelet hightlight_idx[] = {
			U1, U2, U3, U4, U5, U6, U7, U8, U9,
			R1, R2, R3,
			F1, F2, F3,
			L1, L2, L3,
			B1, B2, B3
		};

		for (Facelet a : hightlight_idx)
		{
			if (colorCube[a] == U)
				result.obj_facelet.push_back(a);
		}


		int ref_ori;

		if (numNY == 2)	// 顶面2个角块不是黄色,口诀：2前
		{
			result.code = Hint_Step5_NY2;
			ref_ori = 1;

		}
		else if (numNY == 3) // 顶面3个角块不是黄色,即小鱼，口诀：3上
		{
			result.code = Hint_Step5_NY3_Fish;
			ref_ori = 0;
		}
		else if (numNY == 4)  // 顶面4个角块不是黄色，即十字口诀：4左
		{
			result.code = Hint_Step5_NY4_Cross;
			ref_ori = 2;
		}
		else
			throw "unreachable";


		// 根据UFL角转 U
		auto dd = cc;
		int i;
		for (i = 0; i < 4; ++i)
		{
			if (dd.corner.ori[1] == ref_ori)
				break;

			dd = dd.move(Ux1);
		}

		if (i > 0)
			result.pre_moves.push_back((Move)(Ux1 + i - 1));

		result.moves = formula_packsack[Formula_Fish];


		return result;
	}

	hint_type TeacherBase::step6_getHint()
	{
		hint_type result, invalid_result;
		result.code = Invalid;
		invalid_result.code = Invalid;


		if (m_currStep != Step6_TopCornerPerm)
			return invalid_result;

		code_type state = checkOnlyFinishedState();

		if (state == Invalid)
			return invalid_result;


		if (state >= Finished_TopCornerPerm)		// 已经完成了
		{
			result.code = Hint_Step6_Done;
			return result;
		}


		if (state < Finished_TopCornerOri)	// 还没有完成顶面十字
		{
			result.code = Invalid;
			return result;
		}


		assert(state == Finished_TopCornerOri);


		const CubieCube& cc = m_currCube;


		// 眼的数量
		int numEyes = 0;

		result.obj_facelet = vector<Facelet>{ U1, U2, U3, U4, U5, U6, U7, U8, U9 };

		static Facelet hightlight_idx[][2] = {
			{F1, F3},
			{L1, L3},
			{B1, B3},
			{R1, R3}
		};

		for (int i = 0; i < 4; ++i)
		{
			int j = (i + 1) % 4;

			int d = (cc.corner.perm[j] - cc.corner.perm[i]) % 4;
			d = d < 0 ? d + 4 : d;

			if (d == 1)
			{
				++numEyes;

				for (Facelet a : hightlight_idx[i])
				{
					result.obj_facelet.push_back(a);
				}
			}
		}



		// 判断情况
		if (numEyes == 0)	// 没有眼
		{
			result.code = Hint_Step6_no_eye;
			result.moves = formula_packsack[Formula_SwapCorner];
		}
		else if (numEyes == 1) // 一对眼
		{
			result.code = Hint_Step6_1eye;

			// 把眼转到前面来
			auto dd = cc;
			int i;
			for (i = 0; i < 4; ++i)
			{
				int d = (dd.corner.perm[1] - dd.corner.perm[0]) % 4;
				d = d < 0 ? d + 4 : d;

				if (d == 1)
					break;

				dd = dd.move(Ux1);
			}

			if (i > 0)
				result.pre_moves.push_back((Move)(Ux1 + i - 1));

			result.moves = formula_packsack[Formula_SwapCorner];

		}
		else if (numEyes == 4)  // 4对眼,但还没有转 U 做最后的调整
		{
			result.code = Hint_Step6_4eyes;

			// 最后调整 U
			auto dd = cc;
			int i;
			for (i = 0; i < 4; ++i)
			{
				if (dd.corner.perm[0] == 0)
					break;

				dd = dd.move(Ux1);
			}

			if (i > 0)
				result.pre_moves.push_back((Move)(Ux1 + i - 1));

		}
		else
			throw "unreachable";


		return result;
	}

	hint_type TeacherBase::step7_getHint()
	{
		hint_type result, invalid_result;
		result.code = Invalid;
		invalid_result.code = Invalid;


		if (m_currStep != Step7_TopEdgePerm)
			return invalid_result;

		code_type state = checkOnlyFinishedState();

		if (state == Invalid)
			return invalid_result;


		if (state >= Finished_All)		// 已经完成了
		{
			result.code = Hint_Step7_Done;
			return result;
		}


		if (state < Finished_TopCornerPerm)	// 还没有完成顶面十字
		{
			result.code = Invalid;
			return result;
		}


		assert(state == Finished_TopCornerPerm);


		const CubieCube& cc = m_currCube;


		// 眼的数量
		int numLines = 0;
		int lineIdx = -1;

		

		static Facelet hightlight_idx[] = {R2, F2, L2, B2};

		for (int i = 0; i < 4; ++i)
		{
			if (cc.edge.perm[i] == i)
			{
				++numLines;
				lineIdx = i;
			}
			else
			{
				result.obj_facelet.push_back(hightlight_idx[i]);
			}
		}



		// 判断情况
		if (numLines == 0)	// 没有一排线
		{
			result.code = Hint_Step7_no_line;
			result.moves = formula_packsack[Formula_SwapEdge];
		}
		else if (numLines == 1) // 有一排线
		{
			result.code = Hint_Step7_has_line;

			// 把一排线转到后面
			if (lineIdx == 0)
				result.rotates.push_back(Rot_y3);
			else if (lineIdx == 1)
				result.rotates.push_back(Rot_y2);
			else if (lineIdx == 2)
				result.rotates.push_back(Rot_y1);

			result.moves = formula_packsack[Formula_SwapEdge];

		}
		else
			throw "unreachable";


		return result;
	}

	std::vector<Facelet> TeacherBase::getF2LFacelets()
	{
		std::vector<Facelet> result =
		{
			R4, R5, R6, R7, R8, R9,
			F4, F5, F6, F7, F8, F9,
			D1, D2, D3, D4, D5, D6, D7, D8, D9,
			L4, L5, L6, L7, L8, L9,
			B4, B5, B6, B7, B8, B9
		};

		return result;
	}


	bool TeacherBase::_isFlowerDone() const
	{
		const CubieCube& cc = m_currCube;

		auto ep_inv = cc.edge.perm.inv();

		bool b = true;
		for (int i = 4; i < 8; ++i)
		{
			int pos = ep_inv[i];
			int ori = cc.edge.ori[pos].get();

			if (ori != 0)	// 方向都是 0
			{
				b = false;
				break;
			}

			bool on_bottom = (pos == i);

			if (!(on_bottom || (0 <= pos && pos < 4)))		// 底层棱块都在顶层或者已经归位
			{
				b = false;
				break;
			}


		}

		return b;
	}

	bool TeacherBase::_checkOnlyMiddleEdge() const
	{
		const CubieCube& cc = m_currCube;

		bool b = true;
		for (int i = 8; i < 12; ++i)
		{
			if (!(cc.edge.perm[i] == i && cc.edge.ori[i] == 0))
			{
				b = false;
				break;
			}
		}

		return b;
	}

	hint_type TeacherBase::_step1_getHint_flower()
	{


		const CubieCube& cc = m_currCube;

		CubieCube rot_cc[4];


		cs::permutation<12> rx_ep[4];
		cs::permutation<12> rx_ep_inv[4];
		cs::edge_orientation rx_eo[4];

		CubieCube dd = cc;
		for (int i = 0; i < 4; ++i)
		{
			rx_ep[i] = dd.edge.perm;
			rx_ep_inv[i] = dd.edge.perm.inv();
			rx_eo[i] = dd.edge.ori;

			rot_cc[i] = dd;

			dd = dd.move(Rot_y1);
		}


		// 简单计算块 i 的情况, 已经完成，则返回 OK, 不在右侧，返回 Hint_NeedRotate
		// 否则返回情况码 Hint_Step1_Flower*
		auto _hint = [](
			const edge_orientation& eo,
			const permutation<12> ep_inv,
			int i) -> code_type
		{
			int pos = ep_inv[i];	// 棱块 i 所在的位置

			bool on_top = (0 <= pos && pos < 4);
			bool done1 = on_top && (eo[pos] == 0);	// 已经完成

			if (done1)
			{
				return OK;
			}

			bool on_right = (pos == UR || pos == DR || pos == FR);

			if (!on_right)
				return Hint_NeedRotate;


			code_type res;

			int oo = eo[pos].get();

			if (pos == FR)
			{
				if (oo == 0)
					res = Hint_Step1_Flower1a;
				else
					res = Hint_Step1_Flower2a;
			}
			else if (pos == DR)
			{
				if (oo == 0)
					res = Hint_Step1_Flower1b;
				else
					res = Hint_Step1_Flower2b;
			}
			else
			{
				assert(pos == UR);
				assert(oo != 0);

				res = Hint_Step1_Flower3;
			}

			return res;

		};


		int found = -1;
		int found_hint = std::numeric_limits<int>::max();
		int found_rot = 0;

		int done_cnt = 0;

		for (int i = 4; i < 8; ++i)		// 遍历目标4个块, 选择最优先一个
		{
			for (int j = 0; j < 4; ++j)
			{
				code_type code = _hint(rx_eo[j], rx_ep_inv[j], i);

				if (code == OK)
				{
					++done_cnt;
					break;
				}
				else if (code == Hint_NeedRotate)
				{
					continue;
				}
				else
				{
					assert(Hint_Step1_Flower1a <= code && code <= Hint_Step1_Flower3);

					if (code < found_hint)
					{
						found = i;
						found_hint = code;
						found_rot = j;
					}

					break;
				}

			}// for j
		}// for i

		assert(done_cnt < 4);
		assert(found != -1);


		// 公式
		static vector<Move> formuas[] = {
			vector<Move>{Rx1},	// Hint_Step1_Flower1a,			// 小花情况1b：白色在 FR 朝前
			vector<Move>{Fx3},	// Hint_Step1_Flower2a,			// 小花情况2b：白色在 FR 朝右
			vector<Move>{Rx2},	// Hint_Step1_Flower1b,			// 小花情况1a：白色在 DR 朝下
			vector<Move>{Rx1, Fx3},	// Hint_Step1_Flower2b,			// 小花情况2a：白色在 DR 朝右
			vector<Move>{Rx3, Fx3}	// Hint_Step1_Flower3,				// 小花情况3：白色在 UR 朝右
		};


		static array<Edge, 4> obj[] = 
		{
			// eye, y1, y2, y3
			{FR, BR, BL, FL},	// Hint_Step1_Flower1a,			// 小花情况1b：白色在 FR 朝前
			{FR, BR, BL, FL},	// Hint_Step1_Flower2a,			// 小花情况2b：白色在 FR 朝右
			{DR, DB, DL, DF},	// Hint_Step1_Flower1b,			// 小花情况1a：白色在 DR 朝下
			{DR, DB, DL, DF},	// Hint_Step1_Flower2b,			// 小花情况2a：白色在 DR 朝右
			{UR, UB, UL, UF}	// Hint_Step1_Flower3,			// 小花情况3：白色在 UR 朝右
		};




		hint_type res;
		res.code = (code_type)found_hint;
		
		const auto& raw_moves = formuas[(int)res.code - Hint_Step1_Flower1a];


		// 注意顶面转U避开转动

		dd = rot_cc[found_rot];

		for (Move m : raw_moves)
		{
			int ax = m / 3;

			int pos;
			if (ax == R)
				pos = 0;
			else if (ax == F)
				pos = 1;
			else
				throw "unreachable";

			int i = 0;
			
			for (; i < 4; ++i)
			{

				int x = dd.edge.perm[pos];
				int o = dd.edge.ori[pos].get();

				if (4 <= x && x < 8 && o == 0)	// 需要转U
				{
					dd = dd.move(Ux1);
				}
				else
				{
					break;
				}
			}

			if (i > 0)
				res.moves.push_back(Move(Ux1 + i - 1));

			res.moves.push_back(m);

			
		}
		
		if (found_rot > 0)
		{
			res.rotates = { Move(Rot_y1 + found_rot - 1) };
		}

		res.obj_edge.push_back(obj[(int)res.code - Hint_Step1_Flower1a][found_rot]);

		return res;
	}

	hint_type TeacherBase::_step1_getHint_cross()
	{
		
		hint_type hint;
		hint.code = Hint_Step1_FinalUAndR2;

		const CubieCube& cc = m_currCube;

		CubieCube S[4];

		S[0] = CubieCube::eye();

		for (int i = 1; i < 4; ++i)
		{
			S[i] = S[i - 1].move(Rot_y1);
		}

		for (int i = 0; i < 4; ++i)
		{
			CubieCube dd0 = S[i].inv() * cc * S[i];
			CubieCube dd = dd0;

			if (dd.edge.perm[4] == 4)
				continue;

			int j = 0;

			bool found = false;

			for (; j < 4; ++j)
			{
				if (dd.edge.perm[0] == 4)
				{
					found = true;
					break;
				}

				dd = dd.move(Ux1);
			}

			assert(found);

			if (i > 0)
				hint.rotates.push_back(Move(Rot_y1 + i - 1));

			if (j > 0)
				hint.pre_moves.push_back(Move(Ux1 + j - 1));
				

			hint.moves.push_back(Rx2);

			auto ep_inv = dd0.edge.perm.inv();

			int pos = S[i].edge.perm[ep_inv[4]];
			hint.obj_edge.push_back((Edge)pos);

			break;


		}// for i




		return hint;

	}



	Teacher::Teacher()
	{
		setBottomColor(U);
	}

	void Teacher::setBottomColor(Color c)
	{
		m_bottomColor = c;

		// 转动 m_currCube 使底面颜色正确， 前面颜色不确定
		for (int i = 0; i < 24; ++i)
		{
			const AxisCube axis = AxisCube::valid_table[i];
			if (axis[3] == c)
			{
				m_currCube = m_currCube.rotateToAxis(axis);
			}
		}
	}

	void Teacher::setCurrCube(const CubieCube & cc)
	{
		AxisCubieCube dd = getCurrCube();
		dd.setAbsCubie(cc);
		setCurrCube(dd);
	}

	void Teacher::setCurrCube(const AxisCubieCube & cc)
	{
		if (cc.axis[3] == m_bottomColor)
			m_currCube = cc;
		else
			m_currCube = cc.rotateToAxis(m_currCube.axis);


		// 设置 _base 的 cubie, 设 m_currCube = S * M, 则 _base::m_currCube = eye() * M
		vector<Move> ff = m_currCube.axis.inv().getGenFormula();
		AxisCubieCube ss = AxisCubieCube::eye().move(ff);
		AxisCubieCube dd = ss * m_currCube;

		assert(dd.axis.iseye());

		_base::m_currCube = dd.cubie;
	}

	std::vector<Corner> Teacher::objCornerRel2Abs(const std::vector<Corner>& src)
	{
		std::vector<Corner> res;

		for (size_t i = 0; i < src.size(); ++i)
			res[i] = (Corner)(m_currCube.cubie.corner.perm[src[i]]);

		return res;
	}

	std::vector<Edge> Teacher::objEdgeRel2Abs(const std::vector<Edge>& src)
	{
		std::vector<Edge> res;

		for (size_t i = 0; i < src.size(); ++i)
			res[i] = (Edge)(m_currCube.cubie.edge.perm[src[i]]);

		return res;
	}

	std::vector<Facelet> Teacher::objFaceletRel2Abs(const std::vector<Facelet>& src)
	{
		std::vector<Facelet> res;

		FaceCube fc = m_currCube.toColorCube().toFaceCube();

		for (size_t i = 0; i < src.size(); ++i)
			res[i] = (Facelet)(fc[src[i]]);

		return res;
	}

	bool isEdge(piece_code c)
	{
		return piece_code::UR <= c && c <= piece_code::BR;
	}

	bool isCorner(piece_code c)
	{
		return piece_code::URF <= c && c <= piece_code::DRB;
	}

	bool isCenter(piece_code c)
	{
		return piece_code::UU <= c && c <= piece_code::BB;
	}

	Edge toEdge(piece_code c)
	{
		if (isEdge(c))
			return (Edge)-1;

		return Edge(c);
	}

	Corner toCorner(piece_code c)
	{
		if (isEdge(c))
			return (Corner)-1;

		return Corner((int)c - 20);
	}

	Color toCenter(piece_code c)
	{
		if (isEdge(c))
			return (Color)-1;

		return Color((int)c - 30);
	}

	piece_code toPieceCode(Edge c)
	{
		return piece_code(c);
	}

	piece_code toPieceCode(Corner c)
	{
		return piece_code(20 + (int)c);
	}

	piece_code toPieceCode(Color c)
	{
		return piece_code(30 + (int)c);
	}

}