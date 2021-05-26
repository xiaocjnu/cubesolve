#include "myMatrix.h"
#include <cmath>
#include <memory>

namespace my
{

	vec3 my::normalize(const vec3& a)
	{
		vec3 b;
		double n = sqrt(dot(a, a));

		b(0) = a(0) / n;
		b(1) = a(1) / n;
		b(2) = a(2) / n;

		return b;
	}

	vec4 my::normalize(const vec4& a)
	{
		vec4 b;
		double n = sqrt(dot(a,a));

		b(0) = a(0) / n;
		b(1) = a(1) / n;
		b(2) = a(2) / n;
		b(3) = a(3) / n;

		return b;
	}

	fvec3 my::normalize(const fvec3& a)
	{
		fvec3 b;
		float n = sqrt(dot(a, a));

		b(0) = a(0) / n;
		b(1) = a(1) / n;
		b(2) = a(2) / n;

		return b;
	}

	fvec4 my::normalize(const fvec4& a)
	{
		fvec4 b;
		float n = sqrt(dot(a, a));

		b(0) = a(0) / n;
		b(1) = a(1) / n;
		b(2) = a(2) / n;
		b(3) = a(3) / n;

		return b;
	}


	double dot(const vec2 & a, const vec2 & b)
	{
		return a[0] * b[0] + a[1] * b[1];
	}

	double dot(const vec3& a, const vec3& b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}

	double dot(const vec4& a, const vec4& b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + +a[3] * b[3];
	}

	float dot(const fvec2 & a, const fvec2 & b)
	{
		return a[0] * b[0] + a[1] * b[1];
	}

	float dot(const fvec3& a, const fvec3& b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}

	float dot(const fvec4& a, const fvec4& b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + +a[3] * b[3];
	}

	double magnitude(const vec3 & a)
	{
		return sqrt(dot(a, a));
	}

	float magnitude(const fvec3 & a)
	{
		return sqrt(dot(a, a));
	}

	vec3 cross(const vec3& a, const vec3& b)
	{
		return vec3{
			a[1] * b[2] - b[1] * a[2],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - b[0] * a[1] };
	}

	fvec3 cross(const fvec3& a, const fvec3& b)
	{
		return fvec3{
			a[1] * b[2] - b[1] * a[2],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - b[0] * a[1] };
	}

	mat3 submat3(const mat4 & a)
	{
		return mat3
		{
			a(0,0), a(1,0), a(2,0),
			a(0,1), a(1,1), a(2,1),
			a(0,2), a(1,2), a(2,2)
		};
	}

	fmat3 submat3(const fmat4 & a)
	{
		return fmat3
		{
			a(0,0), a(1,0), a(2,0),
			a(0,1), a(1,1), a(2,1),
			a(0,2), a(1,2), a(2,2)
		};
	}

	vec4 vec3to4(const vec3 a, double b)
	{
		return vec4{ a[0], a[1], a[2], b };
	}

	fvec4 vec3to4(const fvec3 a, float b)
	{
		return fvec4{ a[0], a[1], a[2], b };
	}

	vec3 subvec3(const vec4 & a)
	{
		return vec3{ a[0], a[1], a[2] };
	}

	fvec3 subvec3(const fvec4 & a)
	{
		return fvec3{ a[0], a[1], a[2] };
	}

	//////////////////////////////////////////////////////////////////////////

