/*
1.debug模式编译，魔方状态如下调用solveCubeNoU时，会异常退出，异常码0x05
RRRUUUUUURRDRRDFFFFFFFFFULLLDDLDDLDDULLULLBBBBBBBBBRRD
*/

#include "cs.h"
#include "tools.h"
#include <stdio.h>
#include <ctime>
#include <iostream>
#include <Windows.h>
#include <DbgHelp.h>

using namespace cs;
using namespace std;


// 创建Dump文件
void CreateDumpFile(LPCSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// Dump信息
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;
	// 写入Dump文件内容
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	CloseHandle(hDumpFile);
}
 
// 处理Unhandled Exception的回调函数
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	CreateDumpFile((LPCSTR)"Test.dmp", pException);
	cout << "crash dumped!" << endl;
	system("pause");
	return EXCEPTION_EXECUTE_HANDLER;
}

 
int main(int argc, char* argv[])
{
	//注册异常处理函数
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);

	cs::init();

	string cubestring = "RRRUUUUUURRDRRDFFFFFFFFFULLLDDLDDLDDULLULLBBBBBBBBBRRD";
	string cubedeststring = "FURLUFLBBLLBURBRRUUDDRFFBLFUDDLDFDDFDBFDLBBULURLUBFRRR";
	if(argc > 1){
		cubestring = string(argv[1]);
	}
	// write now time
	clock_t starttime = clock();

	for (size_t i = 0; i < 1; i++)
	{
		/* code */
		fprintf(stdout, "Cube:%s\n", cubestring.c_str());
		char solution[256];
		// solveCubeToDestNoU((char *)cubestring.c_str(), (char *)cubedeststring.c_str(), solution, 25, 1000, 0);
		solveCubeNoU((char *)cubestring.c_str(), solution, 25, 1000, 0);
		fprintf(stdout, "Solution2:%s\n", solution);
	}
	

	clock_t deadtime = clock();

	fprintf(stdout, "Time Cost:%dms \n", deadtime - starttime);

}


namespace cs
{
	void init()
	{
		init_without_search();

		Search::init();
		Search5::init();

		
	}
	void init_without_search()
	{
		AxisCube::init();
		AxisCubieCube::init();
		HumanSolverLBLSearch::init();
		HumanSolverLBL::init();
	}
}