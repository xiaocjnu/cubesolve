#pragma once


namespace my
{
	// 将角度限制在 [0, 360)
	double fixAng360(double a);


	// 将角度限制在 [-180, 180)
	double fixAng180(double a);
}