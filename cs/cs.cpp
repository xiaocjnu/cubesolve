#include "cs.h"
#include <stdio.h>
#include <ctime>

using namespace cs;
using namespace std;

int main(int argc, char* argv[])
{
	cs::init();

	string cubestring = "UUUUUUBLLURRURRUFFLFFLFFLFFFRRDDDDDDLLDLLDBBDBBBBBBRRR";
	if(argc > 1){
		cubestring = string(argv[1]);
	}
	// write now time
	clock_t starttime = clock();

	for (size_t i = 0; i < 1; i++)
	{
		/* code */
		// 5 axis
		Search5 search5 = Search5();
		fprintf(stdout, "Cube:%s\n", cubestring.c_str());
		string solution = search5.solve_no_axis(cubestring, 0);
		fprintf(stdout, "Solution2:%s\n", solution.c_str());
	}
	

	clock_t deadtime = clock();

	fprintf(stdout, "Time Cast:%d \n", deadtime - starttime);

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