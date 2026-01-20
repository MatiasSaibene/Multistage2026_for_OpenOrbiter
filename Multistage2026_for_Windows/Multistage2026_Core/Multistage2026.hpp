#pragma once
#include <array>
#include <string>
#ifndef __MULTISTAGE2026_HPP_
#define __MULTISTAGE2026_HPP_

#include "../../include/Orbitersdk.h"
#include "resource.h"

constexpr int MSVERSION{2026};

constexpr int TBOOSTER{0};
constexpr int TSTAGE{1};
constexpr int TPAYLOAD{2};
constexpr int TFAIRING{3};
constexpr int TINTERSTAGE{4};
constexpr int TLES{5};

constexpr int STAGE_SHUTDOWN{1};
constexpr int STAGE_IGNITED{2};
constexpr int STAGE_WAITING{3};

constexpr int FXMACH{0};
constexpr int FXVENT{1};

constexpr int TLMSECS{7200};

struct VECTOR4F {
	double x;
	double y;
	double z;
	double t;
	VECTOR4F(){
		x = 0.0;
		y = 0.0;
		z = 0.0;
		t = 0.0;
	}
};

struct VECTOR2 {
	double x;
	double y;
};

struct CUTOFF_CONDITIONS {
	double T;
	int stage;
	int rem_stages;
	double r_T;
	double rdot_T;
    std::array<double, 10> remBT;
    CUTOFF_CONDITIONS(){
        T = 0.0;
        stage = 0;
        rem_stages = 0;
        r_T = 0.0;
        rdot_T = 0.0;
        remBT.fill(0.0);
    }
};

struct BATTS {
	bool wBatts;
	double MaxCharge;
	double CurrentCharge;
	BATTS(){
		wBatts = false;
		MaxCharge = 0.0;
		CurrentCharge = 0.0;
	}
};

struct EXPBOLT {
	VECTOR3 pos;
	VECTOR3 dir;
	std::string pstream;
	double anticipation;
	bool wExpbolt;
	THRUSTER_HANDLE threxp_h;
	EXPBOLT(){
		threxp_h = nullptr;
		pos = _V(0, 0, 0);
		dir = _V(0, 0, 0);
		anticipation = 0;
		wExpbolt = false;
	}
};

struct MISC {
	double COG;
	int GNC_Debug;
	bool telemetry;
	int Focus;
	bool thrustrealpos;
	double VerticalAngle;

	//added by rcraig42 to store drag_factor -----------------------------------------------

	double drag_factor;
	std::string PadModule;
	//---------------------------------------------------------------------------------------
	MISC(){
		COG = 0;
		GNC_Debug = 0;
		telemetry = false;
		Focus = 0;
		thrustrealpos = false;
		VerticalAngle = 0;
		drag_factor = 0;
	}
};

struct TEX {
    std::array<std::string, 16> TextureName;
	std::array<SURFHANDLE, 16> hTex;
};

struct PARTICLE {
	std::string ParticleName;
	PARTICLESTREAMSPEC Pss;
	double GrowFactor_size;
	double GrowFactor_rate;
	bool Growing;
	std::string TexName;
	PARTICLE(){
		GrowFactor_size = 0.0;
		GrowFactor_rate = 0.0;
		Growing = false;
		Pss.amax = 0;
		Pss.amin = 0;
		Pss.atmslowdown = 0;
		Pss.atmsmap = PARTICLESTREAMSPEC::ATM_FLAT;
		Pss.growthrate = 0;
		Pss.levelmap = PARTICLESTREAMSPEC::LVL_FLAT;
		Pss.lifetime = 0;
		Pss.lmax = 0;
		Pss.lmin = 0;
		Pss.ltype = PARTICLESTREAMSPEC::DIFFUSE;
		Pss.srcrate = 0;
		Pss.srcsize = 0;
		Pss.srcspread = 0;
		Pss.tex = nullptr;
		Pss.v0 = 0;
	}

};

