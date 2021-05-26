#pragma once

#include "myMatrix.h"
#include "myQuaternion.h"


namespace my
{

	// 把一个转换相对世界坐标平移 m
	mat4 translate(const mat4& a, const vec3& m);

	// 把一个转换相对世界坐标旋转一个四元数（即绕方向 n 旋转角度 deg）
	mat4 rotate(const mat4& a, const quaternion& q);

	// 把一个转换相对世界坐标方向 n 旋转角度 deg 
	mat4 rotate(const mat4& a, const vec3& n, double deg);


	// 把一个转换相对世界坐标旋转一个欧拉角，按照 x,y,z的顺序
	mat4 rotate(const mat4& a, double deg1, double deg2, double deg3);

	// 视锥体投影矩阵，从Viewport 到到标准剪切矩阵
	mat4 frustum(double left, double right, double bottom, double top, double nearZ, double farZ);


	// 正交投影矩阵，从Viewport 到到标准剪切矩阵
	mat4 orthogonal(double left, double right, double bottom, double top, double nearZ, double farZ);

	// 相机透视投影矩阵，从Viewport 到到标准剪切矩阵
	// fov_y y轴的可视角度的一半
	// aspect 宽高比
	mat4 camPerspective(double fov_y, double aspect, double nearZ, double farZ);

	//////////////////////////////////////////////////////////////////////////
	// 坐标系
	class Coordinate
	{
	public:
		Coordinate();

		static Coordinate eye() { return Coordinate(); };

		vec3 getPos() const { return my_pos; };
		void setPos(const vec3& pos) { my_pos = pos; };
		void setPos(double x, double y, double z) { setPos({ x,y,z }); };

		quaternion getQuaternion() const { return my_q; };
		void setQuaternion(const quaternion& q) { my_q = q; };


		void setEuler(double ang1, double ang2, double ang3) { my_q = euler2quaternion(ang1, ang2, ang3); };
		void setEuler(vec3 ang) { setEuler(ang[0], ang[1], ang[2]); };

		mat4 getTransfrom() const;


		// 相对世界坐标平移 m
		void translate(const vec3& m);

		void translate(double x, double y, double z) { translate(vec3{ x,y,z }); };

		// 相对惯性坐标(即与世界坐标方向相同原点不同的坐标系)旋转
		void rotate(const quaternion& q);

		// 相对惯性坐标(即与世界坐标方向相同原点不同的坐标系)旋转欧拉角
		void rotate(double ang1, double ang2, double ang3);

		void rotate(vec3 ang);


		// 相对自身坐标平移 m
		void translateLocal(const vec3& m);

		// 相对自身坐标旋转
		void rotateLocal(const quaternion& q);

		void rotateLocal(double ang1, double ang2, double ang3);


		// 点自身坐标转世界坐标
		vec3 local2world(const vec3& v) const;

		// 点世界坐标转自身坐标
		vec3 world2local(const vec3& v) const;


		// b相对于this, 返回 b 相对于世界， 假设 this 相对于世界
		Coordinate local2world(const Coordinate& b) const;


		// b相对于世界, 返回 b 相对于this， 假设 this 相对于世界
		Coordinate world2local(const Coordinate& b) const;



		// 在世界坐标上,过自身原点，绕方向 n 旋转 ang 角度
		void rotate(const vec3& n, double ang);


		// 在世界坐标上,过点p，绕方向 n 旋转 ang 角度
		void rotate(const vec3& p, const vec3& n, double ang);


		// c = a * b, a相对于世界，b相对于a，则c是b相对于世界
		Coordinate operator*(const Coordinate& b) const;


	protected:
		vec3 my_pos;
		quaternion my_q;
	};

}


