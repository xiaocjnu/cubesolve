//
// Created by lacuz on 2018/12/6.
//


#include "cs.h"
#include "Search.h"
#include "Search5.h"
#include "MyAATools.h"
#include "myTransform.h"
#include "myCubeModel.h"
#include "AxisCube.h"
#include<cmath>

using namespace std;
using namespace cs;
using namespace my;

const Move robot_table[24][2]
        {                    //0-U 1-R 2-F 3-D 4-L  5-B
                Rx1, Fx1,    // {0,1,2,3,4,5} // 0  eye
                Fx1, Lx1,    // {0,2,4,3,5,1} // 1  y'
                Lx1, Bx1,    // {0,4,5,3,1,2} // 2  y2
                Bx1, Rx1,    // {0,5,1,3,2,4} // 3  y
                Ux1, Bx1,    // {1,0,5,4,3,2} // 4
                Fx1, Ux1,    // {1,2,0,4,5,3} // 5
                Dx1, Fx1,    // {1,3,2,4,0,5} // 6  z'
                Bx1, Dx1,    // {1,5,3,4,2,0} // 7
                Ux1, Rx1,    // {2,0,1,5,3,4} // 8
                Rx1, Dx1,    // {2,1,3,5,4,0} // 9  x
                Dx1, Lx1,    // {2,3,4,5,0,1} // 10
                Lx1, Ux1,    // {2,4,0,5,1,3} // 11
                Rx1, Bx1,    // {3,1,5,0,4,2} // 12  x2
                Fx1, Rx1,    // {3,2,1,0,5,4} // 13
                Lx1, Fx1,    // {3,4,2,0,1,5} // 14  z2
                Bx1, Lx1,    // {3,5,4,0,2,1} // 15
                Ux1, Fx1,    // {4,0,2,1,3,5} // 16  z
                Fx1, Dx1,    // {4,2,3,1,5,0} // 17
                Dx1, Bx1,    // {4,3,5,1,0,2} // 18
                Bx1, Ux1,    // {4,5,0,1,2,3} // 19
                Ux1, Lx1,    // {5,0,4,2,3,1} // 20
                Rx1, Ux1,    // {5,1,0,2,4,3} // 21  x'
                Dx1, Rx1,    // {5,3,1,2,0,4} // 22
                Lx1, Dx1,    // {5,4,3,2,1,0} // 23
        };


void init() {
    cs::init();
}

cs::CubieCube ints2CubieCube(int cube[54]) {
    std::array<int, 54> arr;
    for (int i = 0; i < 54; i++) {
        arr[i] = cube[i];
    }
    cs::ColorCube colorCube(arr);
    return cs::CubieCube(colorCube);
}

std::vector<int> CubieCube2ints(CubieCube cubieCube) {
    std::vector<int> arr(54);
    for (int i = 0; i < 54; i++) {
        arr[i] = cubieCube.toColorCube()[i];
    }
    return arr;
}

std::vector<int> getMovesToInts(int srcStatus[54], int destStatus[54]) {

    cs::CubieCube sCube = ints2CubieCube(srcStatus);
    cs::CubieCube dCube = ints2CubieCube(destStatus);

    cs::Search search;
    vector<Move> m = search.solve(sCube, dCube);

    vector<int> rs(m.size());
    for (int i = 0; i < (int) m.size(); i++) {
        rs[i] = m[i];
    }
    return rs;
}


std::vector<int> getStatusToInts(int moves[], int moves_len) {
    cs::CubieCube cc = cs::CubieCube::eye();
    for (int i = 0; i < moves_len; i++) {
        cc = cc.move(moves[i]);
    }
    vector<int> rs(54);
    std::array<int, 54> arr = cc.toColorCube();
    for (int i = 0; i < 54; i++) {
        rs[i] = arr[i];
    }
    return rs;
}