struct PSGROWING {
    std::array<PSTREAM_HANDLE, 3> psh;
	PARTICLESTREAMSPEC pss;
	double GrowFactor_size;
	double GrowFactor_rate;
	THRUSTER_HANDLE th;
	VECTOR3 pos;
	bool growing;
	int status;
	bool counting;
	double doublepstime;
	double basesize;
	double baserate;
	VECTOR3 basepos;
	bool ToBooster;
	int nItem;
	int nEngine;
	bool FirstLoop;
	//int Th_idx;
	PSGROWING()
        : psh{nullptr, nullptr, nullptr},
          pss{0},
          GrowFactor_size(0),
          GrowFactor_rate(0),
          th(nullptr),
          pos(_V(0, 0, 0)),
          growing(false),
          status(0),
          counting(false),
          doublepstime(0),
          basesize(0),
          baserate(0),
          basepos(_V(0, 0, 0)),
          ToBooster(false),
          nItem(0),
          nEngine(0),
          FirstLoop(true)
    {}
};

struct BOOSTER {
	//mandatory
	int N;
	std::string meshname;
	VECTOR3 off;
	double height;
	double diameter;
	double thrust;
	double emptymass;
	double fuelmass;
	double burntime;
	double angle;
	//unmandatory
	double burndelay;
	VECTOR3 speed;
	VECTOR3 rot_speed;
	std::string module;
    std::array<VECTOR3, 4> eng;
    std::array<double, 32> engine_phase;
	std::array<double, 32> engine_amp;
	std::array<double, 32> freq;
	double eng_diameter;
	std::string eng_tex;
	std::string eng_pstream1;
	bool wps1;
	std::string eng_pstream2;
	bool wps2;
	VECTOR3 eng_dir;
	THGROUP_HANDLE Thg_boosters_h;
	EXPBOLT expbolt;
	//derived parameters
	double isp;
	double volume;
	PROPELLANT_HANDLE tank;
	std::array<THRUSTER_HANDLE, 10> th_booster_h;
	std::array<MESHHANDLE, 10> msh_h;
	std::array<int, 10> msh_idh;
	int nEngines;
	double currDelay;
	std::array<VECTOR3, 10> curve;
	double IgnitionTime;
	bool Ignited;
	bool PSSdefined;
	bool ParticlesPacked;
	int ParticlesPackedToEngine;
	BOOSTER(){
		isp = 0.0;
		currDelay = 0.0;
		Thg_boosters_h = nullptr;
		IgnitionTime = 0.0;
		volume = 0.0;
		th_booster_h.fill(nullptr);
		tank = nullptr;
		msh_idh.fill(0);
		msh_h.fill({});
		isp = 0.0;
		currDelay = 0.0;
		Thg_boosters_h = nullptr;
		IgnitionTime = 0.0;
		N = 0;
		wps1 = false;
		wps2 = false;
		off = _V(0, 0, 0);
		height = 0;
		diameter = 0;
		thrust = 0;
		emptymass = 0;
		fuelmass = 0;
		burntime = 0;
		angle = 0;
		burndelay = 0;
		speed = _V(0, 0, 0);
		rot_speed = _V(0, 0, 0);
        eng.fill(_V(0, 0, 0));
		engine_phase.fill(0.0);
		engine_amp.fill(0.0);
		freq.fill(0.0);
		eng_diameter = 0.0;
		eng_dir = _V(0, 0, 0);
		Ignited = false;
		PSSdefined = false;
		nEngines = 0;
		curve.fill(_V(9000000, 100, 0));
		ParticlesPacked = false;
		ParticlesPackedToEngine = 0;
		expbolt = EXPBOLT();
	}
};

struct INTERSTAGE {
	//mandatory
	std::string meshname;
	VECTOR3 off;
	double height;
	double diameter;
	double volume;
	double emptymass;
	//unmandatory
	double separation_delay;
	VECTOR3 speed;
	VECTOR3 rot_speed;
	std::string module;
	MESHHANDLE msh_h;
	int msh_idh;
	double currDelay;
	INTERSTAGE(){
		off = _V(0, 0, 0);
		height = 0.0;
		diameter = 0.0;
		volume = 0.0;
		emptymass = 0.0;
		separation_delay = 0.0;
		speed = _V(0, 0, 0);
		rot_speed = _V(0, 0, 0);
		currDelay = 0.0;
        msh_h = nullptr;
        msh_idh = 0;
	}
};

