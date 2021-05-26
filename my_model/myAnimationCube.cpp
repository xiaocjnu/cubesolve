
#include <array>
#include "myUtils.h"
#include "myAnimationCube.h"

using namespace std;

namespace my
{
     quaternion AnimationCube::axis_cube_to_q[24] =
	{
		{1,0,0,0},
		{-0.70710678118654746172,0,0,-0.70710678118654757274},
		{0,0,0,-1},
		{0.70710678118654757274,0,0,-0.70710678118654746172},
		{-4.3297802811774658128e-17,0.70710678118654746172,4.3297802811774670454e-17,0.70710678118654757274},
		{0.49999999999999988898,0.5,-0.5,0.50000000000000011102},
		{-0.70710678118654746172,0,0.70710678118654757274,0},
		{-0.5,0.49999999999999988898,0.50000000000000011102,0.5},
		{0.50000000000000011102,-0.5,0.5,-0.49999999999999988898},
		{0.70710678118654757274,-0.70710678118654746172,0,0},
		{-0.5,0.49999999999999988898,0.50000000000000011102,-0.5},
		{4.3297802811774670454e-17,-4.3297802811774658128e-17,0.70710678118654757274,-0.70710678118654746172},
		{6.1232339957367660359e-17,-1,0,0},
		{4.3297802811774670454e-17,-0.70710678118654757274,0.70710678118654746172,4.3297802811774658128e-17},
		{6.1232339957367660359e-17,0,1,0},
		{-4.3297802811774658128e-17,0.70710678118654746172,0.70710678118654757274,4.3297802811774670454e-17},
		{0.70710678118654757274,0,0.70710678118654746172,0},
		{-0.5,0.50000000000000011102,-0.49999999999999988898,-0.5},
		{4.3297802811774670454e-17,0.70710678118654757274,4.3297802811774658128e-17,-0.70710678118654746172},
		{0.50000000000000011102,0.5,0.5,-0.49999999999999988898},
		{0.49999999999999988898,0.5,0.5,0.50000000000000011102},
		{-0.70710678118654746172,-0.70710678118654757274,0,0},
		{-0.5,-0.50000000000000011102,0.49999999999999988898,0.5},
		{-4.3297802811774658128e-17,-4.3297802811774670454e-17,0.70710678118654746172,0.70710678118654757274},
	};


	AnimationCube::AnimationCube(CubeModel& _cube)
		: myCube(_cube)
		, myAniCoord(_cube)
	{
	    setFrame((Coordinate)myCube, myCube.getCubeAng());
	}


	AnimationCube::~AnimationCube()
	{

	}
	void AnimationCube::update(double deltaTime)
	{
		// 坐标移动
		myAniCoord.update(deltaTime);


		// cubeang
		_updateCa(deltaTime);
	}


	void AnimationCube::setFrame(double delay, const Coordinate & coord, CubeAng ca, vec3 ref_rot_axis, std::array<int, 6> ang_rot_dir)
	{
		myAniCoord.setFrame(delay, coord, ref_rot_axis);
		_setFrameCa(ca, ang_rot_dir);
		_setRotSpeed(delay);
	}

	void AnimationCube::setFrame(const Coordinate & coord, CubeAng ca, vec3 ref_rot_axis, std::array<int, 6> ang_rot_dir)
	{
		setFrame(myDefaultDelay, coord, ca, ref_rot_axis, ang_rot_dir);
	}

	void AnimationCube::setFrame(double delay, CubeAng ca, std::array<int, 6> ang_rot_dir)
	{
		setFrame(delay, myAniCoord.getDstFrame(), ca,
			myAniCoord.getRefRotAxis(), ang_rot_dir);
	}



	void AnimationCube::setFrame(CubeAng ca, std::array<int, 6> ang_rot_dir)
	{
		setFrame(myDefaultDelay, ca, ang_rot_dir);
	}

	void AnimationCube::setFrame(double delay, const Coordinate & coord, vec3 ref_rot_axis)
	{
		myAniCoord.setFrame(delay, coord, ref_rot_axis);
	}


	void AnimationCube::setFrame(const Coordinate & coord, vec3 ref_rot_axis)
	{
		setFrame(myDefaultDelay, coord, ref_rot_axis);
	}