std::vector<int> getStatusToInts(int srcStatus[54], int moves[], int moves_len) {
    cs::CubieCube cc = ints2CubieCube(srcStatus);
    for (int i = 0; i < moves_len; i++) {
        cc = cc.move(moves[i]);
    }
    vector<int> rs(54);
    std::array<int, 54> arr = cc.toColorCube();
    for (int i = 0; i < 54; i++) {
        rs[i] = arr[i];
    }
    return rs;
}

std::vector<int> getStatusByRelaMoves(int srcStatus[54], std::vector<int> moves) {
    CubieCube cc = ints2CubieCube(srcStatus);
    AxisCubieCube acc = AxisCubieCube(AxisCube::eye(), cc);
    for (int i = 0; i < (int) moves.size(); i++) {
        acc = acc.move(moves[i]);
    }
    cc = acc.cubie;
    return CubieCube2ints(cc);
}

int getRelativeAxisCubeIdx(quaternion qua) {
    Coordinate myCubeCoo;
    myCubeCoo.setQuaternion(qua);
//    vec3 ov =quaternion

    double myCamPitch = 30.0f;        // 原点O到相机原点的俯仰角(与 xy 平面的夹角)，范围 [-89,89]
    double myCamYaw = -60.0f;        // 原点O到相机原点的偏向角(与 x 轴的夹角)
    Coordinate c;
    c.rotate(0, 0, 90);
    c.rotate(0, -myCamPitch, myCamYaw);
    // 查找相对的 U
    double max_u = -1000;
    int axis_u = -1;

    for (int i = 0; i < 6; ++i) {
        double u = dot(myCubeCoo.local2world(CubeModel::rotateAxisTbl[i]),
                       c.local2world({0, 0, 1}));
        if (u > max_u) {
            max_u = u;
            axis_u = i;
        }
    }

    // 查找相对的 R
    double max_r = -1000;
    int axis_r = -1;
    for (int i = 0; i < 6; ++i) {
        if (i == axis_u)
            continue;

        double r = dot(myCubeCoo.local2world(CubeModel::rotateAxisTbl[i]),
                       c.local2world({1, 0, 0}));
        if (r > max_r) {
            max_r = r;
            axis_r = i;
        }
    }

    // 查找 AxisCube
    int axisCube_idx = -1;
    for (int i = 0; i < AxisCube::num_valid_table; ++i) {
        const AxisCube &ac = AxisCube::valid_table[i];
        if (ac[0] == axis_u && ac[1] == axis_r) {
            axisCube_idx = i;
            break;
        }
    }

    return axisCube_idx;
}


int getRelativeMove(int curAxisCubeIdx, int absMove) {
    if (absMove < 0 || curAxisCubeIdx < 0 || curAxisCubeIdx > 23)
        return -1;
    int absFaceIdx = absMove / 3;
    int move_remainder = absMove % 3;
    const AxisCube &relaAxisCube = AxisCube::valid_table[curAxisCubeIdx];

    int relaFaceIdx = -1;
    for (int i = 0; i < 6; i++) {
        if (absFaceIdx == relaAxisCube[i]) {
            relaFaceIdx = i;
            break;
        }
    }
    return relaFaceIdx * 3 + move_remainder;
}


int getAbsMove(int curAxisCubeIdx, int relaMove) {
    if (relaMove < 0 || curAxisCubeIdx < 0 || curAxisCubeIdx > 23)
        return -1;
    int relaFaceIdx2 = relaMove / 3;
    int move_remainder = relaMove % 3;
    const AxisCube &relaAxisCube = AxisCube::valid_table[AxisCube::inv_table[curAxisCubeIdx]];

    int absFaceIdx = -1;
    for (int i = 0; i < 6; i++) {
        if (relaFaceIdx2 == relaAxisCube[i]) {
            absFaceIdx = i;
            break;
        }
    }
    return absFaceIdx * 3 + move_remainder;
}


