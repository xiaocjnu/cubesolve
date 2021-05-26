//
// Created by lacuz on 2018/11/4.
//

#include "MyCubeControlNew.h"
#include "Search.h"
#include "Search5.h"
#include "cs.h"
#include <cmath>

namespace my {
    using namespace std;
    using namespace cs;


    //////////////////////////////////////////////////////////////////////////
    CubeControlNew::CubeControlNew()
            : sceneHandler(cubeScene) {

        cube_coord_to_imu = Coordinate::eye();
//        cube_coord_to_imu.rotate(0, 0, 180); 最新左右相反
        cube_coord_to_imu.rotate(0, 0, 0);
    }

    CubeControlNew::~CubeControlNew() {

    }

    bool CubeControlNew::init(std::string resPath) {
        // 初始化场景
        axisCubieCube = cs::AxisCubieCube::eye();
        cubeScene.is_draw_test_box = false;
        cubeScene.background_color = {0.0f, 0.0f, 0.0f, 0};
        cubeScene.set_default_filename(resPath);
        cubeScene.cube.set_defaut_filename(resPath);


        cubeScene.use_msaa = false;      // 使用系统的抗锯齿


        if (!sceneHandler.init(&esContext))
            return false;

        sceneHandler.setDefaultDelay(0.15f);
        sceneHandler.setCamPitch(40);
        sceneHandler.setCamYaw(-65);
        sceneHandler.myRefPitch = 40;


        cs::init();

        return true;
    }


    void CubeControlNew::update(double deltaTime) {
        sceneHandler.update(deltaTime);
    }

    void CubeControlNew::draw() {
        cubeScene.draw(&esContext);
        //cubeScene.draw_triangle(&esContext);

    }

    void CubeControlNew::onPause() {
        cubeScene.onPause();
    }

    void CubeControlNew::onResume() {
        cubeScene.onResume();
    }

    void CubeControlNew::onSurfaceChanged(int width, int height) {
        esContext.width = width;
        esContext.height = height;

        cubeScene.onSurfaceChanged();
    }


    void CubeControlNew::onCubeReset() {
        axisCubieCube = cs::AxisCubieCube::eye();
        axisAng  = {0, 0, 0, 0, 0, 0};
        sceneHandler.setCoordAniOriginal();
        sceneHandler.cubeAnimation().setFrame(CubeAng(axisCubieCube.getAbsCubie(), axisAng));
//        cubeScene.cube.resetColor();
    }

    void CubeControlNew::onCubeStatus(int cubeStr[54]) {
        std::array<int, 54> arr;
        for (int i = 0; i < 54; i++) {
            arr[i] = cubeStr[i];
        }
        cs::ColorCube colorCube(arr);
        axisCubieCube.setAbsCubie(cs::CubieCube(colorCube));
        sceneHandler.cubeAnimation().setFrame(CubeAng(axisCubieCube.getAbsCubie(), axisAng));
    }


    void CubeControlNew::onCubeQuaternion(double qua[3]) {
        double qx = qua[0];
        double qy = qua[1];
        double qz = qua[2];
        double qw;
        double qsum = qx * qx + qy * qy + qz * qz;


        if (qsum <= 1) {
            qw = sqrt(1 - qsum);
        } else {
            qw = 0;
        }

        imu_q = quaternion(qw, qx, qy, qz);
        imu_qt = imu_q2t * imu_q;

        Coordinate ct = Coordinate::eye();
        ct.setQuaternion(imu_qt);

        cube_coord.setPos(0, 0, 0);
        cube_coord = ct.local2world(cube_coord_to_imu);

        if (use_cube_coord)
            sceneHandler.cubeAnimation().setFrame(cube_coord);
    }

    void CubeControlNew::onCubeAngle(int angles[6]) {
        onCubeAngle(angles, true);
    }

    void CubeControlNew::onCubeAngle(int angles[6], bool is_update) {
        //处理角度
        for (int i = 0; i < 6; i++) {
            //磁吸处理相差1、2个刻度规整处理
            if (angles[i] < 3 && angles[i] > -3) {
                angles[i] = 0;
            }
            axisAng[i] = angles[i] * 7.5;

        }
        if (is_update)
            sceneHandler.cubeAnimation().setFrame(CubeAng(axisCubieCube.getAbsCubie(), axisAng));
    }


    void CubeControlNew::onCubeMoveAbsolute(int m) {
        onCubeMoveAbsolute(Move(m));
    }

    void CubeControlNew::onCubeMoveAbsolute(int m, double delay) {
        onCubeMoveAbsolute(Move(m), delay);
    }

    void CubeControlNew::onCubeMoveAbsolute(cs::Move m) {
        axisCubieCube = axisCubieCube.moveAbs(m);
        sceneHandler.moveAbsolute(m);
    }

