#pragma once
#include <array>
#ifndef _MS_HANGAR_HPP_
#define _MS_HANGAR_HPP_
#define STRICT

#include "../../include/Orbitersdk.h"

class MS_Hangar : public VESSEL4{

    public:
        MS_Hangar(OBJHANDLE hObj, int fmodel);
        ~MS_Hangar();

        void clbkSetClassCaps(FILEHANDLE cfg) override;
	    void clbkLoadStateEx(FILEHANDLE scn, void* vs) override;
	    void clbkSaveState(FILEHANDLE scn) override;
	    int clbkConsumeDirectKey(char* kstate) override;
	    int clbkConsumeBufferedKey(DWORD key, bool down, char* kstate) override;
        bool UpdateHangarAnimations(VECTOR3 pos);
        int ProcessHangar(char* kstate);
        int clbkGeneric(int msgid, int prm, void* context) override;

    private:

        ATTACHMENTHANDLE PadHangar;
        std::array<LightEmitter *, 5> Hangarlight;
	    UINT HookTrX;
	    UINT HookTrZ;
	    UINT HookTrY;
	    UINT CablesTrX;
	    UINT CablesTrY;
	    UINT CablesTrZ;
	    UINT BridgeTrY;
	    UINT TrailerTrX;
	    COLOUR4 col_d;
	    COLOUR4 col_s;
	    COLOUR4 col_a;
	    COLOUR4 col_white;
	    VECTOR3 hangaranims;
};

#endif //! _MS_HANGAR_HPP_