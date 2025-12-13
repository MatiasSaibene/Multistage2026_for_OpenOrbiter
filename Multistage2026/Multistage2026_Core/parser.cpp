#include "Multistage2026.hpp"
#include "simpleini/SimpleIni.h"
#include <string>
#include <format>
#include <math.h>
#include <stdio.h>
#include <sstream>
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
// Parser.cpp
// ==============================================================

void Multistage2026::parseInterstages(const std::string filename, int parsingstage) {
	
	oapiWriteLogV("%s: parseInterstages() filename=%s", GetName(), filename.c_str());

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename.c_str()) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename.c_str());
        return;
    }

	int ins = 0;

	std::string intertxt = std::format("SEPARATION_{}{}", parsingstage + 1, parsingstage + 2);

	stage.at(parsingstage).IntIncremental = 0;
	
	if(!ini.GetSection(intertxt.c_str())){
		std::string msg = std::format("No more sections after {}", intertxt);
		oapiWriteLog(const_cast<char *>(msg.c_str()));
		return;
	} else {
		stage.at(parsingstage).interstage.meshname = ini.GetValue(intertxt.c_str(), "meshname", "");
		stage.at(parsingstage).wInter = true;
		stage.at(parsingstage).IntIncremental = ins + 1;
		std::string off_vec = ini.GetValue(intertxt.c_str(), "off", "0,0,0");
		stage.at(parsingstage).interstage.off = CharToVec(off_vec);
		std::string logvec = std::format("[PARSER] Interstage raw offset loaded: {}, {}, {}", stage.at(parsingstage).interstage.off.x, stage.at(parsingstage).interstage.off.y, stage.at(parsingstage).interstage.off.z);
		oapiWriteLog(const_cast<char*>(logvec.c_str()));
		stage.at(parsingstage).interstage.height = std::stof(ini.GetValue(intertxt.c_str(), "height", "0.0"));
		stage.at(parsingstage).interstage.diameter = std::stof(ini.GetValue(intertxt.c_str(), "diameter", "0.0"));
		stage.at(parsingstage).interstage.emptymass = std::stof(ini.GetValue(intertxt.c_str(), "emptymass", "0.0"));
		stage.at(parsingstage).interstage.separation_delay = std::stof(ini.GetValue(intertxt.c_str(), "separation_delay", "0.0"));
		std::string speed_vec = ini.GetValue(intertxt.c_str(), "speed", "0.0");
		stage.at(parsingstage).interstage.speed = CharToVec(speed_vec);
		std::string rot_speed = ini.GetValue(intertxt.c_str(), "rot_speed", "0,0,0");
		stage.at(parsingstage).interstage.rot_speed = CharToVec(rot_speed);
		stage.at(parsingstage).interstage.module = ini.GetValue(intertxt.c_str(), "module", "Stage");

		ins += 1;
		nInterstages = ins;
	}
	
	std::string logbuff = std::format("{}: Number of Interstages in the ini file: {}", GetName(), nInterstages);
	oapiWriteLog(const_cast<char *>(logbuff.c_str()));

	return;
}
void Multistage2026::parseLes(const std::string filename) {
	
	oapiWriteLogV("%s: parseLes() filename=%s", GetName(), filename.c_str());

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename.c_str()) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename.c_str());
        return;
    }

	std::string lestxt = "LES";

	if(!ini.GetSection(lestxt.c_str())){
		std::string msg = std::format("No more sections after {}", lestxt);
		oapiWriteLog(const_cast<char *>(msg.c_str()));
		return;
	} else {
		Les.meshname = ini.GetValue(lestxt.c_str(), "meshname", "");
		wLes = true;
		std::string off_vec = ini.GetValue(lestxt.c_str(), "off", "0,0,0");
		Les.off = CharToVec(off_vec);
		Les.height = std::stof(ini.GetValue(lestxt.c_str(), "height", "0.0"));
		Les.diameter = std::stof(ini.GetValue(lestxt.c_str(), "diameter", "0.0"));
		Les.emptymass = std::stof(ini.GetValue(lestxt.c_str(), "emptymass", "0.0"));
		std::string speed_vec = ini.GetValue(lestxt.c_str(), "speed", "0,0,0");
		Les.speed = CharToVec(speed_vec);
		std::string rotspeed_vec = ini.GetValue(lestxt.c_str(), "rot_speed", "0,0,0");
		Les.rot_speed = CharToVec(rotspeed_vec);
		Les.module = ini.GetValue(lestxt.c_str(), "module", "Stage");

		std::string logbuff = std::format("{}: LES Found", GetName());
		oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		return;
	}

	return;
}

void Multistage2026::parseAdapter(const std::string filename) {

	oapiWriteLogV("%s: parseAdapter() filename=%s", GetName(), filename.c_str());

	std::string intertxt = std::format("SEPARATION_{}{}", nStages, nStages + 1);

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename.c_str()) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename.c_str());
        return;
    }

	std::string meshname = ini.GetValue(intertxt.c_str(), "meshname", "");

	if(meshname.empty()){
		intertxt = "ADAPTER";
		meshname = ini.GetValue(intertxt.c_str(), "meshname", "");
	}

	if(meshname.empty()) return;

	wAdapter = true;
	Adapter.meshname = meshname;

	Adapter.off = CharToVec(ini.GetValue(intertxt.c_str(), "off", "0,0,0"));
    Adapter.height = std::stod(ini.GetValue(intertxt.c_str(), "height", "0"));
    Adapter.diameter = std::stod(ini.GetValue(intertxt.c_str(), "diameter", "0"));
    Adapter.emptymass = std::stod(ini.GetValue(intertxt.c_str(), "emptymass", "0"));

    oapiWriteLogV("%s: Adapter parsed from section [%s]", GetName(), intertxt.c_str());
	
}