    void CubeControlNew::onCubeMoveAbsolute(cs::Move m, double delay) {
        axisCubieCube = axisCubieCube.moveAbs(m);
        sceneHandler.moveAbsolute(m,delay);
    }

    void CubeControlNew::onCubeMoveRelative(int m) {
        onCubeMoveRelative(Move(m));
    }

    void CubeControlNew::onCubeMoveRelative(int m, double delay) {
        onCubeMoveRelative(Move(m), delay);
    }

    void CubeControlNew::onCubeMoveRelative(cs::Move m) {
        axisCubieCube = axisCubieCube.move(m);
        sceneHandler.moveRelative(m);
    }

    void CubeControlNew::onCubeMoveRelative(cs::Move m, double delay) {
        axisCubieCube = axisCubieCube.move(m);
        sceneHandler.moveRelative(m,delay);
    }

    int CubeControlNew::getCubeAxisCubeIndex() {
        return axisCubieCube.axis.getValidIdx();
    }

    void
    CubeControlNew::onCubeFrameUpdate(double qua[4], int angles[6], int moves[], int moves_len) {
        //处理四元数
        double qx = qua[0];
        double qy = qua[1];
        double qz = qua[2];
        double qw = qua[3];

        imu_q = quaternion(qw, qx, qy, qz);
        imu_qt = imu_q2t * imu_q;

        Coordinate ct = Coordinate::eye();
        ct.setQuaternion(imu_qt);

        cube_coord.setPos(0, 0, 0);
        cube_coord = ct.local2world(cube_coord_to_imu);

        if (use_cube_coord)
            sceneHandler.cubeAnimation().setFrame(cube_coord);

        //处理角度
        for (int i = 0; i < 6; i++) {
            //磁吸处理相差1、2个刻度规整处理
            if (angles[i] < 3 && angles[i] > -3) {
                angles[i] = 0;
            }
            axisAng[i] = angles[i] * 7.5;

        }
        //处理转动
        for(int i=0;i<moves_len;i++){
            axisCubieCube = axisCubieCube.moveAbs(moves[i]);
            sceneHandler.cubeAnimation().setFrame(CubeAng(axisCubieCube.getAbsCubie(), axisAng));
        }
        sceneHandler.cubeAnimation().setFrame(CubeAng(axisCubieCube.getAbsCubie(), axisAng));
    }


    void CubeControlNew::onCubeTouch(double deltaAngleZ) {
        Coordinate c1;
        c1.setQuaternion(imu_q2t);
        c1.rotate({0, 0, deltaAngleZ});
        imu_q2t = c1.getQuaternion();
    }

    void CubeControlNew::onCubeTouchSum(double sumAngleZ) {
        Coordinate c1;
        c1.rotate({0, 0, sumAngleZ});
        imu_q2t = c1.getQuaternion();
    }

    void CubeControlNew::setSceneBg(int color) {
        float a = ((color & 0xff000000) >> 24) / 255.0f;
        int r = (color & 0xff0000) >> 16;
        int g = (color & 0xff00) >> 8;
        int b = (color & 0xff);
        setSceneBg(r, g, b, a);
    }

    void CubeControlNew::setSceneBg(int r, int g, int b, float a) {
        cubeScene.background_color = {(float) r / 255, (float) g / 255, (float) b / 255, a};
        draw();
    }

    void CubeControlNew::setAxisCube(int axisCubeIndex) {
        if (axisCubeIndex >= 0 && axisCubeIndex < 24) {
            Coordinate ct = Coordinate::eye();
            ct.setQuaternion(AnimationCube::axis_cube_to_q[axisCubeIndex]);
            sceneHandler.cubeAnimation().setFrame(ct);
        }
    }

    void CubeControlNew::setArrowOff(bool b) {
        sceneHandler.arrowAnimation().setOnOff(b);
    }

    void CubeControlNew::setArrowMove(int m) {
        sceneHandler.arrowAnimation().setSpeed(180);
        sceneHandler.arrowAnimation().setMove(Move(m));
    }

    void CubeControlNew::setCubeConnected() {
        cubeScene.cube.resetColor();
    }

    void CubeControlNew::setCubeUnConnected() {
        cubeScene.cube.setBodyMaterial(material_t::colored_material({1.0f,1.0f,1.0f,1.0f},0.08,0.4f,0.5f,0.0f,20.0f));
        fvec4 color = { 0.7f, 0.7f, 0.7f, 1.0f };
        for(int i=0;i<54;i++){
            cubeScene.cube.setPasterColorAbs(i,color);
        }
    }

    void CubeControlNew::adjustCubeColor() {
        for(int i=0;i<54;i++){
            cubeScene.cube.adjustPasterColorAbs(i,0.8f,0.6f);
        }
    }



}




