#pragma once

#include <string>

using namespace std;

extern "C" {
    string solveCubeNoU(string facelets, int maxDepth = 25, long probeMax = 1000, long probeMin = 0);
    string solveCubeToDestNoU(string facelets_src, string facelets_dst, int maxDepth = 25, long probeMax = 1000, long probeMin = 0);
    string solveCube(string facelets, int maxDepth = 25, long probeMax = 1000, long probeMin = 0);
    string solveCubeToDest(string facelets_src, string facelets_dst, int maxDepth = 25, long probeMax = 1000, long probeMin = 0);
}