void Multistage2026::parseStages(const std::string filename) {

	oapiWriteLogV("%s: parseInterstages() filename=%s", GetName(), filename.c_str());

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename.c_str()) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename.c_str());
        return;
    }

	std::string stagetxt;
	std::string engtxt;

	int i;
	for (i = 0; i <= 10; i++) {

		//check for interstage!
		parseInterstages(filename, i);

		////////STAGE SECTION//////////////////////////////////////////////////////////////

		stagetxt = std::format("STAGE_{}", i + 1);

		stage.at(i).meshname = ini.GetValue(stagetxt.c_str(), "meshname", "");

		std::string off_vec = ini.GetValue(stagetxt.c_str(), "off", "0,0,0");
		stage.at(i).off = CharToVec(off_vec);

		stage.at(i).height = std::stof(ini.GetValue(stagetxt.c_str(), "height", "0.0"));

		stage.at(i).diameter = std::stof(ini.GetValue(stagetxt.c_str(), "diameter", "0.0"));

		stage.at(i).thrust = std::stof(ini.GetValue(stagetxt.c_str(), "thrust", "0.0"));

		stage.at(i).emptymass = std::stof(ini.GetValue(stagetxt.c_str(), "emptymass", "0.0"));

		stage.at(i).fuelmass = std::stof(ini.GetValue(stagetxt.c_str(), "fuelmass", "0.0"));

		stage.at(i).burntime = std::stof(ini.GetValue(stagetxt.c_str(), "burntime", "0.0"));

		stage.at(i).ignite_delay = std::stof(ini.GetValue(stagetxt.c_str(), "ignite_delay", "0.0"));
		stage.at(i).currDelay = std::stof(ini.GetValue(stagetxt.c_str(), "ignite_delay", "0.0"));

		std::string speed_vec = ini.GetValue(stagetxt.c_str(), "speed", "0,0,0");
		stage.at(i).speed = CharToVec(speed_vec);

		std::string rotspeed_vec = ini.GetValue(stagetxt.c_str(), "rot_speed", "0,0,0");
		stage.at(i).rot_speed = CharToVec(rotspeed_vec);

		stage.at(i).module = ini.GetValue(stagetxt.c_str(), "module", "Stage");

		//added by rcraig42 to get values for ispref and pref----------------------------------------------------------------	


		stage.at(i).ispref = std::stof(ini.GetValue(stagetxt.c_str(), "isp_sl", "0.0"));

		stage.at(i).pref = std::stof(ini.GetValue(stagetxt.c_str(), "pressure_sl", "0.0"));

		//--------------------------------------------------------------------------------------------------------------	




		stage.at(i).pitchthrust = 2 * std::stof(ini.GetValue(stagetxt.c_str(), "pitchthrust", "0.0"));

		if(stage.at(i).pitchthrust == 0) {stage.at(i).defpitch = true;}

		stage.at(i).yawthrust = 2 * std::stof(ini.GetValue(stagetxt.c_str(), "yawthrust", "0.0"));
		if(stage.at(i).yawthrust == 0) {stage.at(i).defyaw = true;}

		stage.at(i).rollthrust = 2 * std::stof(ini.GetValue(stagetxt.c_str(), "rollthrust", "0.0"));
		if(stage.at(i).rollthrust == 0) {stage.at(i).defroll = true;}

		stage.at(i).linearthrust = std::stof(ini.GetValue(stagetxt.c_str(), "linearthrust", "0.0"));

		stage.at(i).linearisp = std::stof(ini.GetValue(stagetxt.c_str(), "linearisp", "0.0"));

		//engines///
		int neng;
		for (neng = 0; neng < 32; neng++) {
			std::string engtxt = std::format("ENG_{}", neng + 1);

			std::string value = ini.GetValue(stagetxt.c_str(), engtxt.c_str(), "");

			if(value.empty()){
				break;
			}

			stage.at(i).engV4.at(neng) = CharToVec4(value);

			stage.at(i).eng.at(neng).x = stage.at(i).engV4.at(neng).x;
			stage.at(i).eng.at(neng).y = stage.at(i).engV4.at(neng).y;
			stage.at(i).eng.at(neng).z = stage.at(i).engV4.at(neng).z;
			if ((stage.at(i).engV4.at(neng).t <= 0) || (stage.at(i).engV4.at(neng).t > 10)) { stage.at(i).engV4.at(neng).t = 1; }

			stage.at(i).nEngines = neng + 1;

		}

		stage.at(i).eng_diameter = std::stof(ini.GetValue(stagetxt.c_str(), "eng_diameter", "0"));
		if(stage.at(i).eng_diameter == 0){
			stage.at(i).eng_diameter = 0.5 * stage.at(i).diameter;
		}

		std::string engdir_vec = ini.GetValue(stagetxt.c_str(), "eng_dir", "0,0,0");
		if(engdir_vec.empty()){
			stage.at(i).eng_dir = _V(0, 0, 1);
		} else {
			stage.at(i).eng_dir = CharToVec(engdir_vec);
		}


		stage.at(i).eng_tex = ini.GetValue(stagetxt.c_str(), "eng_tex", "");

		stage.at(i).eng_pstream1 = ini.GetValue(stagetxt.c_str(), "eng_pstream1", "");
		if(stage.at(i).eng_pstream1.empty()){
			stage.at(i).wps1 = false;
		} else {
			stage.at(i).wps1 = true;
		}

		stage.at(i).eng_pstream2 = ini.GetValue(stagetxt.c_str(), "eng_pstream2", "");
		if(stage.at(i).eng_pstream2.empty()){
			stage.at(i).wps2 = false;
		} else {
			stage.at(i).wps2 = true;
		}

		stage[i].ParticlesPackedToEngine = stoi(dataparsed);

		stage.at(i).ParticlesPackedToEngine = std::stoi(ini.GetValue(stagetxt.c_str(), "particles_packed_to_engine", "0"));
		if(stage.at(i).ParticlesPackedToEngine != 0){
			stage.at(1).ParticlesPacked = true;

			oapiWriteLogV("%s: Particles Packed to Engine %i", GetName(), std::abs(stage.at(i).ParticlesPackedToEngine));
		}


		int transfer = std::stoi(ini.GetValue(stagetxt.c_str(), "reignitable", "0"));
		if(transfer == 0){
			stage.at(i).reignitable = false;
		} else {
			stage.at(i).reignitable = true;
		}

		int tboil = std::stoi(ini.GetValue(stagetxt.
		c_str(), "boiloff", "0"));

		if(tboil == 0){
			stage.at(i).wBoiloff = false;
		} else {
			stage.at(i).wBoiloff = true;
		}


		int battery = std::stoi(ini.GetValue(stagetxt.c_str(), "battery", "0"));
		if(battery == 0){
			stage.at(i).batteries.MaxCharge = 12 * 3600;
			stage.at(i).batteries.wBatts = false;
			stage.at(i).batteries.CurrentCharge = stage.at(i).batteries.MaxCharge;
		} else {
			stage.at(i).batteries.MaxCharge = 3600 * battery;
			stage.at(i).batteries.wBatts = true;
			stage.at(i).batteries.CurrentCharge = stage.at(i).batteries.MaxCharge;
		}


		stage.at(i).ullage.thrust = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_thrust", "0.0"));
		if(stage.at(i).ullage.thrust == 0){
			stage.at(i).ullage.wUllage = false;
		} else {
			stage.at(i).ullage.wUllage = true;
		}

		stage.at(i).ullage.anticipation = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_anticipation", "0.0"));

		stage.at(i).ullage.overlap = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_overlap", "0.0"));

		stage.at(i).ullage.N = std::stoi(ini.GetValue(stagetxt.c_str(), "ullage_N", "0.0"));

		stage.at(i).ullage.angle = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_angle", "0.0"));

		stage.at(i).ullage.diameter = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_diameter", "0.0"));

		stage.at(i).ullage.length = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_length", "0.0"));

		if(stage.at(i).ullage.length == 0){
			stage.at(i).ullage.length = 10 * stage.at(i).ullage.diameter;
		}

		std::string ullage_dir = ini.GetValue(stagetxt.c_str(), "ullage_dir", "0.0");
		stage.at(i).ullage.dir = CharToVec(ullage_dir);

		std::string ullagepos_dir = ini.GetValue(stagetxt.c_str(), "ullage_pos", "0,0,0");
		stage.at(i).ullage.pos = CharToVec(ullagepos_dir);

		stage.at(i).ullage.tex = ini.GetValue(stagetxt.c_str(), "ullage_tex", "");

		stage.at(i).ullage.rectfactor = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_rectfactor", "0.0"));

		if(stage.at(i).ullage.rectfactor == 0){ stage.at(i).ullage.rectfactor = 1;}


		std::string expbolts_pos_vec = ini.GetValue(stagetxt.c_str(), "expbolts_pos", "0,0,0");
		if(expbolts_pos_vec.empty()){
			stage.at(i).expbolt.wExpbolt = false;
		} else {
			stage.at(i).expbolt.wExpbolt = true;
			stage.at(i).expbolt.pos = CharToVec(expbolts_pos_vec);
			stage.at(i).expbolt.pstream = ini.GetValue(stagetxt.c_str(), "expbolts_pstream", "");
			stage.at(i).expbolt.dir = _V(0, 0, 1);
			stage.at(i).expbolt.anticipation = std::stof(ini.GetValue(stagetxt.c_str(), "expbolts_anticipation", "0.0"));
			if(stage.at(i).expbolt.anticipation == 0){
				stage.at(i).expbolt.anticipation = 1;
			}
		}

		 if(!ini.GetSection(stagetxt.c_str())){
			nStages = i;
			oapiWriteLogV("%s: Number of stages in the ini file: %i", GetName(), nStages);
			break;
		 }
	}
}


