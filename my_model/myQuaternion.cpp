#include "myQuaternion.h"

#include <cmath>



namespace my
{
	// 欧拉角转四元数, 欧拉角按 x,y,z 的顺序绕世界坐标旋转
	quaternion euler2quaternion(double ang1, double ang2, double ang3)
	{
		quaternion q1(cos(ang1 / 2.0 * MY_PI / 180.0), sin(ang1 / 2 * MY_PI / 180), 0, 0);
		quaternion q2(cos(ang2 / 2.0 * MY_PI / 180.0), 0,sin(ang2 / 2 * MY_PI / 180), 0);
		quaternion q3(cos(ang3 / 2.0 * MY_PI / 180.0), 0,0,sin(ang3 / 2 * MY_PI / 180));


		return q3 * q2*q1;
	}



	// 轴向角转四元数
	quaternion axial2quaternion(const vec3& n, double ang)
	{
		double dist = sqrt(dot(n,n));

		if (dist == 0)
			return quaternion(1,0,0,0);

		vec3 nn = vec3{n(0)/dist, n(1)/dist, n(2)/dist};

		double c = cos(ang / 2 * MY_PI / 180);
		double s = sin(ang / 2 * MY_PI / 180);

		quaternion q(c, s*nn[0], s*nn[1], s*nn[2]);

		return q;
	}

	quaternion vec2quaternion(const vec3 v)
	{
		return quaternion(0, v[0], v[1], v[2]);
	}

	vec3 quaternion2vec(const quaternion & q)
	{
		return vec3{ q.x, q.y, q.z };
	}


}
