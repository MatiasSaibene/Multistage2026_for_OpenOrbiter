#include <format>
#include <string>
#define STRICT
#include "../../include/Orbitersdk.h"
#include "Multistage2026.hpp"
#include "DevModeCtrl.hpp"
#include "WriteIni.hpp"
#include "simpleini/SimpleIni.h"

WriteIni::WriteIni(){

}

WriteIni::~WriteIni(){

}

void WriteIni::SetFilename(std::string fn){
    
    filename = fn;

}

std::string WriteIni::VecToChar(VECTOR3 v){

    std::string out = std::format("{:.3f}, {:.3f}, {:.3f}", v.x, v.y, v.z);

    return out;
}

void WriteIni::W(std::string b, std::string c){

    CSimpleIniA ini(true, false, false);

    std::string sectiontxt;

    if (ini.LoadFile(filename.c_str()) < 0) {
        oapiWriteLogV("WriteIni::W: Failed to load INI configuration file: %s", filename.c_str());
        return;
    }

    ini.SetValue(sectiontxt.c_str(), b.c_str(), c.c_str());

}

void WriteIni::Wvec(std::string b, VECTOR3 v){

    std::string temp = VecToChar(v);

    W(b, temp);

}

void WriteIni::Wdouble(std::string b, double c){

    std::string temp = std::format("{:.3f}", c);

    W(b, temp);
}

void WriteIni::WLongDouble(std::string b, double c){

    std::string temp = std::format("{}", c);

    W(b, temp);
}

void WriteIni::Wint(std::string b, int c){

    std::string temp = std::format("{}", c);

    W(b, temp);
}

void WriteIni::Wbool_int(std::string b, bool c){

    int q = 0;

    if(c){
        q = 1;
    }

    std::string temp = std::format("{}", q);

    W(b, temp);
}

void WriteIni::SetCurrentSection(std::string Section){

    sectiontxt = Section;
}

bool WriteIni::WritePayload(PAYLOAD pld, UINT idx, bool Erase){

    std::string sectionpld = std::format("PAYLOAD_{}", idx + 1);

    SetCurrentSection(sectionpld);

    if(!Erase){
        W("Meshname", pld.meshname);
        W("Off", pld.MultiOffset);
        Wvec("Rotation", pld.Rotation);
        Wdouble("Height", pld.height);
		Wdouble("Diameter", pld.diameter);
		Wdouble("Mass", pld.mass);
		W("Module", pld.module);
		W("Name", pld.name);
		Wvec("Speed", pld.speed);
		Wvec("Rot_speed", pld.rot_speed);
		Wint("Render", pld.render);
		Wbool_int("Live", pld.live);
    } else {
        W("", "");
    }

    return true;
}

bool WriteIni::WriteFairing(FAIRING frg, bool wFairing){
    std::string sectionFair = "FAIRING";
    SetCurrentSection(sectionFair);

    if(wFairing){
        W("Meshname", frg.meshname.data());
		Wvec("Off", frg.off);
		Wint("N", frg.N);
		Wdouble("Angle", frg.angle);
		Wdouble("Height", frg.height);
		Wdouble("Diameter", frg.diameter);
		Wdouble("EmptyMass", frg.emptymass);
		W("Module", frg.module.data());
		Wvec("Speed", frg.speed);
		Wvec("Rot_Speed", frg.rot_speed);
    } else {
        W("Meshname", "");
		W("Off", "");
		W("N", "");
		W("Angle", "");
		W("Height", "");
		W("Diameter", "");
		W("EmptyMass", "");
		W("Module", "");
		W("Speed", "");
		W("Rot_Speed", "");
    }

    return true;
}

