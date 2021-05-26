#include "myTransform.h"
#include "myQuaternion.h"
#include <cmath>

namespace my
{
	//////////////////////////////////////////////////////////////////////////



	mat4 translate(const mat4& a, const vec3& m)
	{
		mat4 b = mat4::eye();
		b(0, 3) = m(0);
		b(1, 3) = m(1);
		b(2, 3) = m(2);

		return b * a;
	}



	mat4 rotate(const mat4& a, const quaternion& q)
	{
		quaternion q1(0, 1, 0, 0);	// x 轴
		quaternion q2(0, 0, 1, 0);	// y 轴
		quaternion q3(0, 0, 0, 1);	// z 轴

		quaternion q1q = q * q1 *q.inv();
		quaternion q2q = q * q2 *q.inv();
		quaternion q3q = q * q3 *q.inv();


		mat4 b = mat4::eye();
		b(0, 0) = q1q.x;
		b(1, 0) = q1q.y;
		b(2, 0) = q1q.z;

		b(0, 1) = q2q.x;
		b(1, 1) = q2q.y;
		b(2, 1) = q2q.z;

		b(0, 2) = q3q.x;
		b(1, 2) = q3q.y;
		b(2, 2) = q3q.z;

		return b * a;
	}

	mat4 rotate(const mat4& a, const vec3& n, double deg)
	{
		vec3 nn = normalize(n);
		double c = cos(deg * MY_PI / 180);
		double s = sin(deg * MY_PI / 180);

		quaternion q(c, s*n(0), s*n(1), s*n(2));

		return rotate(a, q);
	}


	mat4 rotate(const mat4& a, double deg1, double deg2, double deg3)
	{
		mat4 b = rotate(a, { 1,0,0 }, deg1);
		b = rotate(b, { 0,1,0 }, deg2);
		b = rotate(b, { 0,0,1 }, deg3);
		return b;
	}

	mat4 frustum(double left, double right, double bottom, double top, double nearZ, double farZ)
	{
		double deltaX = right - left;
		double deltaY = top - bottom;
		double deltaZ = farZ - nearZ;

		mat4 frust = mat4::zeros();
		if (nearZ <= 0 || farZ <= 0 || deltaX <= 0 || deltaY <= 0 || deltaZ <= 0)
		{
			return frust;
		}


		frust(0,0) = 2.0 * nearZ / deltaX;
		frust(1,1) = 2.0 * nearZ / deltaY;

		frust(0,2) = (right + left) / deltaX;
		frust(1,2) = (top + bottom) / deltaY;
		frust(2,2) = -(nearZ + farZ) / deltaZ;
		frust(3,2) = -1.0;

		frust(2,3) = -2.0 * nearZ * farZ / deltaZ;

		return frust;
	}

	mat4 orthogonal(double left, double right, double bottom, double top, double nearZ, double farZ)
	{
		double deltaX = right - left;
		double deltaY = top - left;
		double deltaZ = farZ - nearZ;

		mat4 orth = mat4::zeros();
		if (nearZ <= 0 || farZ <= 0 || deltaX <= 0 || deltaY <= 0 || deltaZ <= 0)
		{
			return orth;
		}


		orth(0, 0) = 2.0 / deltaX;
		orth(1, 1) = 2.0 / deltaY;
		orth(2, 2) = 2.0 / deltaZ;

		orth(0, 3) = -(right + left) / deltaX;
		orth(1, 3) = -(top + bottom) / deltaY;
		orth(2, 3) = -(nearZ + farZ) / deltaZ;
		orth(3, 3) = 1.0;

		return orth;
	}

	mat4 camPerspective(double fov_y, double aspect, double nearZ, double farZ)
	{
		mat4 P;

		double frustumW, frustumH;

		frustumH = tan(fov_y / 360.0 * MY_PI) * nearZ;
		frustumW = frustumH * aspect;

		return frustum(-frustumW, frustumW, -frustumH, frustumH, nearZ, farZ);
	}

	Coordinate::Coordinate()
		: my_pos(vec3{ 0,0,0 })
		, my_q(1, 0, 0, 0)
	{
	}

	mat4 Coordinate::getTransfrom() const
	{
		mat4 m = mat4::eye();
		m = my::rotate(m, my_q);
		m = my::translate(m, my_pos);
		return m;
	}

	void Coordinate::translate(const vec3 & m)
	{
		my_pos = my_pos + m;
	}


	void Coordinate::rotate(const quaternion & q)
	{
		my_q = q * my_q;
	}

	void Coordinate::rotate(double ang1, double ang2, double ang3)
	{
		rotate(euler2quaternion(ang1, ang2, ang3));
	}

	void Coordinate::rotate(vec3 ang)
	{
		rotate(ang[0], ang[1], ang[2]);
	}

	void Coordinate::translateLocal(const vec3 & m)
	{
		my_pos = local2world(m);
	}

	void Coordinate::rotateLocal(const quaternion & q)
	{
		my_q = my_q * q;
	}

	void Coordinate::rotateLocal(double ang1, double ang2, double ang3)
	{
		rotateLocal(euler2quaternion(ang1, ang2, ang3));
	}

	vec3 Coordinate::local2world(const vec3& v) const
	{
		quaternion qv(0, v[0], v[1], v[2]);
		quaternion qq = my_q * qv * my_q.inv();

		vec3 v1{ qq.x, qq.y, qq.z };
		return my_pos + v1;
	}

	vec3 Coordinate::world2local(const vec3& v) const
	{
		quaternion qv = vec2quaternion(v - my_pos);
		quaternion qq = my_q.inv() * qv * my_q;

		vec3 v1 = quaternion2vec(qq);
		return v1;
	}

	Coordinate Coordinate::local2world(const Coordinate & b) const
	{
		Coordinate c;
		c.my_pos = local2world(b.getPos());
		c.my_q = my_q * b.my_q;
		return c;
	}

	Coordinate Coordinate::world2local(const Coordinate & b) const
	{
		Coordinate c;
		c.my_pos = world2local(b.getPos());
		c.my_q = my_q.inv() * b.my_q;
		return c;
	}

	void Coordinate::rotate(const vec3& n, double ang)
	{
		quaternion q = axial2quaternion(n, ang);
		rotate(q);
	}

	void Coordinate::rotate(const vec3 & p, const vec3 & n, double ang)
	{
		quaternion q = axial2quaternion(n, ang);
		rotate(q);

		// 移动位置 坐标系 p (原点在 p, 方向和世界坐标相同)
		vec3 pp = my_pos - p;
		quaternion new_qq = q * vec2quaternion(pp) * q.inv();
		my_pos = p + vec3{ new_qq.x, new_qq.y, new_qq.z };

	}

	Coordinate Coordinate::operator*(const Coordinate & b) const
	{
		return local2world(b);
	}


}


