#pragma once

#include <string>

using namespace std;

extern "C" {
    void solveCubeNoU(char * facelets, char *solution, int maxDepth, long probeMax, long probeMin);

    void solveCubeToDestNoU(char * facelets_src, char * facelets_dst, char *solution, int maxDepth, long probeMax, long probeMin);

    void solveCube(char * facelets, char *solution, int maxDepth, long probeMax, long probeMin);

    void solveCubeToDest(char * facelets_src, char * facelets_dst, char *solution, int maxDepth, long probeMax, long probeMin);

    void teststring(char *input, char *output);

}