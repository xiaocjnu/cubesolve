#pragma once

#include "myAnimation.h"
#include "myAnimationCoord.h"
#include "myCubeModel.h"
#include "myCubeScene.h"

namespace my
{
	// 魔方模型动画控制器
	class AnimationCube
	{
	public:
		static quaternion axis_cube_to_q[24];


		AnimationCube(CubeModel& _cube);
		~AnimationCube();


		// 周期性调用,以更新动画, deltaTime 相对上一次调用的时间增量(s)
		void update(double deltaTime);


		// 更新帧，但不会马上更新，会自动有一个平滑滤波过渡
		// delay 以秒为单位的延时时间
		// ref_rot_axis 指示 coord 旋转的方向
		// ang_rot_dir 角度旋转方向，1 顺时针，-1 逆时针, 0 表示最近原则
		void setFrame(double delay, const Coordinate& coord, CubeAng ca,
			vec3 ref_rot_axis = { 0,0,0 }, std::array<int, 6> ang_rot_dir = { 0,0,0,0,0,0 });


		void setFrame(const Coordinate& coord, CubeAng ca,
			vec3 ref_rot_axis = { 0,0,0 }, std::array<int, 6> ang_rot_dir = { 0,0,0,0,0,0 });

		void setFrame(double delay, CubeAng ca, std::array<int, 6> ang_rot_dir = { 0,0,0,0,0,0 });

		void setFrame(CubeAng ca, std::array<int, 6> ang_rot_dir = { 0,0,0,0,0,0 });


		void setFrame(double delay, const Coordinate& coord, vec3 ref_rot_axis = { 0,0,0 });


		void setFrame(const Coordinate& coord, vec3 ref_rot_axis = { 0,0,0 });


		// 旋转指定轴
		void rotAxis(int axis, double ang);

		// 获取目标帧,即最近一次调用 updateFrame 传入的值
		Coordinate getDstFrameCoord() const { return myAniCoord.getDstFrame(); };
		CubeAng getDstFrameCubeAng() const { return my_dstCa; };



		// 设置默认动画延时时间
		void setDefaultDelay(double t);

		double getDefaultDelay() const { return myDefaultDelay; };

	protected:

		void _setFrameCa(const CubeAng& ca, std::array<int, 6> ang_rot_dir);
		void _setRotSpeed(double delay);

		void _updateCa(double deltaTime);
		


		// 一步或者两步到位
		static std::vector<int> _search_1or2_moves(const cs::CubieCube& from, const cs::CubieCube& to);

	

	protected:
		CubeModel& myCube;
		AnimationCoord myAniCoord;

		CubeAng my_dstCa;	// 目标 CubieCube and AxisAng

		double myDefaultDelay = 0.0;	// 默认动画时间


		std::array<double,6> my_dstAng = {0,0,0,0,0,0};	// 动画旋转目标角度, 与 my_dstCa 可能基于不同的 CubieCube, 所以值可能不同
		std::array<double, 6> my_rotSpeed = { 0,0,0,0,0,0 };	// 旋转速度
		std::array<int, 6> my_angRotDir = { 0,0,0,0,0,0 };
	};


}