	void _inv3(const double A[9], double B[9])
	{
		double x[9];
		int p1;
		int p2;
		int p3;
		double absx11;
		double absx21;
		double absx31;
		int itmp;
		memcpy(&x[0], &A[0], 9U * sizeof(double));
		p1 = 0;
		p2 = 3;
		p3 = 6;
		absx11 = std::abs(A[0]);
		absx21 = std::abs(A[1]);
		absx31 = std::abs(A[2]);
		if ((absx21 > absx11) && (absx21 > absx31)) {
			p1 = 3;
			p2 = 0;
			x[0] = A[1];
			x[1] = A[0];
			x[3] = A[4];
			x[4] = A[3];
			x[6] = A[7];
			x[7] = A[6];
		}
		else {
			if (absx31 > absx11) {
				p1 = 6;
				p3 = 0;
				x[0] = A[2];
				x[2] = A[0];
				x[3] = A[5];
				x[5] = A[3];
				x[6] = A[8];
				x[8] = A[6];
			}
		}

		x[1] /= x[0];
		x[2] /= x[0];
		x[4] -= x[1] * x[3];
		x[5] -= x[2] * x[3];
		x[7] -= x[1] * x[6];
		x[8] -= x[2] * x[6];
		if (std::abs(x[5]) > std::abs(x[4])) {
			itmp = p2;
			p2 = p3;
			p3 = itmp;
			absx11 = x[1];
			x[1] = x[2];
			x[2] = absx11;
			absx11 = x[4];
			x[4] = x[5];
			x[5] = absx11;
			absx11 = x[7];
			x[7] = x[8];
			x[8] = absx11;
		}

		x[5] /= x[4];
		x[8] -= x[5] * x[7];
		absx11 = (x[5] * x[1] - x[2]) / x[8];
		absx21 = -(x[1] + x[7] * absx11) / x[4];
		B[p1] = ((1.0 - x[3] * absx21) - x[6] * absx11) / x[0];
		B[p1 + 1] = absx21;
		B[p1 + 2] = absx11;
		absx11 = -x[5] / x[8];
		absx21 = (1.0 - x[7] * absx11) / x[4];
		B[p2] = -(x[3] * absx21 + x[6] * absx11) / x[0];
		B[p2 + 1] = absx21;
		B[p2 + 2] = absx11;
		absx11 = 1.0 / x[8];
		absx21 = -x[7] * absx11 / x[4];
		B[p3] = -(x[3] * absx21 + x[6] * absx11) / x[0];
		B[p3 + 1] = absx21;
		B[p3 + 2] = absx11;
	}

	void _inv4(const double A[16], double B[16])
	{
		int i0;
		signed char ipiv[4];
		double x[16];
		int j;
		signed char p[4];
		int b;
		int jj;
		int jp1j;
		int n;
		int jy;
		int ix;
		int iy;
		double smax;
		double s;
		int i;
		for (i0 = 0; i0 < 16; i0++) {
			B[i0] = 0.0;
			x[i0] = A[i0];
		}

		ipiv[0] = 1;
		ipiv[1] = 2;
		ipiv[2] = 3;
		for (j = 0; j < 3; j++) {
			b = j * 5;
			jj = j * 5;
			jp1j = b + 2;
			n = 4 - j;
			jy = 0;
			ix = b;
			smax = std::abs(x[b]);
			for (iy = 2; iy <= n; iy++) {
				ix++;
				s = std::abs(x[ix]);
				if (s > smax) {
					jy = iy - 1;
					smax = s;
				}
			}

			if (x[jj + jy] != 0.0) {
				if (jy != 0) {
					iy = j + jy;
					ipiv[j] = (signed char)(iy + 1);
					smax = x[j];
					x[j] = x[iy];
					x[iy] = smax;
					ix = j + 4;
					iy += 4;
					smax = x[ix];
					x[ix] = x[iy];
					x[iy] = smax;
					ix += 4;
					iy += 4;
					smax = x[ix];
					x[ix] = x[iy];
					x[iy] = smax;
					ix += 4;
					iy += 4;
					smax = x[ix];
					x[ix] = x[iy];
					x[iy] = smax;
				}

				i0 = (jj - j) + 4;
				for (i = jp1j; i <= i0; i++) {
					x[i - 1] /= x[jj];
				}
			}

			n = 2 - j;
			jy = b + 4;
			iy = jj;
			for (b = 0; b <= n; b++) {
				smax = x[jy];
				if (x[jy] != 0.0) {
					ix = jj + 1;
					i0 = iy + 6;
					i = (iy - j) + 8;
					for (jp1j = i0; jp1j <= i; jp1j++) {
						x[jp1j - 1] += x[ix] * -smax;
						ix++;
					}
				}

				jy += 4;
				iy += 4;
			}
		}

		p[0] = 1;
		p[1] = 2;
		p[2] = 3;
		p[3] = 4;
		if (ipiv[0] > 1) {
			jy = ipiv[0] - 1;
			iy = p[jy];
			p[jy] = 1;
			p[0] = (signed char)iy;
		}

		if (ipiv[1] > 2) {
			jy = ipiv[1] - 1;
			iy = p[jy];
			p[jy] = p[1];
			p[1] = (signed char)iy;
		}

		if (ipiv[2] > 3) {
			jy = ipiv[2] - 1;
			iy = p[jy];
			p[jy] = p[2];
			p[2] = (signed char)iy;
		}

		jy = p[0] - 1;
		b = jy << 2;
		B[b] = 1.0;
		for (j = 1; j < 5; j++) {
			if (B[(j + b) - 1] != 0.0) {
				i0 = j + 1;
				for (i = i0; i < 5; i++) {
					iy = (i + b) - 1;
					B[iy] -= B[(j + (jy << 2)) - 1] * x[(i + ((j - 1) << 2)) - 1];
				}
			}
		}

		jy = p[1] - 1;
		b = jy << 2;
		B[1 + b] = 1.0;
		for (j = 2; j < 5; j++) {
			if (B[(j + b) - 1] != 0.0) {
				i0 = j + 1;
				for (i = i0; i < 5; i++) {
					iy = (i + b) - 1;
					B[iy] -= B[(j + (jy << 2)) - 1] * x[(i + ((j - 1) << 2)) - 1];
				}
			}
		}

		jy = p[2] - 1;
		b = jy << 2;
		B[2 + b] = 1.0;
		for (j = 3; j < 5; j++) {
			if (B[(j + b) - 1] != 0.0) {
				i0 = j + 1;
				for (i = i0; i < 5; i++) {
					iy = b + 3;
					B[iy] -= B[(j + (jy << 2)) - 1] * x[((j - 1) << 2) + 3];
				}
			}
		}

		B[3 + ((p[3] - 1) << 2)] = 1.0;
		for (j = 0; j < 4; j++) {
			jy = j << 2;
			smax = B[3 + jy];
			if (smax != 0.0) {
				B[3 + jy] = smax / x[15];
				for (i = 0; i < 3; i++) {
					b = i + jy;
					B[b] -= B[3 + jy] * x[i + 12];
				}
			}

			smax = B[2 + jy];
			if (smax != 0.0) {
				B[2 + jy] = smax / x[10];
				for (i = 0; i < 2; i++) {
					B[i + jy] -= B[2 + jy] * x[i + 8];
				}
			}

			smax = B[1 + jy];
			if (smax != 0.0) {
				B[1 + jy] = smax / x[5];
				for (i = 0; i < 1; i++) {
					B[jy] -= B[1 + jy] * x[4];
				}
			}

			if (B[jy] != 0.0) {
				B[jy] /= x[0];
			}
		}
	}