void Multistage2026::parseBoosters(const std::string filename) {

	oapiWriteLogV("%s: parseBoosters() filename=%s", GetName(), filename.c_str());

	std::string boostertxt;

	std::string engtxt;

	std::string curvetxt;

	nBoosters = 0;

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename.c_str()) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename.c_str());
        return;
    }

	std::string buff;
	int cc;

	int b;
	for (b = 0; b < 10; b++) {

		////////BOOSTER SECTION//////////////////////////////////////////////////////////////

		boostertxt = std::format("BOOSTER_{}", b + 1);

		if(!ini.GetSection(boostertxt.c_str())){
			std::string msg = std::format("No more booster sections after {}", boostertxt);
			oapiWriteLog(const_cast<char *>(msg.c_str()));
			break;
		}

		booster.at(b).N = std::stoi(ini.GetValue(boostertxt.c_str(), "N", 0));

		booster.at(b).meshname = ini.GetValue(boostertxt.c_str(), "meshname", "");
		if(booster.at(b).meshname.empty()){
			oapiWriteLogV("Booster %s meshname not defined", boostertxt.c_str());
		}

		std::string off_vec = ini.GetValue(boostertxt.c_str(), "off", "(0,0,0.001)");
		
		booster.at(b).off = CharToVec(off_vec);

		booster.at(b).height = std::stof(ini.GetValue(boostertxt.c_str(), "height", "0.0"));

		booster.at(b).angle = std::stof(ini.GetValue(boostertxt.c_str(), "angle", "0.0"));

		booster.at(b).diameter = std::stof(ini.GetValue(boostertxt.c_str(), "diameter", "0.0"));

		booster.at(b).thrust = std::stof(ini.GetValue(boostertxt.c_str(), "thrust", "0.0"));

		booster.at(b).emptymass = std::stof(ini.GetValue(boostertxt.c_str(), "emptymass", "0.0"));


		booster.at(b).fuelmass = std::stof(ini.GetValue(boostertxt.c_str(), "fuelmass", "0.0"));

		booster.at(b).burntime = std::stof(ini.GetValue(boostertxt.c_str(), "burntime", "0.0"));

		
		booster.at(b).burndelay = std::stof(ini.GetValue(boostertxt.c_str(), "burndelay", "0.0"));

		
		booster.at(b).currDelay = booster.at(b).burndelay;


		std::string speed_vec = ini.GetValue(boostertxt.c_str(), "speed", "(3,0,0)");

		booster.at(b).speed = CharToVec(speed_vec);

		std::string rotspeed_vec = ini.GetValue(boostertxt.c_str(), "rot_speed", "(0,-0.1,0)");

		booster.at(b).rot_speed = CharToVec(rotspeed_vec);


		booster.at(b).module = ini.GetValue(boostertxt.c_str(), "module", "Stage");

		//engines///
		int nbeng;
		for (nbeng = 0; nbeng < 5; nbeng++) {
			std::string key = std::format("ENG_{}", nbeng + 1);

			std::string value = ini.GetValue(boostertxt.c_str(), key.c_str(), "");

			if(value.empty()){
				booster.at(b).nEngines = nbeng;
				break;
			}

			booster[b].eng[nbeng] = CharToVec(value);
    		booster[b].nEngines = nbeng + 1;
		}

		booster.at(b).eng_diameter = std::stof(ini.GetValue(boostertxt.c_str(), "eng_diameter", "0.5"));

		booster.at(b).eng_tex = ini.GetValue(boostertxt.c_str(), "eng_tex", "");
		
		if(booster.at(b).eng_tex.empty()){
			oapiWriteLogV("Booster %s eng_tex is not defined", boostertxt.c_str());
		}


		std::string dir_vec = ini.GetValue(boostertxt.c_str(), "eng_dir", "(0, 0, 1)");

		booster.at(b).eng_dir = CharToVec(dir_vec);

		booster.at(b).eng_pstream1 = ini.GetValue(boostertxt.c_str(), "eng_pstream1", "");
		if(booster.at(b).eng_pstream1.empty()){
			oapiWriteLogV("Booster %s eng_pstream1 not defined", boostertxt.c_str());
			booster.at(b).wps1 = false;
		} else {
			booster.at(b).wps1 = true;
		}

		booster.at(b).eng_pstream2 = ini.GetValue(boostertxt.c_str(), "eng_pstream2", "");
		if(booster.at(b).eng_pstream2.empty()){
			oapiWriteLogV("Booster %s eng_pstream2 not defined", boostertxt.c_str());
			booster.at(b).wps2 = false;
		} else {
			booster.at(b).wps2 = true;
		}


		for (cc = 0; cc < 10; cc++) {

			curvetxt = std::format("CURVE_{}", cc + 1);
			std::string curve_vec = ini.GetValue(boostertxt.c_str(), curvetxt.c_str(), "(9000000,100,0)");

			booster.at(b).curve.at(cc) = CharToVec(curve_vec);

			booster.at(b).curve.at(cc).z = 0;

		}

		std::string expbolts_pos_vec = ini.GetValue(boostertxt.c_str(), "expbolts_pos", "0,0,0");
		if(expbolts_pos_vec.empty()){
			booster.at(b).expbolt.wExpbolt = false;
		} else {
			booster.at(b).expbolt.wExpbolt = true;
			booster.at(b).expbolt.pos = CharToVec(expbolts_pos_vec);
			booster.at(b).expbolt.pstream = ini.GetValue(boostertxt.c_str(), "expbolts_pstream", "");
			booster.at(b).expbolt.dir = _V(0, 0, 1);
			std::string expbolts_anticipation = ini.GetValue(boostertxt.c_str(), "expbolts_anticipation", "0.0");
			if(expbolts_anticipation.empty()){
				booster.at(b).expbolt.anticipation = 1;
			} else {
				booster.at(b).expbolt.anticipation = std::stof(expbolts_anticipation);
			}
		}

		if (booster[b].meshname[0] == '0') {
			nBoosters = b;
			logbuff = std::format("{}: Number of boosters group in the ini file: {}", GetName(), nBoosters);
			oapiWriteLog(const_cast<char *>(logbuff.c_str()));
			break;
		}

	}
}

