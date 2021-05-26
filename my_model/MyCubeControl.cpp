//
// Created by lacuz on 2018/11/4.
//

#include "MyCubeControl.h"
#include "Search.h"
#include "Search5.h"
#include <cmath>

namespace my {
    using namespace std;
    using namespace cs;


	//////////////////////////////////////////////////////////////////////////
    CubeControl::CubeControl()
            : sceneHandler(cubeScene) {

        cube_coord_to_imu = Coordinate::eye();
        cube_coord_to_imu.rotate(0, 0, 0);
    }

    CubeControl::~CubeControl() {

    }


#ifdef ANDROID

    void CubeControl::init(EGLNativeWindowType window, std::string resPath) {
        init(resPath);
    }

#endif


    void CubeControl::init(std::string resPath) {
        // 初始化场景
        cubieCube = cs::CubieCube().eye();
        cubeScene.is_draw_test_box = false;
        cubeScene.background_color = {1.0f, 1.0f, 1.0f, 1};
        cubeScene.set_default_filename(resPath);
        cubeScene.cube.set_defaut_filename(resPath);

#ifdef __APPLE__
        cubeScene.use_msaa = false;      // 抗锯齿, iOS 未配置好
#else
        cubeScene.use_msaa = true;      // 抗锯齿
#endif

        sceneHandler.init(&esContext);
        sceneHandler.setDefaultDelay(0.2f);
        sceneHandler.setCamYaw(-90.0f);


        // IMU
        imu_is_frist_data = true;

        cs::Search5::init();


    }


    void CubeControl::update(double deltaTime) {
        sceneHandler.update(deltaTime);
    }

    void CubeControl::draw() {
        cubeScene.draw(&esContext);

    }

    void CubeControl::onPause() {
        cubeScene.onPause();
    }

    void CubeControl::onResume() {
        cubeScene.onResume();
    }

    void CubeControl::onSurfaceChanged(int width, int height) {
        esContext.width = width;
        esContext.height = height;

        cubeScene.onSurfaceChanged();
    }

    void CubeControl::updateStatus(int cubeStr[54]) {
        std::array<int, 54> arr;
        for (int i = 0; i < 54; i++) {
            arr[i] = cubeStr[i];
        }
        cs::ColorCube colorCube(arr);
        cubieCube = cs::CubieCube(colorCube);
        if (cubieCube.verify() == 0) {
            sceneHandler.cubeAnimation().setFrame(CubeAng(cubieCube, axisAng));
        } else {
            //        LOGI("%s<===>isVerify:false",cstr.c_str());
        }
    }


    std::vector<int> CubeControl::updateQAF(int datas[16]) {
        //处理角度
        for (int i = 0; i < 6; i++) {
            //磁吸处理相差1、2个刻度规整处理
            int d = datas[i + 3];
            int a = d / 12;
            int b = d % 12;
            if (b < 3) {
                d = a * 12;
            } else if (b > 9) {
                d = (a + 1) * 12;
            }

            //由刻度变角度 -45~45
            b = d % 12;
            if (b < 6)
                axisAng[i] = b * 7.5f;
            else
                axisAng[i] = (b - 12) * 7.5f;
        }

        //处理公式
        int formula[7];
        for (int i = 0; i < 7; i++) {
            formula[i] = datas[i + 9];
        }

        int count = formula[0] >= lastStep ? formula[0] - lastStep : 255 - lastStep + formula[0]+1;
        if (lastStep == -1 || count == 0) {
            sceneHandler.cubeAnimation().setFrame(CubeAng(cubieCube, axisAng));
        } else if (count > 0 && count < 7) {
            for (int i = 0; i < count; i++) {
                int f = formula[7 - count + i];
                cubieCube = cubieCube.move(cs::Move(f));
            }
            sceneHandler.cubeAnimation().setFrame(CubeAng(cubieCube, axisAng));
        } else if (count >= 7) {
            //丢步
        }
        lastStep = formula[0];

        // 处理陀螺仪
        // 磁力计量程±4800uT，分辨率15uT/LSB；  陀螺仪量程±2000dps,分辨率16.384LSB/(º/s)；  加速度计量程±8g,分辨率4096LSB/g；


        //处理四元数
        double qx = datas[0] / 16384.0f;
        double qy = datas[1] / 16384.0f;
        double qz = datas[2] / 16384.0f;
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

        std::vector<int> arr(54);
        for (int i = 0; i < 54; i++) {
            arr[i] = cubieCube.toColorCube()[i];
        }
        return arr;
    }

