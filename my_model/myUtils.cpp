#include "myUtils.h"


namespace my
{


	double fixAng360(double a)
	{
		while (a >= 360)
		{
			a -= 360;
		}

		while (a < 0)
		{
			a += 360;
		}

		return a;
	}

	double fixAng180(double a)
	{
		while (a >= 180)
		{
			a -= 360;
		}

		while (a < -180)
		{
			a += 360;
		}

		return a;
	}

}