void Multistage2026::parseFairing(const std::string filename) {

	oapiWriteLogV("%s: parseFairing() filename=%s", GetName(), filename.c_str());

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename.c_str()) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename.c_str());
        return;
    }

	hasFairing = false;

	std::string fairingtxt;


	fairingtxt = "FAIRING";

	fairing.N = std::stoi(ini.GetValue(fairingtxt.c_str(), "N", "0"));

	if(fairing.N != 0) {
		hasFairing = true;
		oapiWriteLogV("%s: This Rocket Has Fairing", GetName());
	}

	fairing.meshname = ini.GetValue(fairingtxt.c_str(), "meshname");

	std::string off_vec = ini.GetValue(fairingtxt.c_str(), "off", "0,0,0");
	fairing.off = CharToVec(off_vec);

	fairing.height = std::stof(ini.GetValue(fairingtxt.c_str(), "height", "0.0"));

	fairing.angle = std::stof(ini.GetValue(fairingtxt.c_str(), "angle", "0.0"));

	fairing.diameter = std::stof(ini.GetValue(fairingtxt.c_str(), "diameter", "0.0"));

	fairing.emptymass = std::stof(ini.GetValue(fairingtxt.c_str(), "emptymass", "0.0"));

	std::string speed_vec = ini.GetValue(fairingtxt.c_str(), "speed", "0,0,0");

	if(speed_vec.empty()){
		speed_vec = "(0,-3,0)";
	}

	fairing.speed = CharToVec(speed_vec);

	std::string rotspeed_vec = ini.GetValue(fairingtxt.c_str(), "rot_speed", "0,0,0");

	fairing.rot_speed = CharToVec(rotspeed_vec);

	fairing.module = ini.GetValue(fairingtxt.c_str(), "module", "Stage");

}

void Multistage2026::ArrangePayloadMeshes(const std::string data, int pnl) {
	
	std::string meshnm(data);

	std::string meshnm0, meshnm1, meshnm2, meshnm3, meshnm4;
	std::size_t findFirstSC = meshnm.find_first_of(";");
	if (findFirstSC != meshnm.npos) {
		meshnm0 = meshnm.substr(0, findFirstSC);
		payload.at(pnl).meshname0 = meshnm0;
		std::size_t findSecondSC = meshnm.find_first_of(";", findFirstSC + 1);
		if (findSecondSC != meshnm.npos) {
			meshnm1 = meshnm.substr(findFirstSC + 1, findSecondSC - findFirstSC - 1);
			std::size_t findThirdSC = meshnm.find_first_of(";", findSecondSC + 1);
			if (findThirdSC != meshnm.npos) {
				meshnm2 = meshnm.substr(findSecondSC + 1, findThirdSC - findSecondSC - 1);
				std::size_t findFourthSC = meshnm.find_first_of(";", findThirdSC + 1);
				if (findFourthSC != meshnm.npos) {
					meshnm3 = meshnm.substr(findThirdSC + 1, findFourthSC - findThirdSC - 1);
					meshnm4 = meshnm.substr(findFourthSC + 1, meshnm.npos);
					payload.at(pnl).nMeshes = 5;
				}
				else { meshnm3 = meshnm.substr(findThirdSC + 1, meshnm.npos); payload.at(pnl).nMeshes = 4; }
			}
			else { meshnm2 = meshnm.substr(findSecondSC + 1, meshnm.npos); payload.at(pnl).nMeshes = 3; }
		}
		else { meshnm1 = meshnm.substr(findFirstSC + 1, meshnm.npos); payload.at(pnl).nMeshes = 2; }
	}
	else { meshnm0 = meshnm.substr(0, meshnm.npos); payload.at(pnl).nMeshes = 1; }

	payload.at(pnl).meshname0 = meshnm0;

	if (payload.at(pnl).nMeshes == 5) {
		payload.at(pnl).meshname1 = meshnm1;
		payload.at(pnl).meshname2 = meshnm2;
		payload.at(pnl).meshname3 = meshnm3;
		payload.at(pnl).meshname4 = meshnm4;
	}
	else if (payload.at(pnl).nMeshes == 4) {
		payload.at(pnl).meshname1 = meshnm1;
		payload.at(pnl).meshname2 = meshnm2;
		payload.at(pnl).meshname3 = meshnm3;
	}
	else if (payload.at(pnl).nMeshes == 3) {
		payload.at(pnl).meshname1 = meshnm1;
		payload.at(pnl).meshname2 = meshnm2;
	}
	else if (payload.at(pnl).nMeshes == 2) {
		payload.at(pnl).meshname1 = meshnm1;
	}

}