	void AnimationCube::rotAxis(int axis, double ang)
	{
		CubeAng ca = myCube.getCubeAng();
		ca.rotateAxis(axis, ang);
		setFrame(ca);
	}

// 	void AnimationCube::nextFrame(const cs::CubieCube & cc, std::array<double, 6> axisAng, double delay)
// 	{
// 		//有未执行完的动画，强行执行完
// 				finish();
// 		
// 		
// 				if (delay <= 0.0f)	// 没有动画
// 				{
// 					myDelay = 0.0f;
// 					my_dst.setCubieCube(cc);
// 					my_dst.setAxisAng(axisAng);
// 					myCube.setCubeAng(my_dst);
// 			
// 		
// 					return;
// 				}
// 		
// 				// 动画
// 				myDelay = delay;
// 				my_dst.setCubieCube(cc);
// 				my_dst.setAxisAng(axisAng);
// 		
// 				const cs::CubieCube& cube_cc = myCube.getCubieCube();
// 		
// 				
// 				int numMove = -1;
// 				int move[2];
// 		
// 				if (cube_cc == cc)	// no move
// 				{
// 					numMove = 0;
// 				}
// 				else
// 				{
// 					// 一个轴
// 					for (int i = 0; i < 18; ++i)
// 					{
// 						if (cube_cc.move(i) == cc)
// 						{
// 							numMove = 1;
// 							move[0] = i;
// 							break;
// 						}
// 					}
// 				}
// 		
// 				// 两个轴
// 				if (numMove == -1)
// 				{
// 					for (int i = 0; i < 18; ++i)
// 					{
// 						for (int j = 0; j < 18; ++j)
// 						{
// 							if (cube_cc.move(i).move(j) == cc)
// 							{
// 								numMove = 2;
// 								move[0] = i;
// 								move[1] = j;
// 								break;
// 							}
// 						}
// 					}
// 				}
// 		
// 		
// 				if (numMove == -1)	// 转太多轴，没有动画
// 				{
// 					myDelay = 0.0f;
// 					myCube.setCubieCube(cc);
// 					myCube.setAxisAng(axisAng);
// 					return;
// 				}
// 		
// 		
// 				double deltaAng[6];
// 		
// 				for (int i = 0; i < 6; ++i)
// 				{
// 					deltaAng[i] = axisAng[i] - myCube.getAxisAng()[i];
// 				}
// 		
// 				for (int k = 0; k < numMove; ++k)
// 				{
// 					int ax = move[k] / 3;
// 					int po = move[k] % 3 + 1;
// 		
// 					deltaAng[ax] += 90.0f * po;
// 				}
// 		
// 				// 角度修正
// 		
// 				int idx_max = 0;	// 查找最大的旋转角度
// 				for (int i = 0; i < 6; ++i)
// 				{
// 					while (deltaAng[i] >= 180.0f)
// 					{
// 						deltaAng[i] -= 360.0f;
// 					}
// 		
// 					while (deltaAng[i] < -180.0f)
// 					{
// 						deltaAng[i] += 360.0f;
// 					}
// 		
// 					if (abs(deltaAng[i]) > abs(deltaAng[idx_max]))
// 					{
// 						idx_max = i;
// 					}
// 					
// 				}
// 		
// 		
// 				if (numMove == 0)
// 				{
// 					my_rotAxis[0] = idx_max;
// 				}
// 				else if (numMove == 1)
// 				{
// 					my_rotAxis[0] = move[0] / 3;
// 				}
// 				else // 2
// 				{
// 					my_rotAxis[0] = move[0] / 3;
// 				}
// 		
// 				my_rotAng[0] = deltaAng[my_rotAxis[0]] / myDelay;
// 		
// 				my_rotAxis[1] = my_rotAxis[0] < 3 ? my_rotAxis[0] + 3 : my_rotAxis[0] - 3;
// 				my_rotAng[1] = deltaAng[my_rotAxis[1]] / myDelay;
// 		
// 		
// 				// 直接旋转其他轴
// 				for (int i = 0; i < 6; ++i)
// 				{
// 					// 旋转
// 					if (i != my_rotAxis[0] && i != my_rotAxis[1] && deltaAng[i] != 0)
// 					{
// 						myCube.rotateAxis(i, deltaAng[i]);
// 					}
// 				}
// 		
// 	}
// 	void AnimationCube::nextFrame(const cs::CubieCube & cc, std::array<double, 6> axisAng)
// 	{
// 		nextFrame(cc, axisAng, myDefaultDelay);
// 	}
// 	void AnimationCube::nextFrame(const cs::CubieCube & cc, double delay)
// 	{
// 		std::array<double, 6> axisAng = { 0,0,0,0,0,0 };
// 		nextFrame(cc, axisAng, delay);
// 	}
// 	void AnimationCube::nextFrame(const cs::CubieCube & cc)
// 	{
// 		nextFrame(cc, myDefaultDelay);
// 	}

// 	void AnimationCube::nextFrameRotAxis(int axis, double ang)
// 	{
// 		nextFrameRotAxis(axis, ang, myDefaultDelay);
// 	}
// 	void AnimationCube::finish()
// 	{
// // 		if (isBusy())
// // 		{
// // 			myDelay = 0.0f;
// // 			myCube.setCubeAng(my_dst);
// // 		}
// 
// 	}




void AnimationCube::_setFrameCa(const CubeAng & ca, std::array<int, 6> ang_rot_dir)
	{
		
		my_dstCa = ca;
		my_dstCa.normalize();
		my_dstAng = my_dstCa.getAxisAng();
		my_angRotDir = ang_rot_dir;
		cs::CubieCube dst_cc = my_dstCa.getCubieCube();


		// 判断转动的轴
		const cs::CubieCube& cube_cc = myCube.getCubieCube();
		const array<double, 6>& cube_ang = myCube.getAxisAng();

		vector<int> moves = _search_1or2_moves(cube_cc, dst_cc);


		// 转太多轴，没有smooth, 直接跳状态
		if (moves.size() == 0 && !(cube_cc == dst_cc))
		{
			myCube.setCubeAng(my_dstCa);
			return;
		}


		// 两步只允许对面两个面，否则第一步直接跳转
		if (moves.size() == 2)
		{
			int ax1 = moves[0] / 3;
			int ax2 = moves[1] / 3;


			if (abs(ax1 - ax2) != 3)
			{
				//myCube.setCubieCube(cube_cc.move(moves[0]));
				myCube.setCubeAng(CubeAng(cube_cc.move(moves[0])));
				moves = { moves[1] };
			}
		}




		// 旋转角度大小修正
		array<double, 6> deltaAng;

		for (int i = 0; i < 6; ++i)
		{
			deltaAng[i] = fixAng180(my_dstAng[i] - myCube.getAxisAng()[i]);
		}

		for (int k = 0; k < (int)moves.size(); ++k)
		{
			int ax = moves[k] / 3;
			int po = moves[k] % 3 + 1;

			deltaAng[ax] = fixAng180(deltaAng[ax] + 90.0f * po);
		}

		for (int i = 0; i < 6; ++i)
		{
			// 控制旋转方向
			if (my_angRotDir[i] > 0 && deltaAng[i] < 0)
				deltaAng[i] += 360.0;
			else if (my_angRotDir[i] < 0 && deltaAng[i] > 0)
				deltaAng[i] -= 360.0;
				

			my_dstAng[i] = myCube.getAxisAng()[i] + deltaAng[i];

		}


		

		// 角度修正
		int idx_max = 0;	// 查找最大的旋转角度
		for (int i = 0; i < 6; ++i)
		{
			if (abs(deltaAng[i]) > abs(deltaAng[idx_max]))
			{
				idx_max = i;
			}

		}

		// 只允许对面两个轴有旋转动画
		bool isAni[6] = { false, false, false, false, false, false };


		int ax1;
		if (moves.size() == 0)
			ax1 = idx_max;
		else
			ax1 = moves[0] / 3;


		int ax2 = ax1 < 3 ? ax1 + 3 : ax1 - 3;

		isAni[ax1] = true;
		isAni[ax2] = true;


		// 直接旋转其他轴
		CubeAng _ca = myCube.getCubeAng();

		auto _aa = _ca.getAxisAng();


		for (int i = 0; i < 6; ++i)
		{
			if (!isAni[i])
			{
				_aa[i] = my_dstAng[i];
			}
		}

		_ca.setAxisAng(_aa);
		myCube.setCubeAng(_ca);
	}