struct ADAPTER {
	//mandatory
	std::string meshname;
	VECTOR3 off;
	double height;
	double diameter;
	double emptymass;

	MESHHANDLE msh_h;
	int msh_idh;
	ADAPTER(){
		off = _V(0, 0, 0);
		height = 0.0;
		diameter = 0.0;
		emptymass = 0.0;
        msh_h = nullptr;
        msh_idh = 0;
	}
};

struct LES {
	std::string meshname;
	VECTOR3 off;
	double height;
	double diameter;
	double emptymass;
	VECTOR3 speed;
	VECTOR3 rot_speed;
	std::string module;
	MESHHANDLE msh_h;
	int msh_idh;
	double volume;
	LES(){
		height = 0.0;
		diameter = 0.0;
		emptymass = 0.0;
		off = _V(0, 0, 0);
		speed = _V(0, 0, 0);
		rot_speed = _V(0, 0, 0);
		volume = 0.0;
        msh_idh = 0;
        msh_h = nullptr;
	}
};

struct ULLAGE {
	bool wUllage;
	bool ignited;
	double thrust;
	double anticipation;
	double overlap;
	int N;
	double angle;
	VECTOR3 pos;
	VECTOR3 dir;
	double length;
	double diameter;
	std::string tex;
	THRUSTER_HANDLE th_ullage;
	double rectfactor;
	ULLAGE(){
		wUllage = false;
		ignited = false;
		thrust = 0.0;
		anticipation = 0.0;
		overlap = 0.0;
		N = 0;
		angle = 0.0;
		pos = _V(0, 0, 0);
		dir = _V(0, 0, 0);
		length = 0.0;
		diameter = 0.0;
		rectfactor = 0.0;
        th_ullage = nullptr;
	}
};

struct STAGE {
	//mandatory
	std::string meshname;
	VECTOR3 off;
	double height;
	double diameter;
	double thrust;
	double emptymass;
	double fuelmass;
	double burntime;
	//unmandatory


//added by rcraig42 to store ispref and reference pressure----------------------------------------------

	double ispref;
	double pref;

	double waitforreignition;
	BATTS batteries;
	double ignite_delay;
	VECTOR3 speed;
	VECTOR3 rot_speed;
	std::string module;
	double pitchthrust;
	bool defpitch;
	double yawthrust;
	bool defyaw;
	double rollthrust;
	bool defroll;
	std::array<VECTOR3, 32> eng;
    std::array<double, 32> engine_phase;
	std::array<double, 32> engine_amp;
	std::array<double, 32> freq;
	std::array<VECTOR4F, 32> engV4;
	double eng_diameter;
	std::string eng_tex;
	std::string eng_pstream1;
	bool wps1;
	bool wps2;
	std::string eng_pstream2;
	VECTOR3 eng_dir;
	ULLAGE ullage;
	EXPBOLT expbolt;
	//derived parameters
	double isp;
	double volume;
	PROPELLANT_HANDLE tank;
	std::array<THRUSTER_HANDLE, 32> th_main_h;
	std::array<THRUSTER_HANDLE, 2> th_att_h;
	MESHHANDLE msh_h;
	int msh_idh;
	int nEngines;
	bool wInter;
	int IntIncremental;
	INTERSTAGE interstage;
	bool Ignited;
	double currDelay;
	double IgnitionTime;
	bool reignitable;
	bool DenyIgnition;
	bool wBoiloff;
	int StageState;
	bool ParticlesPacked;
	int ParticlesPackedToEngine;
	double linearthrust;
	double linearisp;
	STAGE(){
		msh_h = nullptr;
		msh_idh = 0;
		th_main_h.fill(nullptr);
		th_att_h.fill(nullptr);
		wps1 = false;
		wps2 = false;
		off = _V(0, 0, 0);
		height = 0.0;
		diameter = 0.0;
		thrust = 0.0;
		emptymass = 0.0;
		fuelmass = 0.0;
		burntime = 0.0;
		waitforreignition = 0.0;
		batteries = BATTS();
		ignite_delay = 0.0;
		speed = _V(0, 0, 0);
		rot_speed = _V(0, 0, 0);
		pitchthrust = 0.0;
		rollthrust = 0.0;
		yawthrust = 0.0;
		tank = nullptr;
		defpitch = false;
		defyaw = false;
		defroll = false;
		eng.fill(_V(0, 0, 0));
		engine_phase.fill(0);
		engine_amp.fill(0);
		freq.fill(0);
		engV4.fill(VECTOR4F());
		eng_diameter = 0.0;
		eng_dir = _V(0, 0, 0);
		ullage = ULLAGE();
		expbolt = EXPBOLT();
		isp = 0.0;
		volume = 0.0;
		nEngines = 0.0;
		wInter = false;
		IntIncremental = 0;
		interstage = INTERSTAGE();
		Ignited = false;
		currDelay = 0.0;
		IgnitionTime = 0.0;
		reignitable = true;
		DenyIgnition = false;
		wBoiloff = false;
		StageState = 0;
		ParticlesPacked = false;
		ParticlesPackedToEngine = 0;
		linearthrust = 0.0;
		linearisp = 0.0;
		ispref = 0.0;
		pref = 0.0;
	}
};