	void _inv3(const float A[9], float B[9])
	{
		float x[9];
		int p1;
		int p2;
		int p3;
		float absx11;
		float absx21;
		float absx31;
		int itmp;
		for (p1 = 0; p1 < 9; p1++) {
			x[p1] = A[p1];
		}

		p1 = 0;
		p2 = 3;
		p3 = 6;
		absx11 = std::abs(A[0]);
		absx21 = std::abs(A[1]);
		absx31 = std::abs(A[2]);
		if ((absx21 > absx11) && (absx21 > absx31)) {
			p1 = 3;
			p2 = 0;
			x[0] = A[1];
			x[1] = A[0];
			x[3] = A[4];
			x[4] = A[3];
			x[6] = A[7];
			x[7] = A[6];
		}
		else {
			if (absx31 > absx11) {
				p1 = 6;
				p3 = 0;
				x[0] = A[2];
				x[2] = A[0];
				x[3] = A[5];
				x[5] = A[3];
				x[6] = A[8];
				x[8] = A[6];
			}
		}

		absx11 = x[1] / x[0];
		x[1] /= x[0];
		absx21 = x[2] / x[0];
		x[2] /= x[0];
		x[4] -= absx11 * x[3];
		x[5] -= absx21 * x[3];
		x[7] -= absx11 * x[6];
		x[8] -= absx21 * x[6];
		if (std::abs(x[5]) > std::abs(x[4])) {
			itmp = p2;
			p2 = p3;
			p3 = itmp;
			x[1] = absx21;
			x[2] = absx11;
			absx11 = x[4];
			x[4] = x[5];
			x[5] = absx11;
			absx11 = x[7];
			x[7] = x[8];
			x[8] = absx11;
		}

		absx11 = x[5] / x[4];
		x[5] /= x[4];
		x[8] -= absx11 * x[7];
		absx11 = (x[5] * x[1] - x[2]) / x[8];
		absx21 = -(x[1] + x[7] * absx11) / x[4];
		B[p1] = ((1.0F - x[3] * absx21) - x[6] * absx11) / x[0];
		B[p1 + 1] = absx21;
		B[p1 + 2] = absx11;
		absx11 = -x[5] / x[8];
		absx21 = (1.0F - x[7] * absx11) / x[4];
		B[p2] = -(x[3] * absx21 + x[6] * absx11) / x[0];
		B[p2 + 1] = absx21;
		B[p2 + 2] = absx11;
		absx11 = 1.0F / x[8];
		absx21 = -x[7] * absx11 / x[4];
		B[p3] = -(x[3] * absx21 + x[6] * absx11) / x[0];
		B[p3 + 1] = absx21;
		B[p3 + 2] = absx11;
	}