bool WriteIni::WriteParticle(PARTICLE prt, UINT idx, bool Erase){

    std::string sectionPart = std::format("PARTICLESTREAM_{}", idx + 1);
    SetCurrentSection(sectionPart);

    if(!Erase){
        W("Name", prt.ParticleName);
		Wdouble("Srcsize", prt.Pss.srcsize);
		Wdouble("Srcrate", prt.Pss.srcrate);
		Wdouble("V0", prt.Pss.v0);
		Wdouble("Srcspread", prt.Pss.srcspread);
		Wdouble("Lifetime", prt.Pss.lifetime);
		Wdouble("Growthrate", prt.Pss.growthrate);
		Wdouble("Atmslowdown", prt.Pss.atmslowdown);

        switch (prt.Pss.ltype){

            case PARTICLESTREAMSPEC::EMISSIVE:{
                W("Ltype", "EMMISIVE");
                break;
            }
            case PARTICLESTREAMSPEC::DIFFUSE:{
                W("Ltype", "DIFFUSE");
                break;
            }

            switch(prt.Pss.levelmap){

                case PARTICLESTREAMSPEC::LVL_FLAT:{
                    W("Levelmap", "LVL_FLAT");
                    break;
                }
                case PARTICLESTREAMSPEC::LVL_LIN:{
                    W("Levelmap", "LVL_LIN");
                    break;
                }
                case PARTICLESTREAMSPEC::LVL_SQRT:{
                    W("Levelmap", "LVL_SQRT");
                    break;
                }
                case PARTICLESTREAMSPEC::LVL_PLIN:{
                    W("Levelmap", "LVL_PLIN");
                    break;
                }
                case PARTICLESTREAMSPEC::LVL_PSQRT:{
                    W("Levelmap", "LVL_PSQRT");
                    break;
                }
            }

            WLongDouble("Lmin", prt.Pss.lmin);
            Wdouble("Lmax", prt.Pss.lmax);

            switch(prt.Pss.atmsmap){
                case PARTICLESTREAMSPEC::ATM_FLAT:{
                    W("Atmsmap", "ATM_FLAT");
                    break;
                }
                case PARTICLESTREAMSPEC::ATM_PLIN:{
                    W("Atmsmap", "ATM_PLIN");
                    break;
                }
                case PARTICLESTREAMSPEC::ATM_PLOG:{
                    W("Atmsmap", "ATM_PLOG");
                    break;
                }
            }

            WLongDouble("Amin", prt.Pss.amin);
            Wdouble("Amax", prt.Pss.amax);
            W("Tex", prt.TexName);

            if(prt.Growing){
                Wdouble("GrowFactor_size", prt.GrowFactor_size);
                Wdouble("GrowFactor_rate", prt.GrowFactor_rate);
            } else {
                Wdouble("GrowFactor_size", 0.0);
                Wdouble("GrowFactor_rate", 0.0);
            }
        }
    } else {
        W("", "");
    }

    return true;
}

bool WriteIni::WriteMisc(MISC msc){

    std::string sectionMisc = "MISC";
    SetCurrentSection(sectionMisc);

    Wdouble("COG", msc.COG);
	Wint("GNC_Debug", msc.GNC_Debug);
	Wbool_int("Telemetry", msc.telemetry);
	Wint("Focus", msc.Focus);
	Wbool_int("Thrust_real_pos", msc.thrustrealpos);
	Wdouble("Vertical_Angle", msc.VerticalAngle * DEG);

    if(msc.PadModule == "EmptyModule"){
        W("Pad_Module", "");
    } else {
        W("Pad_Module", msc.PadModule);
    }

    return true;
}

bool WriteIni::WriteTex(TEX tex, UINT ntextures){

    std::string sectionTex = "TEXTURE_LIST";
    SetCurrentSection(sectionTex);

    std::string nx, trans;

    for(UINT n = 0; n < ntextures; n++){
        trans = tex.TextureName.at(n);

        nx = std::format("TEX_{}", n + 1);
        W(nx, trans);
    }

    for(UINT nrm = ntextures; nrm < 16; nrm++){
        nx = std::format("TEX_{}", nrm + 1);
        W(nx, "");
    }

    return true;

}