struct PAYLOAD {
	//mandatory
	std::string meshname;
	std::string meshname0;
	std::string meshname1;
	std::string meshname2;
	std::string meshname3;
	std::string meshname4;

	//VECTOR3 off[5];
	std::array<VECTOR3, 5> off;
	double height;
	double diameter;
	double mass;
	std::string module;
	std::string name;
	//unmandatory
	std::string MultiOffset;
	VECTOR3 speed;
	VECTOR3 rot_speed;

	double volume;
	int render;
	int nMeshes;
	std::array<MESHHANDLE, 5> msh_h;
	std::array<int, 5> msh_idh;
	VECTOR3 Rotation;
	bool rotated;
	bool live;

	PAYLOAD(){
		off.fill(_V(0, 0, 0));
		msh_h.fill(nullptr);
		msh_idh.fill(0);
		height = 0.0;
		diameter = 0.0;
		mass = 0.0;
		speed = _V(0, 0, 0);
		rot_speed = _V(0, 0, 0);
		volume = 0.0;
		render = 0;
		nMeshes = 0;
		Rotation = _V(0, 0, 0);
		rotated = false;
		live = false;
	}
};

struct FAIRING {
	//mandatory
	int N;
	std::string meshname;
	VECTOR3 off;
	double angle;
	double height;
	double diameter;
	double emptymass;
	//unmandatory
	double volume;
	VECTOR3 speed;
	VECTOR3 rot_speed;
	std::string module;
	std::array<MESHHANDLE, 10> msh_h;
	std::array<int, 10> msh_idh;
	FAIRING(){
		N = 0;
		off = _V(0, 0, 0);
		angle = 0.0;
		height = 0.0;
		diameter = 0.0;
		emptymass = 0.0;
		volume = 0.0;
		speed = _V(0, 0, 0);
		rot_speed = _V(0, 0, 0);
		msh_idh.fill(0);		
        msh_h.fill(nullptr);
	}
};

enum GNC_Comand { CM_ROLL, CM_PITCH, CM_ENGINE, CM_FAIRING, CM_LES, CM_DISABLE_PITCH, CM_DISABLE_ROLL, CM_DISABLE_JETTISON, CM_JETTISON, CM_TARGET, CM_AOA, CM_ATTITUDE, CM_SPIN, CM_INVERSE, CM_ORBIT, CM_ENGINEOUT, CM_DEFAP, CM_GLIMIT, CM_DESTROY, CM_EXPLODE, CM_NOLINE };

struct GNC_STEP {
	double time;
	std::string Comand;
	GNC_Comand GNC_Comand;
	double val_init;
	double val_fin;
	double time_init;
	double time_fin;
	double duration;
	double trval1, trval2, trval3, trval4, trval5, trval6;
	bool executed;
	std::string trchar;
	std::array<bool, 6> wValue;
    GNC_STEP()
        : time(0.0),
          Comand(),           
          GNC_Comand(),       
          val_init(0.0),
          val_fin(0.0),
          time_init(0.0),
          time_fin(0.0),
          duration(0.0),
          trval1(0.0),
          trval2(0.0),
          trval3(0.0),
          trval4(0.0),
          trval5(0.0),
          trval6(0.0),
          executed(false),
          trchar(),
          wValue{}      
    {}
};