std::vector<int> getAxisCubeMove(int formAxisCubeIndx, int toAxisCubeIndx) {

    vector<int> move(2);
    move[0] = InvalidMove;
    move[1] = InvalidMove;

    if (formAxisCubeIndx >= 0 && formAxisCubeIndx <= 23 && toAxisCubeIndx >= 0 &&
        toAxisCubeIndx <= 23) {
        AxisCube a = AxisCube::valid_table[formAxisCubeIndx];
        AxisCube b = AxisCube::valid_table[toAxisCubeIndx];
        for (int i = 0; i < AxisCube::num_valid_table; ++i) {
            Move m1 = AxisCube::gen_table[i][0];
            AxisCube c = a.move(m1);
            if (b[0] == c[0] && b[1] == c[1]) {
                move[0] = m1;
                break;
            }
            Move m2 = AxisCube::gen_table[i][1];
            AxisCube d = a.move(m1).move(m2);
            if (b[0] == d[0] && b[1] == d[1]) {
                move[0] = m1;
                move[1] = m2;
                break;
            }
        }
    }
    return move;
}


std::vector<int> axisCubeChange(int moves[], int len) {
    AxisCube axisCube = AxisCube::valid_table[0];
    for (int i = 0; i < len; i++) {
        axisCube = axisCube.move(moves[i]);
    }
    int axisCube_idx = -1;
    for (int i = 0; i < AxisCube::num_valid_table; ++i) {
        const AxisCube &ac = AxisCube::valid_table[i];
        if (ac[0] == axisCube[0] && ac[1] == axisCube[1]) {
            axisCube_idx = i;
            break;
        }
    }
    vector<int> vct(2);
    if (axisCube_idx >= 0) {
        vct[0] = AxisCube::gen_table[axisCube_idx][0];
        vct[1] = AxisCube::gen_table[axisCube_idx][1];
    }
    return vct;
}

bool isValidateCube(int cStatus[54]) {
    cs::CubieCube sCube = ints2CubieCube(cStatus);
    return sCube.verify() == 0;
}


//从初状态经过公式是否还原
bool isDNF(int startStatus[54], int moves[], int moves_len) {
    std::array<int, 54> arr;
    for (int i = 0; i < 54; i++) {
        arr[i] = startStatus[i];
    }
    cs::ColorCube colorCube(arr);
    cs::CubieCube cube = cs::CubieCube(colorCube);
    for (int i = 0; i < moves_len; i++) {
        cube = cube.move(moves[i]);
    }
    return !cube.iseye();
}


std::vector<int> getRobotSolve(int cube_axis_index, int currentCube[54], int destCube[54]) {
    CubieCube currentCC = ints2CubieCube(currentCube);
    CubieCube destCC = ints2CubieCube(destCube);

    const AxisCube &ac = AxisCube::valid_table[cube_axis_index];

    cs::Search5 search5;
    vector<Move> m = search5.solve_no_axis(currentCC, destCC, ac[0]);
    int code = search5.getError();

    vector<int> mm(m.size());
    if (code == 0) {
        int inv_idx = AxisCube::inv_table[cube_axis_index];
        const AxisCube &cube2robot = AxisCube::valid_table[inv_idx];
        for (auto &val: m) {
            int axis = val / 3;
            int power = val % 3;
            int robot_axis = cube2robot[axis];
            val = (Move) (robot_axis * 3 + power);
        }
        for (int i = 0; i < (int) m.size(); ++i) {
            mm[i] = m[i];
        }
    }
    return mm;
}

int getCubeInRobotAxisIndex(int beforeCube[54], int currentCube[54]) {
    cs::CubieCube bCube = ints2CubieCube(beforeCube);
    cs::CubieCube cCube = ints2CubieCube(currentCube);
    int axis_r = -1;
    for (int i = 0; i < 24; ++i) {
        if (bCube.move(robot_table[i][0]).move(robot_table[i][1]) == cCube) {
            axis_r = i;
            break;
        }
    }
    return axis_r;
}

AxisCube getAxisCubeByIndex(int index) {
    return AxisCube::valid_table[index];
};


