#pragma once


#include "util.h"
#include "AxisCube.h"
#include "FaceCubeBase.h"
#include "CubieCube.h"
#include "AxisCubieCube.h"
#include "CoordCube.h"
#include "CoordCube5.h"
#include "Search.h"
#include "Search5.h"
#include "HumanSolver.h"




namespace cs
{
	// 初始化所有静态变量
	void init();


	// 初始化出 search, search5 需要的所有静态变量， 不需要求解时加快初始化速度
	void init_without_search();
}