struct FX_LAUNCH {
	double H;
	int N;
	double Angle;
	double Distance;
	double CutoffAltitude;
	std::string Ps1;
	std::string Ps2;
	FX_LAUNCH(){
		H = 0.0;
		N = 0;
		Angle = 0.0;
		Distance = 0.0;
		CutoffAltitude = 0.0;
	}
};

struct FX_MACH {
	double mach_min;
	double mach_max;
	std::string pstream;
	bool added;
	std::array<VECTOR3, 10> off;
	//VECTOR3 off[10];
	VECTOR3 dir;
    std::array<PSTREAM_HANDLE, 10> ps_h;
	int nmach;
	FX_MACH(){
		ps_h.fill(nullptr);
		mach_min = 0.0;
		mach_max = 0.0;
		added = false;
		dir = _V(0, 0, 0);
		nmach = 0.0;
		off.fill(_V(0, 0, 0));
	}
};

struct FX_VENT {
	std::array<double, 11> time_fin;
	std::string pstream;
	std::array<VECTOR3, 11> off;
	std::array<VECTOR3, 11> dir;
    std::array<PSTREAM_HANDLE, 11> ps_h;
	std::array<bool, 11> added;
	int nVent;
	FX_VENT(){
		ps_h.fill(nullptr);
		nVent = 0;
		time_fin.fill(0.0);
		off.fill(_V(0, 0, 0));
		dir.fill(_V(0, 0, 0));
		added.fill(false);
	}
};

struct Fuel_Levels {
    std::array<double, 10> Stage_Fuel_Lvls;
    std::array<double, 10> Boosters_Fuel_Lvls;
};

class Multistage2026 : public VESSEL4{

    friend class DevModeDlg;
    //DevModeDlg *DMD;

    public:

        Multistage2026(OBJHANDLE hObj, int fmodel);
        ~Multistage2026();

        //void CreateDMD();
        //void DestroyDMD();
        void clbkSetClassCaps(FILEHANDLE cfg) override;
        int clbkConsumeBufferedKey(DWORD key, bool down, char* kstate) override;
        int clbkConsumeDirectKey(char* kstate) override;
        void clbkLoadStateEx(FILEHANDLE scn, void* vs) override;
        void clbkSaveState(FILEHANDLE scn) override;
        void clbkPostStep(double simt, double simdt, double mjd) override;
        void clbkPreStep(double simt, double simdt, double mjd) override;
        void clbkPostCreation() override;
        bool clbkDrawHUD(int mode, const HUDPAINTSPEC* hps, oapi::Sketchpad* skp) override;
        int clbkGeneric(int msgid, int prm, void* context) override;

        PSTREAM_HANDLE AddExhaustStreamGrowing(THRUSTER_HANDLE  th, const VECTOR3& pos, PARTICLESTREAMSPEC* pss, bool growing, double growfactor_size, double growfactor_rate, bool count, bool ToBooster, int N_Item, int N_Engine);
        void ManageParticles(double dt, bool prestep);
        VECTOR4F _V4(double x, double y, double z, double t);
	    VECTOR2 _V2(double x, double y);
        bool IsOdd(int integer);
        void AutoJettison();
        void initGlobalVars();
        void VehicleSetup();
        void RotatePayload(int pns, int nm, VECTOR3 anglesrad);
        void LoadMeshes();
        void UpdateMass();
        void UpdatePMI();
        void UpdateOffsets();
	    void CreateUllageAndBolts();
        bool parseinifile(const std::string filename);
        void parseStages(const std::string filename);
        void parseBoosters(const std::string filename);
        void parseInterstages(const std::string filename, int parsingstage);
        void parseAdapter(const std::string filename);
        void parseLes(const std::string filename);
        void parseFairing(const std::string filename);
        void parsePayload(const std::string filename);
        void parseMisc(const std::string filename);
        void parseTexture(const std::string filename);
        void parseParticle(const std::string filename);
        void parseFXMach(const std::string filename);
        void parseFXVent(const std::string filename);
        void parseFXLaunch(const std::string filename);
        VECTOR3 GetBoosterPos(int nBooster, int N);
        void ArrangePayloadMeshes(const std::string &data, int pnl);
        std::string GetProperPayloadMeshName(int pnl, int n);
        void ArrangePayloadOffsets(const std::string &data, int pnl);
        void Jettison(int type, int current);
        void Spawn(int type, int current);
        VECTOR3 CharToVec(const std::string &str);
        VECTOR4F CharToVec4(const std::string &str);
	    void CreateRCS();
	    void CreateMainThruster();
        SURFHANDLE GetProperExhaustTexture(const std::string &name);