bool WriteIni::WriteBooster(BOOSTER bst, UINT idx, bool Erase){
	
    std::string sectionbst = std::format("BOOSTER_{}", idx + 1);

	SetCurrentSection(sectionbst);

	if (!Erase){
		Wint("N", bst.N);
		Wdouble("Angle", bst.angle * DEG);
		W("Meshname", bst.meshname);
		Wvec("Off", bst.off);
		Wdouble("Height", bst.height);
		Wdouble("Diameter", bst.diameter);
		Wdouble("Thrust", bst.thrust);
		Wdouble("EmptyMass", bst.emptymass);
		Wdouble("FuelMass", bst.fuelmass);
		Wdouble("Burntime", bst.burntime);
		Wdouble("BurnDelay", bst.burndelay);
		Wdouble("Eng_diameter", bst.eng_diameter);
		W("Eng_tex", bst.eng_tex);
		W("Eng_pstream1", bst.eng_pstream1);
		W("Eng_pstream2", bst.eng_pstream2);

		if ((bst.eng_dir.x != 0) && (bst.eng_dir.y != 0) && (bst.eng_dir.z != 1)){
			Wvec("Eng_dir", bst.eng_dir);
		}

		switch (bst.nEngines){
		    case 1:{
			    Wvec("Eng_1", bst.eng.at(0));
			    break;
		    }
		    case 2:{
			    Wvec("Eng_1", bst.eng.at(0));
			    Wvec("Eng_2", bst.eng.at(1));
			    break;
            }
		    case 3:{
			    Wvec("Eng_1", bst.eng.at(0));
			    Wvec("Eng_2", bst.eng.at(1));
			    Wvec("Eng_3", bst.eng.at(2));
			    break;
            }
		    case 4:{
			    Wvec("Eng_1", bst.eng.at(0));
			    Wvec("Eng_2", bst.eng.at(1));
			    Wvec("Eng_3", bst.eng.at(2));
			    Wvec("Eng_4", bst.eng.at(3));
			    break;
            }
		}

		if (bst.expbolt.wExpbolt){
			Wvec("Expbolts_Pos", bst.expbolt.pos);
			W("Expbolts_Pstream", bst.expbolt.pstream);
			Wdouble("Expbolts_Anticipation", bst.expbolt.anticipation);
		}

		for (UINT i = 0; i < 10; i++){
			if (bst.curve.at(i).x != 9000000){
				std::string curvec, curvet;
				curvec = std::format("({:.1f},{:.1f})", bst.curve.at(i).x, bst.curve.at(i).y);
				curvet = std::format("Curve_%i", i + 1);
				W(curvet, curvec);
			}
		}
		Wvec("Speed", bst.speed);
		Wvec("Rot_speed", bst.rot_speed);
	} else {
		W("", "");
	}

	return true;
}

