#pragma once

#include "util.h"
#include "CubieCube.h"
#include "CoordCube.h"


namespace cs
{
	class Search
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

		// 只允许5个轴，禁止 B
		bool use_5axis = false;

		Search();

		int numberOfProbes() const { return m_probe; };

		int length() const { return m_sol; }

		int getError() const { return m_error; };

		std::vector<Move> getMoves() const { return m_moves; };

		static void init();

		std::string solutionToString() const;

		std::string solve(std::string facelets);

		// 把魔方 facelets_src 变成与 facelets_dst 相同
		std::string solve(std::string facelets_src, std::string facelets_dst);

		std::vector<Move> solve(CubieCube cc);

		// 把魔方 src 变成与 dst 相同
		std::vector<Move> solve(CubieCube src, CubieCube dst);

		std::vector<Move> solve(CubieCube cc, int maxDepth, int probeMax = 10000, int probeMin = 0);

		std::string next(int probeMax = 100000, int probeMin = 0);

		void search();

		/**
		* @return
		*      0: Found or Probe limit exceeded
		*      1: Try Next Power
		*      2: Try Next Axis
		*/
		int phase1(CoordCube node, int ssym, int maxl, int lm);

		/**
		* @return
		*      0: Found or Probe limit exceeded
		*      1: Try Next Power
		*      2: Try Next Axis
		*/
// 		int phase1opt(CoordCube ud, CoordCube rl, CoordCube fb, long ssym, int maxl, int lm);
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
		int phase2(CoordCube node, int maxl, int depth, int lm);


	protected:

		CubieCube m_cc;
		int64 m_selfSym;
		int m_length1;
		int m_depth1;
		int m_maxDep2;
		int m_sol;

		std::vector<Move> m_moves;
		//string m_solution;
		int m_probe;
		int m_probeMax;
		int m_probeMin;
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
		*/
		int m_error;
	};
}