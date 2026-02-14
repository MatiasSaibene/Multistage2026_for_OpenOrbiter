#define STRICT
/*********************************************************************************************
This file is part of Multistage2015 project
Copyright belogs to Fred18 for module implementation and its code
Biggest Credit goes to Vinka for his idea of Multistage.dll. None of his code was used here since his addons are all closed source.
Credit goes to Face for having pointed me to the GetPrivateProfileString
Credit goes to Hlynkacg for his OrientForBurn function which was the basis on which I developed the Attitude Function.

Multistage2015 is distributed FREEWARE. Its code is distributed along with the dll. Nobody is authorized to exploit the module or the code or parts of them commercially directly or indirectly.
You CAN distribute the dll together with your addon but in this case you MUST:
-	Include credit to the author in your addon documentation;
-	Add to the addon documentation the official link of Orbit Hangar Mods for download and suggest to download the latest and updated version of the module.
You CAN use parts of the code of Multistage2015, but in this case you MUST:
-	Give credits in your copyright header and in your documentation for the part you used.
-	Let your project be open source and its code available for at least visualization by other users.
You CAN NOT use the entire code for making and distributing the very same module claiming it as your work entirely or partly.
You CAN NOT claim that Multistage2015 is an invention of yourself or a work made up by yourself, or anyhow let intend that is not made and coded by the author.
You install and use Multistage2015 at your own risk, author will not be responsible for any claim or damage subsequent to its use or for the use of part of it or of part of its code.
*********************************************************************************************/
// ==============================================================
//						MultiStage2015
//                  
//					       By Fred18
//                  
//
// Particles.cpp
// ==============================================================

#define STRICT
#include "../../include/Orbitersdk.h"
#include "Multistage2026.hpp"
//############################################################################//

constexpr int PARTICLEUPDATECYCLE{1};
constexpr float PARTICLEOVERLAPCYCLE{0.1};

void Multistage2026::ManageParticles(double dt, bool prestep){
	if (GetAtmPressure() > 10e-5) {
		if (prestep) { particlesdt += dt; }
		if (particlesdt > PARTICLEUPDATECYCLE)
		{

			for (int i = 0; i < nPsh; i++)
			{
				if (psg[i].growing)
				{
					if (prestep) {
						if (psg[i].status == 0) { psg[i].status = 1; }
						else { psg[i].status = 0; }
						psg[i].pss.growthrate = psg[i].baserate + psg[i].GrowFactor_rate * log10(RefPressure / GetAtmPressure());
						double deltasize = psg[i].GrowFactor_size * log10(RefPressure / GetAtmPressure());
						psg[i].pss.srcsize = psg[i].basesize + deltasize;
						VECTOR3 thdir;

						if (!psg[i].ToBooster){
							GetThrusterDir(stage->at(psg[i].nItem).th_main_h.at(psg[i].nEngine), thdir);
						} else {
							GetThrusterDir(booster->at(psg[i].nItem).th_booster_h.at(psg[i].nEngine), thdir);
						}

						thdir.x *= -1;
						thdir.y *= -1;
						thdir.z *= -1;
						psg[i].pos.x = psg[i].basepos.x + thdir.x * deltasize * 0.5;
						psg[i].pos.y = psg[i].basepos.y + thdir.y * deltasize * 0.5;
						psg[i].pos.z = psg[i].basepos.z + thdir.z * deltasize * 0.5;
						if (!psg[i].ToBooster)
						{
							if (currentStage <= psg[i].nItem) {
								psg[i].psh[psg[i].status] = AddExhaustStream(stage->at(psg[i].nItem).th_main_h.at(psg[i].nEngine), psg[i].pos, &psg[i].pss);
							}
						} else {
							if (currentBooster <= psg[i].nItem) {
								psg[i].psh[psg[i].status] = AddExhaustStream(booster->at(psg[i].nItem).th_booster_h.at(psg[i].nEngine), psg[i].pos, &psg[i].pss);
							}
						}
						psg[i].counting = true;

					}


				}

			}
			if (!prestep) { particlesdt = 0; }
		}

		if (!prestep)
		{
			for (int i = 0; i < nPsh; i++)
			{
				if (psg[i].growing)
				{
					if (psg[i].counting)
					{
						psg[i].doublepstime += dt;
						if (psg[i].doublepstime >= PARTICLEOVERLAPCYCLE)
						{
							if (!psg[i].FirstLoop)
							{
								if (psg[i].status == 0)//{todel=1;}else{todel=0;}
								{
									DelExhaustStream(psg[i].psh[1]);
								}
								else if (psg[i].status == 1) {
									DelExhaustStream(psg[i].psh[0]);
								}
								psg[i].counting = false;
								psg[i].doublepstime = 0;

							}
							else {
								DelExhaustStream(psg[i].psh[2]);
								psg[i].FirstLoop = false;
							}
						}
					}
				}
			}
		}
	}
	return;
}