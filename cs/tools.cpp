#include "tools.h"
#include "Search5.h"
#include "string.h"
#include <ctime>

using namespace std;
using namespace cs;

extern "C" {

    void solveCubeNoU(char * facelets, char *solution, int maxDepth, long probeMax, long probeMin){
        Search5 search5 = Search5();
        search5.set_param(maxDepth, probeMax, probeMin);
        string result = search5.solve_no_axis(facelets, 0);
        memcpy(solution, result.c_str(), result.length() + 1);
    }

    void solveCubeToDestNoU(char * facelets_src, char * facelets_dst, char *solution, int maxDepth, long probeMax, long probeMin){
        Search5 search5 = Search5();
        search5.set_param(maxDepth, probeMax, probeMin);
        string result = search5.solve_no_axis(facelets_src, facelets_dst, 0);
        memcpy(solution, result.c_str(), result.length() + 1);
    }

    void solveCube(char * facelets, char *solution, int maxDepth, long probeMax, long probeMin){
        Search5 search5 = Search5();
        search5.set_param(maxDepth, probeMax, probeMin);
        string result = search5.solve(facelets);
        memcpy(solution, result.c_str(), result.length() + 1);
    }

    void solveCubeToDest(char * facelets_src, char * facelets_dst, char *solution, int maxDepth, long probeMax, long probeMin){
        Search5 search5 = Search5();
        search5.set_param(maxDepth, probeMax, probeMin);
        string result = search5.solve(facelets_src, facelets_dst);
        memcpy(solution, result.c_str(), result.length() + 1);
    }

    void teststring(char *input, char *output){
        char* name = (char*)"David";
        
        char solution[100];
        char *facelets = (char *)"RRFUUFBLFUUUFRRFDDUFRLFDLFDFRRLDBLDBULLULDBBDLBBUBBRRD";
        solveCube(facelets, solution, 25, 1000, 0);
        memcpy(output, name, strlen(name) + 1);
    }

    void init(){
        Search5::init();
    }

    int testPerformance(int times, bool showPrint){
        string cubestring = "RRRUUUUUURRDRRDFFFFFFFFFULLLDDLDDLDDULLULLBBBBBBBBBRRD";
        string cubedeststring = "FURLUFLBBLLBURBRRUUDDRFFBLFUDDLDFDDFDBFDLBBULURLUBFRRR";
        // write now time
        clock_t starttime = clock();

        for (size_t i = 0; i < times; i++)
        {
            /* code */
            if(showPrint){
                fprintf(stdout, "Cube:%s\n", cubestring.c_str());
            }
            char solution[256];
            solveCubeToDestNoU((char *)cubestring.c_str(), (char *)cubedeststring.c_str(), solution, 25, 1000, 0);
            // solveCubeNoU((char *)cubestring.c_str(), solution, 25, 1000, 0);
            // fprintf(stdout, "Solution2:%s\n", solution);
        }

        clock_t deadtime = clock();

        if(showPrint){
            fprintf(stdout, "Time Cost:%dms \n", deadtime - starttime);
        }

        return deadtime - starttime;
    }

}
