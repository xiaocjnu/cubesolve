// 层先法AI教学


#pragma once

#include "cs.h"
#include <vector>


namespace AiTeachingLBL
{
	using namespace cs;


	// 角块棱块的编码，把角块和棱块统一到一个编码中
	enum class piece_code
	{
		Invalid = -1,
		//Edges, 与 cs::Edge 编码相同
		// 0    1   2   3   4   5   6   7   8   9   10  11
		UR = 0, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR,

		// Corners, = cs::Corner + 20
		//20+ [0    1    2    3    4    5    6    7]
		URF = 20,  UFL, ULB, UBR, DFR, DLF, DBL, DRB,

		// Centers = cs::Color + 30
		// 30 + [0 1   2   3   4   5]
		UU = 30,   RR, FF, DD, LL, BB
	};


	bool isEdge(piece_code c);
	bool isCorner(piece_code c);
	bool isCenter(piece_code c);

	Edge toEdge(piece_code c);
	Corner toCorner(piece_code c);
	Color toCenter(piece_code c);

	piece_code toPieceCode(Edge c);
	piece_code toPieceCode(Corner c);
	piece_code toPieceCode(Color c);	// center to piece_code


	// 把枚举当常量使用，复原过程中所用到的状态常量，名字按照白色底的名称
	enum code_type
	{
		Invalid = -1,	// 非法状态，可用在任意非法的地方
		OK = 0,		// OK, 可以用在一起OK的地方，例如已经复原


		// 教学步骤
		Step0_Basics = 100,			// 基础知识
		Step1_BottomCross,			// 底层十字架
		Step2_BottomCorner,			// 复原底层4个白色角块
		Step3_MiddleEdge,			// 复原中层4个棱块
		Step4_TopCross,				// 复原顶层黄色十字
		Step5_TopCornerOri,			// 复原顶层4个角块色向，即完成顶面黄色
		Step6_TopCornerPerm,		// 复原顶面4个角块的位置（或排列）
		Step7_TopEdgePerm,			// 复原顶面棱块的位置（或排列）


		// 已完成进度, 后一个进度包含前一个
		Finished_Init = 200,			// 初始状态，什么都没有完成
		Finished_Flower,				// 已经完成了小花
		Finished_BottomCross,			// 已经完成白色十字架
		Finished_BottomCorner,			// 已经完成复原底层4个白色角块
		Finished_MiddleEdge,			// 已经完成复原中层4个棱块
		Finished_TopCross,				// 已经完成复原顶层黄色十字
		Finished_TopCornerOri,			// 已经完成复原顶层4个角块色向，即完成顶面黄色
		Finished_TopCornerPerm,			// 已经完成复原顶面4个角块的位置（或排列）
		Finished_All,					// 已经完成复原全部


		// 提示码 for 中间过程，xxxx_getHint 并不返回

		Hint_NeedRotate = 300,			// 需要转体
		Hint_NeedU,						// 需要转 U

		// 提示码 for step1, 情况优先级高在前
		Hint_Step1_Done = 400,			// 已经完成了这一步

		Hint_Step1_Flower1a,			// 小花情况1b：白色在 FR 朝前
		Hint_Step1_Flower2a,			// 小花情况2b：白色在 FR 朝右
		Hint_Step1_Flower1b,			// 小花情况1a：白色在 DR 朝下
		Hint_Step1_Flower2b,			// 小花情况2a：白色在 DR 朝右
		Hint_Step1_Flower3,				// 小花情况3：白色在 UR 朝右

		Hint_Step1_FinalUAndR2,			// 小花已经完成，可以直接转到底层


		// 提示码 for step2
		Hint_Step2_Done = 420,				// 已经完成了这一步

		Hint_Step2_Top1_ori1,			// 目标块在顶面，白色朝右
		Hint_Step2_Top2_ori0,			// 目标块在顶面，白色朝上
		Hint_Step2_Top3_ori2,			// 目标块在顶面，白色朝前
		Hint_Step2_Bottom1_ori2,		// 目标块在底面，白色朝右
		Hint_Step2_Bottom2_ori1,		// 目标块在底面，白色朝前
		Hint_Step2_Bottom3_ori0,		// 目标块在底面，白色朝下


		// 提示码 for step3
		Hint_Step3_Done = 440,			// 已经完成了这一步
		Hint_Step3_5CW,					// 把棱块转到后方，使用五顺四逆
		Hint_Step3_5CCW,				// 把棱块转到前，使用五逆四顺
		Hint_Step3_NotOnTop,			// 棱块不在顶层，需要移出


		// 提示码 for step 4
		Hint_Step4_Done = 460,		// 已经完成了这一步
		Hint_Step4_Point,			// 黄色1点
		Hint_Step4_Corner,			// 黄色拐角
		Hint_Step4_Line,			// 黄色一字


		// 提示码 for step 5
		Hint_Step5_Done = 480,		// 已经完成了这一步
		Hint_Step5_NY2,				// 顶面2个角块不是黄色,口诀：2前
		Hint_Step5_NY3_Fish,		// 顶面3个角块不是黄色,即小鱼，口诀：3上
		Hint_Step5_NY4_Cross,		// 顶面4个角块不是黄色，即十字口诀：4左

		// 提示码 for step 6
		Hint_Step6_Done = 500,		// 已经完成了这一步
		Hint_Step6_no_eye,			// 没有眼
		Hint_Step6_1eye,			// 一对眼
		Hint_Step6_4eyes,			// 4对眼,但还没有转 U 做最后的调整


