#include "myAnimationArrow.h"


namespace my
{
	//////////////////////////////////////////////////////////////////////////
	// ArrowAnimation
	//////////////////////////////////////////////////////////////////////////

	vec3 AnimationArrow::pos_table[6] =
	{
		{0,0,1},	// U
		{1,0,0},	// R
		{0,-1,0},	// F
		{0,0,-1},	// D
		{-1,0,0},	// L
		{0,1,0}		// B
	};

	vec3 AnimationArrow::euler_table[6] =
	{
		{0,0,0},	// U
		{0,90,0},	// R
		{90,0,0},	// F
		{180,0,0},	// D
		{0,-90,0},	// L
		{-90,0,0}	// B
	};



	AnimationArrow::AnimationArrow(CubeScene& scene)
		: myScene(scene)
	{
	}

	void AnimationArrow::update(double deltaTime)
	{
		myScene.setArrowPoseAsCube();

		ModelGroup* arrow_group = myScene.getArrowGroup();

		if (myIsOn && arrow_group && arrow_group->models.size() > 0)
		{
			Model& arrow = arrow_group->models[0];

			double da = mySpeed * deltaTime;
			arrow.rotateLocal(0, 0, da);

			
		}
	}

	bool AnimationArrow::isOn() const
	{
		return myIsOn;
	}

	void AnimationArrow::setOnOff(bool b)
	{
		myScene.showArrow(b);
		myIsOn = b;
	}

	void AnimationArrow::setMove(cs::Move mv)
	{
		


		int mm = (int)mv;

		if (0 <= mm && mm < 18)
		{
			int ax = mm / 3;
			int po = mm % 3;

			myAxis = ax;
			myDir = (po == 2 ? false : true);


			ModelGroup* arrow_group = myScene.getArrowGroup();

			if (arrow_group && arrow_group->models.size() > 0)
			{
				Model& arrow = arrow_group->models[0];
				arrow.setPos(pos_table[ax] * myScene.cube_arrow_offset);
				arrow.setEuler(euler_table[ax]);
				
				if (myDir) // 顺时针
				{
					arrow.rotateLocal(0, 180, 0);
				}

			}

		}
	}


}


