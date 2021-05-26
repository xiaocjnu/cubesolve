//
// Created by lacuz on 2019/1/28.
//

#include "mySectionAnalysis.h"
#include "HumanSolver.h"
#include "AxisCubieCube.h"

using namespace std;
using namespace cs;
namespace my {

    SectionAnalysis::SectionAnalysis() {
    }


    SectionAnalysis::~SectionAnalysis() {
    }

    void SectionAnalysis::init(cs:: CubieCube startCC,vector<int> moves){
        this->startCC = startCC;
        this->moves = moves;
        anlysis();
    }

    void SectionAnalysis::anlysis(){
        bool isDCrossS = false;
        bool isDLayerS = false;
        bool isSecondLayerS = false;
        bool isUCrossS = false;
        bool isULayerS = false;
        bool isUCornerS = false;
        bool isSolved = false;
        cs::CubieCube newCC = startCC;
        for (int i = 0; i < (int)moves.size(); i++) {
            newCC = newCC.move(moves[i]);
            for (int j = 0; j < 6; j++) {//遍历六种状态
                AxisCubieCube ac = AxisCubieCube::eye();
                ac.axis = AxisCube::valid_table[axis[j]];
                ac.setAbsCubie(newCC);
                if (!isDCrossS) {
                    isDCrossS = humanSolver.isDCrossSolved(ac);
                    if (isDCrossS) {
                        _dcross_sec.index = i;
                        _dcross_sec.cCube = newCC;
                        _dcross_sec.axixCubeIndex = axis[j];
                    }
                }
                if (!isDLayerS) {
                    isDLayerS = humanSolver.isDLayerSolved(ac);
                    if (isDLayerS) {
                        _dlayer_sec.index = i;
                        _dlayer_sec.cCube = newCC;
                        _dlayer_sec.axixCubeIndex = axis[j];
                    }
                }
                if (!isSecondLayerS) {
                    isSecondLayerS = humanSolver.isF2LSolved(ac);
                    if (isSecondLayerS) {
                        _2layer_sec.index = i;
                        _2layer_sec.cCube = newCC;
                        _2layer_sec.axixCubeIndex = axis[j];
                    }
                }
                if (!isUCrossS) {
                    isUCrossS = humanSolver.isUCrossSolved(ac);
                    if (isUCrossS) {
                        _ucross_sec.index = i;
                        _ucross_sec.cCube = newCC;
                        _ucross_sec.axixCubeIndex = axis[j];
                    }
                }
                if (!isULayerS) {
                    isULayerS = humanSolver.isOLLSolved(ac);
                    if (isULayerS) {
                        _ulayer_sec.index = i;
                        _ulayer_sec.cCube = newCC;
                        _ulayer_sec.axixCubeIndex = axis[j];
                    }
                }
                if (!isUCornerS) {
                    isUCornerS = humanSolver.isUCornerSolved(ac);
                    if (isUCornerS) {
                        _ucornner_sec.index = i;
                        _ucornner_sec.cCube = newCC;
                        _ucornner_sec.axixCubeIndex = axis[j];
                    }
                }
                if (!isSolved) {
                    isSolved = ac.isSolved();
                    if (isSolved) {
                        _slove_sec.index = i;
                        _slove_sec.cCube = newCC;
                        _slove_sec.axixCubeIndex = axis[j];
                    }
                }
            }
        }
    }

    bool SectionAnalysis::isCFOP(){
        if(_dcross_sec.index==0 || _dcross_sec.index==_dlayer_sec.index){
            return false;
        }
        if(_2layer_sec.index==0 || _2layer_sec.index!=_dlayer_sec.index){
            return false;
        }
        if(_ulayer_sec.index==0 || _ulayer_sec.index!=_ucross_sec.index){
            return false;
        }
        if(_slove_sec.index==0 ){
            return false;
        }
        return  true;
    }

    bool SectionAnalysis::isLBL(){
        if(_dcross_sec.index==0 || _dcross_sec.index==_dlayer_sec.index){
            return false;
        }
        if(_dlayer_sec.index==0 || _dlayer_sec.index==_2layer_sec.index){
            return false;
        }
        if(_2layer_sec.index==0 || _2layer_sec.index==_ucross_sec.index){
            return false;
        }
        if(_ucross_sec.index==0 || _ucross_sec.index==_ulayer_sec.index){
            return false;
        }
        if(_ulayer_sec.index==0 || _ulayer_sec.index==_ucornner_sec.index){
            return false;
        }
        if(_ucornner_sec.index==0 || _ucornner_sec.index==_slove_sec.index){
            return false;
        }
        return  true;
    }
}