void Multistage2026::ArrangePayloadOffsets(const std::string &data, int pnl) {
    std::stringstream ss(data);
    std::string token;
    int idx = 0;

    while (std::getline(ss, token, ';') && idx < payload[pnl].nMeshes) {
        token.erase(std::remove(token.begin(), token.end(), ' '), token.end());
        payload[pnl].off.at(idx++) = CharToVec(token);
    }
}

void Multistage2026::parsePayload(const std::string filename) {

	oapiWriteLogV("%s: parsePayload() filename=%s", GetName(), filename.c_str());

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename.c_str()) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename.c_str());
        return;
    }
	
	std::string payloadtxt;

	int pnl;

	for (pnl = 0; pnl <= 10; pnl++) {

		payloadtxt = std::format("PAYLOAD_{}", pnl + 1);

		payload.at(pnl).nMeshes = 0;

		if(!ini.GetSection(payloadtxt.c_str())){
			nPayloads = pnl;
			oapiWriteLogV("%s: Number of Payloads in the ini file: %i", GetName(), nPayloads);
			break;
		}

		payload.at(pnl).meshname = ini.GetValue(payloadtxt.c_str(), "meshname", "");

		std::string meshlist = ini.GetValue(payloadtxt.c_str(), "meshname", "");

		if(!meshlist.empty()){
			ArrangePayloadMeshes(meshlist.data(), pnl);
		} else {
			payload.at(pnl).nMeshes = 0;
		}


		ArrangePayloadOffsets(meshlist, pnl);

		payload.at(pnl).height = std::stof(ini.GetValue(payloadtxt.c_str(), "height", "0.0"));

		payload.at(pnl).diameter = std::stof(ini.GetValue(payloadtxt.c_str(), "diameter", "0.0"));

		payload.at(pnl).mass = std::stof(ini.GetValue(payloadtxt.c_str(), "mass", "0.0"));

		payload.at(pnl).module = ini.GetValue(payloadtxt.c_str(), "module", "Stage");

		payload.at(pnl).name = ini.GetValue(payloadtxt.c_str(), "name", "");

		std::string speed_vec = ini.GetValue(payloadtxt.c_str(), "speed", "0,0,0");
		payload.at(pnl).speed = CharToVec(speed_vec);

		std::string rotspeed_vec = ini.GetValue(payloadtxt.c_str(), "rot_speed", "0,0,0");
		payload.at(pnl).rot_speed = CharToVec(rotspeed_vec);

		std::string rotation_vec = ini.GetValue(payloadtxt.c_str(), "rotation", "0,0,0");
		payload.at(pnl).Rotation = _V(0, 0, 0);
		payload.at(pnl).rotated = false;

		payload.at(pnl).Rotation = operator*(CharToVec(rotation_vec), RAD);

		if(length(payload.at(pnl).Rotation) > 0) {payload.at(pnl).rotated = true;}


		payload.at(pnl).render = std::stoi(ini.GetValue(payloadtxt.c_str(), "render", "0"));
		if (payload.at(pnl).render != 1) {
			payload.at(pnl).render = 0;
		}
		
		int check = std::stoi(ini.GetValue(payloadtxt.c_str(), "live", "0"));
		if (check == 1) {
			payload.at(pnl).live = true;
		} else {
			payload.at(pnl).live = false;
		}

	}


}

