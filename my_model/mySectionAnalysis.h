//
// Created by lacuz on 2019/1/28.
//

#pragma once


#include "CubieCube.h"
#include "HumanSolver.h"

namespace my {
    class SectionAnalysis {
    public:

        // 章节信息
        struct infos_section {
            int index;
            cs::CubieCube cCube;
            int axixCubeIndex;
        };

        cs:: CubieCube startCC;
        std::vector<int> moves; //还原步骤
        long analysis_time;

        infos_section _dcross_sec;
        infos_section _dlayer_sec;
        infos_section _2layer_sec;
        infos_section _ucross_sec;
        infos_section _ulayer_sec;
        infos_section _ucornner_sec;
        infos_section _slove_sec;

        int axis[6] = {0, 4, 11, 15, 18, 23};//遍历各个底面 

        cs::HumanSolver humanSolver;

    public:
        SectionAnalysis();
        ~SectionAnalysis();

        void init(cs:: CubieCube startCC, std::vector<int> moves);

        void anlysis();

        bool isCFOP();

        bool isLBL();
    };


}
