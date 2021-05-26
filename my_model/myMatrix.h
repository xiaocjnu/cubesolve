// 简单的固定大小矩阵，只适用于小矩阵
// 按列优先顺序排列


#pragma once

#include <initializer_list>
#include <cmath>

namespace my
{
	template<typename T, int M, int N>
	class Matrix
	{
	public:
		typedef  Matrix<T, M, N> my_t;



		Matrix() {}

		Matrix(T* buf)
		{
			for (int i = 0; i < numel(); ++i)
			{
				data[i] = buf[i];
			}
		}


		Matrix(const std::initializer_list<T>& _list)
		{
			int i = 0;
			for (auto a : _list)
			{
				data[i++] = a;
			}
		}




		static int get_m() { return M; }
		static int get_n() { return N; }
		static int numel() { return M * N; }


		int _get_m() const { return M; }
		int _get_n() const { return N; }
		int _numel() const { return M * N; }


#ifndef SWIG
		T operator()(int i) const
		{
			return data[i];
		}

		T& operator()(int i)
		{
			return data[i];
		}

		T operator[](int i) const
		{
			return data[i];
		}

		T& operator[](int i)
		{
			return data[i];
		}

		T operator()(int i, int j) const
		{
			return data[i + j * M];
		}

		T& operator()(int i, int j)
		{
			return data[i + j * M];
		}

#endif

		T get(int i) const
		{
			return data[i];
		}

		void set(int i, const T& v)
		{
			data[i] = v;
		}


		T get(int i, int j) const
		{
			return data[i + j * M];
		}

		void set(int i, int j, const T& v)
		{
			data[i + j * M] = v;
		}


		static my_t zeros()
		{
			my_t a;
			for (int i = 0; i < numel(); ++i)
			{
				a[i] = 0;
			}
			return a;
		}

		static my_t ones()
		{
			my_t a;
			for (int i = 0; i < numel(); ++i)
			{
				a[i] = 1;
			}
			return a;
		}

		static my_t eye()
		{
			my_t a;
			for (int i = 0; i < M; ++i)
			{
				for (int j = 0; j < N; ++j)
				{
					if (i == j)
						a(i, j) = 1;
					else
						a(i, j) = 0;
				}

			}

			return a;
		}


		Matrix<T, N, M> transpose() const
		{
			Matrix<T, N, M> b;
			for (int i = 0; i < N; ++i)
			{
				for (int j = 0; j < M; ++j)
				{
					b(i, j) = (*this)(j, i);
				}
			}
			return b;
		}


		bool operator == (const my_t& b) const
		{
			for (int i = 0; i < numel(); ++i)
			{
				if (get(i) != b(i))
				{
					return false;
				}
			}
			return true;
		}

		my_t operator + (const my_t& b) const
		{
			my_t c;
			for (int i = 0; i < numel(); ++i)
			{
				c(i) = (*this)(i) + b(i);
			}
			return c;
		}

		my_t operator - (const my_t& b) const
		{
			my_t c;
			for (int i = 0; i < numel(); ++i)
			{
				c(i) = (*this)(i) - b(i);
			}
			return c;
		}


		my_t operator + (T s) const
		{
			my_t c;
			for (int i = 0; i < numel(); ++i)
			{
				c(i) = (*this)(i) + s;
			}
			return c;
		}

		my_t operator - (T s) const
		{
			my_t c;
			for (int i = 0; i < numel(); ++i)
			{
				c(i) = (*this)(i) - s;
			}
			return c;
		}

		my_t operator * (T s) const
		{
			my_t c;
			for (int i = 0; i < numel(); ++i)
			{
				c(i) = (*this)(i) * s;
			}
			return c;
		}


		Matrix<T, M, 1> sub_col(int j) const
		{
			Matrix<T, M, 1> c;
			for (int i = 0; i < M; ++i)
			{
				c[i] = (*this)(i, j);
			}
			return c;
		}

		template<int K>
		Matrix<T, M, N + K> join_col(const Matrix<T, M, K>& b) const
		{
			Matrix<T, M, N + K> C;
			for (int i = 0; i < M; ++i)
			{
				for (int j = 0; j < N; ++j)
				{
					C(i, j) = (*this)(i, j);
				}

				for (int j = N; j < N + K; ++j)
				{
					C(i, j) = b(i, j - N);
				}
			}

			return C;
		}


		template<int K>
		Matrix<T, M+K, N> join_row(const Matrix<T, K, N>& b) const
		{
			Matrix<T, M+K, N> C;
			for (int j = 0; j < N; ++j)
			{
				for (int i = 0; i < M; ++i)
				{
					C(i, j) = (*this)(i, j);
				}

				for (int i = M; i < M + K; ++i)
				{
					C(i, j) = b(i-M, j);
				}
			}

			return C;
		}

