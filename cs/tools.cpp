#include "tools.h"
#include "Search5.h"
#include "string.h"

using namespace std;
using namespace cs;

extern "C" {

    void solveCubeNoU(char * facelets, char *solution, int maxDepth, long probeMax, long probeMin){
        Search5 search5 = Search5();
        search5.set_param(maxDepth, probeMax, probeMin);
        string result = search5.solve_no_axis(facelets, 0);
        memcpy(solution, result.c_str(), result.length() + 1);
        printf("result:(%s) (len:%d) (solution len:%d) \n", result.c_str(), result.length(), strlen(solution));
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
        strcpy(output, name);
    }

}