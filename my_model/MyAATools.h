/**
 * 静态工具类
 * 用于魔方相关数据，与场景无关的计算
 * Created by lacuz on 2018/12/6.
**/

#pragma once

#include "my.h"

//静态初始化
void init();

//colorCube 转 cubiecube
cs::CubieCube ints2CubieCube(int cube[54]);

std::vector<int> CubieCube2ints(cs::CubieCube cubieCube);

//获取一个状态到另一个状态的公式
std::vector<int> getMovesToInts(int srcStatus[54], int destStatus[54]);

//从初始状态经过公式获取新的状态
std::vector<int> getStatusToInts(int moves[],int moves_len);
std::vector<int> getStatusToInts(int srcStatus[54],int moves[],int moves_len);

//从startStatus状态经过相对公式获取新的状态
std::vector<int> getStatusByRelaMoves(int startStatus[54],std::vector<int> moves);

//获取四元数对应axiscubeIndex
int getRelativeAxisCubeIdx(my::quaternion qua);

//查找相对公式，axisCubeIdx当前魔方状态下标，参考AxisCube类，absMove为绝对公式
int getRelativeMove(int curAxisCubeIdx,int absMove);

//查找绝对公式，beforeAxisCubeIdx转动前魔方状态下标，参考AxisCube类，relaMove为相对公式
int getAbsMove(int beforeAxisCubeIdx,int relaMove);

//查找整体转动公式
std::vector<int> getAxisCubeMove(int formAxisCubeIndx,int toAxisCubeIndx);

//整体转动公式转换，转成至多两步
std::vector<int> axisCubeChange(int moves[],int len);

//魔方状态是否正确
bool isValidateCube(int cStatus[54]);

//从初状态经过公式是否还原
bool isDNF(int startStatus[54],int moves[],int moves_len);

//获取机器人魔方打乱，没有U
std::vector<int> getRobotSolve(int cube_axis_index, int currentCube[54], int destCube[54]);

//确定魔方对应机器人r轴
int getCubeInRobotAxisIndex(int beforeCube[54], int currentCube[54]);

cs::AxisCube getAxisCubeByIndex(int index);

/**
 * 判断魔方是否执行了某条公式
 * @param topColor 顶面颜色0-白 1-红 2-绿 3-黄 4-橙 5-蓝 -1-所有面
 * @param bleHistory 魔方蓝牙所传历史记录
 * @param len1 bleHistory长度
 * @param moves 所执行公式，仅支持[Ux1,Bx3] [Mx1,Mx3] [Mx2n]
 * @param len2 moves长度
 * @return
 */
bool bleMovesComprares(int topColor, int bleHistory[], int len1,int moves[],int len2);