	protected:
		T data[M*N];
	};

	template<typename T, int M, int N>
	Matrix<T, M, N> operator+(T s, const Matrix<T, M, N>& b)
	{
		return b + s;
	}
	
	template<typename T, int M, int N>
	Matrix<T, M, N> operator-(T s, const Matrix<T, M, N>& b)
	{
		Matrix<T, M, N> c;
		for (int i = 0; i < c.numel(); ++i)
		{
			c(i) = s - b[i];
		}
		return c;
	}


	template<typename T, int M, int N>
	Matrix<T, M, N> operator*(T s, const Matrix<T, M, N>& b)
	{
		return b * s;
	}


	template<typename T, int M, int N, int K>
	Matrix<T, M, K> operator* (const Matrix<T, M, N>& a, const Matrix<T, N, K>& b)
	{
		Matrix<T, M, K> c;
		for (int i = 0; i < M; ++i)
		{
			for (int k = 0; k < K; ++k)
			{
				T sum = 0;
				for (int j = 0; j < N; ++j)
				{
					sum += a(i,j) * b(j, k);
				}
				c(i, k) = sum;
			}
		}

		return c;
	}


	//////////////////////////////////////////////////////////////////////////
	// 按元素相乘
	template<typename T, int M, int N>
	Matrix<T, M, N> elemMul(const Matrix<T, M, N>& a, const Matrix<T, M, N>& b)
	{
		Matrix<T, M, N> c;

		for (int i = 0; i < c.numel(); ++i)
		{
			c[i] = a[i] * b[i];
		}

		return c;
	}

	template<int M, int N>
	Matrix<double, M, N> mat_f2d(const Matrix<float, M, N>& a)
	{
		Matrix<double, M, N> b;
		for (int i = 0; i < b.numel(); ++i)
		{
			b[i] = a[i];
		}
		return b;
	}

	template<int M, int N>
	Matrix<float, M, N> mat_d2f(const Matrix<double, M, N>& a)
	{
		Matrix<float, M, N> b;
		for (int i = 0; i < b.numel(); ++i)
		{
			b[i] = (float)a[i];
		}
		return b;
	}


	//////////////////////////////////////////////////////////////////////////

	typedef Matrix<float, 4, 1> fvec4;
	typedef Matrix<float, 1, 4> frowvec4;
	typedef Matrix<float, 4, 4> fmat4;

	typedef Matrix<float, 3, 1> fvec3;
	typedef Matrix<float, 1, 3> frowvec3;
	typedef Matrix<float, 3, 3> fmat3;

	typedef Matrix<float, 3, 4> fmat34;
	typedef Matrix<float, 4, 3> fmat43;

	typedef Matrix<float, 2, 1> fvec2;


	typedef Matrix<double, 4, 1> vec4;
	typedef Matrix<double, 1, 4> rowvec4;
	typedef Matrix<double, 4, 4> mat4;

	typedef Matrix<double, 3, 1> vec3;
	typedef Matrix<double, 1, 3> rowvec3;
	typedef Matrix<double, 3, 3> mat3;

	typedef Matrix<double, 3, 4> mat34;
	typedef Matrix<double, 4, 3> mat43;

	typedef Matrix<double, 2, 1> vec2;


	typedef Matrix<int, 2, 1> ivec2;
	typedef Matrix<int, 3, 1> ivec3;
	typedef Matrix<int, 4, 1> ivec4;



#define MY_PI       3.14159265358979323846   // pi
#define MY_PI_F       3.14159265358979323846f   // pi
	//////////////////////////////////////////////////////////////////////////
	vec3 normalize(const vec3& a);
	vec4 normalize(const vec4& a);


	fvec3 normalize(const fvec3& a);
	fvec4 normalize(const fvec4& a);


	mat3 inv(const mat3& A);
	mat4 inv(const mat4& A);

	fmat3 inv(const fmat3& A);
	fmat4 inv(const fmat4& A);

	// 向量点乘
	double dot(const vec2& a, const vec2& b);
	double dot(const vec3& a, const vec3& b);
	double dot(const vec4& a, const vec4& b);

	float dot(const fvec2& a, const fvec2& b);
	float dot(const fvec3& a, const fvec3& b);
	float dot(const fvec4& a, const fvec4& b);

	// 向量长度
	double magnitude(const vec3& a);
	float magnitude(const fvec3& a);

	// 向量叉乘
	vec3 cross(const vec3& a, const vec3& b);
	fvec3 cross(const fvec3& a, const fvec3& b);

	mat3 submat3(const mat4& a);
	fmat3 submat3(const fmat4& a);

	vec4 vec3to4(const vec3 a, double b);
	fvec4 vec3to4(const fvec3 a, float b);

	vec3 subvec3(const vec4& a);
	fvec3 subvec3(const fvec4& a);
}




