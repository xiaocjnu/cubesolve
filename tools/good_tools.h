// 一些好用的工具函数


#pragma once

#include "cs.h"
#include <vector>
#include <set>
#include <string>
#include "tools_typedef.h"
#include "CubeFrames.h"


//////////////////////////////////////////////////////////////////////////
// 类型转换(复制)



std::vector<uchar> CharVectorToUCharVector(const std::vector<char>& a);
std::vector<char> UCharVectorToCharVector(const std::vector<uchar>& a);


//////////////////////////////////////////////////////////////////////////

// 随机打乱的 CubieCube
cs::CubieCube randomCube();


// 求解复原公式
std::vector<cs::Move> solveCube(const cs::CubieCube& cc);
std::vector<int> solveCubeIV(const cs::CubieCube& cc);

// 求解从状态 src 到 dst 的公式
std::vector<cs::Move> solveCube(const cs::CubieCube& src, const cs::CubieCube& dst);
std::vector<int> solveCubeIV(const cs::CubieCube& src, const cs::CubieCube& dst);


// 求解打乱公式
std::vector<cs::Move> solveCubeSramble(const cs::CubieCube& cc);
std::vector<int> solveCubeSrambleIV(const cs::CubieCube& cc);

// 求解复原公式, 不能转 U
std::vector<cs::Move> solveCubeNoU(const cs::CubieCube& cc);
std::vector<int> solveCubeNoU_IV(const cs::CubieCube& cc);

// 求解从状态 src 到 dst 的公式, 不能转 U
std::vector<cs::Move> solveCubeNoU(const cs::CubieCube& src, const cs::CubieCube& dst);
std::vector<int> solveCubeNoU_IV(const cs::CubieCube& src, const cs::CubieCube& dst);


// 求解复原公式, 返回相对公式，不能转 U
std::vector<cs::Move> solveCubeNoU(const cs::AxisCubieCube& cc);
std::vector<int> solveCubeNoU_IV(const cs::AxisCubieCube& cc);

// 求解从状态 src 到 dst 的公式, 返回相对公式，不能转 U
std::vector<cs::Move> solveCubeNoU(const cs::AxisCubieCube& src, const cs::AxisCubieCube& dst);
std::vector<int> solveCubeNoU_IV(const cs::AxisCubieCube& src, const cs::AxisCubieCube& dst);


// 随机打乱公式
std::vector<cs::Move> randomSrambleFormula();
std::vector<int> randomSrambleFormulaIV();


// 首先按比例缩放double 类型数组aa得到a，然后对 a 进行取整，得到结果 b, 严格满足 sum(b) = total, 误差尽量小
std::vector<int> round_vector_eq_sum(const std::vector<double>& aa, int total);


// 将公式变成字符串, 出错则返回空字符串
std::string formulaToStr(const std::vector<cs::Move>& moves, std::string seperator = " ");
std::string formulaToStr(const std::vector<int>& moves, std::string seperator = " ");


// 以程序员格式将公式变成字符串, 出错则返回空字符串
std::string formulaToCodeStr(const std::vector<cs::Move>& moves, std::string seperator = ", ");
std::string formulaToCodeStr(const std::vector<int>& moves, std::string seperator = ", ");


// 将相对公式转为虚拟魔方的公式，只允许 U U' R R' F F' D D' L L' B B' M M' x x' y y' z z'
std::vector<cs::Move> toVirtualMoves(const std::vector<cs::Move>& moves);
std::vector<int> toVirtualMoves(const std::vector<int>& moves);


// 获取虚拟魔方合法的 move
std::set<cs::Move> getValidVirtualMoves();

//////////////////////////////////////////////////////////////////////////
// 一些数据校验的有用函数
//////////////////////////////////////////////////////////////////////////


// 修正旧的虚拟魔方记录数据
CubeFrames correct_old_type_data_of_virtual_cube(const CubeFrames& frames);

// 修正旧的智能魔方记录数据
CubeFrames correct_old_type_data_of_smart_cube(const CubeFrames& frames);


// 打乱公式 moves 转字符串，格式如：F1L2R2L2B3L3U2B2F3B2U3B2U1R3F3B2U2B1L2D1U1B3F2L2R1
// 仅支持绝对公式，出错返回空字符串
std::string scramble_formula_moves_to_str(std::vector<cs::Move>moves);


// 打乱公式字符串转moves, 出错返回空
std::vector<cs::Move> scramble_formula_str_to_moves(std::string s);


// 检查打乱公式和记录是否可以复原

bool check_scramble_and_frames(const cs::CubieCube& cube, const CubeFrames& frames);
bool check_scramble_and_frames(const cs::CubieCube& cube, const std::vector<char>& frames);

bool check_scramble_and_frames(const std::vector<cs::Move>& scramble, const CubeFrames& frames);
bool check_scramble_and_frames(const std::vector<cs::Move>& scramble, const std::vector<char>& frames);

bool check_scramble_and_frames(const std::string& scramble, const CubeFrames& frames);
bool check_scramble_and_frames(const std::string& scramble, const std::vector<char>& frames);



// 修正 frames 的时间, 使其时间和等于总时间，单位是毫秒。
// frames 的时间会乘以一个比例，仅当比例属于 [0.9, 1.1] 是才能成功修正，否则忽略
// 失败返回空
CubeFrames correct_frames_ds_time(int total, const CubeFrames& frames);

// 用于旧数据修改
// 先调用 correct_frames_ds_time，如果失败，则进一步处理：
//     当比例大于1.1时且 第一步是0，将时间差不到第一步；
//     当比例小于0.9且第一步时间大于 1000ms，设为0
CubeFrames correct_frames_ds_time_old_data(int total, const CubeFrames& frames);


// 修正 frames 的时间总和是否等于总时间，单位是毫秒。
// 失败或者frames为空返回 false
bool check_frames_ds_time(int total, const CubeFrames& frames);