bool bleMovesComprares(int topColor, int bleHistory[], int len1, int moves[], int len2) {

    // 获取moves公式的真实长度
    int len2R = 0;
    for (int i = 0; i < len2; i++) {
        int m = moves[i];
        if (m >= Ux1 && m <= Bx3) {
            len2R++;
            if (m % 3 == 1) len2R++;
        } else if (Ex1 <= m && m <= Sx3) {
            len2R += 2;
            if (m % 3 == 1) len2R += 2;
        } else if (Ex2n <= m && m <= Sx2n) {
            len2R += 4;
        } else {
            //其他公式不支持
            return false;
        }
    }
    if (len1 <= 0 || len2 <= 0 || len1 < len2R)
        return false;

    for (int axisIndex = 0; axisIndex < AxisCube::num_valid_table; axisIndex++) {
        const AxisCube &ac = AxisCube::valid_table[axisIndex];
        if (topColor == ac[0] || topColor == -1) {
            AxisCubieCube axisCubieCube = AxisCubieCube::eye();
            axisCubieCube.axis = AxisCube::fromValidIdx(axisIndex);
            bool flag = true;
            for (int i = 0, j = len1 - len2R; i < len2; i++) {
                int idx = axisCubieCube.axis.getValidIdx();
                int hm1 = AxisCubieCube::formulaA2R(idx, Move(bleHistory[j++]));
                if (Ux1 <= moves[i] && moves[i] <= Bx3 && moves[i] % 3 == 1) {
                    int hm2 = AxisCubieCube::formulaA2R(idx, Move(bleHistory[j++]));
                    if (hm1 != hm2 || !(Ux1 <= hm1 && hm1 <= Bx3 && hm1 % 3 != 1) ||
                        hm1 / 3 != moves[i] / 3) {
                        flag = false;
                        break;
                    }
                } else if (Ux1 <= moves[i] && moves[i] <= Bx3) {
                    if (hm1 != moves[i]) {
                        flag = false;
                        break;
                    }
                } else if (moves[i] == Mx1) {
                    int hm2 = AxisCubieCube::formulaA2R(idx, Move(bleHistory[j++]));
                    if ((hm1 != Lx3 && hm1 != Rx1) || (hm1 == Lx3 && hm2 != Rx1) ||
                        (hm1 == Rx1 && hm2 != Lx3)) {
                        flag = false;
                        break;
                    }
                } else if (moves[i] == Mx3) {
                    int hm2 = AxisCubieCube::formulaA2R(idx, Move(bleHistory[j++]));
                    if ((hm1 != Lx1 && hm1 != Rx3) || (hm1 == Lx1 && hm2 != Rx3) ||
                        (hm1 == Rx3 && hm2 != Lx1)) {
                        flag = false;
                        break;
                    }
                } else if (moves[i] == Mx2 || moves[i] == Mx2n) {
                    int hm2 = AxisCubieCube::formulaA2R(idx, Move(bleHistory[j++]));
                    int hm3 = AxisCubieCube::formulaA2R(idx, Move(bleHistory[j++]));
                    int hm4 = AxisCubieCube::formulaA2R(idx, Move(bleHistory[j++]));
                    if (abs(hm2 / 3 - hm1 / 3) != 3 || (hm2 % 3 + hm1 % 3 != 2) ||
                        abs(hm3 / 3 - hm4 / 3) != 3 || (hm3 % 3 + hm4 % 3 != 2)) {
                        flag = false;
                        break;
                    } else if (hm1 != Lx3 && hm1 != Rx1 && hm1 != Lx1 && hm1 != Rx3) {
                        flag = false;
                        break;
                    } else if ((hm1 == Lx1 || hm1 == Rx3) && hm3 != Lx1 && hm3 != Rx3) {
                        flag = false;
                        break;
                    } else if ((hm1 == Lx3 || hm1 == Rx1) && hm3 != Lx3 && hm3 != Rx1) {
                        flag = false;
                        break;
                    }
                }
                axisCubieCube = axisCubieCube.move(moves[i]);
            }
            if (flag) return true;
        }
    }
    return false;
}