void Multistage2026::parseParticle(const std::string filename) {

	oapiWriteLogV("%s: parseParticle() filename=%s", GetName(), filename.c_str());

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename.c_str()) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename.c_str());
        return;
    }

	std::string partxt;
	std::string dataparsed;

	int npart;
	for (npart = 0; npart < 13; npart++) {

		partxt = std::format("PARTICLESTREAM_{}", npart + 1);

		dataparsed = ini.GetValue(partxt.c_str(), "name", "");
		if(dataparsed.empty()){
			nParticles = npart;
			break;
		}
		Particle.at(npart).ParticleName = dataparsed;

		dataparsed = ini.GetValue(partxt.c_str(), "srcsize", "0.0");
		Particle.at(npart).Pss.srcsize = stof(dataparsed);

		dataparsed = ini.GetValue(partxt.c_str(), "srcrate", "0.0");
		Particle.at(npart).Pss.srcrate = stof(dataparsed);

		dataparsed = ini.GetValue(partxt.c_str(), "V0", "0.0");
		Particle.at(npart).Pss.v0 = stof(dataparsed);

		dataparsed = ini.GetValue(partxt.c_str(), "srcspread", "0.0");
		Particle.at(npart).Pss.srcspread = stof(dataparsed);

		dataparsed = ini.GetValue(partxt.c_str(), "lifetime", "0.0");
		Particle.at(npart).Pss.lifetime = stof(dataparsed);

		dataparsed = ini.GetValue(partxt.c_str(), "growthrate", "0.0");
		Particle.at(npart).Pss.growthrate = stof(dataparsed);

		dataparsed = ini.GetValue(partxt.c_str(), "atmslowdown", "0.0");
		Particle.at(npart).Pss.atmslowdown = stof(dataparsed);

		dataparsed = ini.GetValue(partxt.c_str(), "ltype", "");
		if(dataparsed == "EMISSIVE"){
			Particle.at(npart).Pss.ltype = PARTICLESTREAMSPEC::EMISSIVE;
		} else if (dataparsed == "DIFFUSE"){
			Particle.at(npart).Pss.ltype = PARTICLESTREAMSPEC::DIFFUSE;
		} else {
			oapiWriteLogV("%s: WARNING! PARTICLE STREAM LTYPE NOT DEFINED! USING EMISSIVE", GetName());
			Particle.at(npart).Pss.ltype = PARTICLESTREAMSPEC::EMISSIVE;
		}

		dataparsed = ini.GetValue(partxt.c_str(), "levelmap", "");
		if (dataparsed == "LVL_LIN"){
			Particle.at(npart).Pss.levelmap = PARTICLESTREAMSPEC::LVL_LIN;
		} else if (dataparsed == "LVL_FLAT"){
			Particle.at(npart).Pss.levelmap = PARTICLESTREAMSPEC::LVL_FLAT;
		} else if (dataparsed == "LVL_SQRT"){
			Particle.at(npart).Pss.levelmap = PARTICLESTREAMSPEC::LVL_SQRT;
		} else if (dataparsed == "LVL_PLIN"){
			Particle.at(npart).Pss.levelmap = PARTICLESTREAMSPEC::LVL_PLIN;
		} else if (dataparsed == "LVL_PSQRT"){
			Particle.at(npart).Pss.levelmap = PARTICLESTREAMSPEC::LVL_PSQRT;
		} else {
			oapiWriteLogV("%s: WARNING! PARTICLE STREAM LEVEL MAP NOT DEFINED! USING LVL_LIN", GetName());
			Particle.at(npart).Pss.levelmap = PARTICLESTREAMSPEC::LVL_LIN;
		}

		dataparsed = ini.GetValue(partxt.c_str(), "lmin", "0.0");
		Particle.at(npart).Pss.lmin = stof(dataparsed);
		dataparsed = ini.GetValue(partxt.c_str(), "lmax", "0.0");
		Particle.at(npart).Pss.lmax = stof(dataparsed);
		dataparsed = ini.GetValue(partxt.c_str(), "atmsmap", "");
		if (dataparsed == "ATM_FLAT"){
			Particle.at(npart).Pss.atmsmap = PARTICLESTREAMSPEC::ATM_FLAT;
		} else if (dataparsed == "ATM_PLIN"){
			Particle.at(npart).Pss.atmsmap = PARTICLESTREAMSPEC::ATM_PLIN;
		} else if (dataparsed == "ATM_PLOG"){
			Particle.at(npart).Pss.atmsmap = PARTICLESTREAMSPEC::ATM_PLOG;
		} else {
			oapiWriteLogV("%s: WARNING PARTICLE ATMOSFERIC MAP NOT DEFINED, USED PLIN", GetName());
			Particle.at(npart).Pss.atmsmap = PARTICLESTREAMSPEC::ATM_PLIN;
		}

		dataparsed = ini.GetValue(partxt.c_str(), "amin", "0.0");
		Particle.at(npart).Pss.amin = stof(dataparsed);
		dataparsed = ini.GetValue(partxt.c_str(), "amax", "0.0");
		Particle.at(npart).Pss.amax = stof(dataparsed);

		dataparsed = ini.GetValue(partxt.c_str(), "tex", "");
		if(dataparsed.empty()){
			dataparsed = "Contrail3";
			oapiWriteLogV("%s: WARINING PARTICLE TEXTURE NOT DEFINED, USED CONTRAIL3", GetName());
		}

		Particle.at(npart).Pss.tex = oapiRegisterExhaustTexture(const_cast<char *>(dataparsed.c_str()));

		Particle.at(npart).TexName = dataparsed;

		dataparsed = ini.GetValue(partxt.c_str(), "GrowFactor_size", "0.0");
		Particle.at(npart).GrowFactor_size = stof(dataparsed);

		dataparsed = ini.GetValue(partxt.c_str(), "GrowFactor_rate", "0.0");
		Particle.at(npart).GrowFactor_rate = stof(dataparsed);

		if((Particle.at(npart).GrowFactor_rate == 0) && (Particle.at(npart).GrowFactor_size == 0)){
			Particle.at(npart).Growing = false;
		} else {
			Particle.at(npart).Growing = true;
		}
	}
}

void Multistage2026::parseFXMach(const std::string filename){

	oapiWriteLogV("%s: parseFXMach() filename=%s", GetName(), filename.c_str());

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename.c_str()) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename.c_str());
        return;
    }

	std::string fxmtxt = "FX_MACH";

	FX_Mach.pstream = ini.GetValue(fxmtxt.c_str(), "pstream", "");
	if(FX_Mach.pstream.empty()){
		wMach = false;
	} else {
		wMach = true;
	}

	
	FX_Mach.mach_min = std::stof(ini.GetValue(fxmtxt.c_str(), "mach_min", "0.0"));

	FX_Mach.mach_max = std::stof(ini.GetValue(fxmtxt.c_str(), "mach_max", "0.0"));

	for (int nmach = 0; nmach < 10; nmach++){
		std::string txtbuff = std::format("off_{}", nmach + 1);
		
		std::string fxmachoff_vec = ini.GetValue(fxmtxt.c_str(), txtbuff.c_str(), "0,0,0");

		FX_Mach.off.at(nmach) = CharToVec(fxmachoff_vec);
		if(fxmachoff_vec.empty()){
			FX_Mach.nmach = nmach;
			break;
		}
	}
	
	std::string fxmachdir_vec = ini.GetValue(fxmtxt.c_str(), "dir", "0,0,0");
	FX_Mach.dir = CharToVec(fxmachdir_vec);
	FX_Mach.added = false;

}

void Multistage2026::parseFXVent(const std::string filename)
{
	oapiWriteLogV("%s: parseFXVent() filename=%s", GetName(), filename.c_str());

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename.c_str()) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename.c_str());
        return;
    }

	std::string fxvtxt = "FX_VENT";
	std::string itemtxt;
	std::string numtxt;

	FX_Vent.pstream = ini.GetValue(fxvtxt.c_str(), "pstream", "");
	if(FX_Vent.pstream.empty()){
		wVent = false;
	} else {
		wVent = true;
	}


	FX_Vent.nVent = 0;
	for (int fv = 1; fv <= 10; fv++)
	{
		FX_Vent.added.at(fv) = false;

		itemtxt = std::format("off_{}", fv);

		std::string off_vec = ini.GetValue(fxvtxt.c_str(), itemtxt.c_str(), "0,0,0");
		FX_Vent.off.at(fv) = CharToVec(off_vec);

		itemtxt = std::format("dir_{}", fv);
		std::string dir_vec = ini.GetValue(fxvtxt.c_str(), itemtxt.c_str(), "0,0,0");

		if(dir_vec.empty()){
			FX_Vent.nVent = fv - 1;
			break;
		}
		FX_Vent.dir.at(fv) = CharToVec(dir_vec);

		itemtxt = std::format("time_fin_{}", fv);
		FX_Vent.time_fin.at(fv) = 0;

		FX_Vent.time_fin.at(fv) = std::stof(ini.GetValue(fxvtxt.c_str(), itemtxt.c_str(), "0.0"));
	}
}

