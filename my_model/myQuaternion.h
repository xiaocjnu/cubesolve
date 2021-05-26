#pragma once
#include "myMatrix.h"


namespace my
{
	template <typename T>
	class Quaternion_t
	{
	public:
		T w;
		T x;
		T y;
		T z;

		Quaternion_t() {};

		Quaternion_t(T _w, T _x, T _y, T _z)
			: w(_w)
			, x(_x)
			, y(_y)
			, z(_z)
		{
		}


		static Quaternion_t zeros()
		{
			return Quaternion_t(0, 0, 0, 0);
		}

		static Quaternion_t eye()
		{
			return Quaternion_t(1, 0, 0, 0);
		}


		Quaternion_t operator+(const Quaternion_t& b) const
		{
			Quaternion_t c;
			c.w = this->w + b.w;
			c.x = this->x + b.x;
			c.y = this->y + b.y;
			c.z = this->z + b.z;
			return c;
		}

		Quaternion_t operator-(const Quaternion_t& b) const
		{
			Quaternion_t c;
			c.w = this->w - b.w;
			c.x = this->x - b.x;
			c.y = this->y - b.y;
			c.z = this->z - b.z;
			return c;
		}


		Quaternion_t operator*(const Quaternion_t& b) const
		{
			Quaternion_t c;
			c.w = this->w * b.w - this->x * b.x - this->y * b.y - this->z * b.z;
			c.x = this->w * b.x + b.w * this->x + this->y * b.z - b.y * this->z;
			c.y = this->w * b.y + b.w * this->y + this->z * b.x - this->x * b.z;
			c.z = this->w * b.z + b.w * this->z + this->x * b.y - b.x * this->y;
			return c;
		}


		Quaternion_t operator*(T s) const
		{
			Quaternion_t c;
			c.w = this->w * s;
			c.x = this->x * s;
			c.y = this->y * s;
			c.z = this->z * s;
			return c;
		}

		Quaternion_t conj() const
		{
			return Quaternion_t(w, -x, -y, -z);
		}

		T norm2() const
		{
			return w * w + x * x + y * y + z * z;
		}

		T abs() const
		{
			return sqrt(norm2());
		}

		Quaternion_t inv() const
		{
			return (T(1) / norm2()) * conj();
		}

	};


	template<typename T>
	Quaternion_t<T> operator*(T s, const Quaternion_t<T>& b)
	{
		return b * s;
	}


	//////////////////////////////////////////////////////////////////////////

	


	//////////////////////////////////////////////////////////////////////////
	typedef Quaternion_t<double> quaternion;



	// 欧拉角转四元数, 欧拉角按 x,y,z 的顺序绕世界坐标旋转
	quaternion euler2quaternion(double ang1, double ang2, double ang3);

	// 轴向角转四元数
	quaternion axial2quaternion(const vec3& n, double ang);


	// 用四元数表示向量
	quaternion vec2quaternion(const vec3 v);

	// 获取四元数表示的向量，假设 w = 0
	vec3 quaternion2vec(const quaternion& q);
}
