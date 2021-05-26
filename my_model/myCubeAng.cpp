#include <cmath>
#include "myCubeAng.h"
#include "util.h"
#include "myUtils.h"

using namespace std;


namespace my
{

	const int CubeAng::myAngCorrectTbl[6][4] =
	{
		{1,2,4,5},	// 0(U)
		{0,2,3,5},  // 1(R)
		{0,1,3,4},	// 2(F)
		{1,2,4,5},	// 3(D)
		{0,2,3,5},  // 4(L)
		{0,1,3,4},	// 5(B)
	};

	CubeAng::CubeAng()
	{
		myCubieCube = cs::CubieCube::eye();
		myAxisAng = { 0,0,0,0,0,0 };
		myAxisAngIdx = { 0,0,0,0,0,0 };
	}

	CubeAng::CubeAng(const cs::CubieCube cc)
	{
		myCubieCube = cc;
		myAxisAng = { 0,0,0,0,0,0 };
		myAxisAngIdx = { 0,0,0,0,0,0 };
	}

	CubeAng::CubeAng(const cs::CubieCube cc, const ang_t ang)
	{
		myCubieCube = cc;
		setAxisAng(ang);
		myAxisAngIdx = { 0,0,0,0,0,0 };
	}

	CubeAng::CubeAng(const cs::CubieCube cc, const ang_t ang, const std::array<int, 6> angIdx)
	{
		myCubieCube = cc;
        setAxisAng(ang);
		setAxisAngIdx(angIdx);
	}

	CubeAng::CubeAng(const CubeAng & _right)
	{
		myCubieCube = _right.myCubieCube;
		myAxisAng = _right.myAxisAng;
		myAxisAngIdx = _right.myAxisAngIdx;
	}

	void CubeAng::setCubieCube(const cs::CubieCube & cc)
	{
		// 是否可以一步转到位

		bool b = true;
		for (int m = 0; m < 18; ++m)
		{
			if (myCubieCube.move(m) == cc)
			{
				int ax = m / 3;
				int po = m % 3 + 1;

				myAxisAngIdx[ax] = _fixAngIdx(myAxisAngIdx[ax] + po);


				b = false;
				break;
			}
		}

		// 转M层两步到位
		for (int m = 0; b && m < 9; ++m)
		{
			int ax1 = m / 3;
			int po1 = m % 3;

			int ax2 = ax1 + 3;
			int po2 = 2 - po1;

			int m2 = ax2 * 3 + po2;

			po1 = po1 + 1;
			po2 = po2 + 1;

			if (myCubieCube.move(m).move(m2) == cc)
			{
				myAxisAngIdx[ax1] = _fixAngIdx(myAxisAngIdx[ax1] + po1);
				myAxisAngIdx[ax2] = _fixAngIdx(myAxisAngIdx[ax2] + po2);


				b = false;
				break;
			}
		}


		myCubieCube = cc;

		for (int i = 0; i < 6; ++i)
		{
			myAxisAng[i] = 0;
		}
	}

	void CubeAng::setAxisAng(const ang_t& ang)
	{

	    myAxisAng = ang;
	    _correctAng();
	}

	bool CubeAng::checkAxisAng(const ang_t& ang)
	{
		bool ret = true;

		int next = 0;
		for (int i = 0; i < 6; ++i)
		{
			if (i == next)
			{
				if (ang[i] == 0)
					next = i + 1;
				else
					next = i + 3;
			}
			else if (ang[i] != 0)
			{
				ret = false;
				break;
			}
		}
		return ret;
	}


	void CubeAng::setAxisAngIdx(const std::array<int, 6>& idx)
	{
		for (int i = 0; i < 6; ++i)
		{
			myAxisAngIdx[i] = _fixAngIdx(idx[i]);
		}
	}

	void CubeAng::rotateAxis(int axis, double ang)
	{
		rotateAxis_no_normalize(axis, ang);
		normalize();
		
	}

	void CubeAng::rotateAxis_no_normalize(int axis, double ang)
	{
		if (axis < 0 || axis >= 6 || ang == 0)
			return;

		myAxisAng[axis] += ang;
	}


	void CubeAng::normalize()
	{
		for (int axis = 0; axis < 6; ++axis)
		{
			while (myAxisAng[axis] >= 45)
			{
				myAxisAng[axis] = fixAng180(myAxisAng[axis] - 90.0);
				myAxisAngIdx[axis] = _fixAngIdx(myAxisAngIdx[axis] + 1);
				myCubieCube = myCubieCube.move(3 * axis);
			}


			while (myAxisAng[axis] < -45)
			{
				myAxisAng[axis] = fixAng180(myAxisAng[axis] + 90.0);
				myAxisAngIdx[axis] = _fixAngIdx(myAxisAngIdx[axis] - 1);
				myCubieCube = myCubieCube.move(3 * axis + 2);
			}


			assert(-45 <= myAxisAng[axis] && myAxisAng[axis] < 45);
		}

		_correctAng();
		
	}

	CubeAng & CubeAng::operator=(const CubeAng & _right)
	{
		myCubieCube = _right.myCubieCube;
		myAxisAng = _right.myAxisAng;
		myAxisAngIdx = _right.myAxisAngIdx;

		return *this;
	}

	int CubeAng::_fixAngIdx(int a)
	{
		int b = a % 4;
		b = b < 0 ? b + 4 : b;
		return b;
	}

	void CubeAng::_correctAngByAxis(int ax)
	{
		if (ax < 0 || ax >= 6)
			return;

		for (int j = 0; j < 4; ++j)
		{
			myAxisAng[myAngCorrectTbl[ax][j]] = 0.0;
		}
	}


	void CubeAng::_correctAng()
	{
		int max_ax = 0;

		for (int i = 1; i < 6; ++i) {
			if (abs(myAxisAng[i]) > abs(myAxisAng[max_ax]))
				max_ax = i;
		}

		_correctAngByAxis(max_ax);
	}

}

