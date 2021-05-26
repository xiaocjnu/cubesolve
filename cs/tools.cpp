#include "tools.h"
#include "Search5.h"

using namespace std;
using namespace cs;

extern "C" {
    string solveCubeNoU(string facelets, int maxDepth, long probeMax, long probeMin){
        Search5 search5 = Search5();
        string solution = search5.solve_no_axis(facelets, 0, maxDepth, probeMax, probeMin);
        return solution;
    }

    string solveCubeToDestNoU(string facelets_src, string facelets_dst, int maxDepth, long probeMax, long probeMin){
        Search5 search5 = Search5();
        string solution = search5.solve_no_axis(facelets_src, facelets_dst, 0, maxDepth, probeMax, probeMin);
        return solution;
    }

    string solveCube(string facelets, int maxDepth, long probeMax, long probeMin){
        Search5 search5 = Search5();
        string solution = search5.solve(facelets, maxDepth, probeMax, probeMin);
        return solution;
    }

    string solveCubeToDest(string facelets_src, string facelets_dst, int maxDepth, long probeMax, long probeMin){
        Search5 search5 = Search5();
        string solution = search5.solve(facelets_src, facelets_dst, maxDepth, probeMax, probeMin);
        return solution;
    }

}