    void CubeControl::updateGyro(int datas[3]) {

        // gyro 原始坐标右手坐标系
        double gyroX = datas[0] / 16.384f;
        double gyroY = datas[1] / 16.384f;
        double gyroZ = datas[2] / 16.384f;

        imu_gyro = vec3{gyroX, gyroY, gyroZ};
        //doGyro();
    }

    void CubeControl::updateACM(int *datas) {

        // 加速度计与陀螺仪坐标相同
        double accX = -datas[0] / 4096.0f;
        double accY = -datas[1] / 4096.0f;
        double accZ = -datas[2] / 4096.0f;

        imu_acc = vec3{accX, accY, accZ};
        // 磁力计坐标
        double magX = datas[8] * 0.15f;
        double magY = datas[7] * 0.15f;
        double magZ = -datas[9] * 0.15f;

        imu_mag = vec3{magX, magY, magZ};


        // 安卓代码移植 java 实现
        if (use_cube_coord) {
//             doAcc();
//             doMag();
        }


    }

    void CubeControl::doGyro() {
    }


    void CubeControl::doAcc() {
    }

    void CubeControl::doMag() {
        doMagCal();


        vec3 cal_mag = imu_mag - mag_offset;

        vec3 v_north{0, 1, 0};    // 北方向向量
        vec3 v_gravity{0, 0, 1};    // 重力向量

        double coef = 0.5;


        // 加速度校正垂直姿态

        Coordinate coord_qm = Coordinate::eye();
        coord_qm.setQuaternion(imu_qm);

        double abs_mag = magnitude(cal_mag);
        if (abs_mag >= 6.0f && abs_mag <= 100.0f)   // 明显有干扰忽略
        {
            vec3 mm = normalize(cal_mag);

            vec3 m_north = coord_qm.world2local(v_north);
            vec3 m_gravity = coord_qm.world2local(v_gravity);


            vec3 mh = mm - dot(mm, m_gravity) * m_gravity;    // mm 在水平方向的分量
            mh = normalize(mh);

            vec3 _cross = cross(normalize(mh), normalize(m_north));
            double abs_cross = magnitude(_cross);
            _cross = normalize(_cross);

            double rot_th = asin(abs_cross);
            double rot_th2 = coef * rot_th;

            double c = cos(0.5f * rot_th2);
            double s = sin(0.5f * rot_th2);

            quaternion qd{c, s * _cross[0], s * _cross[1], s * _cross[2]};

            coord_qm.setQuaternion(coord_qm.getQuaternion() * qd);

        }

        imu_qm = coord_qm.getQuaternion();
        imu_q2m = imu_qm * imu_q.inv();


        cube_coord = coord_qm.local2world(cube_coord_to_imu);
        sceneHandler.cubeAnimation().setFrame(cube_coord);
    }