        void FLY(double simtime, double simdtime, double mjdate);

        VECTOR3 RotateVecZ(VECTOR3 input, double Angle);
	    PARTICLE GetProperPS(const std::string &name);
	    void InitializeDelays();

        void ToggleComplexFlight();
        VECTOR3 hms(double time);
        double RemBurnTime(int rbtstage, double level);
        double BoosterRemBurnTime(int rbtbooster, double level);
        double StageDv(int dvstage);
        double DvAtStage(int dvatstage);
        double StageRemDv(int dvstage);
        double CurrentStageRemDv();
        VECTOR3 RotateVector(const VECTOR3& input, double angle, const VECTOR3& rotationaxis);

        void UpdateLivePayloads();
        MATRIX3 RotationMatrix(VECTOR3 angles);
        void CheckForAdditionalThrust(int pns);
        void CheckForFX(int fxtype, double param);


        void WriteGNCFile();
        void parseGuidanceFile(const std::string filename);

        void VinkaComposeGNCSteps();
	    GNC_STEP VinkaComposeSpecificGNCSteps(GNC_STEP gnc);
        void VinkaDeleteStep(int q);
	    void VinkaRearrangeSteps();
	    void VinkaAddStep(std::string input);
	    int VinkaCountSteps();

        void Attitude(double pitch, double roll, double heading, double pitchrate, double rollrate, double yawrate);

        int VinkaGetStep(double met);
        void VinkaPitch(int step);
        void VinkaRoll(int step);
        void VinkaEngine(int step);
        void VinkaConsumeStep(int step);
        void VinkaAutoPilot();
        void VinkaCheckInitialMet();
        double GetProperHeading();
        double GetHeading();
        double GetProperRoll(double RequestedRoll);
        double initlong, initlat;
        int VinkaFindFirstPitch();
        int VinkaFindRoll();
        void VinkaUpdateRollTime();
        bool APstat;
        double finalv(double Abside, double Apo, double Peri);

        void killAP();
	    double VinkaFindEndTime();
        void Guidance_Debug();

        void ToggleAttCtrl(bool Pitch, bool Yaw, bool Roll);
	    void ToggleAP();

        double GetVPerp();
        void SetNewAltSteps(double newstep1, double newstep2, double newstep3, double newstep4);
        void EvaluateComplexFlight();
        void ComplexFlight();

        void CalculateAltSteps(double planetmass);
	    ////COMPUTATIONAL GRAVITY TURN EVALUATION

        double GetProperNforCGTE(double time);
	    bool CGTE(double psi0);

        //Some More Utilities
        double GetMassAtStage(int MassStage, bool empty);
        double CalcAzimuth();
        double getabsacc();
        double getacc();
        double GetOS();
        int SignOf(double X);

        void Boiloff();

        void CalcMECO();
        bool CutoffCheck();
        void FEstimate();

        void FStaging();

        void SetPegPitchLimit(double newlimit);
        void SetPegMajorCycleInterval(double newinterval);

        void InitPEG();
        void Navigate();
        double b0(int j, double t_j);
        double b_(int n, int j, double t_j);
        double c0(int j, double t_j);
        double c_(int n, int j, double t_j);
        double a(int j, double t_j);

        void CalculateTargets();
        void MajorCycle();

        void PEG();

        void Failure();
        void FailuresEvaluation();
        bool CheckForFailure(double met);
        void boom();

        void Telemetry();
        int WriteTelemetryFile(int initline);
	    void parseTelemetryFile(const std::string filename);

