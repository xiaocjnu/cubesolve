#pragma once

#include "myCubeModel.h"
#include "myCubeScene.h"

namespace my
{
	//////////////////////////////////////////////////////////////////////////
	// Arrow 动画控制器
	class AnimationArrow
	{
	public:
		static vec3 pos_table[6];
		static vec3 euler_table[6];


		AnimationArrow(CubeScene& scene);


		// 周期性调用,以更新动画, deltaTime 相对上一次调用的时间增量(s)
		void update(double deltaTime);


		// true 表示开启显示，false 表示不开启
		bool isOn() const;

		// 开启显示, true 开启，false on
		void setOnOff(bool b);


		// 设置公式, 只支持基本公式，如 U1, U2 箭头顺时针转，U3 箭头逆时针转
		void setMove(cs::Move mv);

		double getSpeed() const { return mySpeed; };
		void setSpeed(double speed) { mySpeed = speed; };


	protected:
		CubeScene& myScene;


		int myAxis = 0;		// 轴 0 - 5 分别对应 URFDLB
		bool myDir = true;		// 方向, true 顺时针， false 逆时针
		double mySpeed = 10.0;		// 旋转速度 deg/s
		bool myIsOn = false;		// 是否开启
	};
}



