#pragma once


//
// Created by lacuz on 2019/01/09.
//



#include "AxisCubieCube.h"
#include "myScene.h"
#include "myCubeScene.h"
#include "FaceCubeBase.h"
#include "myCubeSceneHandler.h"
#include "myQuaternion.h"
#include <chrono>
#include <list>
#include <array>


namespace my {


    class CubeControlNew {
    public:
        cs::AxisCubieCube axisCubieCube;
        CubeScene cubeScene;
        CubeSceneHandler sceneHandler;

        std::array<double,6> axisAng = {0, 0, 0, 0, 0, 0};

        ESContext esContext;


        // IMU 运算相关
        bool use_cube_coord = true;    // 暂时使用，保持兼容性。是否在 c++ 代码中计算魔方姿态, 否则在 java 中计算.

        bool paused = false;

    protected:
        quaternion imu_q;   // 原始四元数
        quaternion imu_qt = quaternion::eye();        // touch 校准后的姿态
        quaternion imu_q2t = quaternion::eye();     // imu_qt = imu_q2t * imu_q, 绕世界坐标旋转



        Coordinate cube_coord = Coordinate::eye();      // 魔方坐标

        // 魔方传感器有倾斜 -135 deg: U ->z，BL -> x,  FL -> y
        Coordinate cube_coord_to_imu;       // 魔方相对于 imu 坐标， 在构造函数中初始化


    public:

        bool init(std::string resPath);

        // 周期性调用，以更新 sceneHandler 数据
        void update(double deltaTime);

        // 画一帧
        void draw();

        void onPause();

        void onResume();

        void onSurfaceChanged(int width, int height);


        //充值魔方状态
        void onCubeReset();

        //更新魔方状态
        void onCubeStatus(int cubeStr[54]);

        //更新魔方四元数、角度、公式,返回魔方状态,is_update:是否更新
        void onCubeQuaternion(double qua[3]);

        //更新魔方角度
        void onCubeAngle(int angles[6]);
        void onCubeAngle(int angles[6], bool is_update);

        //执行绝对公式
        void onCubeMoveAbsolute(int m);
        void onCubeMoveAbsolute(int m,double delay);
        void onCubeMoveAbsolute(cs::Move m);
        void onCubeMoveAbsolute(cs::Move m,double delay);

        //执行相对公式
        void onCubeMoveRelative(int m);
        void onCubeMoveRelative(int m,double delay);
        void onCubeMoveRelative(cs::Move m);
        void onCubeMoveRelative(cs::Move m,double delay);

        //获取当前魔方状态
        int getCubeAxisCubeIndex();

        //同时更新四元数，角度，公式
        void onCubeFrameUpdate(double qua[4],int angles[6],int moves[],int moves_len);

        // 手触校准, 绕 z 轴的变化量 degree
        void onCubeTouch(double deltaAngleZ);
        // 手触校准, 绕 z 轴的总变化量 degree
        void onCubeTouchSum(double sumAngleZ);

        // 设置场景背景
        void setSceneBg(int color);
        void setSceneBg(int r,int g, int b,float alpha);

        //从AxisCube::eye 到 AxisCube[index]
        void setAxisCube(int axisCubeIndex);

        //设置场景箭头操作
        void setArrowOff(bool b);
        void setArrowMove(int move);

        //设置魔方连接和未连接样式
        void setCubeConnected();
        void setCubeUnConnected();

        //调整魔方颜色，变暗
        void adjustCubeColor();

        CubeControlNew();

        ~CubeControlNew();

    };


}
