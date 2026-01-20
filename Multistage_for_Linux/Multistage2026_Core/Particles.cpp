#define STRICT
#include "../../include/Orbitersdk.h"
#include "Multistage2026.hpp"

constexpr int PARTICLEUPDATECYCLE{1};
constexpr float PARTICLEOVERLAPCYCLE{0.1};

void Multistage2026::ManageParticles(double dt, bool prestep) {
    if (GetAtmPressure() > 10e-5) {
        if (prestep) {
            particlesdt += dt;
        }

        if (particlesdt > PARTICLEUPDATECYCLE) {
            for (int i = 0; i < nPsh; i++) {
                if (psg[i].growing) {
                    if (prestep) {
                        THRUSTER_HANDLE hTh = nullptr;
                        try {
                            if (!psg[i].ToBooster) {
                                if (psg[i].nItem < (int)stage->size() && psg[i].nEngine < (int)stage->at(psg[i].nItem).th_main_h.size()) {
                                    hTh = stage->at(psg[i].nItem).th_main_h[psg[i].nEngine];
                                }
                            } else {
                                if (psg[i].nItem < (int)booster->size() && psg[i].nEngine < (int)booster->at(psg[i].nItem).th_booster_h.size()) {
                                    hTh = booster->at(psg[i].nItem).th_booster_h[psg[i].nEngine];
                                }
                            }
                        } catch (...) { hTh = nullptr; }

                        if (hTh == nullptr) {
                            psg[i].growing = false;
                            continue;
                        }

                        if (psg[i].status == 0) { psg[i].status = 1; }
                        else { psg[i].status = 0; }

                        psg[i].pss.growthrate = psg[i].baserate + psg[i].GrowFactor_rate * log10(RefPressure / GetAtmPressure());
                        double deltasize = psg[i].GrowFactor_size * log10(RefPressure / GetAtmPressure());
                        psg[i].pss.srcsize = psg[i].basesize + deltasize;

                        VECTOR3 thdir;
                        GetThrusterDir(hTh, thdir);

                        thdir.x *= -1; thdir.y *= -1; thdir.z *= -1;
                        psg[i].pos.x = psg[i].basepos.x + thdir.x * deltasize * 0.5;
                        psg[i].pos.y = psg[i].basepos.y + thdir.y * deltasize * 0.5;
                        psg[i].pos.z = psg[i].basepos.z + thdir.z * deltasize * 0.5;

                        if (!psg[i].ToBooster) {
                            if (currentStage <= psg[i].nItem) {
                                psg[i].psh[psg[i].status] = AddExhaustStream(hTh, psg[i].pos, &psg[i].pss);
                            }
                        } else {
                            if (currentBooster <= psg[i].nItem) {
                                psg[i].psh[psg[i].status] = AddExhaustStream(hTh, psg[i].pos, &psg[i].pss);
                            }
                        }
                        psg[i].counting = true;
                    }
                }
            }
            if (!prestep) { particlesdt = 0; }
        }

        if (!prestep) {
            for (int i = 0; i < nPsh; i++) {
                if (psg[i].growing && psg[i].counting) {
                    psg[i].doublepstime += dt;
                    if (psg[i].doublepstime >= PARTICLEOVERLAPCYCLE) {
                        if (!psg[i].FirstLoop) {
                            if (psg[i].status == 0) {
                                if (psg[i].psh[1]) DelExhaustStream(psg[i].psh[1]);
                            } else {
                                if (psg[i].psh[0]) DelExhaustStream(psg[i].psh[0]);
                            }
                            psg[i].counting = false;
                            psg[i].doublepstime = 0;
                        } else {
                            if (psg[i].psh[2]) DelExhaustStream(psg[i].psh[2]);
                            psg[i].FirstLoop = false;
                        }
                    }
                }
            }
        }
    }
}