bool WriteIni::WriteStage(STAGE stg, UINT idx, bool Erase){
	
    std::string sectionstage = std::format("STAGE_{}", idx + 1);

	SetCurrentSection(sectionstage);
	if (!Erase){
		W("Meshname", stg.meshname);
		Wvec("Off", stg.off);
		Wdouble("Height", stg.height);
		Wdouble("Diameter", stg.diameter);
		Wdouble("Thrust", stg.thrust);
		Wdouble("EmptyMass", stg.emptymass);
		Wdouble("FuelMass", stg.fuelmass);
		Wdouble("Burntime", stg.burntime);
		
        for (UINT i = 0; i < stg.nEngines; i++){
			std::string engbuff = std::format("Eng_{}", i + 1);
			Wvec(engbuff, stg.eng.at(i));
		}

		for (UINT j = stg.nEngines; j < 32; j++){
			std::string engbuff = std::format("Eng_{}", j + 1);
			W(engbuff, "");
		}

		if (stg.ignite_delay > 0){
			Wdouble("Ignite_delay", stg.ignite_delay);
		} else {
			W("Ignite_delay", "");
		}

		if (stg.batteries.wBatts){
			Wdouble("Battery", stg.batteries.MaxCharge / 3600);
		} else {
			W("Battery", "");
		}

		if (stg.reignitable){
			int val = 1;
			Wint("Reignitable", val);
		} else {
			int val = 0;
			Wint("Reignitable", val);
		}

		if (stg.defpitch){
			W("Pitchthrust", "");
		} else {
			Wdouble("Pitchthrust", stg.pitchthrust);
		}
		if (stg.defroll){
			W("Rollthrust", "");
		} else {
			Wdouble("Rollthrust", stg.rollthrust);
		}
		if (stg.defyaw){
			W("Yawthrust", "");
		} else {
			Wdouble("Yawthrust", stg.yawthrust);
		}
		if (stg.linearthrust > 0){
			Wdouble("Linearthrust", stg.linearthrust);
		} else {
			W("Linearthrust", "");
		}
		if ((stg.linearisp != 0) && (stg.linearisp != stg.isp * 100)){
			Wdouble("Linear_isp", stg.linearisp);
		} else {
			W("Linear_isp", "");
		}

		Wdouble("Eng_diameter", stg.eng_diameter);

		if (stg.eng_tex.empty()) {
			W("Eng_tex", "");
		} else {
			W("Eng_tex", stg.eng_tex);
		}
		if (stg.eng_pstream1.empty()) {
			W("Eng_pstream1", "");
		} else {
			W("Eng_pstream1", stg.eng_pstream1);
		}

		if (stg.eng_pstream2.empty()) {
			W("Eng_pstream2", "");
		} else {
			W("Eng_pstream2", stg.eng_pstream2);
		}

		if (stg.ParticlesPacked){
			Wint("Particles_packed_to_engine", stg.ParticlesPackedToEngine);
		}
		Wvec("Eng_dir", stg.eng_dir);

		W("Module", stg.module);
		Wvec("Speed", stg.speed);
		Wvec("Rot_speed", stg.rot_speed);

		if (stg.ullage.wUllage){
			Wdouble("Ullage_Thrust", stg.ullage.thrust);
			Wdouble("Ullage_Anticipation", stg.ullage.anticipation);
			Wdouble("Ullage_Overlap", stg.ullage.overlap);
			Wint("Ullage_N", stg.ullage.N);
			Wdouble("Ullage_Angle", stg.ullage.angle);
			Wdouble("Ullage_Diameter", stg.ullage.diameter);
			Wdouble("Ullage_Length", stg.ullage.length);
			Wvec("Ullage_dir", stg.ullage.dir);
			Wvec("Ullage_pos", stg.ullage.pos);
			Wdouble("Ullage_Rectfactor", stg.ullage.rectfactor);
		} else {
			W("Ullage_Thrust", "");
			W("Ullage_Anticipation", "");
			W("Ullage_Overlap", "");
			W("Ullage_N", "");
			W("Ullage_Angle", "");
			W("Ullage_Diameter", "");
			W("Ullage_Length", "");
			W("Ullage_dir", "");
			W("Ullage_pos", "");
			W("Ullage_Rectfactor", "");
		}

		if (stg.expbolt.wExpbolt){
			Wvec("Expbolt_pos", stg.expbolt.pos);
			W("Expbolt_pstream", stg.expbolt.pstream);
			Wdouble("Expbolt_anticipation", stg.expbolt.anticipation);
		} else {
			W("Expbolt_pos", "");
			W("Expbolt_pstream", "");
			W("Expbolt_anticipation", "");
		}

		//// INTERSTAGE
		std::string sectioninter = std::format("SEPARATION_{}{}", idx, idx + 1);
		SetCurrentSection(sectioninter);

		if (idx > 0) {
			if (stg.wInter){
				W("Meshname", stg.interstage.meshname);
				Wvec("Off", stg.interstage.off);
				Wdouble("Height", stg.interstage.height);
				Wdouble("Diameter", stg.interstage.diameter);
				Wdouble("EmptyMass", stg.interstage.emptymass);
				if (stg.interstage.separation_delay > 0){
					Wdouble("Separation_delay", stg.interstage.separation_delay);
				} else {
					W("Separation_delay", "");
				}

				W("Module", stg.interstage.module);
				Wvec("Speed", stg.interstage.speed);
				Wvec("Rot_speed", stg.interstage.rot_speed);
			} else {

				W("Meshname", "");
				W("Off", "");
				W("Height", "");
				W("Diameter", "");
				W("EmptyMass", "");
				W("Separation_delay", "");
				W("Module", "");
				W("Speed", "");
				W("Rot_speed", "");
			}
		}
	} else {
		W("", "");
	}

	return true;
}

