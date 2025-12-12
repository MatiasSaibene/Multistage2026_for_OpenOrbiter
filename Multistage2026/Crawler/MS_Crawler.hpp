#pragma once
#include <array>
#include <string>
#ifndef _MS_CRAWLER_HPP_
#define _MS_CRAWLER_HPP_
#define STRICT
#include "../../include/Orbitersdk.h"


constexpr int NANIM{64};
constexpr int NCING{8};

class MS_Crawler : public VESSEL4{

    public:

        MS_Crawler(OBJHANDLE hObj, int fmodel);
        ~MS_Crawler();

        void clbkSetClassCaps(FILEHANDLE cfg) override;
	    void clbkLoadStateEx(FILEHANDLE scn, void* vs) override;
    	void clbkSaveState(FILEHANDLE scn) override;
	    int clbkConsumeBufferedKey(DWORD key, bool down, char* kstate) override;
	    void clbkPreStep(double simt, double simdt, double mjd) override;
	    void clbkPostCreation() override;
	    void MoveAround(bool FWD, bool BWD, bool LFT, bool RGT);
        void UpdateAnimations(bool Reset);
        void DefineAnimations();
        void UpdateLifter();

    private:

        bool FWD, BWD, LFT, RGT;
        double Speed;
        double LengthCrawl;
	    double rt;
	    double earth_circ;
	    double each_deg;
    	double AngSpeed;
	    double TangentSpd;

        std::array<std::array<UINT, NCING>, NANIM> animCing;
        std::array<std::array<double, NCING>, NANIM> StateCing;
        std::array<std::array<double, NCING>, NANIM> Initials;
        std::array<std::array<MGROUP_TRANSLATE *, NCING>, NANIM> Tr1;
        std::array<std::array<MGROUP_ROTATE *, NCING>, NANIM> Rot1;
        std::array<std::array<MGROUP_TRANSLATE *, NCING>, NANIM> Tr2;
        std::array<std::array<MGROUP_ROTATE *, NCING>, NANIM> Rot2;
        std::array<std::array<MGROUP_TRANSLATE *, NCING>, NANIM> Tr3;
        std::array<std::array<MGROUP_ROTATE *, NCING>, NANIM> Rot3;
        std::array<std::array<MGROUP_TRANSLATE *, NCING>, NANIM> Tr4;
        std::array<std::array<MGROUP_ROTATE *, NCING>, NANIM> Rot4;

        UINT AnimLifter;
        double StateLifter;
	    double SpeedLifter;
	    MGROUP_TRANSLATE* Lifter;
	    MGROUP_SCALE* Pistons;
	    bool GoingUp, GoingDown;
	    VECTOR3 CameraOff;

        std::array<LightEmitter *, 4> lights;

        COLOUR4 col_d;
    	COLOUR4 col_s;
    	COLOUR4 col_a;
    	COLOUR4 col_white;

        OBJHANDLE hPad;
	    VESSEL4* vPad;
	    VESSELSTATUS2 vsPad;
	    ATTACHMENTHANDLE AttPadCrawler, AttPadRocket, AttCrawlerPad;

        MESHHANDLE msh;
        bool PadDetached;

        std::string PadMesh;
        std::string PadModule;
        bool wCustomPadModule;
        bool NoPad;

};

#endif //!_MS_CRAWLER_HPP_