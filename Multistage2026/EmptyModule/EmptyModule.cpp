#include "EmptyModule.hpp"
#include <array>
#include <cmath>

#define ORBITER_MODULE

//Creation
EmptyModule::EmptyModule(OBJHANDLE hObj, int fmodel) : VESSEL4(hObj, fmodel){

}

EmptyModule::~EmptyModule(){

}

void EmptyModule::clbkSetClassCaps(FILEHANDLE cfg){

    SetEmptyMass(1000);
    SetClipRadius(0.01);
    double ro = 10;

    std::array<TOUCHDOWNVTX, 4> td;

    for(int i = 0; i < 4; i++){
        td.at(i).damping = 3e5;
        td.at(i).mu = 3;
        td.at(i).mu_lng = 3;
        td.at(i).stiffness = 3e5;
    }

    td.at(0).pos.x = -cos(30 * RAD) * ro;
    td.at(0).pos.y = -sin(30 * RAD) * ro;
    td.at(0).pos.z = 0.01;

    td.at(1).pos.x = 0;
    td.at(1).pos.y = 1 * ro;
    td.at(1).pos.z = 0.01;

    td.at(2).pos.x = cos(30 * RAD) * ro;
    td.at(2).pos.y = -sin(30 * RAD) * ro;
    td.at(2).pos.z = 0.01;

    td.at(3).pos.x = 0;
    td.at(3).pos.y = 0;
    td.at(3).pos.z = 5 * ro;

    SetTouchdownPoints(td.data(), 4);
}

void EmptyModule::clbkLoadStateEx(FILEHANDLE scn, void* vs){
	char* line;
	while (oapiReadScenario_nextline(scn, line)) {
		ParseScenarioLineEx(line, vs);
	}
}

void EmptyModule::clbkSaveState(FILEHANDLE scn){
	SaveDefaultState(scn);
}

//Creation
DLLCLBK void InitModule(HINSTANCE hModule){

}

DLLCLBK void ExitModule(HINSTANCE hModule){

}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hvessel, int flightmodel){
    return new EmptyModule(hvessel, flightmodel);
}

DLLCLBK void ovcExit(VESSEL* vessel){
    if (vessel)delete(EmptyModule*)vessel;
}