    void CubeControl::doMagCal() {
        //         mag_offset = { 3.5, 33.5, 0 };
        //         return;


        if (mag_cal_buffer_xy.empty())    // 初始化
        {

        }


        vec3 mag_raw = imu_mag;

        quaternion q = imu_q;
        Coordinate coord = Coordinate::eye();
        coord.setQuaternion(q);

        vec3 z_c2w = coord.local2world({0, 0, 1});
        double cos_z2z = dot(z_c2w, vec3{0, 0, 1});    // 魔方z轴与世界z轴夹角的cos


        // 水平旋转检测, x,y 轴
        if (abs(cos_z2z) > 0.70710678)    // z轴夹角(不分正反)小于45 deg
        {
            double yaw = (180.0 / MY_PI) * atan2(2 * (q.w * q.z + q.x * q.y),
                                                 1 - 2 * (q.y * q.y + q.z * q.z));    // 航向角
            yaw = yaw < 0 ? yaw + 360 : yaw;    // [0, 360)

            int idx_yaw = int(yaw / 90.0f);        // 8 个方向

            list<vec3> &buf = mag_cal_buffer_xy[idx_yaw];

            if ((int) buf.size() >= mag_cal_max_samples)
                buf.pop_front();

            buf.push_back(mag_raw);
        } else    // z轴与水平面夹角小于45 deg, z 轴 在水平方向旋转
        {
            quaternion qz = q * quaternion(0, 0, 0, 1) * q.inv();    // 局部转世界

            double yaw = (180.0 / MY_PI) * atan2(qz.y, qz.x);    // z轴在水平面投影的航向角
            yaw = yaw < 0 ? yaw + 360 : yaw;    // [0, 360)

            int idx_yaw = int(yaw / 90.0f);        // 8 个方向

            list<vec3> &buf = mag_cal_buffer_z[idx_yaw];

            if ((int) buf.size() >= mag_cal_max_samples)
                buf.pop_front();

            buf.push_back(mag_raw);
        }


        // 检查是否达到校准条件
        bool is_cal = true;

        for (int i = 0; i < (int) mag_cal_buffer_xy.size(); ++i) {
            if ((int) mag_cal_buffer_xy[i].size() < mag_cal_min_samples) {
                is_cal = false;
                break;
            }

            if ((int) mag_cal_buffer_z[i].size() < mag_cal_min_samples) {
                is_cal = false;
                break;
            }
        }

        // 校准
        if (is_cal) {
            vector<vec3> raw;
            for (int i = 0; i < (int) mag_cal_buffer_xy.size(); ++i) {
                for (vec3 a : mag_cal_buffer_xy[i])
                    raw.push_back(a);

                for (vec3 a : mag_cal_buffer_z[i])
                    raw.push_back(a);
            }


            _ball_fit_ret_t bf = _ball_fit(raw, 0.5f, 0.01f, 5000);

            if (bf.converge && bf.r >= 6.0f && bf.r <= 100.f) {

                double coef = 0.2;

                if (mag_cal_cnt == 0) {
                    mag_offset = bf.p;
                    mag_cal_cnt = 1;
                } else {
                    mag_offset = coef * bf.p + (1.0f - coef) * mag_offset;

                    ++mag_cal_cnt;

                    if (mag_cal_cnt == 0)   // 避免溢出再次是0
                        mag_cal_cnt = 1;
                }

                for (int i = 0; i < (int) mag_cal_buffer_xy.size(); ++i) {
                    mag_cal_buffer_xy[i].clear();
                    mag_cal_buffer_z[i].clear();
                }

            }
        }


    }

    CubeControl::_ball_fit_ret_t
    CubeControl::_ball_fit(const std::vector<vec3> raw, double coef, double tol, int maxIter) {
        _ball_fit_ret_t ret;

        // 设置初始值
        vec3 p = {0, 0, 0};
        //         for (vec3 a : raw)
        //             p = p + a;
        //
        //         p = (1.0f / raw.size()) * p;
        //


        double r = 1.0;
        bool converge = false;

        int iter = 0;
        for (; iter < maxIter; ++iter) {
            // 求偏导数
            vec3 dp = {0, 0, 0};
            double dr = 0.0f;

            for (int i = 0; i < (int) raw.size(); ++i) {
                double ri = magnitude(raw[i] - p);
                vec3 dp_i = 2.0f * (ri - r) / ri * (p - raw[i]);
                double dr_i = -2.0f * (ri - r);

                dp = dp + dp_i;
                dr += dr_i;
            }

            dp = dp * (1.0f / raw.size());
            dr = dr / raw.size();

            p = p - coef * dp;
            r = r - coef * dr;


            if (magnitude(dp) < tol && abs(dr) < tol) {
                converge = true;
                ++iter;
                break;
            }
        }


        ret.p = p;
        ret.r = r;
        ret.iter = iter;
        ret.converge = converge;

        return ret;
    }


    void CubeControl::keyClick(int type, int key) {
        if (key == -1) {
            cubieCube = cs::CubieCube::eye();
            cubeScene.cube.setCubeAng(CubeAng(cs::CubieCube::eye()));
            sceneHandler.setCoordAniOriginal();
        } else {
            if (type == 1)
                sceneHandler.moveRelative(Move(key));
            else
                sceneHandler.moveAbsolute(Move(key));

        }

        if (type == 0) {
            cubieCube = cubieCube.move(key);
        }

    }

    vector<Move> CubeControl::getSolve() {
        cs::Search search;
        vector<Move> m = search.solve(cubeScene.cube.getCubieCube());
        //        int *result;
        //        for (int i = 0; i < m.size(); ++i) {
        //           result[i] = (int) m.data()[i];
        //        }
        return m;
    }

    vector<int> CubeControl::getSolveI() {
        vector<Move> m = getSolve();
        vector<int> mm(m.size());

        for (int i = 0; i < (int) m.size(); ++i) {
            mm[i] = m[i];
        }

        return mm;
    }

