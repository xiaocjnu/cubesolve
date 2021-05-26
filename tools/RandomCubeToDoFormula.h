// 用来生成 CFOP 和 层先法公式练习所用的状态

#include "cs.h"


// 随机生成一个魔方，用来练习公式，还差一个公式就完成 F2L
// axis 0 - 23, 指定生成魔方的姿态，未完成的对当天axis_cube 下，右前方的对。
// idx 用来指定公式，范围是 0 - 40, 用来指定公式
// 返回 AxisCubieCube, 参数非法则返回 eye
cs::AxisCubieCube randomCubeToDoFormulaF2L(int axis, int idx);

// 见 randomCubeToDoFormulaF2L(int axis, int idx), axis 随机
cs::AxisCubieCube randomCubeToDoFormulaF2L(int axis);



// 随机生成一个魔方，用来练习公式，还差一个公式就完成 F2L，未完成的对颜色随机（指定底），顶层需要先转U调整。
// bottom 0 - 5 (URFDLB), 指定生成魔方的底面颜色
// idx 用来指定公式，范围是 0 - 40, 用来指定公式
// 返回 AxisCubieCube, 参数非法则返回 eye
cs::AxisCubieCube randomCubeToDoFormulaF2L_v2(int bottom, int idx);


// 见 randomCubeToDoFormulaF2L_v2(int axis, int idx), axis 随机
cs::AxisCubieCube randomCubeToDoFormulaF2L_v2(int bottom);



// 随机生成一个魔方，用来练习公式，还差一个公式就完成 OLL
// axis 0 - 23, 指定生成魔方的姿态
// idx 用来指定公式，范围是 0 - 56, 用来指定公式
// 返回 AxisCubieCube, 参数非法则返回 eye
cs::AxisCubieCube randomCubeToDoFormulaOLL(int axis, int idx);

// 见 randomCubeToDoFormulaOLL(int axis, int idx), axis 随机
cs::AxisCubieCube randomCubeToDoFormulaOLL(int axis);


// 随机生成一个魔方，用来练习公式，还差一个公式就完成 OLL，顶层需要先转U调整。
// bottom 0 - 5 (URFDLB), 指定生成魔方的底面颜色
// idx 用来指定公式，范围是 0 - 56, 用来指定公式
// 返回 AxisCubieCube, 参数非法则返回 eye
cs::AxisCubieCube randomCubeToDoFormulaOLL_v2(int bottom, int idx);


// 见 randomCubeToDoFormulaOLL_v2(int axis, int idx), axis 随机
cs::AxisCubieCube randomCubeToDoFormulaOLL_v2(int bottom);


// 随机生成一个魔方，用来练习公式，还差一个公式就完成 PLL
// axis 0 - 23, 指定生成魔方的姿态
// idx 用来指定公式，范围是 0 - 20, 用来指定公式
// 返回 AxisCubieCube, 参数非法则返回 eye
cs::AxisCubieCube randomCubeToDoFormulaPLL(int axis, int idx);

// 见 randomCubeToDoFormulaPLL(int axis, int idx), axis 随机
cs::AxisCubieCube randomCubeToDoFormulaPLL(int axis);


// 随机生成一个魔方，用来练习公式，还差一个公式就完成 PLL，顶层需要先转U调整。
// axis 0 - 23, 指定生成魔方的姿态
// idx 用来指定公式，范围是 0 - 20, 用来指定公式
// 返回 AxisCubieCube, 参数非法则返回 eye
cs::AxisCubieCube randomCubeToDoFormulaPLL_v2(int bottom, int idx);

// 见 randomCubeToDoFormulaPLL_v2(int axis, int idx), axis 随机
cs::AxisCubieCube randomCubeToDoFormulaPLL_v2(int bottom);