bool WriteIni::WriteLes(LES les, bool wLes){
	std::string sectionles = "LES";

	SetCurrentSection(sectionles);

	if (wLes)
	{
		W("Meshname", les.meshname.data());
		Wvec("Off", les.off);
		Wdouble("Height", les.height);
		Wdouble("Diameter", les.diameter);
		Wdouble("EmptyMass", les.emptymass);
		W("Module", les.module.data());
		Wvec("Speed", les.speed);
		Wvec("Rot_speed", les.rot_speed);
	}
	else {
		W("Meshname", "");
		W("Off", "");
		W("Height", "");
		W("Diameter", "");
		W("EmptyMass", "");
		W("Module", "");
		W("Speed", "");
		W("Rot_speed", "");
	}
	return true;
}

bool WriteIni::WriteAdapter(ADAPTER adp, bool wAdapter){
	std::string sectionadap = "ADAPTER";

	SetCurrentSection(sectionadap);

	if (wAdapter){
		W("Meshname", adp.meshname);
		Wvec("Off", adp.off);
		Wdouble("Height", adp.height);
		Wdouble("Diameter", adp.diameter);
		Wdouble("EmptyMass", adp.emptymass);
	} else {
		W("Meshname", "");
		W("Off", "");
		W("Height", "");
		W("Diameter", "");
		W("EmptyMass", "");
	}
	return true;
}

bool WriteIni::WriteFX_Mach(FX_MACH fxm, bool wMach){
	std::string sectionfxm = "FX_MACH";

	SetCurrentSection(sectionfxm);
	std::string buff;
	if (wMach){
		W("pstream", fxm.pstream);
		Wdouble("Mach_min", fxm.mach_min);
		Wdouble("Mach_max", fxm.mach_max);
		Wvec("Dir", fxm.dir);
		for (UINT i = 0; i < fxm.nmach; i++){
			buff = std::format("Off_%i", i + 1);
			Wvec(buff, fxm.off.at(i));
		}
	} else {
		W("pstream", "");
		W("Mach_min", "");
		W("Mach_max", "");
		W("Dir", "");
		for (UINT i = 0; i < 10; i++){
			buff = std::format("Off_%i", i + 1);
			W(buff, "");
		}
	}

	return true;
}

bool WriteIni::WriteFX_Vent(FX_VENT fxv, bool wVent){
	
	std::string sectionvent = "FX_VENT";
	
	SetCurrentSection(sectionvent);

	std::string buff;

	if (wVent){
		W("pstream", fxv.pstream);
		for (UINT i = 1; i <= fxv.nVent; i++){
			buff = std::format("Off_{}", i);
			Wvec(buff, fxv.off.at(i));
			buff = std::format("Time_fin_{}", i);
			Wdouble(buff, fxv.time_fin.at(i));
			buff = std::format("Dir_{}", i);
			Wvec(buff, fxv.dir.at(i));
		}

	} else {
		W("pstream", "");
		for (UINT i = 1; i <= 10; i++){
			buff = std::format("Off_{}", i);
			W(buff, "");
			buff = std::format("Time_fin_{}", i);
			W(buff, "");
			buff = std::format("Dir_{}", i);
			W(buff, "");
		}
	}

	return true;
}

bool WriteIni::WriteFX_Launch(FX_LAUNCH fxl, bool wLaunch){
	
	std::string sectionlaunch = "FX_LAUNCH";

	SetCurrentSection(sectionlaunch);

	std::string buff;
	if (wLaunch){
		Wint("N", fxl.N);
		Wdouble("Height", fxl.H);
		Wdouble("Angle", fxl.Angle);
		Wdouble("Distance", fxl.Distance);
		Wdouble("CutoffAltitude", fxl.CutoffAltitude);
		W("Pstream1", fxl.Ps1);
		W("Pstream2", fxl.Ps2);
	} else {
		W("N", "");
		W("Height", "");
		W("Angle", "");
		W("Distance", "");
		W("CutoffAltitude", "");
		W("Pstream1", "");
		W("Pstream2", "");
	}

	return true;
}
