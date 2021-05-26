#pragma once

#include "myMatrix.h"
#include "myQuaternion.h"
#include "myTransform.h"

namespace my
{
	// 相机模型，成像裁剪前为正方形
	class Camera : public Coordinate
	{
	public:

		// 构造相机, fov 为视野的角度，即正方形的对角线上的角度
		Camera(double fov = 60);

		~Camera();


		double getFov() const { return my_fov; };
		void setFov(double fov) { my_fov = fov; };
		
		mat4 getViewMatrix() const;

		// 获取投影矩阵, aspect 为宽高比
		mat4 getProjMatrix(double aspect, double nearZ, double farZ) const;

	protected:
		double my_fov;
		double my_f;		// 焦距
		double my_len;	// 传感器边长，传感器为正方形


	};

}


