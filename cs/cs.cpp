#include "cs.h"
#include "tools.h"
#include <stdio.h>
#include <ctime>

using namespace cs;
using namespace std;

int main(int argc, char* argv[])
{
	cs::init();

	string cubestring = "RRFUUFBLFUUUFRRFDDUFRLFDLFDFRRLDBLDBULLULDBBDLBBUBBRRD";
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
		solveCubeToDestNoU((char *)cubestring.c_str(), (char *)cubedeststring.c_str(), solution, 25, 1000, 0);
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