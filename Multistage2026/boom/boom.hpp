#pragma once
#ifndef __BOOM_HPP_
#define __BOOM_HPP_
#define STRICT
#include "../../include/Orbitersdk.h"
//Vessel class

class boom : public VESSEL4{

    public:

        boom(OBJHANDLE hObj, int fmodel);
        ~boom();

        void clbkSetClassCaps(FILEHANDLE cfg) override;
	    void clbkLoadStateEx(FILEHANDLE scn, void* vs) override;
	    void clbkSaveState(FILEHANDLE scn) override;

	    void clbkPostStep(double simt, double simdt, double mjd) override;

	    void clbkPostCreation() override;

	private:

        PROPELLANT_HANDLE ph_h;
	    THRUSTER_HANDLE th_h;
	    THGROUP_HANDLE thg_h;
    	MESHHANDLE msh_h;
    	int msh_idh;
	    int MyID2;
	    double deltat1;
        
};


#endif //!__BOOM_HPP_

