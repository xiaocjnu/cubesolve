#pragma once

#include "util.h"
#include "CubieCube.h"
#include "CoordCube5.h"


/**
	* facelet:
	*             |************|
	*             |*U1**U2**U3*|
	*             |************|
	*             |*U4**U5**U6*|
	*             |************|
	*             |*U7**U8**U9*|
	*             |************|
	* ************|************|************|************|
	* *L1**L2**L3*|*F1**F2**F3*|*R1**R2**F3*|*B1**B2**B3*|
	* ************|************|************|************|
	* *L4**L5**L6*|*F4**F5**F6*|*R4**R5**R6*|*B4**B5**B6*|
	* ************|************|************|************|
	* *L7**L8**L9*|*F7**F8**F9*|*R7**R8**R9*|*B7**B8**B9*|
	* ************|************|************|************|
	*             |************|
	*             |*D1**D2**D3*|
	*             |************|
	*             |*D4**D5**D6*|
	*             |************|
	*             |*D7**D8**D9*|
	*             |************|
*/

namespace cs
{
	// 解魔方，智能旋转5个轴，B轴不能旋转.
	class Search5
	{


	public:

		int phase1_fcnt = 0;
		int phase2_fcnt = 0;
		int phase1_steps = 0;
		int phase2_steps = 0;

		// determines if a " . " separates the phase1 and phase2 parts of the solver string 
		// like in F' R B R L2 F . U2 U D for example.<br>
		bool use_separator = false;

		// determines if a tag such as "(21f)" will be appended to the solution.
		bool append_length = false;

		Search5();

		int numberOfProbes() const { return m_probe; };

		int length() const { return m_sol; }

		int getError() const { return m_error; };

		std::vector<Move> getMoves() const { return m_moves; };

		static void init();

		std::string solutionToString() const;


		

		
		/* 解魔方，54个字符串表示魔方颜色(URFDLB)状态, 如
		* UBULURUFURURFRBRDRFUFLFRFDFDFDLDRDBDLULBLFLDLBUBRBLBDB
		* FBLLURRFBUUFBRFDDFUULLFRDDLRFBLDRFBLUUBFLBDDBUURRBLDDR
		* @return The solution string or an error code:
		*      Error 1: There is not exactly one facelet of each colour
		*      Error 2: Not all 12 edges exist exactly once
		*      Error 3: Flip error: One edge has to be flipped
		*      Error 4: Not all corners exist exactly once
		*      Error 5: Twist error: One corner has to be twisted
		*      Error 6: Parity error: Two corners or two edges have to be exchanged
		*      Error 7: No solution exists for the given maxDepth
		*      Error 8: Probe limit exceeded, no solution within given probMax
		*/
		std::string solve(std::string facelets);

		// 把魔方 facelets_src 变成与 facelets_dst 相同
		std::string solve(std::string facelets_src, std::string facelets_dst);

		std::vector<Move> solve(CubieCube cc);


		// 把魔方 src 变成与 dst 相同
		std::vector<Move> solve(CubieCube src, CubieCube dst);

		std::string solve_no_axis(std::string facelets, int axis);

		// 把魔方 facelets_src 变成与 facelets_dst 相同
		std::string solve_no_axis(std::string facelets_src, std::string facelets_dst, int axis);

		// set probe
		void set_param(int maxDepth=25, int probeMax=10000, int probeMin=0);

		// 指定不能旋转的轴解魔方
		// axis 取值 0,1,2,3,4,5, 代表 U,R,F,D,L,B
		std::vector<Move> solve_no_axis(CubieCube cc, int axis);

		std::vector<Move> solve_no_axis(CubieCube src, CubieCube dst, int axis);

		// =======================================
		std::string next();

		void search();

		/**
		* @return
		*      0: Found or Probe limit exceeded
		*      1: Try Next Power
		*      2: Try Next Axis
		*/
		int phase1(CoordCube5 node, long ssym, int maxl, int lm);

		/**
		* @return
		*      0: Found or Probe limit exceeded
		*      1: Try Next Power
		*      2: Try Next Axis
		*/
// 		int phase1opt(CoordCube5 ud, CoordCube5 rl, CoordCube5 fb, long ssym, int maxl, int lm);
// 
// 		string searchopt();

		/**
		* @return
		*      0: Found or Probe limit exceeded
		*      1: Try Next Power
		*      2: Try Next Axis
		*/
		int initPhase2();

		//-1: no solution found
		// X: solution with X moves shorter than expectation. Hence, the length of the solution is  depth - X
		int phase2(CoordCube5 node, int maxl, int depth, int lm);


	protected:

		CubieCube m_cc;
		int64 m_selfSym;
		int m_length1;
		int m_depth1;
		int m_maxDep2;
		int m_sol = 25;

		std::vector<Move> m_moves;
		//string m_solution;
		int m_probe;
		int m_probeMax = 1000;
		int m_probeMin = 0;
		bool m_found;
		//int m_verbose;
		bool m_isRec = false;


		/* 错误代码
		*    0 : success 
		*	-1 : There is not exactly one facelet of each colour<br>
		*   -2 : Not all 12 edges exist exactly once<br>
		*   -3 : Flip error : One edge has to be flipped<br>
		*   -4 : Not all corners exist exactly once<br>
		*   -5 : Twist error : One corner has to be twisted<br>
		*   -6 : Parity error : Two corners or two edges have to be exchanged<br>
		*   -7 : No solution exists for the given maxDepth<br>
		*   -8 : Probe limit exceeded, no solution within given probMax
		*   -9 : other error
		*   -10 : not found error
		*/
		int m_error;
	};
}