	void AnimationCube::_setRotSpeed(double delay)
	{
		for (int i = 0; i < 6; ++i)
		{
			my_rotSpeed[i] = abs(my_dstAng[i] - myCube.getAxisAng()[i]) / delay;
		}
	}


	void AnimationCube::_updateCa(double deltaTime)
	{


		CubeAng ca = myCube.getCubeAng();
		cs::CubieCube last_cc = ca.getCubieCube();
		auto axisAng = ca.getAxisAng();


		for (int ax = 0; ax < 6; ++ax)
		{
			double dst_ang = my_dstAng[ax];
			double ang = axisAng[ax];

			double da = (dst_ang - ang);

			if (da == 0.0)
			{
				my_rotSpeed[ax] = 0.0;
			}
			else
			{
				double _da;
				if (my_rotSpeed[ax] == 0)
				{
					_da = da;
				}
				else
				{
					_da = da / abs(da) * my_rotSpeed[ax] * deltaTime;

					if (abs(_da) > abs(da))
					{
						_da = da;
						my_rotSpeed[ax] = 0.0;
					}
				}

				ca.rotateAxis_no_normalize(ax, _da);
			}

			
		}

		ca.normalize();
		myCube.setCubeAng(ca);

		// 已经改变 CubieCube, 重新计算
		if (!(ca.getCubieCube() == last_cc))
		{
			_setFrameCa(my_dstCa, my_angRotDir);
		}




	}
	std::vector<int> AnimationCube::_search_1or2_moves(const cs::CubieCube & from, const cs::CubieCube & to)
	{

		std::vector<int> moves;

		if (from == to)	// no move
		{
			return moves;
		}


		// 一个轴
		for (int i = 0; i < 18; ++i)
		{
			if (from.move(i) == to)
			{
				moves.push_back(i);
				return moves;
			}
		}

		// 两个轴
		for (int i = 0; i < 18; ++i)
		{
			for (int j = 0; j < 18; ++j)
			{
				if (from.move(i).move(j) == to)
				{
					moves.push_back(i);
					moves.push_back(j);
					return moves;
				}
			}
		}

		return moves;
	}
	void AnimationCube::setDefaultDelay(double t)
	{
		myDefaultDelay = t;
		myAniCoord.setDefaultDelay(t);
	}
}