void Multistage2026::parseFXLaunch(const std::string filename)
{
	char fxLtxt[128];
	sprintf(fxLtxt, "FX_LAUNCH");

	GetPrivateProfileString(fxLtxt, "N", buffreset, dataparsed, MAXLEN, filename);
	FX_Launch.N = atoi(dataparsed);
	GetPrivateProfileString(fxLtxt, "Height", buffreset, dataparsed, MAXLEN, filename);
	FX_Launch.H = atof(dataparsed);
	GetPrivateProfileString(fxLtxt, "Angle", buffreset, dataparsed, MAXLEN, filename);
	FX_Launch.Angle = atof(dataparsed);
	GetPrivateProfileString(fxLtxt, "Distance", buffreset, dataparsed, MAXLEN, filename);
	FX_Launch.Distance = atof(dataparsed);
	GetPrivateProfileString(fxLtxt, "CutoffAltitude", buffreset, dataparsed, MAXLEN, filename);
	FX_Launch.CutoffAltitude = atof(dataparsed);
	GetPrivateProfileString(fxLtxt, "pstream1", buffreset, dataparsed, MAXLEN, filename);
	sprintf(FX_Launch.Ps1, dataparsed);
	GetPrivateProfileString(fxLtxt, "pstream2", buffreset, dataparsed, MAXLEN, filename);
	sprintf(FX_Launch.Ps2, dataparsed);

	if (FX_Launch.N >= 1) { wLaunchFX = TRUE; }
}
void Multistage2026::parseTexture(const std::string filename) {
	char textxt[128];
	char bufftxt[128];
	sprintf(textxt, "TEXTURE_LIST");

	int texn;
	for (texn = 0; texn < 16; texn++) {
		sprintf(bufftxt, "TEX_%i", texn + 1);

		GetPrivateProfileString(textxt, bufftxt, buffreset, dataparsed, MAXLEN, filename);


		if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { nTextures = texn; break; }
		char checktxt[MAXLEN];
		int k;
		for (k = 0; k < MAXLEN; k++) {
			tex.TextureName[k][texn] = dataparsed[k];
			checktxt[k] = dataparsed[k];
		}

		sprintf(logbuff, "%s: Texture n.%i Loaded %s", GetName(), texn + 1, checktxt);
		oapiWriteLog(logbuff);
		tex.hTex[texn] = oapiRegisterExhaustTexture(checktxt);
	}
	return;
}
void Multistage2026::parseMisc(const std::string filename) {
	char Misctxt[64];
	sprintf(Misctxt, "MISC");

	GetPrivateProfileString(Misctxt, "COG", buffreset, dataparsed, MAXLEN, filename);
	Misc.COG = atof(dataparsed);
	GetPrivateProfileString(Misctxt, "GNC_DEBUG", buffreset, dataparsed, MAXLEN, filename);
	Misc.GNC_Debug = atoi(dataparsed);
	int tval;
	GetPrivateProfileString(Misctxt, "TELEMETRY", buffreset, dataparsed, MAXLEN, filename);
	tval = atoi(dataparsed);
	if (tval == 1) { Misc.telemetry = TRUE; }
	else { Misc.telemetry = FALSE; }
	GetPrivateProfileString(Misctxt, "FOCUS", buffreset, dataparsed, MAXLEN, filename);
	Misc.Focus = atoi(dataparsed);
	int trp;
	GetPrivateProfileString(Misctxt, "THRUST_REAL_POS", buffreset, dataparsed, MAXLEN, filename);
	trp = atoi(dataparsed);
	if (trp == 1) { Misc.thrustrealpos = TRUE; sprintf(logbuff, "%s: Thrust in Real Position", GetName()); oapiWriteLog(logbuff); }
	else { Misc.thrustrealpos = FALSE; }
	Misc.VerticalAngle = 0;
	GetPrivateProfileString(Misctxt, "VERTICAL_ANGLE", buffreset, dataparsed, MAXLEN, filename);
	Misc.VerticalAngle = atof(dataparsed) * RAD;

	//added by rcraig42 to retrieve drag_factor from ini --------------------------------------------------------

	GetPrivateProfileString(Misctxt, "drag_factor", buffreset, dataparsed, MAXLEN, filename);
	Misc.drag_factor = atof(dataparsed);

	//------------------------------------------------------------------------------------------------------------
	GetPrivateProfileString(Misctxt, "PAD_MODULE", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(Misc.PadModule, "EmptyModule"); }
	else { sprintf(Misc.PadModule, dataparsed); }


	return;
}


bool Multistage2026::parseinifile(const std::string filename) {
	int r;

	parseStages(filename);
	parseBoosters(filename);
	//parseInterstages(filename);
	parsePayload(filename);
	parseLes(filename);
	parseFairing(filename);
	parseMisc(filename);
	parseTexture(filename);
	parseParticle(filename);
	parseFXMach(filename);
	parseFXVent(filename);
	parseFXLaunch(filename);
	parseAdapter(filename);
	//parseSound(filename);
	return true;
}

void Multistage2026::parseTelemetryFile(const std::string filename)
{
	char filebuff[MAXLEN];
	sprintf(filebuff, OrbiterRoot);
	strcat(filebuff, "\\");
	strcat(filebuff, filename);
	string line;
	ifstream tlm_file(filebuff);

	loadedtlmlines = 0;
	if (tlm_file.is_open())
	{

		while (getline(tlm_file, line))
		{
			if (!line.empty()) {
				const char* cstr = line.c_str();

				sscanf(cstr, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &ReftlmAlt[loadedtlmlines].x, &ReftlmAlt[loadedtlmlines].y, &ReftlmSpeed[loadedtlmlines].y, &ReftlmPitch[loadedtlmlines].y, &ReftlmThrust[loadedtlmlines].y, &ReftlmMass[loadedtlmlines].y, &ReftlmVv[loadedtlmlines].y, &ReftlmAcc[loadedtlmlines].y);
				if (ReftlmAlt[loadedtlmlines].x == 0) { continue; }
				ReftlmSpeed[loadedtlmlines].x = ReftlmAlt[loadedtlmlines].x;
				ReftlmPitch[loadedtlmlines].x = ReftlmAlt[loadedtlmlines].x;
				ReftlmThrust[loadedtlmlines].x = ReftlmAlt[loadedtlmlines].x;
				ReftlmMass[loadedtlmlines].x = ReftlmAlt[loadedtlmlines].x;
				ReftlmVv[loadedtlmlines].x = ReftlmAlt[loadedtlmlines].x;
				ReftlmAcc[loadedtlmlines].x = ReftlmAlt[loadedtlmlines].x;

				loadedtlmlines++;
			}
		}
	}
}

