//
// Created by lacuz on 2018/11/4.
//

#pragma once


#include "CubieCube.h"
#include "myScene.h"
#include "myCubeScene.h"
#include "FaceCubeBase.h"
#include "myCubeSceneHandler.h"
#include "myQuaternion.h"
#include <chrono>
#include <list>
#include <array>


namespace my {


    class CubeControl {
    public:
        int lastStep = -1;
		std::array<double, 6> axisAng = {0, 0, 0, 0, 0, 0};

        cs::CubieCube cubieCube;
        CubeScene cubeScene;
        CubeSceneHandler sceneHandler;


        ESContext esContext;


        // IMU 运算相关
        bool use_cube_coord = true;    // 暂时使用，保持兼容性。是否在 c++ 代码中计算魔方姿态, 否则在 java 中计算.

        bool paused = false;

    protected:
        vec3 imu_gyro;  // 原始陀螺仪
        vec3 imu_acc;   // 原始加速度计
        vec3 imu_mag;   // 未校准原始数据
        quaternion imu_q;   // 原始四元数

        bool imu_is_frist_data = true;
        std::chrono::time_point<std::chrono::system_clock> imu_time;


        quaternion imu_qm = quaternion::eye();     // 磁力计校准后姿态
        quaternion imu_q2m = quaternion::eye();     // imu_qm = imu_q2m * imu_q, 绕世界坐标旋转


        quaternion imu_qt = quaternion::eye();        // touch 校准后的姿态
        quaternion imu_q2t = quaternion::eye();     // imu_qt = imu_q2t * imu_q, 绕世界坐标旋转



        Coordinate cube_coord = Coordinate::eye();      // 魔方坐标

        // 魔方传感器有倾斜 -135 deg: U ->z，BL -> x,  FL -> y
        Coordinate cube_coord_to_imu;       // 魔方相对于 imu 坐标， 在构造函数中初始化

        // 磁力计校准
        vec3 mag_offset = {0, 0, 0};   // 特殊编码表示未初始化过
        int mag_cal_cnt = 0;

        int mag_cal_min_samples = 2;        // 每个位置的最少采样点
        int mag_cal_max_samples = 20;        // 每个位置的最少采样点
        std::array<std::list<vec3>, 4> mag_cal_buffer_xy;
        std::array<std::list<vec3>, 4> mag_cal_buffer_z;


    public:
#ifdef ANDROID

        void init(EGLNativeWindowType window, std::string resPath);

#endif

        void init(std::string resPath);

        // 周期性调用，以更新 sceneHandler 数据
        void update(double deltaTime);

        // 画一帧
        void draw();

        void onPause();

        void onResume();

        void onSurfaceChanged(int width, int height);


        //更新魔方状态
        void updateStatus(int cubeStr[54]);

        //更新魔方四元数、角度、公式,返回魔方状态
        std::vector<int> updateQAF(int datas[16]);

        //更新魔方陀螺仪
        void updateGyro(int datas[3]);

        //更新魔方加速度，电池，磁力计
        void updateACM(int datas[]);

        //执行公式，用于虚拟魔方,0绝对公式 1相对公式
        void keyClick(int type, int key);

        //获取当前魔方还原公式
        std::vector<cs::Move> getSolve();

        // 方便 java 调用
        std::vector<int> getSolveI();

        //获取魔方打乱
        std::vector<cs::Move> getSolve(int axis_r, cs::CubieCube destCubieCube);

        // 方便 java 调用
        std::vector<int> getSolveI(int axis_r, cs::CubieCube destCubieCube);

        /**
         * 此方法后面弃用，使用getRobotSolve(int beforeCubeStatus[54], int toCubeStatus[54])弃用
        * 使用机器人还原智能魔方
        * bCubeStatus：发送指给机器人前，魔方状态
        * play_fomula：打乱公式，Move下标
        * formula_len:打乱公式长度
        * 后面需要做异常回调
        */
        std::vector<int> getRobotSolve(int bCubeStatus[54], int play_fomula[], int formula_len);
        /**
        * 使用机器人还原智能魔方
        * beforeCubeStatus：发送Rx1给机器人前，魔方状态
        * toCubeStatus:打乱目标状态
        * 后面需要做异常回调
        */
        std::vector<int> getRobotSolve(int beforeCubeStatus[54], int toCubeStatus[54]);

        //获取校正后的魔方四元数
        quaternion getCubeQuaternion();

        //处理陀螺仪计算
        void doGyro();

        //处理加速度
        void doAcc();

        //处理磁力计
        void doMag();

        // 磁力计校准
        void doMagCal();


        // 手触校准, 绕 z 轴的变化量 degree
        void onTouch(double deltaAngleZ);


        CubeControl();

        ~CubeControl();


    private:
        struct _ball_fit_ret_t {
            vec3 p;        // 球心
            double r;    // 半径
            int iter;    // 迭代次数
            bool converge;    // 是否收敛
        };

        // 最小二乘拟合球
        _ball_fit_ret_t
        _ball_fit(const std::vector<vec3> raw, double coef, double tol, int maxIter);


    };


}