    vector<Move> CubeControl::getSolve(int axis_r, cs::CubieCube destCubieCube) {
        // 查找相对的 U
        int axis_u = -1;
        double max_u = -1000;
        for (int i = 0; i < 6; ++i) {
            double u = dot(cube_coord.local2world(CubeModel::rotateAxisTbl[i]),
                           Coordinate::eye().local2world({0, 0, 1}));
            if (u > max_u) {
                max_u = u;
                axis_u = i;
            }
        }

        cs::Search5 search5;
        vector<Move> m = search5.solve_no_axis(cubieCube, destCubieCube, axis_u);
        int code = search5.getError();

        // 查找 AxisCube
        int axisCube_idx = -1;
        for (int i = 0; i < AxisCube::num_valid_table; ++i) {
            const AxisCube &ac = AxisCube::valid_table[i];
            if (ac[0] == axis_u && ac[1] == axis_r) {
                axisCube_idx = i;
                break;
            }
        }

        int inv_idx = AxisCube::inv_table[axisCube_idx];
        const AxisCube &cube2robot = AxisCube::valid_table[inv_idx];

        for (auto &val: m) {
            int axis = val / 3;
            int power = val % 3;
            int robot_axis = cube2robot[axis];
            val = (Move) (robot_axis * 3 + power);
        }
        return m;
    }


    vector<int>
    CubeControl::getRobotSolve(int bCubeStatus[54], int play_fomula[], int formula_len) {

        vector<int> m;
        std::array<int, 54> bArr;
        for (int i = 0; i < 54; i++) {
            bArr[i] = bCubeStatus[i];
        }
        cs::ColorCube bCC(bArr);
        cs::CubieCube bCube = cs::CubieCube(bCC);

        //确定魔方对应机器人r轴
        int axis_r = -1;
        if (bCube.move(Move::Ux1) == cubieCube) {
            axis_r = 0;
        } else if (bCube.move(Move::Rx1) == cubieCube) {
            axis_r = 1;
        } else if (bCube.move(Move::Fx1) == cubieCube) {
            axis_r = 2;
        } else if (bCube.move(Move::Dx1) == cubieCube) {
            axis_r = 3;
        } else if (bCube.move(Move::Lx1) == cubieCube) {
            axis_r = 4;
        } else if (bCube.move(Move::Bx1) == cubieCube) {
            axis_r = 5;
        } else {
            //出错，魔方未发送旋转，未放入机器人，或其他原因
            return m;
        }

        cs::CubieCube dcc = cs::CubieCube::eye();
        for (int i = 0; i < formula_len; i++) {
            dcc = dcc.move(play_fomula[i]);
        }
        m = getSolveI(axis_r, dcc);
        return m;

    }

    vector<int>
    CubeControl::getRobotSolve(int bCubeStatus[54], int toCubeStatus[54]) {

        vector<int> m;
        std::array<int, 54> bArr;
        for (int i = 0; i < 54; i++) {
            bArr[i] = bCubeStatus[i];
        }
        cs::ColorCube bCC(bArr);
        cs::CubieCube bCube = cs::CubieCube(bCC);

        //确定魔方对应机器人r轴
        int axis_r = -1;
        if (bCube.move(Move::Ux1) == cubieCube) {
            axis_r = 0;
        } else if (bCube.move(Move::Rx1) == cubieCube) {
            axis_r = 1;
        } else if (bCube.move(Move::Fx1) == cubieCube) {
            axis_r = 2;
        } else if (bCube.move(Move::Dx1) == cubieCube) {
            axis_r = 3;
        } else if (bCube.move(Move::Lx1) == cubieCube) {
            axis_r = 4;
        } else if (bCube.move(Move::Bx1) == cubieCube) {
            axis_r = 5;
        } else {
            //出错，魔方未发送旋转，未放入机器人，或其他原因
            return m;
        }

        for (int i = 0; i < 54; i++) {
            bArr[i] = toCubeStatus[i];
        }
        cs::ColorCube dCC(bArr);
        cs::CubieCube dCube = cs::CubieCube(dCC);

        m = getSolveI(axis_r, dCube);
        return m;

    }

    vector<int> CubeControl::getSolveI(int axis_r, cs::CubieCube destCubieCube) {
        vector<Move> m = getSolve(axis_r, destCubieCube);
        vector<int> mm(m.size());

        for (int i = 0; i < (int) m.size(); ++i) {
            mm[i] = m[i];
        }

        return mm;
    }

    quaternion CubeControl::getCubeQuaternion() {
        return cube_coord.getQuaternion();
    }

	void CubeControl::onTouch(double deltaAngleZ)
	{
		Coordinate c1;
		c1.setQuaternion(imu_q2t);
		c1.rotate({ 0,0,deltaAngleZ });
		imu_q2t = c1.getQuaternion();
	}

}




