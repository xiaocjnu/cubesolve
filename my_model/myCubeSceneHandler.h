#pragma once

#include "myCubeScene.h"
#include "myAnimationCoord.h"
#include "myAnimationCube.h"
#include "myAnimationArrow.h"
#include "AxisCube.h"
#include "myMatrix.h"
#include "util.h"

// 控制魔方状态，角度和3D模型坐标，相对旋转公式和动画的超级类

namespace my
{
	class CubeSceneHandler
	{
	public:

		CubeSceneHandler(CubeScene& scene);

		// 初始化, srcCube == nullptr 重新加载
		bool init(ESContext* esContext, const CubeModel* srcCube = nullptr);

		// 按绝对公式旋转, 不支持 x,y,z 等会转中心块的公式
		void moveAbsolute(cs::Move mv,double delay);
		void moveAbsolute(cs::Move mv);

		// 相对公式，支持 x,y,z 等会转中心块的公式
		void moveRelative(cs::Move mv,double delay);
		void moveRelative(cs::Move mv);

		// 根据 myRelativeFormulaMode 自动选择相对公式或绝对公式
		void move(cs::Move mv, double delay);
		void move(cs::Move mv);

		// 相对公式模式
		bool getRelativeFormulaMode() const { return myRelativeFormulaMode; };
		void setRelativeFormulaMode(bool b) { myRelativeFormulaMode = b; };

		// 相机与世界坐标原点O的距离，范围 (0,inf)
		double getCamDistance() const { return myCamDistance; };
		void setCamDistance(double d);

		// 原点O到相机原点的俯仰角(与 xy 平面的夹角)，范围 [-89,89]
		double getCamPitch() const { return myCamPitch; };
		void setCamPitch(double a);

		// 原点O到相机原点的偏向角(与 x 轴的夹角)
		double getCamYaw() const { return myCamYaw; };
		void setCamYaw(double a);

		void setCam(double dist, double pitch, double yaw);


		// 计算相对公式时相对于相机，默认true. false则用 myRefAng 代替 myCamAng
		bool getRefToCam() const { return myRefPitchToCam; };
		void setRefToCam(bool b) { myRefPitchToCam = b; };

		// myRefToCam 为 false 时使用，用来代替 myCamAng 计算相对公式,范围 [-89,89]
		double getRefPitch() const { return myRefPitch; };
		void setRefPitch(double a);

		// 周期性调用以实现动画
		void update(double deltaTime);


		// 设置默认动画时间
		void setDefaultDelay(double t);

		// 获取相对公式的 axisCube 在 axisCube::valid_table 中的下标
		int getRelativeAxisCubeIdx();

		// 获取相对公式的 axisCube
		cs::AxisCube getRelativeAxisCube();


		AnimationCube& cubeAnimation() {return myCubeAni; };
		AnimationArrow& arrowAnimation() { return myArrowAni; };

		void setCoordAniOriginal();


		// 设置是否显示箭头
		void showArrow(bool b);



		// 返回坐标处的贴纸编号, 0 - 53, -1 表示无贴纸
		// 输入坐标范围是相对坐标 -1.0 to 1.0, x = -1.0 表示屏幕左边，y=-1.0表示屏幕下方
		int hitFacelet_norm(double x, double y);

		// 返回坐标处的贴纸编号, 0 - 53, -1 表示无贴纸
		// 输入坐标范围是相对坐标 0 to Width - 1 or Height - 1
		int hitFacelet(int x, int y);


		// 返回坐标处的角块编号, 0 - 7, -1 表示无
		int hitCorner_norm(double x, double y);

		// 返回坐标处的角块编号, 0 - 7, -1 表示无
		int hitCorner(int x, int y);


		// 返回坐标处的棱块编号, 0 - 11, -1 表示无
		int hitEdge_norm(double x, double y);

		// 返回坐标处的棱块编号, 0 - 11, -1 表示无
		int hitEdge(int x, int y);

		// 返回坐标处的中心块编号, 0 - 5, -1 表示无
		int hitCenter_norm(double x, double y);

		// 返回坐标处的中心块编号, 0 - 5, -1 表示无
		int hitCenter(int x, int y);


	protected:
		void _update_cam();


		const double angle_eps = 1e-5;

	public:
		CubeScene& myCubeScene;
		CubeModel& myCube;
		Camera& myCam;

		AnimationCube myCubeAni;
		//AnimationCoord myCoordAni;
		AnimationArrow myArrowAni;

		bool myIsInited = false;


		bool myRelativeFormulaMode = true;		// 相对公式模式


		double myCamDistance = 200;	// 相机与世界坐标原点O的距离，范围 (0,inf)
		double myCamPitch = 30.0f;		// 原点O到相机原点的俯仰角(与 xy 平面的夹角)，范围 [-89,89]
		double myCamYaw = -60.0f;		// 原点O到相机原点的偏向角(与 x 轴的夹角)

		bool myRefPitchToCam = false;		// 计算相对公式时相对于相机，默认true. false则用 myRefPitch 代替 myCamPitch
		bool myRefYawToCam = true;


		double myRefPitch = 0.0f;		// myRefToCam 为 false 时使用，用来代替 myCamPitch 计算相对公式,范围 [-89,89]
		double myRefYaw = -60.0f;

		
	};
}