#pragma once
#ifndef __WRITEINI_HPP_
#define __WRITEINI_HPP_
#define STRICT
#include "Multistage2026.hpp"

class WriteIni{

    friend class DevModeDlg;

    public:

        WriteIni();
        ~WriteIni();

        bool WritePayload(PAYLOAD pld, UINT idx, bool Erase = false);
        bool WriteFairing(FAIRING frg, bool wFairing);
        bool WriteParticle(PARTICLE prt, UINT idx, bool Erase = false);
        bool WriteMisc(MISC msc);
        bool WriteTex(TEX tex, UINT ntextures);
        bool WriteBooster(BOOSTER bst, UINT idx, bool Erase = false);
        bool WriteStage(STAGE stg, UINT idx, bool Erase = false);
        bool WriteLes(LES les, bool wLes);
        bool WriteAdapter(ADAPTER adp, bool wAdapter);
        bool WriteFX_Mach(FX_MACH fxm, bool wMach);
        bool WriteFX_Vent(FX_VENT fxv, bool wVent);
        bool WriteFX_Launch(FX_LAUNCH fxl, bool wLaunch);

        void W(std::string b, std::string c);
        void Wvec(std::string b, VECTOR3 v);
        void Wdouble(std::string b, double c);
        void WLongDouble(std::string b, double c);
        void Wint(std::string b, int c);
        void Wbool_int(std::string b, bool c);

        //char *VecToChar(VECTOR3 v);
        std::string VecToChar(VECTOR3 v);
        void SetFilename(std::string fn);
        void SetCurrentSection(std::string Section);
        std::string sectiontxt;

    private:

        std::string filename;
};

#endif //!__WRITEINI_HPP_