		// 提示码 for step 7
		Hint_Step7_Done = 500,		// 已经完成了这一步
		Hint_Step7_no_line,			// 顶层侧面没有出现一排颜色相同
		Hint_Step7_has_line,		// 顶层侧面有一排颜色相同


	};

	// 公式背包序号
	enum formula_packsack_type
	{
		Formula_RURU,	// 0 上左下右
		Formula_5CW,	// 1 五顺四逆
		Formula_5CCW,	// 2 五逆四顺
		Formula_Cross,	// 3 十字公式
		Formula_Fish,	// 4 小鱼公式
		Formula_SwapCorner,	// 5 换角公式
		Formula_SwapEdge	// 7 换棱公式
	};

	struct hint_type
	{
		code_type code;
		std::vector<Move> rotates;		// 转体提示
		std::vector<Move> pre_moves;	// 前置公式，有些情况需要先转 U 进行调整
		std::vector<Move> moves;		// 公式

		// 以下是目标块或贴纸，指的是相对的位置，不是绝对的块 ID
		std::vector<Corner> obj_corner;
		std::vector<Edge> obj_edge;
		std::vector<Facelet> obj_facelet;

	};


	// 教学基类，基本的解法等，仅仅支持固定黄色底（白顶绿前）的标准姿态
	class TeacherBase
	{
	protected:
		TeacherBase() = default;


	public:


		// 获取当前正在进行的步骤，任意时刻都可以设置，有可能改变教学进程
		// 合法值只能是 Step_*
		// 如何设置非法值，则忽略
		void setCurrStep(code_type a);



		// 当前是否已经完成的状态 state, state 取值范围是 Finished_*
		// 如果已经完成超过此状态，也返回true, 输入非法则返回 false
		bool isFinished(code_type state) const;


		// 当前是否仅仅完成了状态state 而未完成后续的状态, 
		// state 取值范围是 Finished_*, 输入非法则返回 false
		bool isOnlyFinished(code_type state) const;


		// 检查当前仅仅完成的状态，返回 OnlyFinished_*
		code_type checkOnlyFinishedState() const;



		// 获取提示码，返回值取值范围 Hint_Step1_* 或 Invalid
		// 小花时不把 U 放入到 pre_moves， 而是放在 moves 中， 可以通过 moves 判断。
		// 做十字是把 U 前置到 pre_moves
		hint_type step1_getHint();


		// 获取提示码，返回值取值范围 Hint_Step2_* 或 Invalid
		hint_type step2_getHint();

		// 获取提示码，返回值取值范围 Hint_Step3_* 或 Invalid
		hint_type step3_getHint();

		// 获取提示码，返回值取值范围 Hint_Step4_* 或 Invalid
		hint_type step4_getHint();

		// 获取提示码，返回值取值范围 Hint_Step5_* 或 Invalid
		hint_type step5_getHint();

		// 获取提示码，返回值取值范围 Hint_Step6_* 或 Invalid
		hint_type step6_getHint();

		// 获取提示码，返回值取值范围 Hint_Step7_* 或 Invalid
		hint_type step7_getHint();


		// 获取底面两层所有贴纸编号，用于高亮
		static std::vector<Facelet> getF2LFacelets();


	public:
		static std::vector<std::vector<Move>> formula_packsack;		// 公式背包
		
	private:

		// 检查是否完成了黄色小花
		bool _isFlowerDone() const;

		// 仅仅检查4个中层棱块是否完成
		bool _checkOnlyMiddleEdge() const;

		// step1_getHint 中间过程：小花
		hint_type _step1_getHint_flower();

		// step1_getHint 中间过程：十字
		hint_type _step1_getHint_cross();


	protected:

		code_type m_currStep = Step0_Basics;		// 当前正在进行的步骤, 注意：黄色小花不是单独的步骤
		CubieCube m_currCube = CubieCube::eye();	// 当前魔方状态

	};


	// 教学类，与页面相关，需要建一个对象，支持6面底，默认白色底
	class Teacher : public TeacherBase
	{

	private:
		typedef TeacherBase _base;

	public:
		Teacher();
	

		void setBottomColor(Color c);
		Color getBottomColor() const { return m_bottomColor; };


		// 获取当前魔方状态
		AxisCubieCube getCurrCube() const { return m_currCube; };

		// 设置当前魔方状态，传入解决的 CubieCube, 不会改变 axis
		void setCurrCube(const CubieCube& cc);


		// 设置当前魔方状态，如果cc.axis底面等于 m_bottomColor, 则直接赋值
		// 都会自动旋转时底面颜色等于 m_bottomColor, 而前面颜色未定
		void setCurrCube(const AxisCubieCube& cc);



		// 目标角块相对位置转绝对块编号
		std::vector<Corner> objCornerRel2Abs(const std::vector<Corner>& src);

		// 目标棱块块相对位置转绝对块编号
		std::vector<Edge> objEdgeRel2Abs(const std::vector<Edge>& src);

		// 目标角块相对位置转绝对块编号
		std::vector<Facelet> objFaceletRel2Abs(const std::vector<Facelet>& src);





	protected:
		Color m_bottomColor = U;
		code_type m_currStep = Step0_Basics;		// 当前正在进行的步骤, 注意：黄色小花不是单独的步骤

		AxisCubieCube m_currCube = AxisCubieCube::eye();		// 当前魔方状态


	};


}



