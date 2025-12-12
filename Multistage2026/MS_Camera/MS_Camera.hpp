#pragma once
#ifndef _MS_CAMERA_HPP_
#define _MS_CAMERA_HPP_
#define STRICT

#include "../../include/Orbitersdk.h"

class MS_Camera : public VESSEL4{

    public:

        MS_Camera(OBJHANDLE hObj, int fmodel);

        ~MS_Camera();

        void clbkSetClassCaps(FILEHANDLE cfg) override;

        void clbkLoadStateEx(FILEHANDLE scn, void* vs) override;

        void clbkSaveState(FILEHANDLE scn) override;

        void clbkPostStep(double simt, double simdt, double mjd) override;

        int clbkConsumeBufferedKey(DWORD key, bool down, char* kstate) override;

        bool clbkDrawHUD(int mode, const HUDPAINTSPEC* hps, oapi::Sketchpad* skp) override;

        int clbkGeneric(int msgid, int prm, void* context) override;

        static bool InputNewRefVessel(void* id, char* str, void* usrdata);


    private:

        std::string RefVessel;
        double RefHeight;
        oapi::Pen* penwhite;
        VESSEL4* v;
        double Distance;
        double Polar;
        double Azimuth;
        double Aperture;
        OBJHANDLE hMS;

};

#endif //!_MS_CAMERA_HPP_