        double CalculateFullMass();
        void ResetVehicle(VECTOR3 hangaranimsV, bool Ramp);
        void Ramp(bool alreadyramp);

        int GetMSVersion();

        void CreateLaunchFX();

        void CreateHangar();
        void CreateCamera();
        bool CheckForDetach();

        void AttachToMSPad(OBJHANDLE hPad);
        double GetMET();
        bool GetAutopilotStatus();
        VECTOR3 GetAPTargets();

    private:
	    
        int MyID;
	    THGROUP_HANDLE thg_h_main;
        double particlesdt;
        bool GrowingParticles;
        double RefPressure;
        std::array<GNC_STEP, 150> Gnc_step;
        double TotalHeight;
        std::string dataparsed;
        int Configuration;
	    double CogElev;

        std::string OrbiterRoot;
        std::string fileini;
        std::string guidancefile;

        int nStages;
        int nBoosters;
        int nInterstages;
        int nPayloads;
        int nTextures;
        int nParticles;
        int currentStage;
        int currentBooster;
        int currentInterstage;
        int currentPayload;
        bool hasFairing;
        bool wLes;
        bool wMach;
        bool wVent;
        bool wLaunchFX;
        int wFairing;

        std::array<STAGE, 10> stage;
	    std::array<BOOSTER, 10> booster;

        std::array<PAYLOAD, 10> payload;
        FAIRING fairing;
        MISC Misc;
        TEX tex;
        std::array<PARTICLE, 16> Particle;
        double currentDelta;
        int nPsh;
        int nPsg;
        PSGROWING* psg;

        ADAPTER Adapter;
	    LES Les;
        FX_MACH FX_Mach;
	    FX_VENT FX_Vent;
	    FX_LAUNCH FX_Launch;

	    bool wBoosters;
	    bool wAdapter;
        std::string logbuff;
        double MET;

        double stage_ignition_time;
        std::array<ATTACHMENTHANDLE, 10> live_a;

        double lvl;

        std::array<std::array<UINT, 32>, 10> exhaustN;

        int nsteps;

        double perror;
        double yerror;
        double rerror;
        double pintegral;
        double kp;
        double kd;
        double ki;

        double Tu;
        double Ku;

        VECTOR3 MaxTorque;

        double VinkaAzimuth;
	    double VinkaMode;

        bool AJdisabled;
        bool rolldisabled;
        bool pitchdisabled;

        int Gnc_running;

        std::string value1, value2, value3, value4, value5, value6;

        bool spinning;
        bool AttCtrl;
        bool PitchCtrl;
        bool YawCtrl;
        bool RollCtrl;

        double DeltaUpdate;
        bool wPeg;
        double TgtPitch;

        std::array<double, 4> altsteps;

        bool stepsloaded;

        bool Complex;
        double UpdateComplex;

        double GT_InitPitch;
	    double GT_IP_Calculated;

        bool runningPeg;
        ORBITPARAM op;
        ELEMENTS el;
        double epsfin;
        double eps;
        CUTOFF_CONDITIONS coc;
        double MECO_TEST;
        std::array<double, 100> coeff;
        double avgcoeff;
        int MECO_Counter;
        double TMeco;
        double PegPitchLimit;

        std::array<double, 10> r_T;
        std::array<double, 10> rdot_T;
        std::array<double, 10> T;
        std::array<double, 10> DeltaA;
        std::array<double, 10> DeltaB;
        std::array<double, 10> A;
        std::array<double, 10> B;
        std::array<double, 10> delta_rdot;
        std::array<double, 10> delta_r;
        std::array<double, 10> r_in;
        std::array<double, 10> rdot_in;
        std::array<double, 10> VthetaAtStaging;
        std::array<double, 10> OmegaS;
        std::array<double, 10> a_fin;

        double tgtapo;
        double tgtperi;
        double tgtinc;
        double tgtabside;
        double tgtvv;
        double Azimuth;
        double mu;
        double rt;
        double g0;

