#pragma once


#include <array>
#include "CubieCube.h"



namespace my
{
	// 处理 CubieCube, AxisAng 的类
	class CubeAng
	{
	public:

		typedef std::array<double, 6> ang_t;


		CubeAng();

		CubeAng(const cs::CubieCube cc);

		CubeAng(const cs::CubieCube cc, const ang_t ang);

		CubeAng(const cs::CubieCube cc, const ang_t ang, const std::array<int,6> angIdx);

		CubeAng(const CubeAng& _right);


		// 设置 CubieCube, AxisAng 自动置 0
		void setCubieCube(const cs::CubieCube& cc);

		const cs::CubieCube& getCubieCube() const { return myCubieCube; };

		// 设置轴的偏移角度，取值范围不限制, 只能相对两个轴同时非0值
		// 否则自动容错处理，对最大绝对值的角度相邻的四个轴置0
		void setAxisAng(const ang_t& ang);

		// 检查 axisAng 是否合法
		static bool checkAxisAng(const ang_t& ang);

		// 返回轴的角度
		const ang_t& getAxisAng() const { return myAxisAng; };


		// 返回轴偏移角度下标, 取值智能是 0,1,2,3
		const std::array<int, 6>& getAxisAngIdx() const { return myAxisAngIdx; };

		void setAxisAngIdx(const std::array<int, 6>& idx);

		//std::array<int, 6>& axisAngIdx() { return myAxisAngIdx; };


		// 轴 axis 旋转角度 ang, 自动容错，自动改变 cubieCube，使角度范围在[-45,45)
		// axis 取值 [0,6), ang 任意值
		// 由于角度范围可以超过45, 则自动将范围调整至[-45,45)(会改变cubieCube)
		void rotateAxis(int axis, double ang);

		// 轴 axis 旋转角度 ang, 自动容错，自动改变 cubieCube，使角度范围在[-45,45)
		// axis 取值 [0,6), ang 任意值
		void rotateAxis_no_normalize(int axis, double ang);


		// 标准化， 把角度规整成 [-45, 40) 的范围, 可能会改变 cubiecube
		void normalize();

		CubeAng& operator=(const CubeAng& _right);


	protected:
		static int _fixAngIdx(int a);

		// 转动某个轴，自动对其他轴进行容错
		void _correctAngByAxis(int ax);

		// 自动容错处理，对最大绝对值的角度相邻的四个轴置0
		void _correctAng();

	protected:
		cs::CubieCube myCubieCube;

		// 中心块的偏移角度，取值范围不限制, 只能相对两个轴同时非0值, 顺时针是正方向
		ang_t myAxisAng;
		std::array<int, 6> myAxisAngIdx;	// 取值 0,1,2,3, 中心块角度中心，分别代表 0, 90, 180, 270 度(顺时针)




		// 容错表
		static const int myAngCorrectTbl[6][4];
	};



}


