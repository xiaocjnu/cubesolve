#pragma once

#include "myCubeModel.h"
#include "myCubeScene.h"
#include "myAnimationCoord.h"

namespace my
{
	//////////////////////////////////////////////////////////////////////////
	// Coordination 动画控制器
	class AnimationCoord
	{
	public:

		AnimationCoord(Coordinate& coord);


		// 周期性调用,以更新动画, deltaTime 相对上一次调用的时间增量(s)
		void update(double deltaTime);


		// true 表示动画执行中，false 表示未执行或者执行完


		// 更新帧，但不会马上更新，会自动有一个平滑滤波过渡
		// delay, 以秒为单位的延时时间
		// coord 目标姿态, ref_rot_axis 参考旋转轴，可以引导旋转方向
		// 实际旋转方向由 ref_rot_axis 在 从当前到目标计算出来的旋转方向(最小角度原则)上的投影的方向
		// 即旋转方法绕 ref_rot_axis所指示的大概方向逆时针旋转, 当 ref_rot_axis = (0,0,0) 或者与旋转方向垂直时，使用原来的旋转方向
		void setFrame(double delay, const Coordinate& coord, vec3 ref_rot_axis = vec3({ 0,0,0 }));
		
		
		void setFrame(const Coordinate& coord, vec3 ref_rot_axis = vec3({ 0,0,0 }));




		// 获取目标帧,即最近一次调用 updateFrame 传入的值
		Coordinate getDstFrame() const { return my_dst; };


		vec3 getRefRotAxis() const { return my_refRotAxis; };


		// 设置默认动画延时时间
		void setDefaultDelay(double t) { myDefaultDelay = t; };

		double getDefaultDelay() const { return myDefaultDelay; };


	protected:

		void _cal_rotate(const quaternion& from, const quaternion& to,
			double& rog_ang, vec3& rot_axis);


	protected:
		Coordinate& myCoord;
		Coordinate my_dst;	// 目标坐标

		vec3 my_refRotAxis = {0,0,0};		// 旋转转轴

		double my_transSpeed = 0.0;
		double my_rotSpeed = 0.0;
		double myDefaultDelay = 0.0;



	};


}