        //Navigation vars
        double r;
        double v;
        double z;
        VECTOR3 hvec;
        VECTOR3 rvec;
        VECTOR3 vvec;
        double h;
        VECTOR3 rhat;
        VECTOR3 hhat;
        VECTOR3 thetahat;
        VECTOR3 fhat;
        double thrust;
        double mass;
        std::array<double, 10> a_;
        std::array<double, 10> v_e;
        std::array<double, 10> tau_;
        double omega;
        double rdot;
        double vtheta;
        VECTOR3 ShipSpeed;
        double VertVel;
        int NN;
        double PegMajorCycleInterval;

        //feedback vars
        double g;
        double cent;
        double g_term;
        double cent_term;
        int J;

        double UpdatePegTimer;
	    double PegDesPitch;

        //FAILURES
        int failureProbability;
        int timeOfFailure;
        bool wFailures;
        bool failed;

        std::string tlmfile;

        std::array<VECTOR2, TLMSECS> tlmAlt;
        std::array<VECTOR2, TLMSECS> tlmSpeed;
        std::array<VECTOR2, TLMSECS> tlmPitch;
        std::array<VECTOR2, TLMSECS> tlmThrust;
        std::array<VECTOR2, TLMSECS> tlmMass;
        std::array<VECTOR2, TLMSECS> tlmVv;
        std::array<VECTOR2, TLMSECS> tlmAcc;
        std::array<VECTOR2, TLMSECS> ReftlmAlt;
        std::array<VECTOR2, TLMSECS> ReftlmSpeed;
        std::array<VECTOR2, TLMSECS> ReftlmPitch;
        std::array<VECTOR2, TLMSECS> ReftlmThrust;
        std::array<VECTOR2, TLMSECS> ReftlmMass;
        std::array<VECTOR2, TLMSECS> ReftlmVv;
        std::array<VECTOR2, TLMSECS> ReftlmAcc;

        double updtlm;
        double writetlmTimer;
        int tlmidx;

        int tlmnlines;
	    int loadedtlmlines;
        double updtboiloff;
	    bool wReftlm;

        VECTOR3 hangaranims;
        FILEHANDLE scenario;
        FILEHANDLE config;
        int loadedCurrentBooster;
        int loadedCurrentInterstage;
        int loadedCurrentStage;
        int loadedCurrentPayload;
        int loadedwFairing;
        int loadedConfiguration;
        bool loadedGrowing;
        double loadedMET;
        VESSELSTATUS2 vs2;
        MATRIX3 RotMatrix;
        bool DeveloperMode;
        bool killDMD;
        NOTEHANDLE note;

        std::array<std::array<MGROUP_ROTATE *, 5>, 10> payloadrotatex;
        std::array<std::array<MGROUP_ROTATE *, 5>, 10> payloadrotatey;
        std::array<std::array<MGROUP_ROTATE *, 5>, 10> payloadrotatez;

        std::array<std::array<UINT, 5>, 10> RotatePayloadAnim_x;
        std::array<std::array<UINT, 5>, 10> RotatePayloadAnim_y;
        std::array<std::array<UINT, 5>, 10> RotatePayloadAnim_z;

        std::array<std::array<ANIMATIONCOMPONENT_HANDLE, 5>, 10> anim_x;
        std::array<std::array<ANIMATIONCOMPONENT_HANDLE, 5>, 10> anim_y;
        std::array<std::array<ANIMATIONCOMPONENT_HANDLE, 5>, 10> anim_z;

        OBJHANDLE hramp;
        OBJHANDLE hhangar;
        OBJHANDLE hCrawler;
        VESSEL4* vCrawler;
        VESSEL4* vramp;
        VESSEL4* vhangar;
        VESSELSTATUS2 vsramp, vshangar;
        ATTACHMENTHANDLE AttToRamp, padramp, AttToHangar, PadHangar, AttToCrawler;
        bool wRamp;
        bool NoMoreRamp;

        COLOUR4 col_d;
        COLOUR4 col_s;
        COLOUR4 col_a;
        COLOUR4 col_white;

        double th_main_level;
        double launchFx_level;

        bool HangarMode;
        bool wCrawler;
        bool wCamera;
        double CamDLat, CamDLng;
        bool AttToMSPad;
        VECTOR3 MsPadZ;

};

#endif //!__MULTISTAGE2026_HPP_