	void _inv4(const float x[16], float y[16])
	{
		float A[16];
		int i0;
		signed char ipiv[4];
		int j;
		signed char p[4];
		int c;
		int jBcol;
		int ix;
		int k;
		float smax;
		float s;
		int kAcol;
		int i;
		for (i0 = 0; i0 < 16; i0++) {
			y[i0] = 0.0F;
			A[i0] = x[i0];
		}

		for (i0 = 0; i0 < 4; i0++) {
			ipiv[i0] = (signed char)(1 + i0);
		}

		for (j = 0; j < 3; j++) {
			c = j * 5;
			jBcol = 0;
			ix = c;
			smax = std::abs(A[c]);
			for (k = 2; k <= 4 - j; k++) {
				ix++;
				s = std::abs(A[ix]);
				if (s > smax) {
					jBcol = k - 1;
					smax = s;
				}
			}

			if (A[c + jBcol] != 0.0F) {
				if (jBcol != 0) {
					ipiv[j] = (signed char)((j + jBcol) + 1);
					ix = j;
					jBcol += j;
					for (k = 0; k < 4; k++) {
						smax = A[ix];
						A[ix] = A[jBcol];
						A[jBcol] = smax;
						ix += 4;
						jBcol += 4;
					}
				}

				i0 = (c - j) + 4;
				for (i = c + 1; i + 1 <= i0; i++) {
					A[i] /= A[c];
				}
			}

			jBcol = c;
			kAcol = c + 4;
			for (i = 1; i <= 3 - j; i++) {
				smax = A[kAcol];
				if (A[kAcol] != 0.0F) {
					ix = c + 1;
					i0 = (jBcol - j) + 8;
					for (k = 5 + jBcol; k + 1 <= i0; k++) {
						A[k] += A[ix] * -smax;
						ix++;
					}
				}

				kAcol += 4;
				jBcol += 4;
			}
		}

		for (i0 = 0; i0 < 4; i0++) {
			p[i0] = (signed char)(1 + i0);
		}

		for (k = 0; k < 3; k++) {
			if (ipiv[k] > 1 + k) {
				jBcol = p[ipiv[k] - 1];
				p[ipiv[k] - 1] = p[k];
				p[k] = (signed char)jBcol;
			}
		}

		for (k = 0; k < 4; k++) {
			c = p[k] - 1;
			y[k + ((p[k] - 1) << 2)] = 1.0F;
			for (j = k; j + 1 < 5; j++) {
				if (y[j + (c << 2)] != 0.0F) {
					for (i = j + 1; i + 1 < 5; i++) {
						y[i + (c << 2)] -= y[j + (c << 2)] * A[i + (j << 2)];
					}
				}
			}
		}

		for (j = 0; j < 4; j++) {
			jBcol = j << 2;
			for (k = 3; k >= 0; k += -1) {
				kAcol = k << 2;
				if (y[k + jBcol] != 0.0F) {
					y[k + jBcol] /= A[k + kAcol];
					for (i = 0; i + 1 <= k; i++) {
						y[i + jBcol] -= y[k + jBcol] * A[i + kAcol];
					}
				}
			}
		}
	}

	mat3 inv(const mat3 & A)
	{
		mat3 B;
		_inv3(&(((mat3&)A)[0]), &B[0]);
		return B;
	}

	mat4 inv(const mat4 & A)
	{
		mat4 B;
		_inv4(&(((mat3&)A)[0]), &B[0]);
		return B;
	}

	fmat3 inv(const fmat3 & A)
	{
		fmat3 B;
		_inv3(&(((fmat3&)A)[0]), &B[0]);
		return B;
	}

	fmat4 inv(const fmat4 & A)
	{
		fmat4 B;
		_inv4(&(((fmat3&)A)[0]), &B[0]);
		return B;
	}

}