void Multistage2026::parseGuidanceFile(const std::string filename) {
	nsteps = 1;


	char filebuff[MAXLEN];
	string line;
	sprintf(filebuff, OrbiterRoot);
	strcat(filebuff, "\\");
	strcat(filebuff, guidancefile);//Config\\multistage\\guidance_shuttle.txt");

	sprintf(logbuff, "%s: Guidance File present: %s", GetName(), guidancefile);
	oapiWriteLog(logbuff);
	ifstream gnc_file(filebuff);
	if (gnc_file.is_open())
	{

		while (getline(gnc_file, line))

		{
			if (!line.empty()) {

				for (int i = 0; i < 6; i++) { Gnc_step[nsteps].wValue[i] = FALSE; }

				std::size_t findEqual = line.find_first_of("=");
				if (findEqual != line.npos) {
					string mettime = line.substr(0, findEqual);

					Gnc_step[nsteps].time = atof(&mettime[0]);

					std::size_t findLineEnd = line.find_first_of(")");
					if (findLineEnd != line.npos) {

						std::size_t findOpenP = line.find_first_of("(");
						if (findOpenP != line.npos) {
							string comand = line.substr(findEqual + 1, findOpenP - findEqual - 1);
							sprintf(Gnc_step[nsteps].Comand, &comand[0]);

							string values = line.substr(findOpenP + 1, findLineEnd - findOpenP - 1);
							Gnc_step[nsteps].wValue[0] = TRUE;
							std::size_t findFirstComma = values.find_first_of(",");
							if (findFirstComma != values.npos) {
								value1 = values.substr(0, findFirstComma);
								Gnc_step[nsteps].wValue[1] = TRUE;


								std::size_t findSecondComma = values.find_first_of(",", findFirstComma + 1);

								if (findSecondComma != values.npos) {

									value2 = values.substr(findFirstComma + 1, findSecondComma - findFirstComma - 1);
									Gnc_step[nsteps].wValue[2] = TRUE;

									std::size_t findThirdComma = values.find_first_of(",", findSecondComma + 1);

									if (findThirdComma != values.npos) {

										value3 = values.substr(findSecondComma + 1, findThirdComma - findSecondComma - 1);
										Gnc_step[nsteps].wValue[3] = TRUE;

										std::size_t findFourthComma = values.find_first_of(",", findThirdComma + 1);
										if (findFourthComma != values.npos) {
											value4 = values.substr(findThirdComma + 1, findFourthComma - findThirdComma - 1);
											Gnc_step[nsteps].wValue[4] = TRUE;
											std::size_t findFifthComma = values.find_first_of(",", findFourthComma + 1);
											if (findFifthComma != values.npos) {
												value5 = values.substr(findFourthComma + 1, findFifthComma - findFourthComma - 1);
												value6 = values.substr(findFifthComma + 1, string::npos);
												Gnc_step[nsteps].wValue[5] = TRUE;
											}
											else { value5 = values.substr(findFourthComma + 1, string::npos); }
										}
										else { value4 = values.substr(findThirdComma + 1, values.npos); }
									}
									else { value3 = values.substr(findSecondComma + 1, values.npos); }
								}
								else { value2 = values.substr(findFirstComma + 1, values.npos); }
							}
							else { value1 = values.substr(0, values.npos); }
						}
					}
					if (Gnc_step[nsteps].wValue[0]) { Gnc_step[nsteps].trval1 = atof(&value1[0]); }
					else { Gnc_step[nsteps].trval1 = 0; }
					if (Gnc_step[nsteps].wValue[1]) { Gnc_step[nsteps].trval2 = atof(&value2[0]); }
					else { Gnc_step[nsteps].trval2 = 0; }
					if (Gnc_step[nsteps].wValue[2]) { Gnc_step[nsteps].trval3 = atof(&value3[0]); }
					else { Gnc_step[nsteps].trval3 = 0; }
					if (Gnc_step[nsteps].wValue[3]) { Gnc_step[nsteps].trval4 = atof(&value4[0]); }
					else { Gnc_step[nsteps].trval4 = 0; }
					if (Gnc_step[nsteps].wValue[4]) { Gnc_step[nsteps].trval5 = atof(&value5[0]); }
					else { Gnc_step[nsteps].trval5 = 0; }
					if (Gnc_step[nsteps].wValue[5]) { Gnc_step[nsteps].trval6 = atof(&value6[0]); }
					else { Gnc_step[nsteps].trval6 = 0; }

				}
				else {
					sprintf(Gnc_step[nsteps].Comand, "noline");
					Gnc_step[nsteps].GNC_Comand = CM_NOLINE;
					Gnc_step[nsteps].time_fin = -10000;
					Gnc_step[nsteps].trval1 = 0;
					Gnc_step[nsteps].trval2 = 0;
					Gnc_step[nsteps].trval3 = 0;
					Gnc_step[nsteps].trval4 = 0;
					Gnc_step[nsteps].trval5 = 0;
					Gnc_step[nsteps].trval6 = 0;
				}


				for (int l = 0; l < line.size(); l++) {
					line[l] = tolower(line[l]);
				}

				std::size_t foundDisable = line.find("disable");
				std::size_t foundPlay = line.find("playsound");

				if (foundDisable != std::string::npos) {
					std::size_t foundDisPitch = line.find("pitch");
					std::size_t foundDisRoll = line.find("roll");
					std::size_t foundDisJett = line.find("jettison");

					if (foundDisPitch != std::string::npos) {
						sprintf(Gnc_step[nsteps].Comand, "disablepitch");
					}
					else if (foundDisRoll != std::string::npos) {
						sprintf(Gnc_step[nsteps].Comand, "disableroll");
					}
					else if (foundDisJett != std::string::npos) {
						sprintf(Gnc_step[nsteps].Comand, "disablejettison");
					}

				}
				else if (foundPlay != std::string::npos) {
					std::size_t findopen = line.find_first_of("(");
					std::size_t findclose = line.find_first_of(")");
					string filename = line.substr(findopen + 1, findclose - findopen - 1);
					sprintf(Gnc_step[nsteps].Comand, "playsound");
					filename.copy(Gnc_step[nsteps].trchar, MAXLEN, 0);
				}




				++nsteps;
			}
			else { continue; }


		}

		nsteps -= 1;

		gnc_file.close();
	}

	VinkaComposeGNCSteps();
	VinkaRearrangeSteps();
	nsteps = VinkaCountSteps();
	return;
}
