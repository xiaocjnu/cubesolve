#include "myAnimationCoord.h"


namespace my
{
	AnimationCoord::AnimationCoord(Coordinate & coord)
		: myCoord(coord)
	{
		my_dst = myCoord;
	}
	void AnimationCoord::update(double deltaTime)
	{

		
		vec3 deltaTrans = my_dst.getPos() - myCoord.getPos();

		vec3 dTrans;

		if (deltaTrans == vec3{ 0,0,0 })  // 不动
		{
			dTrans = deltaTrans;
			my_transSpeed = 0;
		}
		else // 动
		{
			if (my_transSpeed == 0)
			{
				dTrans = deltaTrans;
			}
			else
			{
				dTrans = normalize(deltaTrans) * my_transSpeed * deltaTime;

				if (magnitude(dTrans) > magnitude(deltaTrans))
				{
					dTrans = deltaTrans;
					my_transSpeed = 0;
				}
			}
		}

		

		myCoord.translate(dTrans);


		// 计算旋转角度

		double rot_ang;
		vec3 rot_axis;

		_cal_rotate(myCoord.getQuaternion(), my_dst.getQuaternion(), rot_ang, rot_axis);

		if (rot_ang == 0.0)  // 不动
			my_rotSpeed = 0.0;
		else // 动
		{
			double _rot_ang;
			if (my_rotSpeed == 0.0)
			{
				_rot_ang = rot_ang;
			}
			else
			{
				_rot_ang = (rot_ang) / abs(rot_ang) * my_rotSpeed * deltaTime;

				if (abs(_rot_ang) > abs(rot_ang))
				{
					_rot_ang = rot_ang;
					my_rotSpeed = 0.0;
				}
			}

			myCoord.rotate(rot_axis, _rot_ang);
		}

		

	}
	void AnimationCoord::setFrame(double delay, const Coordinate & coord, vec3 ref_rot_axis)
	{
		my_dst = coord;
		my_refRotAxis = ref_rot_axis;


		// 计算速度
		vec3 dTrans = my_dst.getPos() - myCoord.getPos();

		if (dTrans == vec3{ 0,0,0 })
		{
			my_transSpeed = 0.0;
		}
		else
		{
			my_transSpeed = magnitude(dTrans) / delay;
		}


		// 计算旋转角度
		double rot_ang;
		vec3 rot_axis;

		_cal_rotate(myCoord.getQuaternion(), my_dst.getQuaternion(), rot_ang, rot_axis);


		if (rot_ang == 0.0)
		{
			my_rotSpeed = 0.0;
		}
		else
		{
			my_rotSpeed = abs(rot_ang) / delay;
		}
	}
	void AnimationCoord::setFrame(const Coordinate& coord, vec3 ref_rot_axis)
	{
		setFrame(myDefaultDelay, coord, ref_rot_axis);
	}

	void AnimationCoord::_cal_rotate(const quaternion & from, const quaternion & to, double & rot_ang, vec3& rot_axis)
	{
		// 计算旋转角度
		const quaternion& q1 = from;
		const quaternion& q2 = to;

		quaternion qd = q2 * q1.inv();

		vec3 v{ qd.x, qd.y, qd.z };
		double w = qd.w;

		// 绕最小角度旋转
		if (w < 0)
		{
			w = -w;
			v = -1.0 * v;
		}

		double dd = dot(my_refRotAxis, v);

		if (dd < 0)// 绕参考方向
		{
			w = -w;
			v = -1.0 * v;
		}



		rot_ang = 0.0;
		rot_axis = { 0,0,0 };

		if (w < 1)
		{
			double abs_v = sqrt(dot(v, v));

			if (abs_v == 0)
			{
				rot_ang = 0.0;
				rot_axis = { 0,0,0 };
			}
			else
			{
				rot_ang = 2.0f * acos(w) * 180.0 / MY_PI ;	// 角速度
				rot_axis = (1.0 / abs_v) * v;
			}
		}
	}

}


