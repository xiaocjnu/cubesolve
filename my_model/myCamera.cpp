#include "myCamera.h"
#include <cmath>
#include "myTransform.h"

namespace my
{
	Camera::Camera(double fov)
		: my_fov(fov)
	{
	}

	Camera::~Camera()
	{
	}

	mat4 Camera::getViewMatrix() const
	{
		return inv(getTransfrom());
	}
	mat4 Camera::getProjMatrix(double aspect, double nearZ, double farZ) const
	{
		double h_diag = nearZ * tan(MY_PI * my_fov / 180.0f / 2.0f);	// 正方形对角线的一半
		double h_len = h_diag / sqrt(2.0f);	// 正方形边长的一半

		double hx, hy;
		if (aspect >= 1)	// 宽比高大
		{
			hx = h_len;
			hy = h_len / aspect;
		}
		else
		{
			hx = h_len * aspect;
			hy = h_len;
		}

		return frustum(-hx, hx, -hy, hy, nearZ, farZ);
	}
}


