#pragma once
#ifndef __EMPTYMODULE_HPP_
#define __EMPTYMODULE_HPP_
#define STRICT

#include "../../include/Orbitersdk.h"

class EmptyModule :public VESSEL4 {

    public:
	    EmptyModule(OBJHANDLE hObj, int fmodel);
	    ~EmptyModule();
	    void clbkSetClassCaps(FILEHANDLE cfg) override;
	    void clbkLoadStateEx(FILEHANDLE scn, void* vs) override;
	    void clbkSaveState(FILEHANDLE scn) override;
};

#endif //!__EMPTYMODULE_HPP_