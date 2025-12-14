#include "Multistage2026.hpp"
#include <array>
#include <filesystem>
#include <sstream>
#include <string>
#define ORBITER_MODULE

HWND hDlg;

typedef struct {
	HINSTANCE hDLL;
} GDIParams;
GDIParams g_Param;

void OpenDlgClbk(void* context);
//BOOL CALLBACK MsgProc (HWND, UINT, WPARAM, LPARAM);
void CloseDlg(HWND hDlg)
{
	oapiCloseDialog(hDlg);
}

Multistage2026::Multistage2026(OBJHANDLE hObj, int fmodel) :VESSEL4(hObj, fmodel) {

	DeveloperMode = false;
	//Ms15=this;
	HangarMode = false;

	//DMD = nullptr;

    std::filesystem::path OR_path = std::filesystem::current_path();
    OrbiterRoot = OR_path.string();

	nStages = 0;

	nBoosters = 0;

	nInterstages = 0;

	nTextures = 0;

	nParticles = 0;

	currentStage = 0;

	currentBooster = 0;

	currentInterstage = 0;

	CogElev = 0;

	Misc.VerticalAngle = 0;

	Misc.drag_factor = 1;

	wBoosters = false;

	wFairing = 0;

	wLes = false;

	wAdapter = false;

	wMach = false;

	wVent = false;

	wLaunchFX = false;

	Complex = false;

	stage_ignition_time = 0;

	currentDelta = 0.0;

	perror = 0.0;

	yerror = 0.0;

	rerror = 0.0;

	for (unsigned int i = 0; i < 10; i++) {
		stage.at(i).Ignited = false;
		stage.at(i).reignitable = true;
		stage.at(i).batteries.wBatts = false;
		stage.at(i).waitforreignition = 0;
		stage.at(i).StageState = STAGE_SHUTDOWN;
		stage.at(i).DenyIgnition = false;
		stage.at(i).ParticlesPacked = false;
		stage.at(i).ParticlesPackedToEngine = 0;
		stage.at(i).defpitch = false;
		stage.at(i).defroll = false;
		stage.at(i).defyaw = false;
		payload.at(i) = PAYLOAD();
	}

	for (unsigned int i = 0; i < 10; i++) {
		booster.at(i) = BOOSTER();
		booster.at(i).Ignited = false;
		booster.at(i).ParticlesPacked = false;
		booster.at(i).ParticlesPackedToEngine = 0;
	}

	MET = 0.0;

	APstat = false;

	AJdisabled = false;

	rolldisabled = false;

	pitchdisabled = false;

	Gnc_running = 0;
	spinning = false;

	lvl = 1.0;

	PegDesPitch = 35 * RAD;

	PegPitchLimit = 35 * RAD;

	DeltaUpdate = 0;

	GT_InitPitch = 89.5 * RAD;

	UpdatePegTimer = 0.0;

	UpdateComplex = 0.0;

	wPeg = false;

	PegMajorCycleInterval = 0.1;

	runningPeg = false;

	AttCtrl = true;

	PitchCtrl = true;

	YawCtrl = true;

	RollCtrl = true;

	TgtPitch = 0;

	eps = -9000000000000;

	failureProbability = -1000;

	timeOfFailure = -10000000;

	wFailures = false;

	failed = false;

	killDMD = false;

	stepsloaded = false;

	for (unsigned int i = 0; i < 150; i++)
	{
		Gnc_step.at(i).GNC_Comand = CM_NOLINE;
	}

	updtlm = 0.0;

	tlmidx = 0;

	writetlmTimer = 0.0;

	tlmnlines = 0;

	wReftlm = false;


	for (int i = 0; i < TLMSECS; i++)
	{
		tlmAlt.at(i).x = 0;
		tlmSpeed.at(i).x = 0;
		tlmPitch.at(i).x = 0;
		tlmThrust.at(i).x = 0;
		tlmMass.at(i).x = 0;
		tlmVv.at(i).x = 0;
		tlmAcc.at(i).x = 0;

		tlmAlt.at(i).y = 0;
		tlmSpeed.at(i).y = 0;
		tlmPitch.at(i).y = 0;
		tlmThrust.at(i).y = 0;
		tlmMass.at(i).y = 0;
		tlmVv.at(i).y = 0;
		tlmAcc.at(i).y = 0;
	}

	updtboiloff = 0.0;

	for (unsigned int i = 0; i < 100; i++) {
		coeff.at(i) = 0.0;
	}

	avgcoeff = 0.0;

	MECO_Counter = 0;

	MECO_TEST = 0.0;
	TMeco = 600.0;
	nPsg = 0;
	particlesdt = 0.0;
	GrowingParticles = false;

	RefPressure = 101400.0;


	wRamp = false;
	NoMoreRamp = false;
	col_d.a = 0;
	col_d.b = 1;
	col_d.g = 0.8;
	col_d.r = 0.9;
	col_s.a = 0;
	col_s.b = 1;
	col_s.g = 0.8;
	col_s.r = 1.9;
	col_a.a = 0;
	col_a.b = 0;
	col_a.g = 0;
	col_a.r = 0;
	col_white.a = 0;
	col_white.b = 1;
	col_white.g = 1;
	col_white.r = 1;

	th_main_level = 0.0;

	launchFx_level = 0.0;

	hangaranims = _V(1.3, -10, 57.75);

	wCrawler = false;

	wCamera = false;

	AttToMSPad = false;

	MsPadZ = _V(0, 0, -50);

	for (int i = 0; i < 10; i++) {
		A.at(i) = 0.0;
	}

	AttToCrawler = nullptr;

	AttToHangar = nullptr;

	AttToRamp = nullptr;

	Azimuth = 0.0;

	for (int i = 0; i < 10; i++) {
		B.at(i) = 0.0;
	}

	CamDLat = 0.0;

	CamDLng = 0.0;

	Configuration = 0;

	for (int i = 0; i < 10; i++) {
		DeltaA.at(i) = 0.0;
	}

	for (int i = 0; i < 10; i++) {
		DeltaB.at(i) = 0.0;
	}

	GT_IP_Calculated = 0.0;

	J = 0;

	Ku = 0.0;

	MaxTorque = _V(0, 0, 0);

	MyID = 0.0;

	NN = 0;

	for (int i = 0; i < 10; i++) {
		OmegaS.at(i) = 0.0;
	}

	PadHangar = nullptr;

	for (int i = 0; i < 7200; i++) {
		ReftlmAcc.at(i) = VECTOR2(0, 0);
	}

	for (int i = 0; i < 7200; i++) {
		ReftlmAlt.at(i) = VECTOR2(0, 0);
	}

	for (int i = 0; i < 7200; i++) {
		ReftlmMass.at(i) = VECTOR2(0, 0);
	}

	for (int i = 0; i < 7200; i++) {
		ReftlmSpeed.at(i) = VECTOR2(0, 0);
	}

	for (int i = 0; i < 7200; i++) {
		ReftlmPitch.at(i) = VECTOR2(0, 0);
	}

	for (int i = 0; i < 7200; i++) {
		ReftlmThrust.at(i) = VECTOR2(0, 0);
	}

	for (int i = 0; i < 7200; i++) {
		ReftlmVv.at(i) = VECTOR2(0, 0);
	}

	RotMatrix = {};

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			RotatePayloadAnim_x.at(i).at(j) = 0;
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			RotatePayloadAnim_y.at(i).at(j) = 0;
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			RotatePayloadAnim_z.at(i).at(j) = 0;
		}
	}

	ShipSpeed = _V(0, 0, 0);

	for (int i = 0; i < 10; i++) {
		T.at(i) = 0.0;
	}

	TotalHeight = 0.0;

	Tu = 0.0;

	VertVel = 0.0;

	VinkaAzimuth = 0.0;

	VinkaMode = 0.0;

	for (int i = 0; i < 10; i++) {
		VthetaAtStaging.at(i) = 0.0;
	}

	for (int i = 0; i < 10; i++) {
		a_.at(i) = 0.0;
	}

	for (int i = 0; i < 10; i++) {
		a_fin.at(i) = 0.0;
	}

	for (int i = 0; i < 4; i++) {
		altsteps.at(i) = 0.0;
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			anim_x.at(i).at(j) = nullptr;
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			anim_y.at(i).at(j) = nullptr;
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			anim_z.at(i).at(j) = nullptr;
		}
	}

	cent = 0.0;

	cent_term = 0.0;

	coc = {};

	config = nullptr;

	currentPayload = 0;

	for (int i = 0; i < 10; i++) {
		delta_r.at(i) = 0.0;
	}

	for (int i = 0; i < 10; i++) {
		delta_rdot.at(i) = 0.0;
	}

	el = {};

	epsfin = 0.0;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 32; j++) {
			exhaustN.at(i).at(j) = 0;
		}
	}

	fhat = _V(0, 0, 0);

	g = 0.0;

	g0 = 0.0;

	g_term = 0.0;

	h = 0.0;

	h = 0.0;

	hCrawler = nullptr;

	hasFairing = false;

	hhangar = nullptr;

	hhat = _V(0, 0, 0);

	hramp = nullptr;

	hvec = _V(0, 0, 0);

	initlat = 0.0;

	initlong = 0.0;

	kd = 0.0;

	ki = 0.0;

	kp = 0.0;

	for (int i = 0; i < 10; i++) {
		live_a.at(i) = nullptr;
	}

	loadedConfiguration = 0;

	loadedCurrentBooster = 0;

	loadedCurrentInterstage = 0;

	loadedCurrentPayload = 0;

	loadedCurrentStage = 0;

	loadedGrowing = false;

	loadedMET = 0.0;

	loadedtlmlines = 0.0;

	loadedwFairing = 0.0;

	mass = 0.0;

	mu = 0.0;

	nPayloads = 0;

	nPsh = 0;

	note = nullptr;

	nsteps = 0;

	omega = 0.0;

	op = {};

	padramp = nullptr;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			payloadrotatex.at(i).at(j) = nullptr;
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			payloadrotatey.at(i).at(j) = nullptr;
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			payloadrotatez.at(i).at(j) = nullptr;
		}
	}

	pintegral = 0.0;

	psg = {};

	r = 0.0;

	for (int i = 0; i < 10; i++) {
		r_T.at(i) = 0.0;
	}

	for (int i = 0; i < 10; i++) {
		r_in.at(i) = 0.0;
	}

	rdot = 0.0;

	for (int i = 0; i < 10; i++) {
		rdot_T.at(i) = 0.0;
	}

	for (int i = 0; i < 10; i++) {
		rdot_in.at(i) = 0.0;
	}

	rhat = _V(0, 0, 0);

	rt = 0.0;

	rvec = _V(0, 0, 0);

	scenario = nullptr;

	for (int i = 0; i < 10; i++) {
		tau_.at(i) = 0.0;
	}

	tgtabside = 0.0;

	tgtinc = 0.0;

	tgtapo = 0.0;

	tgtperi = 0.0;

	tgtvv = 0.0;

	thetahat = _V(0, 0, 0);

	thg_h_main = nullptr;

	thrust = 0;

	v = 0.0;

	vCrawler = nullptr;

	for (int i = 0; i < 10; i++) {
		v_e.at(i) = 0.0;
	}

	vhangar = nullptr;

	vramp = nullptr;

	vs2 = {};

	vshangar = {};

	vsramp = {};

	vtheta = 0.0;

	vvec = _V(0, 0, 0);

	z = 0.0;

	tex = {0};
}

Multistage2026::~Multistage2026() {
	if (psg)delete[]psg;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			delete payloadrotatex.at(i).at(j);
			delete payloadrotatey.at(i).at(j);
			delete payloadrotatez.at(i).at(j);
		}
	}

	//delete Ms15;
}

VECTOR4F Multistage2026::_V4(double x, double y, double z, double t) {
	VECTOR4F v4;
	v4.x = x;
	v4.y = y;
	v4.z = z;
	v4.t = t;
	return v4;
}

VECTOR2 Multistage2026::_V2(double x, double y)
{
	VECTOR2 v2;
	v2.x = x;
	v2.y = y;
	return v2;
}
//returns 1 if X is positive or -1 if X is negative
int Multistage2026::SignOf(double X)
{
	return X / abs(X);
}

//returns true if a number is odd

bool Multistage2026::IsOdd(int integer)
{
	if (integer % 2 == 0)
		return true;
	else
		return false;
}

//transforms std::string variable into VECTOR3
VECTOR3 Multistage2026::CharToVec(const std::string& str) {
    
    std::string s = str;
    s.erase(std::remove(s.begin(), s.end(), '('), s.end());
    s.erase(std::remove(s.begin(), s.end(), ')'), s.end());

    
    std::replace(s.begin(), s.end(), ';', ',');

    std::stringstream ss(s);
    double x = 0.0, y = 0.0, z = 0.0;
    char comma;

    ss >> x >> comma >> y >> comma >> z;

    return _V(x, y, z);
}

//transforms std::string variable into VECTOR4F
VECTOR4F Multistage2026::CharToVec4(const std::string &str) {
    std::string s = str;
    s.erase(std::remove(s.begin(), s.end(), '('), s.end());
    s.erase(std::remove(s.begin(), s.end(), ')'), s.end());
    std::replace(s.begin(), s.end(), ';', ',');

    std::stringstream ss(s);
    double x = 0.0, y = 0.0, z = 0.0, t = 0.0;
    char comma;

    ss >> x >> comma >> y >> comma >> z >> comma >> t;

    VECTOR4F outvec;
    outvec.x = x;
    outvec.y = y;
    outvec.z = z;
    outvec.t = t;

    return outvec;
}

//Function to Rotate a Vector3 around Z axis of a given Angle
VECTOR3 Multistage2026::RotateVecZ(VECTOR3 input, double Angle) {
	VECTOR3 output;
	output = _V(input.x * cos(Angle) - input.y * sin(Angle), input.x * sin(Angle) + input.y * cos(Angle), input.z);
	return output;
}

MATRIX3 Multistage2026::RotationMatrix(VECTOR3 angles)
{
	MATRIX3 m;
	MATRIX3 RM_X, RM_Y, RM_Z;
	RM_X = _M(1, 0, 0, 0, cos(angles.x), -sin(angles.x), 0, sin(angles.x), cos(angles.x));
	RM_Y = _M(cos(angles.y), 0, sin(angles.y), 0, 1, 0, -sin(angles.y), 0, cos(angles.y));
	RM_Z = _M(cos(angles.z), -sin(angles.z), 0, sin(angles.z), cos(angles.z), 0, 0, 0, 1);
	m = mul(RM_Z, mul(RM_Y, RM_X));
	return m;
}

void Multistage2026::ResetVehicle(VECTOR3 hangaranimsV = _V(1.3, -10, 57.75), bool Ramp = false){

	// 	 if(HangarMode)
	 //{
		// SetAttachmentParams(PadHangar,_V(0,0,100),_V(0,0,1),_V(0,1,0));
	 //}

	ClearMeshes();
	ClearThrusterDefinitions();
	ClearPropellantResources();
	ClearAttachments();

	//	 	if((RampCreated)&&(!RampDeleted))


	initGlobalVars();
	wRamp = Ramp;
	if (wRamp)
	{
		oapiDeleteVessel(hramp);
		wRamp = false;
	}


	std::filesystem::path tempFile = OrbiterRoot + "\\" + fileini;
    oapiWriteLogV("%s: Config File: %s", GetName(), tempFile.c_str());
	parseinifile(tempFile.string());

	currentBooster = loadedCurrentBooster;
	currentInterstage = loadedCurrentInterstage;
	currentStage = loadedCurrentStage;
	currentPayload = loadedCurrentPayload;
	wFairing = loadedwFairing;
	Configuration = loadedConfiguration;
	GrowingParticles = loadedGrowing;

	MET = loadedMET;
	if ((currentInterstage > currentStage) || (currentInterstage > nInterstages) || (currentInterstage >= stage.at(currentStage).IntIncremental)) { stage.at(currentStage).wInter = false; }

	if ((wFairing == 1) && (hasFairing == false)) { wFairing = 0; }
	if (Configuration == 0) {			//if only configuration is defined, reset everything
		if (hasFairing == true) {
			wFairing = 1;
		}
		currentStage = 0;
		currentPayload = 0;
		currentBooster = 0;
		currentInterstage = 0;
	}

	parseGuidanceFile(guidancefile);
	UpdateOffsets();

	VehicleSetup();
	LoadMeshes();

	//vs2.arot=_V(0,0,0);
	vs2.rvel = _V(0, 0, 0);

	clbkSetStateEx(&vs2);

	hangaranims = hangaranimsV;

	clbkPostCreation();


	return;
}

//Returns current Heading
double Multistage2026::GetHeading(){
	double Heading;
	oapiGetHeading(GetHandle(), &Heading);
	return Heading;
}

//returns Vehicle Orbital Speed
double Multistage2026::GetOS() {
	OBJHANDLE hearth = GetSurfaceRef();
	VECTOR3 relvel;
	GetRelativeVel(hearth, relvel);
	double os = length(relvel);
	return os;
}

//returns Vechicle Mass at a certain stage
double Multistage2026::GetMassAtStage(int MassStage, bool empty = true) {
	double Mass = 0;

	int add = 0;
	if (empty == true) { add = 1; }
	else { add = 0; }

	for (int i = MassStage; i < nStages; i++) {
		Mass += stage.at(i).emptymass;
	}
	for (int j = MassStage + add; j < nStages; j++) {
		Mass += stage.at(j).fuelmass;
	}
	for (int k = 0; k < nPayloads; k++) {
		Mass += payload.at(k).mass;
	}
	if (hasFairing) {
		Mass += fairing.emptymass;
	}
	if (wAdapter) {
		Mass += Adapter.emptymass;
	}
	if (wLes) {
		Mass += Les.emptymass;
	}

	return Mass;
}

//Returns Remainign Stage DV
double Multistage2026::StageDv(int dvstage){
	double sdv = stage.at(dvstage).isp * log(GetMassAtStage(dvstage, false) / GetMassAtStage(dvstage, true));

	return sdv;
}

//Returns Remaining DV at a certain Stage
double Multistage2026::DvAtStage(int dvatstage) {
	double rdvas = 0;
	for (int i = dvatstage; i < nStages; i++) {
		rdvas += StageDv(i);
	}
	return rdvas;
}

//Returns Current Stage Remaining DV
double Multistage2026::CurrentStageRemDv() {
	double csrd;
	csrd = stage.at(currentStage).isp * log((GetMassAtStage(currentStage, true) + GetPropellantMass(stage.at(currentStage).tank)) / GetMassAtStage(currentStage, true));
	return csrd;
}


//Returns Any Stage Rmeaining DV
double Multistage2026::StageRemDv(int dvstage)
{
	double srd;
	srd = stage[dvstage].isp * log((GetMassAtStage(dvstage, true) + GetPropellantMass(stage[dvstage].tank)) / GetMassAtStage(dvstage, true));

	return srd;
}

//Returns Remaining Stage Burn Time
double Multistage2026::RemBurnTime(int rbtstage, double level = 1){
	double BT;
	BT = stage.at(rbtstage).isp * GetPropellantMass(stage.at(rbtstage).tank) / (stage.at(rbtstage).thrust * level);

	return BT;
}

//Returns Remaining Boosters Group Burn Time
double Multistage2026::BoosterRemBurnTime(int rbtbooster, double level = 1) {
	double BT;
	BT = booster.at(rbtbooster).isp * GetPropellantMass(booster.at(rbtbooster).tank) / ((booster.at(rbtbooster).thrust * booster.at(rbtbooster).N) * level);

	return BT;
}

//Global Variables initialization
void Multistage2026::initGlobalVars(){
	nStages = 0;
	nBoosters = 0;
	nInterstages = 0;
	nTextures = 0;
	nParticles = 0;
	currentStage = 0;
	currentBooster = 0;
	currentInterstage = 0;
	CogElev = 0;
	Misc.VerticalAngle = 0;
	Misc.drag_factor = 1;
	std::filesystem::path OR_path = std::filesystem::current_path();
	OrbiterRoot = OR_path.string(); //initialize orbiter root with the current directory
	wBoosters = false;
	wFairing = 0;
	wLes = false;
	wAdapter = false;
	wMach = false;
	wVent = false;
	wLaunchFX = false;
	Complex = false;
	stage_ignition_time = 0;

	currentDelta = 0;
	perror = 0;
	yerror = 0;
	rerror = 0;

	int i;
	for (i = 0; i < 10; i++) {
		stage.at(i) = STAGE();
		stage.at(i).Ignited = false;
		stage.at(i).reignitable = true;
		stage.at(i).batteries.wBatts = false;
		stage.at(i).waitforreignition = 0;
		stage.at(i).StageState = STAGE_SHUTDOWN;
		stage.at(i).DenyIgnition = false;
		stage.at(i).ParticlesPacked = false;
		stage.at(i).ParticlesPackedToEngine = 0;
		stage.at(i).defpitch = false;
		stage.at(i).defroll = false;
		stage.at(i).defyaw = false;
		payload.at(i) = PAYLOAD();
	}

	int ii;
	for (ii = 0; ii < 10; ii++) {
		booster.at(ii) = BOOSTER();
		booster.at(ii).Ignited = false;
		booster.at(ii).ParticlesPacked = false;
		booster.at(ii).ParticlesPackedToEngine = 0;
	}

	MET = 0;
	APstat = false;
	AJdisabled = false;
	rolldisabled = false;
	pitchdisabled = false;

	Gnc_running = 0;
	spinning = false;
	lvl = 1;

	PegDesPitch = 35 * RAD;
	PegPitchLimit = 35 * RAD;
	DeltaUpdate = 0;
	GT_InitPitch = 89.5 * RAD;
	UpdatePegTimer = 0;
	UpdateComplex = 0;
	wPeg = false;
	PegMajorCycleInterval = 0.1;
	runningPeg = false;

	AttCtrl = true;
	PitchCtrl = true;
	YawCtrl = true;
	RollCtrl = true;
	TgtPitch = 0;

	eps = -9000000000000;

	failureProbability = -1000;
	timeOfFailure = -10000000;
	wFailures = false;
	failed = false;

	//DMD = 0;
	killDMD = false;
	stepsloaded = false;

	for (int q = 0; q < 150; q++){
		Gnc_step.at(q).GNC_Comand = CM_NOLINE;
	}

	updtlm = 0;
	tlmidx = 0;
	writetlmTimer = 0;
	tlmnlines = 0;
	wReftlm = false;
	for (int q = 0; q < TLMSECS; q++){
		tlmAlt.at(q).x = 0;
		tlmSpeed.at(q).x = 0;
		tlmPitch.at(q).x = 0;
		tlmThrust.at(q).x = 0;
		tlmMass.at(q).x = 0;
		tlmVv.at(q).x = 0;
		tlmAcc.at(q).x = 0;

		tlmAlt.at(q).y = 0;
		tlmSpeed.at(q).y = 0;
		tlmPitch.at(q).y = 0;
		tlmThrust.at(q).y = 0;
		tlmMass.at(q).y = 0;
		tlmVv.at(q).y = 0;
		tlmAcc.at(q).y = 0;
	}

	updtboiloff = 0;
	for (int h = 0; h < 100; h++)
	{
		coeff.at(h) = 0;
	}
	avgcoeff = 0;
	MECO_Counter = 0;

	MECO_TEST = 0;
	TMeco = 600;
	nPsg = 0;
	particlesdt = 0;
	GrowingParticles = false;

	RefPressure = 101400;

	wRamp = false;
	NoMoreRamp = false;
	col_d.a = 0;
	col_d.b = 1;
	col_d.g = 0.8;
	col_d.r = 0.9;
	col_s.a = 0;
	col_s.b = 1;
	col_s.g = 0.8;
	col_s.r = 1.9;
	col_a.a = 0;
	col_a.b = 0;
	col_a.g = 0;
	col_a.r = 0;
	col_white.a = 0;
	col_white.b = 1;
	col_white.g = 1;
	col_white.r = 1;

	th_main_level = 0;
	launchFx_level = 0;
	hangaranims = _V(1.3, -10, 57.75);
	wCrawler = false;
	wCamera = false;
	AttToMSPad = false;
	MsPadZ = _V(0, 0, -50);

}

PSTREAM_HANDLE Multistage2026::AddExhaustStreamGrowing(THRUSTER_HANDLE  th, const VECTOR3& pos, PARTICLESTREAMSPEC* pss = 0, bool growing = false, double growfactor_size = 0, double growfactor_rate = 0, bool count = true, bool ToBooster = false, int N_Item = 0, int N_Engine = 0){

	PSTREAM_HANDLE psh = AddExhaustStream(th, pos, pss);

	psg[nPsg].pss = *pss;
	psg[nPsg].psh.at(2) = psh;
	psg[nPsg].th = th;
	//psg[nPsg].psh.at(0)=psh;
	psg[nPsg].pos = pos;
	psg[nPsg].GrowFactor_rate = growfactor_rate;
	psg[nPsg].GrowFactor_size = growfactor_size;
	psg[nPsg].growing = growing;
	psg[nPsg].baserate = psg[nPsg].pss.growthrate;
	psg[nPsg].basesize = psg[nPsg].pss.srcsize;
	psg[nPsg].basepos = psg[nPsg].pos;
	psg[nPsg].ToBooster = ToBooster;
	psg[nPsg].nItem = N_Item;
	psg[nPsg].nEngine = N_Engine;

	if(count){
		nPsg++;
	}

	return psh;
}

//Create RCS 
void Multistage2026::CreateRCS() {

	if (stage.at(currentStage).pitchthrust == 0) {
		stage.at(currentStage).pitchthrust = 0.25 * stage.at(currentStage).thrust * stage.at(currentStage).height;//Empirical Values
	}
	if (stage.at(currentStage).yawthrust == 0) {
		stage.at(currentStage).yawthrust = 0.25 * stage.at(currentStage).thrust * stage.at(currentStage).height;//Empirical Values
	}
	if (stage.at(currentStage).rollthrust == 0) {
		stage.at(currentStage).rollthrust = 0.1 * stage.at(currentStage).thrust * stage.at(currentStage).diameter * 0.5 * 0.5; //Empirical Values
	}
	//pitch up
	stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 1), _V(0, 1, 0), 2 * stage.at(currentStage).pitchthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, -1), _V(0, -1, 0), 2 * stage.at(currentStage).pitchthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_PITCHUP);

	MaxTorque.x = 2 * GetThrusterMax(stage.at(currentStage).th_att_h.at(0));

	//pitch down
	stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 1), _V(0, -1, 0), 2 * stage.at(currentStage).pitchthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, -1), _V(0, 1, 0), 2 * stage.at(currentStage).pitchthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_PITCHDOWN);

	//yaw left
	stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 1), _V(-1, 0, 0), 2 * stage.at(currentStage).yawthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, -1), _V(1, 0, 0), 2 * stage.at(currentStage).yawthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_YAWLEFT);

	MaxTorque.y = 2 * GetThrusterMax(stage.at(currentStage).th_att_h.at(0));
	//yaw right
	stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 1), _V(1, 0, 0), 2 * stage.at(currentStage).yawthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, -1), _V(-1, 0, 0), 2 * stage.at(currentStage).yawthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_YAWRIGHT);

	//roll left
	stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(1, 0, 0), _V(0, 1, 0), 2 * stage.at(currentStage).rollthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(-1, 0, 0), _V(0, -1, 0), 2 * stage.at(currentStage).rollthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_BANKLEFT);

	MaxTorque.z = 2 * GetThrusterMax(stage.at(currentStage).th_att_h.at(0));
	//roll right
	stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(1, 0, 0), _V(0, -1, 0), 2 * stage.at(currentStage).rollthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(-1, 0, 0), _V(0, 1, 0), 2 * stage.at(currentStage).rollthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_BANKRIGHT);
	if (stage.at(currentStage).linearthrust > 0) {

		if (stage.at(currentStage).linearisp <= 0) { stage.at(currentStage).linearisp = stage.at(currentStage).isp * 100; }

		stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 0), _V(0, 0, 1), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);
		stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, 0), _V(0, 0, 1), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);
		CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_FORWARD);

		stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 0), _V(0, 0, -1), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);
		stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, 0), _V(0, 0, -1), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);
		CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_BACK);

		stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 0), _V(-1, 0, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);
		stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, 0), _V(-1, 0, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);
		CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_LEFT);

		stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);
		stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);
		CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_RIGHT);

		stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 0), _V(0, 1, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);
		stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, 0), _V(0, 1, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);
		CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_UP);

		stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 0), _V(0, -1, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);
		stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, 0), _V(0, -1, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);
		CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_DOWN);
	}

}

//Creates Main Thrusters and relevant Exhausts
void Multistage2026::CreateMainThruster() {

	if (stage.at(currentStage).nEngines == 0) {    //if no stage engines are defined there will be anyway one displayed
		stage.at(currentStage).nEngines = 1;
		stage.at(currentStage).eng.at(0).x = 0;
		stage.at(currentStage).eng.at(0).y = 0;
		stage.at(currentStage).eng.at(0).z = -stage.at(currentStage).height * 0.5;

	}



	//added by rcraig42 to handle ispref in ini but no pressure, to default to earth -------------------------

	if ((stage.at(currentStage).ispref >= 0) && (stage.at(currentStage).pref == 0)) {
		stage.at(currentStage).pref = 101400.0;
	}

	//-------------------------------------------------------------------------------------------------------	

	 //modded by rcraig42 to add ispref and pref to createthruster -------------------------------------------------

	if (Misc.thrustrealpos) {
		for (int i = 0; i < stage.at(currentStage).nEngines; i++) {
			stage.at(currentStage).th_main_h.at(i) = CreateThruster(stage.at(currentStage).off, stage.at(currentStage).eng_dir, stage.at(currentStage).thrust / stage.at(currentStage).nEngines, stage.at(currentStage).tank, stage.at(currentStage).isp, stage.at(currentStage).ispref, stage.at(currentStage).pref);
		}
	} else {
		for (int i = 0; i < stage.at(currentStage).nEngines; i++) {
			stage.at(currentStage).th_main_h.at(i) = CreateThruster(_V(0, 0, 0), _V(0, 0, 1), stage.at(currentStage).thrust / stage.at(currentStage).nEngines, stage.at(currentStage).tank, stage.at(currentStage).isp, stage.at(currentStage).ispref, stage.at(currentStage).pref);
		}
	}

	thg_h_main = CreateThrusterGroup(stage.at(currentStage).th_main_h.data(), stage.at(currentStage).nEngines, THGROUP_MAIN);
	SetDefaultPropellantResource(stage.at(currentStage).tank);

	SURFHANDLE ChoosenTexture = nullptr; //Initialization of Texture


	int i;
	for (i = 0; i < stage.at(currentStage).nEngines; i++) {

		exhaustN.at(currentStage).at(i) = AddExhaust(stage.at(currentStage).th_main_h.at(i), 10 * stage.at(currentStage).eng_diameter * stage.at(currentStage).engV4.at(i).t, stage.at(currentStage).eng_diameter * stage.at(currentStage).engV4.at(i).t, stage.at(currentStage).eng.at(i), operator*(stage.at(currentStage).eng_dir, -1), GetProperExhaustTexture(stage.at(currentStage).eng_tex));

		if (!stage.at(currentStage).ParticlesPacked) {
			if (stage.at(currentStage).wps1) {
				PARTICLESTREAMSPEC Pss1 = GetProperPS(stage.at(currentStage).eng_pstream1).Pss;
				AddExhaustStreamGrowing(stage.at(currentStage).th_main_h.at(i), stage.at(currentStage).eng.at(i), &Pss1, GetProperPS(stage.at(currentStage).eng_pstream1).Growing, GetProperPS(stage.at(currentStage).eng_pstream1).GrowFactor_size, GetProperPS(stage.at(currentStage).eng_pstream1).GrowFactor_rate, true, false, currentStage, i);

				oapiWriteLogV("%s: Stage n.%i Engine Exhaust Stream Added: %s to engine n.%i", GetName(), currentStage + 1, stage.at(currentStage).eng_pstream1.c_str(), i + 1);
			}
			if (stage.at(currentStage).wps2) {
				PARTICLESTREAMSPEC Pss2 = GetProperPS(stage.at(currentStage).eng_pstream2).Pss;
				AddExhaustStreamGrowing(stage.at(currentStage).th_main_h.at(i), stage.at(currentStage).eng.at(i), &Pss2, GetProperPS(stage.at(currentStage).eng_pstream2).Growing, GetProperPS(stage.at(currentStage).eng_pstream2).GrowFactor_size, GetProperPS(stage.at(currentStage).eng_pstream2).GrowFactor_rate, true, false, currentStage, i);

				oapiWriteLogV("%s: Stage n.%i Engine Exhaust Stream Added: %s to engine n.%i", GetName(), currentStage + 1, stage.at(currentStage).eng_pstream2.c_str(), i + 1);
			}
		}
		oapiWriteLogV("%s: Stage n. %i Engines Exhaust Added--> number of engines: %i , diameter: %.3f, position x: %.3f y: %.3f z: %.3f", GetName(), currentStage + 1, stage.at(currentStage).nEngines, stage.at(currentStage).eng_diameter * stage.at(currentStage).engV4.at(i).t, stage.at(currentStage).eng.at(i).x, stage.at(currentStage).eng.at(i).y, stage.at(currentStage).eng.at(i).z);

	}

	if (stage.at(currentStage).ParticlesPacked)
	{
		std::array<PARTICLESTREAMSPEC, 2> partpacked;
		partpacked.at(0) = GetProperPS(stage.at(currentStage).eng_pstream1).Pss;
		partpacked.at(1) = GetProperPS(stage.at(currentStage).eng_pstream2).Pss;


		int engine = abs(stage.at(currentStage).ParticlesPackedToEngine) - 1;

		VECTOR3 thdir; std::array<VECTOR3, 2> Pos;
		GetThrusterDir(stage.at(currentStage).th_main_h.at(engine), thdir);
		thdir.x *= -1;
		thdir.y *= -1;
		thdir.z *= -1;
		if (stage.at(currentStage).ParticlesPackedToEngine > 0) {
			Pos.at(0) = stage.at(currentStage).eng.at(engine);
			Pos.at(1) = stage.at(currentStage).eng.at(engine);
		} else {
			double Posx = 0;
			double Posy = 0;
			double Posz = 0;
			for (int x = 0; x < stage.at(currentStage).nEngines; x++)
			{
				Posx += stage.at(currentStage).eng.at(x).x;
				Posy += stage.at(currentStage).eng.at(x).y;
				Posz += stage.at(currentStage).eng.at(x).z;
			}

			Posx /= stage.at(currentStage).nEngines;
			Posy /= stage.at(currentStage).nEngines;
			Posz /= stage.at(currentStage).nEngines;

			Pos.at(0).x = Posx;
			Pos.at(0).y = Posy;
			Pos.at(0).z = Posz;
			Pos.at(1) = Pos.at(0);


		}
		if (stage.at(currentStage).wps1) {
			AddExhaustStreamGrowing(stage.at(currentStage).th_main_h.at(engine), Pos.at(0), &partpacked.at(0), GetProperPS(stage.at(currentStage).eng_pstream1).Growing, GetProperPS(stage.at(currentStage).eng_pstream1).GrowFactor_size, GetProperPS(stage.at(currentStage).eng_pstream1).GrowFactor_rate, true, false, currentStage, engine);
			oapiWriteLogV("%s: Stage n.%i Engine Packed Exhaust Stream Added: %s to engine n.%i", GetName(), currentStage + 1, stage.at(currentStage).eng_pstream1.c_str(), engine + 1);
		}
		if (stage.at(currentStage).wps2) {
			AddExhaustStreamGrowing(stage.at(currentStage).th_main_h.at(engine), Pos.at(1), &partpacked.at(1), GetProperPS(stage.at(currentStage).eng_pstream2).Growing, GetProperPS(stage.at(currentStage).eng_pstream2).GrowFactor_size, GetProperPS(stage.at(currentStage).eng_pstream2).GrowFactor_rate, true, false, currentStage, engine);
			oapiWriteLogV( "%s: Stage n.%i Engine Packed Exhaust Stream Added: %s to engine n.%i", GetName(), currentStage + 1, stage.at(currentStage).eng_pstream2.c_str(), engine + 1);
		}

	}




	if (stage.at(currentStage).DenyIgnition) {
		for (int i = 0; i < stage.at(currentStage).nEngines; i++)
		{
			SetThrusterResource(stage.at(currentStage).th_main_h.at(i), nullptr);
		}
	}


	LightEmitter* le = AddPointLight(stage.at(currentStage).eng.at(0), 200, 1e-3, 0, 2e-3, col_d, col_s, col_a);
	le->SetIntensityRef(&th_main_level);

	return;
}

//Get Boosters Position given group number and booster number inside the group
VECTOR3 Multistage2026::GetBoosterPos(int nBooster, int N) {

	VECTOR3 bpos = booster.at(nBooster).off;
	double arg = booster.at(nBooster).angle * RAD + (N - 1) * 2 * PI / booster.at(nBooster).N;
	VECTOR3 bposdef = _V(bpos.x * cos(arg) - bpos.y * sin(arg), bpos.x * sin(arg) + bpos.y * cos(arg), bpos.z);

	return bposdef;
}

std::string Multistage2026::GetProperPayloadMeshName(int pnl, int n){

	if (n == 0) {
		return payload.at(pnl).meshname0;
	}
	else if (n == 1) {
		return payload.at(pnl).meshname1;
	}
	else if (n == 2) {
		return payload.at(pnl).meshname2;
	}
	else if (n == 3) {
		return payload.at(pnl).meshname3;
	}
	else if (n == 4) {
		return payload.at(pnl).meshname4;
	}
	else {
		return payload.at(pnl).meshname0;
	}
}

void Multistage2026::RotatePayload(int pns, int nm, VECTOR3 anglesrad) {

	VECTOR3 state = _V(anglesrad.x / (2 * PI), anglesrad.y / (2 * PI), anglesrad.z / (2 * PI));//portion of 2*PI to rotate
	VECTOR3 reference;

	if (nm == 0) { reference = _V(0, 0, 0); }
	else { reference = operator-(payload.at(pns).off.at(nm), payload.at(pns).off.at(0)); }

	if (!DeveloperMode) {
		payloadrotatex.at(pns).at(nm) = new MGROUP_ROTATE(payload.at(pns).msh_idh.at(nm), NULL, NULL, reference, _V(1, 0, 0), (float)2 * PI);
		RotatePayloadAnim_x.at(pns).at(nm) = CreateAnimation(0);
		anim_x.at(pns).at(nm) = AddAnimationComponent(RotatePayloadAnim_x.at(pns).at(nm), 0.0f, 1.0f, payloadrotatex.at(pns).at(nm));
		payloadrotatey.at(pns).at(nm) = new MGROUP_ROTATE(payload.at(pns).msh_idh.at(nm), NULL, NULL, reference, _V(0, 1, 0), (float)2 * PI);
		RotatePayloadAnim_y.at(pns).at(nm) = CreateAnimation(0);
		anim_y.at(pns).at(nm) = AddAnimationComponent(RotatePayloadAnim_y.at(pns).at(nm), 0.0f, 1.0f, payloadrotatey.at(pns).at(nm));

		payloadrotatez.at(pns).at(nm) = new MGROUP_ROTATE(payload.at(pns).msh_idh.at(nm), NULL, NULL, reference, _V(0, 0, 1), (float)2 * PI);
		RotatePayloadAnim_z.at(pns).at(nm) = CreateAnimation(0);
		anim_z.at(pns).at(nm) = AddAnimationComponent(RotatePayloadAnim_z.at(pns).at(nm), 0.0f, 1.0f, payloadrotatez.at(pns).at(nm));

	} else {
		SetAnimation(RotatePayloadAnim_z.at(pns).at(nm), 0);
		SetAnimation(RotatePayloadAnim_y.at(pns).at(nm), 0);
		SetAnimation(RotatePayloadAnim_x.at(pns).at(nm), 0);
	}


	SetAnimation(RotatePayloadAnim_x.at(pns).at(nm), state.x);
	SetAnimation(RotatePayloadAnim_y.at(pns).at(nm), state.y);
	SetAnimation(RotatePayloadAnim_z.at(pns).at(nm), state.z);


	return;
}

VECTOR3 Multistage2026::RotateVector(const VECTOR3& input, double angle, const VECTOR3& rotationaxis)
{
	// To rotate a vector in 3D space we'll need to build a matrix, these are the variables treqired to do so.
	MATRIX3 rMatrix;
	double c = cos(angle);
	double s = sin(angle);
	double t = 1.0 - c;
	double x = rotationaxis.x;
	double y = rotationaxis.y;
	double z = rotationaxis.z;

	// Build rotation matrix
	rMatrix.m11 = (t * x * x + c);
	rMatrix.m12 = (t * x * y - s * z);
	rMatrix.m13 = (t * x * z + s * y);
	rMatrix.m21 = (t * x * y + s * z);
	rMatrix.m22 = (t * y * y + c);
	rMatrix.m23 = (t * y * z - s * x);
	rMatrix.m31 = (t * x * z - s * y);
	rMatrix.m32 = (t * y * z + s * x);
	rMatrix.m33 = (t * z * z + c);

	// Perform Rotation
	VECTOR3 output = mul(rMatrix, input); // multiply the input vector by our rotation matrix to get our output vector
	return output; // Return rotated vector
}

//Load Meshes
void Multistage2026::LoadMeshes() {

	int q;
	for (q = currentStage; q < nStages; q++) {
		VECTOR3 pos = stage.at(q).off;
		stage.at(q).msh_h = oapiLoadMeshGlobal(stage.at(q).meshname.c_str());
		oapiWriteLogV("%s: Stage n.%i Mesh Preloaded: %s", GetName(), q + 1, stage.at(q).meshname.c_str());
		stage.at(q).msh_idh = AddMesh(stage.at(q).msh_h, &pos);
		oapiWriteLogV("%s: Stage n.%i Mesh Added Mesh: %s @ x:%.3f y:%.3f z:%.3f", GetName(), q + 1, stage.at(q).meshname.c_str(), pos.x, pos.y, pos.z);
		if (stage.at(q).wInter == true) {

			VECTOR3 inspos = stage.at(q).interstage.off;
			std::string logvec = std::format("[LOADMESH] Interstage off: {}, {}, {}", inspos.x, inspos.y, inspos.z);
			oapiWriteLog(const_cast<char*>(logvec.c_str()));
			stage.at(q).interstage.msh_h = oapiLoadMeshGlobal(stage.at(q).interstage.meshname.c_str());
			oapiWriteLogV("%s: Interstage Mesh Preloaded for Stage %i", GetName(), q + 1);
			stage.at(q).interstage.msh_idh = AddMesh(stage.at(q).interstage.msh_h, &inspos);
			oapiWriteLogV("%s: Interstage Mesh Added: %s @ x:%.3f y:%.3f z:%.3f", GetName(), stage.at(q).interstage.meshname.c_str(), inspos.x, inspos.y, inspos.z);
		}
	}



	for (int pns = currentPayload; pns < nPayloads; pns++) {

		if (!payload.at(pns).live) {
			for (int nm = 0; nm < payload.at(pns).nMeshes; nm++) {

				VECTOR3 pos = payload.at(pns).off.at(nm);
				std::string meshname = GetProperPayloadMeshName(pns, nm);
				payload.at(pns).msh_h.at(nm) = oapiLoadMeshGlobal(meshname.c_str());
				oapiWriteLogV("%s Payload Mesh Preloaded %i", GetName(), pns + 1);
				payload.at(pns).msh_idh.at(nm) = AddMesh(payload.at(pns).msh_h.at(nm), &pos);
				meshname = GetProperPayloadMeshName(pns, nm);
				oapiWriteLogV("%s: Payload n.%i Mesh Added: %s @ x:%.3f y:%.3f z:%.3f", GetName(), pns + 1, meshname.c_str(), pos.x, pos.y, pos.z);
				if ((payload.at(pns).render == 0) && (wFairing == 1)) {
					SetMeshVisibilityMode(payload.at(pns).msh_idh.at(nm), MESHVIS_NEVER);
				}
				//if(payload.at(pns).rotated){
				RotatePayload(pns, nm, payload.at(pns).Rotation);
				//	}


				/// ATTACHMENT POINTS MUST BE CREATED ANYWAY TO GET THE PAYLOAD BACK ONCE RELOADING THE SCENARIO
			}
		}//else{
		VECTOR3 direction, normal;
		if (!payload.at(pns).rotated) {
			direction = _V(0, 0, 1); normal = _V(0, 1, 0);
		} else {
			direction = payload.at(pns).Rotation;
			VECTOR3 rotation;
			rotation = payload.at(pns).Rotation;
			direction = mul(RotationMatrix(rotation), _V(0, 0, 1));
			normal = mul(RotationMatrix(rotation), _V(0, 1, 0));
			normalise(normal);
			normalise(direction);
		}

		live_a.at(pns) = CreateAttachment(false, payload.at(pns).off.at(0), direction, normal, "MS2015", false);

		//}
	}
	int qb;
	for (qb = currentBooster; qb < nBoosters; qb++) {
		VECTOR3 bpos = booster.at(qb).off;
		VECTOR3 bposxy = bpos;
		bposxy.z = 0;
		double bro = length(bposxy);
		int NN;
		for (NN = 1; NN < booster.at(qb).N + 1; NN++) {
			std::string boosmhname = std::format("{}_{}", booster.at(qb).meshname, NN);

			double arg = booster.at(qb).angle * RAD + (NN - 1) * 2 * PI / booster.at(qb).N;
			VECTOR3 bposdef = _V(bpos.x * cos(arg) - bpos.y * sin(arg), bpos.x * sin(arg) + bpos.y * cos(arg), bpos.z);
			booster.at(qb).msh_h.at(NN) = oapiLoadMeshGlobal(boosmhname.c_str());
			oapiWriteLogV("%s: Booster Mesh Preloaded: %s", GetName(), boosmhname.c_str());
			booster.at(qb).msh_idh.at(NN) = AddMesh(booster.at(qb).msh_h.at(NN), &bposdef);
			oapiWriteLogV("%s: Booster Mesh Added Mesh: %s @ x:%.3f y:%.3f z:%.3f", GetName(), boosmhname.c_str(), bposdef.x, bposdef.y, bposdef.z);
		}
	}

	if (wFairing == 1) {
		VECTOR3 fpos = fairing.off;
		VECTOR3 fposxy = fpos;
		fposxy.z = 0;
		double fro = length(fposxy);
		int NF;
		for (NF = 1; NF < fairing.N + 1; NF++) {
			std::string fairmshname = std::format("{}_{}", fairing.meshname, NF);
			VECTOR3 fposdef = _V(fro * cos(fairing.angle * RAD + (NF - 1) * 2 * PI / fairing.N), fro * sin(fairing.angle * RAD + (NF - 1) * 2 * PI / fairing.N), fpos.z);
			fairing.msh_h.at(NF) = oapiLoadMeshGlobal(fairmshname.c_str());
			oapiWriteLogV("%s: Fairing Mesh Preloaded: %s", GetName(), fairmshname.c_str());
			fairing.msh_idh.at(NF) = AddMesh(fairing.msh_h.at(NF), &fposdef);
			oapiWriteLogV("%s: Fairing Mesh Added Mesh: %s @ x:%.3f y:%.3f z:%.3f", GetName(), fairmshname.c_str(), fposdef.x, fposdef.y, fposdef.z);
		}
	}
	if (wAdapter == true) {
		VECTOR3 adappos = Adapter.off;
		Adapter.msh_h = oapiLoadMeshGlobal(Adapter.meshname.c_str());
		oapiWriteLogV("%s: Adapter Mesh Preloaded", GetName());
		Adapter.msh_idh = AddMesh(Adapter.msh_h, &adappos);
		oapiWriteLogV("%s: Adapter Mesh Added: %s @ x:%.3f y:%.3f z:%.3f", GetName(), Adapter.meshname.c_str(), adappos.x, adappos.y, adappos.z);

	}
	if (wLes == true)
	{
		VECTOR3 LesPos = Les.off;
		Les.msh_h = oapiLoadMeshGlobal(Les.meshname.c_str());
		oapiWriteLogV("%s: Les Mesh Preloaded", GetName());
		Les.msh_idh = AddMesh(Les.msh_h, &LesPos);
		oapiWriteLogV("%s: Les Mesh Added %s @ x:%.3f y:%.3f z:%.3f", GetName(), Les.meshname.c_str(), LesPos.x, LesPos.y, LesPos.z);
	}

	return;
}

//Updates PMI, Cross Sections etc.
void Multistage2026::UpdatePMI() {
	
	double TotalVolume = 0;
	TotalHeight = 0;
	int i;
	for (i = currentStage; i < nStages; i++) {
		TotalHeight += stage.at(i).height;
		stage.at(i).volume = stage.at(i).height * 0.25 * PI * stage.at(i).diameter * stage.at(i).diameter;
		TotalVolume += stage.at(i).volume;
		stage.at(i).interstage.volume = stage.at(i).interstage.height * 0.25 * PI * stage.at(i).interstage.diameter * stage.at(i).interstage.diameter;
		TotalVolume += stage.at(i).interstage.volume;
	}
	int q;
	double CSBoosters = 0;
	for (q = currentBooster; q < nBoosters; q++) {
		booster.at(q).volume = booster.at(q).N * booster.at(q).height * 0.25 * PI * booster.at(q).diameter * booster.at(q).diameter;
		CSBoosters += 0.25 * PI * booster.at(q).diameter * booster.at(q).diameter;
		//	TotalVolume+=booster.at(q).volume;
	}

	int k;
	for (k = currentPayload; k < nPayloads; k++) {
		//Not Considered in Height
		payload.at(k).volume = payload.at(k).height * 0.25 * PI * payload.at(k).diameter * payload.at(k).diameter;
		TotalVolume += payload.at(k).volume;

	}

	if (wFairing == 1) {
		//TotalHeight+=fairing.height; //Not Considered in Height
		fairing.volume = fairing.height * 0.25 * PI * fairing.diameter * fairing.diameter;
		TotalVolume += fairing.volume;
	}
	if (wLes == true) {
		Les.volume = Les.height * 0.25 * PI * Les.diameter * Les.diameter;
		TotalVolume += Les.volume;
	}
	double PhiEq = sqrt(4 * TotalVolume / (PI * TotalHeight));

	double CSX, CSY, CSZ;
	CSX = TotalHeight * PhiEq;
	CSY = CSX;
	CSZ = PI * 0.25 * PhiEq * PhiEq + CSBoosters;
	SetCrossSections(_V(CSX, CSY, CSZ));

	double IZ = (PhiEq * 0.5) * (PhiEq * 0.5) * 0.5;
	double IX, IY;
	IX = (3 * (PhiEq * 0.5) * (PhiEq * 0.5) + TotalHeight * TotalHeight) / 12;
	IY = IX;
	SetPMI(_V(IX, IY, IZ));
	//  SetSize(10*TotalHeight);
	if (Configuration == 0) {
		SetSize(stage.at(0).height * 0.5 + Misc.COG);
		//if(GetSize()<=0){SetSize(stage.at(0).height*0.5+Misc.COG);}
	}
	else {
		SetSize(TotalHeight);
	}
	//	oapiWriteLogV("####### SIZE: %.3f",GetSize());
	return;
}

//Update Mass of the Vehicle on call
void Multistage2026::UpdateMass() {

	double EM = stage.at(currentStage).emptymass;

	int s;
	for (s = currentStage + 1; s < nStages; s++) {
		EM += stage.at(s).emptymass;
		if (stage.at(s).wInter == true) {
			EM += stage.at(s).interstage.emptymass;
		}
	}

	int bs;
	for (bs = currentBooster; bs < nBoosters; bs++) {
		EM += (booster.at(bs).emptymass * booster.at(bs).N);
	}

	int pns;
	for (pns = currentPayload; pns < nPayloads; pns++) {
		EM += payload.at(pns).mass;
	}
	if (wFairing == 1) {
		EM += 2 * fairing.emptymass;
	}
	if (wAdapter == true) {
		EM += Adapter.emptymass;
	}
	if (wLes == true) {
		EM += Les.emptymass;
	}

	SetEmptyMass(EM);

	return;
}

//Update mesh offsets
void Multistage2026::UpdateOffsets() {
	currentDelta = stage.at(currentStage).off.z;
	int i;
	for (i = currentStage; i < nStages; i++) {
		stage.at(i).off.z -= currentDelta;
		if (stage.at(i).wInter == true) {
			stage.at(i).interstage.off.z -= currentDelta;
		}
	}
	int p;
	for (p = currentPayload; p < nPayloads; p++) {
		for (int s = 0; s < payload.at(p).nMeshes; s++) {
			payload.at(p).off.at(s).z -= currentDelta;
		}
	}

	int z;
	for (z = currentBooster; z < nBoosters; z++) {
		booster.at(z).off.z -= currentDelta;
	}

	if (wFairing == 1) {
		fairing.off.z -= currentDelta;
	}

	if (wAdapter == true) {
		Adapter.off.z -= currentDelta;
	}
	if (wLes == true) {
		Les.off.z -= currentDelta;
	}
	return;
}

void Multistage2026::UpdateLivePayloads() {

	for (int pns = currentPayload; pns < nPayloads; pns++) {
		if (payload.at(pns).live) {
			VESSELSTATUS2 vslive;
			memset(&vslive, 0, sizeof(vslive));
			vslive.version = 2;
			OBJHANDLE checklive = oapiGetVesselByName(payload.at(pns).name.data());
			if (oapiIsVessel(checklive)) {
				ATTACHMENTHANDLE liveatt;
				VESSEL3* livepl;
				livepl = (VESSEL4*)oapiGetVesselInterface(checklive);
				liveatt = livepl->CreateAttachment(true, _V(0, 0, 0), _V(0, 0, -1), _V(0, 1, 0), "MS2026", false);
				AttachChild(checklive, live_a.at(pns), liveatt);
				if (payload.at(pns).mass <= 0) {
					payload.at(pns).mass = livepl->GetMass();
				}
				if (payload.at(pns).height <= 0) {
					payload.at(pns).height = livepl->GetSize();
					payload.at(pns).diameter = payload.at(pns).height * 0.1;
				}
			} else {

				VESSEL4 *v;
				OBJHANDLE hObj;
				ATTACHMENTHANDLE liveatt;
				GetStatusEx(&vslive);
				hObj = oapiCreateVesselEx(payload.at(pns).name.c_str(), payload.at(pns).module.c_str(), &vslive);


				if (oapiIsVessel(hObj)) {
					v = (VESSEL4*)oapiGetVesselInterface(hObj);

					liveatt = v->CreateAttachment(true, _V(0, 0, 0), _V(0, 0, -1), _V(0, 1, 0), "MS2015", false);

					AttachChild(hObj, live_a.at(pns), liveatt);
					if (payload.at(pns).mass <= 0) {
						payload.at(pns).mass = v->GetMass();
					}
					if (payload.at(pns).height <= 0) {
						payload.at(pns).height = v->GetSize();
						payload.at(pns).diameter = payload.at(pns).height * 0.1;
					}
				}
			}
		}
	}
	UpdateMass();
	UpdatePMI();
	return;
}

//Returns the particlestream specification to use or the empty one if not found
PARTICLE Multistage2026::GetProperPS(const std::string &name){
	
	std::string lower_name = name;

	std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(),
        [](unsigned char c){ return std::tolower(c); }
    );

	std::string checktxt;
	int nt = 0;
	int k;
	for (nt = 0; nt < 16; nt++) {
		
		checktxt = Particle.at(nt).ParticleName;
		
		std::transform(checktxt.begin(), checktxt.end(), checktxt.begin(),
        [](unsigned char c){ return std::tolower(c); }
    	);

		if(name != checktxt){
			return Particle.at(nt);
		}

	}
	return Particle.at(15);
}

//returns the texture to be used or the empty one
SURFHANDLE Multistage2026::GetProperExhaustTexture(const std::string &name){

	std::string checktxt;
	int nt = 0;
	int k;
	for (nt = 0; nt < nTextures; nt++) {
		checktxt = tex.TextureName.at(nt);
		
		if (name != checktxt) {

			return tex.hTex.at(nt);
		}

	}

	return nullptr;

}

//creates Ullage engine and exhausts
void Multistage2026::CreateUllageAndBolts() {
	if (stage.at(currentStage).ullage.wUllage)
	{
		stage.at(currentStage).ullage.ignited = false;
		stage.at(currentStage).ullage.th_ullage = CreateThruster(_V(0, 0, 0), _V(0, 0, 1), stage.at(currentStage).ullage.thrust, stage.at(currentStage).tank, 100000);
		/*VECTOR3 *ullpos=new VECTOR3[stage.at(currentStage).ullage.N];
		VECTOR3 *ulldir=new VECTOR3[stage.at(currentStage).ullage.N];
*/
//double angleprog=0;
		for (int i = 0; i < stage.at(currentStage).ullage.N; i++){
			double ull_angle;
			if (IsOdd((int)stage.at(currentStage).ullage.N)){
				if (i < (int)(stage.at(currentStage).ullage.N * 0.5)) {
					ull_angle = stage.at(currentStage).ullage.angle * RAD + (i) * ((2 * PI / stage.at(currentStage).ullage.N) / stage.at(currentStage).ullage.rectfactor);

					//angleprog+=ull_angle;
				} else if (i == (int)(stage.at(currentStage).ullage.N * 0.5)) {
					ull_angle = PI + stage.at(currentStage).ullage.angle * RAD;
					/*sprintf(logbuff,"i: %i ull_angle: %.3f",i,ull_angle*DEG);
				 oapiWriteLog(logbuff);*/
				} else {
					ull_angle = stage.at(currentStage).ullage.angle * RAD + PI + (i - (int)(stage.at(currentStage).ullage.N * 0.5)) * ((2 * PI / stage.at(currentStage).ullage.N) / stage.at(currentStage).ullage.rectfactor);
					/*sprintf(logbuff,"i: %i ull_angle: %.3f",i,ull_angle*DEG);
					oapiWriteLog(logbuff);*/
				}
			} else {
				ull_angle = stage.at(currentStage).ullage.angle * RAD + (i) * (2 * PI / stage.at(currentStage).ullage.N);
			}
			VECTOR3 ulldir = RotateVecZ(stage.at(currentStage).ullage.dir, ull_angle);
			VECTOR3 ullpos = RotateVecZ(stage.at(currentStage).ullage.pos, ull_angle);
			//VECTOR3 ull_ofs=operator+(GetBoosterPos(bi,biii),RotateVecZ(booster.at(bi).eng.at(bii),angle));

			AddExhaust(stage.at(currentStage).ullage.th_ullage, stage.at(currentStage).ullage.length, stage.at(currentStage).ullage.diameter, ullpos, ulldir, GetProperExhaustTexture(stage.at(currentStage).ullage.tex));
		}
	}

	if (stage.at(currentStage).expbolt.wExpbolt)
	{
		PARTICLESTREAMSPEC Pss3 = GetProperPS(stage.at(currentStage).expbolt.pstream).Pss;
		stage.at(currentStage).expbolt.threxp_h = CreateThruster(stage.at(currentStage).expbolt.pos, stage.at(currentStage).expbolt.dir, 0, stage.at(currentStage).tank, 100000, 100000);
		AddExhaustStream(stage.at(currentStage).expbolt.threxp_h, &Pss3);
	}

	return;
}

//Setting up of the vehicle, it's the setclass caps, but called after the ini is parsed
void Multistage2026::VehicleSetup() {


	SetRotDrag(_V(0.7, 0.7, 0.06)); //Got from Vinka

	///PROPELLANTS
	int bk;
	for (bk = currentBooster; bk < nBoosters; bk++) {
		booster.at(bk).tank = CreatePropellantResource(booster.at(bk).fuelmass * booster.at(bk).N);
		oapiWriteLogV("%s: booster n. %i Tank Added: %.3f kg", GetName(), bk + 1, booster.at(bk).fuelmass * booster.at(bk).N);
	}

	int k;
	for (k = nStages - 1; k > currentStage - 1; k--) {
		stage.at(k).tank = CreatePropellantResource(stage.at(k).fuelmass);
		oapiWriteLogV("%s: Stage n. %i Tank Added: %.3f kg", GetName(), k + 1, stage.at(k).fuelmass);
	}

	////ISP
	int r;
	for (r = currentStage; r < nStages; r++) {
		stage.at(r).isp = stage.at(r).thrust * stage.at(r).burntime / stage.at(r).fuelmass;
	}

	int br;
	for (br = currentBooster; br < nBoosters; br++) {
		booster.at(br).isp = (booster.at(br).thrust * booster.at(br).N) * booster.at(br).burntime / (booster.at(br).fuelmass * booster.at(br).N);
	}

	if (psg) {
    delete[] psg;
    psg = nullptr;
	}

	nPsh = 0;
	for (int pp = 0; pp < nStages; pp++)
	{
		for (int ppp = 0; ppp < stage.at(pp).nEngines; ppp++)
		{
			nPsh++;
		}
	}
	for (int pb = 0; pb < nBoosters; pb++)
	{
		for (int ppb = 0; ppb < booster.at(pb).N; ppb++)
		{
			for (int ppbb = 0; ppbb < booster.at(pb).nEngines; ppbb++)
			{
				nPsh++;
			}
		}

	}
	nPsh *= 2;
	//if(psg){delete[] psg;}

	psg = new PSGROWING[nPsh];

	for (int ps = 0; ps < nPsh; ps++)
	{
		psg[ps].GrowFactor_rate = 0;
		psg[ps].GrowFactor_size = 0;
		psg[ps].growing = false;
		psg[ps].pos = _V(0, 0, 0);
		psg[ps].psh.at(0) = nullptr;
		psg[ps].psh.at(1) = nullptr;
		psg[ps].psh.at(2) = nullptr;

		psg[ps].th = nullptr;
		psg[ps].pss = Particle.at(15).Pss;
		psg[ps].status = 1;
		psg[ps].counting = false;
		psg[ps].doublepstime = 0;
		psg[ps].baserate = 0;
		psg[ps].basesize = 0;
		psg[ps].basepos = _V(0, 0, 0);
		psg[ps].FirstLoop = true;

	}
	/////MAIN THRUSTERS

	CreateMainThruster();

	//attitude thrusters
	CreateRCS();

	//add Boosters Engines
	int bi, bii, biii;
	for (bi = currentBooster; bi < nBoosters; bi++) {
		for (int bn = 0; bn < booster.at(bi).N; bn++)
		{
			VECTOR3 pos, dir;
			if (Misc.thrustrealpos) {
				pos = GetBoosterPos(bi, bn);
				dir = booster.at(bi).eng_dir;

			} else {
				pos = _V(0, 0, 0);
				dir = _V(0, 0, 1);
			}

			booster.at(bi).th_booster_h.at(bn) = CreateThruster(pos, dir, booster.at(bi).thrust, booster.at(bi).tank, booster.at(bi).isp);
		}
		booster.at(bi).Thg_boosters_h = CreateThrusterGroup(booster.at(bi).th_booster_h.data(), booster.at(bi).N, THGROUP_USER);

		Particle.at(13).Pss.srcsize = stage.at(0).diameter;
		Particle.at(14).Pss.srcsize = 0.5 * stage.at(0).diameter;

		if (booster.at(bi).nEngines == 0) {
			for (bii = 0; bii < booster.at(bi).N; bii++) {
				booster.at(bi).eng.at(bii) = _V(0, 0, -booster.at(bi).height * 0.5);
				VECTOR3 engofs = operator+(GetBoosterPos(bi, bii), booster.at(bi).eng.at(bii));
				AddExhaust(booster.at(bi).th_booster_h.at(bii), 10 * booster.at(bi).eng_diameter, booster.at(bi).eng_diameter, engofs, operator*(booster.at(bi).eng_dir, -1), GetProperExhaustTexture(booster.at(bi).eng_tex));

				if (booster.at(bi).wps1) {
					PARTICLESTREAMSPEC Pss4 = GetProperPS(booster.at(bi).eng_pstream1).Pss;
					AddExhaustStreamGrowing(booster.at(bi).th_booster_h.at(bii), engofs, &Pss4, GetProperPS(booster.at(bi).eng_pstream1).Growing, GetProperPS(booster.at(bi).eng_pstream1).GrowFactor_size, GetProperPS(booster.at(bi).eng_pstream1).GrowFactor_rate, true, true, bi, bii);
					oapiWriteLogV("%s: Booster Group n.%i Engine Exhaust Stream Added: %s to engine n.%i", GetName(), bi + 1, booster.at(bi).eng_pstream1.c_str(), bii + 1);
				}
				if (booster.at(bi).wps2) {
					PARTICLESTREAMSPEC Pss5 = GetProperPS(booster.at(bi).eng_pstream2).Pss;
					AddExhaustStreamGrowing(booster.at(bi).th_booster_h.at(bii), engofs, &Pss5, GetProperPS(booster.at(bi).eng_pstream2).Growing, GetProperPS(booster.at(bi).eng_pstream2).GrowFactor_size, GetProperPS(booster.at(bi).eng_pstream2).GrowFactor_rate, true, true, bi, bii);
					oapiWriteLogV("%s: Booster Group n.%i Engine Exhaust Stream Added: %s to engine n.%i", GetName(), bi + 1, booster.at(bi).eng_pstream2.c_str(), bii + 1);
				}

				oapiWriteLogV("%s: Booster Engines Exhaust Added--> Booster Group: %i number of engines: %i , diameter: %.3f, position x: %.3f y: %.3f z: %.3f", GetName(), bi + 1, booster.at(bi).nEngines, booster.at(bi).eng_diameter, engofs.x, engofs.y, engofs.z);
			}
		} else {

			for (bii = 0; bii < booster.at(bi).nEngines; bii++) {
				for (biii = 1; biii < booster.at(bi).N + 1; biii++) {
					double angle = booster.at(bi).angle * RAD + (biii - 1) * 2 * PI / booster.at(bi).N;

					VECTOR3 engofs = operator+(GetBoosterPos(bi, biii), RotateVecZ(booster.at(bi).eng.at(bii), angle));

					AddExhaust(booster.at(bi).th_booster_h.at(biii - 1), 10 * booster.at(bi).eng_diameter, booster.at(bi).eng_diameter, engofs, operator*(booster.at(bi).eng_dir, -1), GetProperExhaustTexture(booster.at(bi).eng_tex));

					if (booster.at(bi).wps1) {
						PARTICLESTREAMSPEC Pss6 = GetProperPS(booster.at(bi).eng_pstream1).Pss;
						AddExhaustStreamGrowing(booster.at(bi).th_booster_h.at(biii - 1), engofs, &Pss6, GetProperPS(booster.at(bi).eng_pstream1).Growing, GetProperPS(booster.at(bi).eng_pstream1).GrowFactor_size, GetProperPS(booster.at(bi).eng_pstream1).GrowFactor_rate, true, true, bi, biii - 1);
						oapiWriteLogV("%s: Booster Group n.%i Engine Exhaust Stream Added: %s to engine n.%i", GetName(), bi + 1, booster.at(bi).eng_pstream1.c_str(), biii);
					}
					if (booster.at(bi).wps2) {
						PARTICLESTREAMSPEC Pss7 = GetProperPS(booster.at(bi).eng_pstream2).Pss;
						AddExhaustStreamGrowing(booster.at(bi).th_booster_h.at(biii - 1), engofs, &Pss7, GetProperPS(booster.at(bi).eng_pstream2).Growing, GetProperPS(booster.at(bi).eng_pstream2).GrowFactor_size, GetProperPS(booster.at(bi).eng_pstream2).GrowFactor_rate, true, true, bi, biii - 1);
						oapiWriteLogV("%s: Booster Group n.%i Engine Exhaust Stream Added: %s to engine n.%i", GetName(), bi + 1, booster.at(bi).eng_pstream2.c_str(), biii);
					}

					oapiWriteLogV("%s: Booster Engines Exhaust Added--> Booster Group: %i number of engines: %i , diameter: %.3f, position x: %.3f y: %.3f z: %.3f", GetName(), bi + 1, booster.at(bi).nEngines, booster.at(bi).eng_diameter, engofs.x, engofs.y, engofs.z);
				}
			}
		}

		if (booster.at(bi).expbolt.wExpbolt){
			booster.at(bi).expbolt.threxp_h = CreateThruster(booster.at(bi).expbolt.pos, booster.at(bi).expbolt.dir, 0, booster.at(bi).tank, 100000, 100000);
			PARTICLESTREAMSPEC Pss8 = GetProperPS(booster.at(bi).expbolt.pstream).Pss;
			AddExhaustStream(booster.at(bi).expbolt.threxp_h, &Pss8);
		}
	}
	//Ullage
	CreateUllageAndBolts();
	
	CogElev = Misc.COG;

	//added by rcraig42 to set drag_factor to 1 if not set in ini ------------------------------------------------

	if (Misc.drag_factor <= 0) {
		Misc.drag_factor = 1;
	}

	SetCW(0.2 * Misc.drag_factor, 0.5, 1.5, 1.5);		// Modded to miltiply drag in z_pos direction by drag_factor

	//-----------------------------------------------------------------------------------------------------------
	std::array<VECTOR3, 3> intdpts;
	std::array<VECTOR3, 12> tdpts;
	
	 /////////////////NEW DEFINITION FOR RAMP WITH ATTACHMENT!!!!!
	intdpts.at(0) = _V(0, 1, 0);
	intdpts.at(2) = _V(-0.866, -0.5, 0);
	intdpts.at(1) = _V(0.866, -0.5, 0);

	for (int j = 0; j < 3; j++)
	{
		tdpts.at(j) = intdpts.at(j);

	}
	//////////////////////END//////////////////////////////////////

	tdpts.at(3) = tdpts.at(0);
	tdpts.at(4) = tdpts.at(1);
	tdpts.at(5) = tdpts.at(2);

	tdpts.at(3).z *= -0.1;
	tdpts.at(4).z *= -0.1;
	tdpts.at(5).z *= -0.1;

	std::array<TOUCHDOWNVTX, 12> td;
	for (int i = 0; i < 12; i++){
		td.at(i).pos = tdpts.at(i);
		td.at(i).damping = 3e6;
		td.at(i).mu = 3;
		td.at(i).mu_lng = 3;
		td.at(i).stiffness = 3e7;
	}

	SetTouchdownPoints(td.data(), 6);

	if (currentBooster < nBoosters) wBoosters = true;

	UpdateMass();
	UpdatePMI();

	return;
}

//Spawns Item
void Multistage2026::Spawn(int type, int current) {

	std::string mn;
	VESSELSTATUS2 vs;
	memset(&vs, 0, sizeof(vs));
	vs.version = 2;
	GetStatusEx(&vs);
	VECTOR3 ofs;
	VECTOR3 rofs, rvel = { vs.rvel.x, vs.rvel.y, vs.rvel.z }, vrot = { vs.vrot.x,vs.vrot.y,vs.vrot.z }, arot = { vs.arot.x,vs.arot.y,vs.arot.z };
	VECTOR3 vel;

	switch (type) {

		case TBOOSTER:
			int i;
			for (i = 1; i < booster.at(current).N + 1; i++) {
				GetMeshOffset(booster.at(current).msh_idh.at(i), ofs);

				vel = RotateVecZ(booster.at(current).speed, booster.at(current).angle * RAD + (i - 1) * 2 * PI / booster.at(current).N);
				Local2Rel(ofs, vs.rpos);
				GlobalRot(vel, rofs);
				vs.rvel.x = rvel.x + rofs.x;
				vs.rvel.y = rvel.y + rofs.y;
				vs.rvel.z = rvel.z + rofs.z;
				double arg = booster.at(current).angle * RAD + (i - 1) * 2 * PI / booster.at(current).N;
				vs.vrot.x = vrot.x + booster.at(current).rot_speed.x * cos(arg) - booster.at(current).rot_speed.y * sin(arg);
				vs.vrot.y = vrot.y + booster.at(current).rot_speed.x * sin(arg) + booster.at(current).rot_speed.y * cos(arg);
				vs.vrot.z = vrot.z + booster.at(current).rot_speed.z;

				std::string mn2 = std::format("{}_{}", booster.at(current).meshname, i);

				oapiCreateVesselEx(mn.c_str(), booster.at(current).module.c_str(), &vs);
				oapiWriteLogV("%s: Booster n.%i jettisoned name: %s @%.3f", GetName(), current + 1, mn.c_str(), MET);

			}
			break;
		case TSTAGE:

			GetMeshOffset(stage.at(current).msh_idh, ofs);
			vel = _V(stage.at(current).speed.x, stage.at(current).speed.y, stage.at(current).speed.z);
			Local2Rel(ofs, vs.rpos);
			GlobalRot(vel, rofs);
			vs.rvel.x = rvel.x + rofs.x;
			vs.rvel.y = rvel.y + rofs.y;
			vs.rvel.z = rvel.z + rofs.z;
			vs.vrot.x = vrot.x + stage.at(current).rot_speed.x;
			vs.vrot.y = vrot.y + stage.at(current).rot_speed.y;
			vs.vrot.z = vrot.z + stage.at(current).rot_speed.z;

			mn = stage.at(current).meshname;

			oapiCreateVesselEx(mn.c_str(), stage.at(current).module.c_str(), &vs);
			oapiWriteLogV("%s: Stage n.%i jettisoned name: %s @%.3f", GetName(), current + 1, mn.c_str(), MET);
			stage_ignition_time = MET;
			break;

		case TPAYLOAD:
			if (!payload.at(current).live) {
				GetMeshOffset(payload.at(current).msh_idh.at(0), ofs);
				vel = _V(payload.at(current).speed.x, payload.at(current).speed.y, payload.at(current).speed.z);
				Local2Rel(ofs, vs.rpos);
				GlobalRot(vel, rofs);
				vs.rvel.x = rvel.x + rofs.x;
				vs.rvel.y = rvel.y + rofs.y;
				vs.rvel.z = rvel.z + rofs.z;
				vs.vrot.x = vrot.x + payload.at(current).rot_speed.x;
				vs.vrot.y = vrot.y + payload.at(current).rot_speed.y;
				vs.vrot.z = vrot.z + payload.at(current).rot_speed.z;

				if (payload.at(current).rotated) {
					MATRIX3 RotMatrix, RotMatrix_Def;
					GetRotationMatrix(RotMatrix);
					VECTOR3 rotation;
					rotation = payload.at(current).Rotation;
					RotMatrix_Def = mul(RotMatrix, RotationMatrix(rotation));
					vs.arot.x = atan2(RotMatrix_Def.m23, RotMatrix_Def.m33);
					vs.arot.y = -asin(RotMatrix_Def.m13);
					vs.arot.z = atan2(RotMatrix_Def.m12, RotMatrix_Def.m11);
				}
				OBJHANDLE hpl;
				hpl = oapiCreateVesselEx(payload.at(current).name.c_str(), payload.at(current).module.c_str(), &vs);
				if (currentPayload + 1 == Misc.Focus) {
					oapiSetFocusObject(hpl);
				}
			} else {
				if (GetAttachmentStatus(live_a.at(current))) {
					OBJHANDLE live = GetAttachmentStatus(live_a.at(current));
					VESSEL4* v;
					v = (VESSEL4*)oapiGetVesselInterface(live);

					VECTOR3 dir, rot; //only for the get params
					GetAttachmentParams(live_a.at(current), ofs, dir, rot);
					DetachChild(live_a.at(current), 0);//length(payload.at(current).speed));

					vel = _V(payload.at(current).speed.x, payload.at(current).speed.y, payload.at(current).speed.z);
					Local2Rel(ofs, vs.rpos);
					GlobalRot(vel, rofs);
					vs.rvel.x = rvel.x + rofs.x;
					vs.rvel.y = rvel.y + rofs.y;
					vs.rvel.z = rvel.z + rofs.z;
					vs.vrot.x = vrot.x + payload.at(current).rot_speed.x;
					vs.vrot.y = vrot.y + payload.at(current).rot_speed.y;
					vs.vrot.z = vrot.z + payload.at(current).rot_speed.z;

					if (payload.at(current).rotated) {
						MATRIX3 RotMatrix, RotMatrix_Def;
						GetRotationMatrix(RotMatrix);
						VECTOR3 rotation;
						rotation = payload.at(current).Rotation;
						RotMatrix_Def = mul(RotMatrix, RotationMatrix(rotation));
						vs.arot.x = atan2(RotMatrix_Def.m23, RotMatrix_Def.m33);
						vs.arot.y = -asin(RotMatrix_Def.m13);
						vs.arot.z = atan2(RotMatrix_Def.m12, RotMatrix_Def.m11);
					}
					v->clbkSetStateEx(&vs);
					if (currentPayload + 1 == Misc.Focus) {
						oapiSetFocusObject(live);
					}
				}
			}
			oapiWriteLogV("%s: Payload n.%i jettisoned name: %s @%.3f", GetName(), current + 1, payload.at(current).name.c_str(), MET);

			break;
		case TFAIRING:

			int ii;
			for (ii = 1; ii < fairing.N + 1; ii++) {

				GetMeshOffset(fairing.msh_idh.at(ii), ofs);

				//double vro=length(fairing.speed);
				//vel =_V(vro*cos(fairing.angle*RAD+(ii-1)*2*PI/fairing.N),vro*sin(fairing.angle*RAD+(ii-1)*2*PI/fairing.N),fairing.speed.z);
				vel = RotateVecZ(fairing.speed, fairing.angle * RAD + (ii - 1) * 2 * PI / fairing.N);

				Local2Rel(ofs, vs.rpos);
				GlobalRot(vel, rofs);
				vs.rvel.x = rvel.x + rofs.x;
				vs.rvel.y = rvel.y + rofs.y;
				vs.rvel.z = rvel.z + rofs.z;

				double arg = (ii - 1) * 2 * PI / fairing.N;
				vs.vrot.x = vrot.x + fairing.rot_speed.x * cos(arg) - fairing.rot_speed.y * sin(arg);
				vs.vrot.y = vrot.y + fairing.rot_speed.x * sin(arg) + fairing.rot_speed.y * cos(arg);
				vs.vrot.z = vrot.z + fairing.rot_speed.z;

				std::string mn2 = std::format("{}_{}", fairing.meshname, ii);

				oapiCreateVesselEx(mn.c_str(), fairing.module.c_str(), &vs);
				oapiWriteLogV("%s: Fairing jettisoned: name %s @%.3f", GetName(), mn.c_str(), MET);
			}

			break;
		case TINTERSTAGE:

			GetMeshOffset(stage.at(current).interstage.msh_idh, ofs);

			vel = _V(stage.at(current).interstage.speed.x, stage.at(current).interstage.speed.y, stage.at(current).interstage.speed.z);
			Local2Rel(ofs, vs.rpos);
			GlobalRot(vel, rofs);
			vs.rvel.x = rvel.x + rofs.x;
			vs.rvel.y = rvel.y + rofs.y;
			vs.rvel.z = rvel.z + rofs.z;
			vs.vrot.x = vrot.x + stage.at(current).interstage.rot_speed.x;
			vs.vrot.y = vrot.y + stage.at(current).interstage.rot_speed.y;
			vs.vrot.z = vrot.z + stage.at(current).interstage.rot_speed.z;

			mn = stage.at(current).interstage.meshname;

			oapiCreateVesselEx(mn.c_str(), stage.at(current).interstage.module.c_str(), &vs);
			oapiWriteLogV("%s: Interstage of stage %i jettisoned name: %s @%.3f", GetName(), current + 1, mn.c_str(), MET);

			break;
		case TLES:

			GetMeshOffset(Les.msh_idh, ofs);

			vel = _V(Les.speed.x, Les.speed.y, Les.speed.z);
			Local2Rel(ofs, vs.rpos);
			GlobalRot(vel, rofs);
			vs.rvel.x = rvel.x + rofs.x;
			vs.rvel.y = rvel.y + rofs.y;
			vs.rvel.z = rvel.z + rofs.z;
			vs.vrot.x = vrot.x + Les.rot_speed.x;
			vs.vrot.y = vrot.y + Les.rot_speed.y;
			vs.vrot.z = vrot.z + Les.rot_speed.z;

			mn = Les.meshname;

			oapiCreateVesselEx(mn.c_str(), Les.module.c_str(), &vs);
			oapiWriteLogV("%s: Les jettisoned name: %s @%.3f", GetName(), mn.c_str(), MET);
			break;


		}
	return;
}

//Jettison item
void Multistage2026::Jettison(int type, int current){

	switch (type){

		case TBOOSTER:

			Spawn(type, current);

			int i;
			for (i = 1; i < booster.at(current).N + 1; i++) {
				DelMesh(booster.at(current).msh_idh.at(i));
			}

			DelThrusterGroup(booster.at(current).Thg_boosters_h, false);

			DelPropellantResource(booster.at(current).tank);
			currentBooster += 1;

			UpdateMass();
			UpdatePMI();

			if (currentBooster >= nBoosters) {
				wBoosters = false;
			}
			break;

		case TSTAGE:

			Spawn(type, current);


			DelMesh(stage.at(current).msh_idh);
			ClearThrusterDefinitions();
			DelPropellantResource(stage.at(current).tank);
			currentStage += 1;

			UpdateMass();
			UpdatePMI();
			CreateUllageAndBolts();
			CreateMainThruster();
			CreateRCS();
			ShiftCG(_V(0, 0, (stage.at(current + 1).off.z - stage.at(current).off.z)));


			SetCameraOffset(_V(0, 0, 0));

			break;

		case TPAYLOAD:

			Spawn(type, current);

			if (!payload.at(current).live) {
				for (int ss = 0; ss < payload.at(current).nMeshes; ss++){
					DelMesh(payload.at(current).msh_idh[ss]);
				}
			}
			currentPayload += 1;
			UpdateMass();
			UpdatePMI();

			break;

		case TFAIRING:
			Spawn(type, current);
			int pns;
			for (pns = currentPayload; pns < nPayloads; pns++) {
				if (!payload.at(pns).live) {
					for (int s = 0; s < payload.at(pns).nMeshes; s++) {
						SetMeshVisibilityMode(payload.at(pns).msh_idh.at(s), MESHVIS_EXTERNAL);
					}
				}
			}
			int ii;
			for (ii = 1; ii < fairing.N + 1; ii++) {
				DelMesh(fairing.msh_idh.at(ii));
			}


			wFairing = 0;
			UpdateMass();
			UpdatePMI();
			break;
		case TLES:
			Spawn(type, current);
			DelMesh(Les.msh_idh);
			wLes = false;
			UpdateMass();
			UpdatePMI();
			break;
		case TINTERSTAGE:
			Spawn(type, current);
			DelMesh(stage.at(current).interstage.msh_idh);
			currentInterstage += 1;
			stage.at(current).wInter = false;
			UpdateMass();
			UpdatePMI();
			break;

		}
	return;
}

//initialize correctly the delays if flying from a scenario not starting on ground
void Multistage2026::InitializeDelays() {

	//initialize booster burn delays, only if stage is 0 and if Met is positive
	if ((currentStage == 0) && (MET > 0)) {
		for (int kb = currentBooster; kb < nBoosters; kb++) {
			if (booster.at(kb).currDelay > 0) {
				booster.at(kb).currDelay -= MET;
			}
		}
	}

	//initialize stages using already used by vinka STAGE_IGNITION_TIME even if it should be called "STAGE_JETTISONED_TIME"
	else if (currentStage > 0) {
		double delta = MET - stage_ignition_time;
		if (delta < stage.at(currentStage).currDelay) {
			stage.at(currentStage).currDelay -= delta;
		}

	}
	return;
}


void Multistage2026::AutoJettison() {

	if (currentBooster < nBoosters) {

		if (GetPropellantMass(booster.at(currentBooster).tank) <= 0.000001) {
			Jettison(TBOOSTER, currentBooster);

		}
	}

	if (currentStage < nStages - 1) {
		if ((currentStage == 0) && (currentBooster < nBoosters)) { return; }
		else if (GetPropellantMass(stage.at(currentStage).tank) <= 0.1) {
			Jettison(TSTAGE, currentStage);

		}

	}
	if ((stage.at(currentStage).wInter == true) && (stage.at(currentStage).interstage.currDelay <= 0)) {
		Jettison(TINTERSTAGE, currentStage);
	}
	return;
}

void Multistage2026::Guidance_Debug() {
	int step = VinkaGetStep(MET);
	double DesiredPitch;
	if (Gnc_step.at(step).GNC_Comand == CM_ROLL) {
		DesiredPitch = (Gnc_step.at(step).val_init + (Gnc_step.at(step).val_fin - Gnc_step.at(step).val_init) * (MET - Gnc_step.at(step).time_init) / ((Gnc_step[VinkaFindFirstPitch()].time - 1) - Gnc_step.at(step).time_init)) * RAD;//88*RAD;
		double heading;
		oapiGetHeading(GetHandle(), &heading);
		sprintf(oapiDebugString(), "MET: %.1f Step: %i P: %.2f (%.2f) H: %.2f (%.2f)", MET, step, GetPitch() * DEG, DesiredPitch * DEG, heading * DEG, VinkaAzimuth * DEG);
	}
	else if (Gnc_step.at(step).GNC_Comand == CM_PITCH) {
		DesiredPitch = (Gnc_step.at(step).val_init + (Gnc_step.at(step).val_fin - Gnc_step.at(step).val_init) * (MET - Gnc_step.at(step).time_init) / (Gnc_step.at(step).time_fin - Gnc_step.at(step).time_init)) * RAD;
		sprintf(oapiDebugString(), "MET: %.1f Step: %i P: %.2f (%.2f) Delta: %.1f", MET, step, GetPitch() * DEG, DesiredPitch * DEG, GetPitch() * DEG - DesiredPitch * DEG);
	}
	else {
		sprintf(oapiDebugString(), "MET: %.1f Step: %i", MET, step);
	}

	return;
}

void Multistage2026::ComplexFlight(){

	UpdateComplex += oapiGetSimStep();

	if (UpdateComplex >= 1){
		UpdateComplex = 0;

		for (int i = 0; i < stage.at(currentStage).nEngines; i++)
		{

			double newMax = (stage.at(currentStage).thrust / stage.at(currentStage).nEngines) * (1 + (stage.at(currentStage).engine_amp.at(i) * sin(2 * PI / stage.at(currentStage).freq.at(i) * MET + stage.at(currentStage).engine_phase.at(i))));
			SetThrusterMax0(stage.at(currentStage).th_main_h.at(i), newMax);
		}
		if (wBoosters)
		{
			for (int j = 0; j < booster.at(currentBooster).N; j++)
			{
				double newMax = booster.at(currentBooster).thrust * (1 + (booster.at(currentBooster).engine_amp.at(j) * sin(2 * PI / booster.at(currentBooster).freq.at(j) * MET + booster.at(currentBooster).engine_phase.at(j))));
				SetThrusterMax0(booster.at(currentBooster).th_booster_h.at(j), newMax);
			}
		}
	}


	return;
}

void Multistage2026::Boiloff()
{
	updtboiloff += oapiGetSimStep();

	if (updtboiloff >= 3600)
	{
		updtboiloff = 0;
		for (int i = currentStage; i < nStages; i++)
		{
			if (stage.at(i).wBoiloff)
			{
				double propmass = GetPropellantMass(stage.at(i).tank);
				propmass -= 1;
				SetPropellantMass(stage.at(i).tank, propmass);

			}
		}
	}
	return;
}

void Multistage2026::FLY(double simtime, double simdtime, double mjdate){
	if (APstat) {
		if (stage.at(currentStage).currDelay > 0) {
			 stage.at(currentStage).currDelay -= simdtime; stage.at(currentStage).StageState = STAGE_WAITING; 
			}
		if (stage.at(currentStage).interstage.currDelay > 0) {
			stage.at(currentStage).interstage.currDelay -= simdtime;
		}
	}
	if (wBoosters) {
		for (int nb = 0; nb < nBoosters; nb++) {
			if (booster.at(nb).currDelay > 0) {
				booster.at(nb).currDelay -= simdtime;
			}
		}
	}
	if (!AJdisabled) { 
		AutoJettison(); 
	}
	std::array<double, 10> Level;
	std::array<double, 10> btime;

	if ((!stage.at(currentStage).Ignited) && (stage.at(currentStage).currDelay <= 0) && (stage.at(currentStage).StageState == STAGE_WAITING)) {
		SetThrusterGroupLevel(THGROUP_MAIN, 1);
		stage.at(currentStage).Ignited = true;
		stage.at(currentStage).IgnitionTime = MET;
		stage.at(currentStage).StageState = STAGE_IGNITED;
		oapiWriteLogV("%s Stage n: %i ignited @%.1f", GetName(), currentStage + 1, stage.at(currentStage).IgnitionTime);
	}
	//MET+=simdtime;


	//BOOSTERS SECTION
	if ((wBoosters) && (GetThrusterGroupLevel(THGROUP_MAIN) > 0.95)) {

		int kb;
		for (kb = currentBooster; kb < nBoosters; kb++) {
			if (booster.at(kb).currDelay <= 0) {
				if (booster.at(kb).Ignited == false) {
					booster.at(kb).Ignited = true;
					booster.at(kb).IgnitionTime = MET;
					
					oapiWriteLogV("%s Booster n: %i ignited @%.1f", GetName(), kb + 1, booster.at(kb).IgnitionTime);
				} else {

					btime.at(kb) = MET - booster.at(kb).IgnitionTime;
					Level.at(kb) = 1;
					double m, q;
					int qq;
					for (qq = 0; qq < 10; qq++) {

						if (btime.at(kb) > booster.at(kb).curve.at(qq).x) {
							if (qq < 9) {

								if (btime.at(kb) < booster.at(kb).curve.at(qq + 1).x) {

									m = (booster.at(kb).curve.at(qq + 1).y - booster.at(kb).curve.at(qq).y) / (booster.at(kb).curve.at(qq + 1).x - booster.at(kb).curve.at(qq).x);
									q = booster.at(kb).curve.at(qq).y - m * booster.at(kb).curve.at(qq).x;
									Level.at(kb) = (m * btime.at(kb) + q) / 100;
								}

							} else {
								m = (booster.at(kb).curve.at(qq).y - booster.at(kb).curve.at(qq).y) / (booster.at(kb).curve.at(qq).x - booster.at(kb).curve.at(qq).x);
								q = booster.at(kb).curve.at(qq).y - m * booster.at(kb).curve.at(qq).x;
								Level.at(kb) = (m * btime.at(kb) + q) / 100;

							}
						}
					}
				}
				SetThrusterGroupLevel(booster.at(kb).Thg_boosters_h, Level.at(kb));
			}
		}
	}

	if (APstat) {
		VinkaAutoPilot();
		if (Misc.GNC_Debug == 1) {
			Guidance_Debug();
		}
		if (MET > VinkaFindEndTime()) {
			killAP();
			APstat = false;
		}

	}

	//avoid reignition of not reignitable stages by setting thurst to 0
	if ((!stage.at(currentStage).reignitable) && (stage.at(currentStage).Ignited))
	{

		if (GetThrusterGroupLevel(THGROUP_MAIN) == 0) {
			stage.at(currentStage).waitforreignition += simdtime;
			if (stage.at(currentStage).waitforreignition >= 3) {
				for (int i = 0; i < stage.at(currentStage).nEngines; i++){
					SetThrusterResource(stage.at(currentStage).th_main_h.at(i), nullptr);
				}
				stage.at(currentStage).DenyIgnition = true;

			}
		}
	}
	if (tlmidx < TLMSECS) {
		Telemetry();
	}

	if (stage.at(currentStage).batteries.wBatts){
		stage.at(currentStage).batteries.CurrentCharge -= oapiGetSimStep();
		if (stage.at(currentStage).batteries.CurrentCharge <= 0) {
			stage.at(currentStage).batteries.CurrentCharge = 0;
			ClearThrusterDefinitions();
		}
	}

	if ((Complex) && (GetDrag() > 1000)){
		ComplexFlight();
		AddForce(_V(0, 2 * GetDrag() * sin(GetAOA()), 0), _V(0, 0, TotalHeight));
		AddForce(_V(2 * GetDrag() * sin(GetSlipAngle()), 0, 0), _V(0, 0, TotalHeight));
		if (GetDrag() > 500000) { 
			if ((abs(GetAOA()) > 45 * RAD) || (abs(GetSlipAngle()) > 45 * RAD)) {
				boom();
			} 
		}
	}

	if (stage.at(currentStage).ullage.wUllage){
		if ((!stage.at(currentStage).ullage.ignited) && (stage.at(currentStage).currDelay < stage.at(currentStage).ullage.anticipation)){
			SetThrusterLevel(stage.at(currentStage).ullage.th_ullage, 1);
			stage.at(currentStage).ullage.ignited = true;
		} else if ((stage.at(currentStage).ullage.ignited) && (stage.at(currentStage).IgnitionTime != 0) && (MET - stage.at(currentStage).IgnitionTime > stage.at(currentStage).ullage.overlap)){
			SetThrusterLevel(stage.at(currentStage).ullage.th_ullage, 0);
		}
	}

	if (stage.at(currentStage).expbolt.wExpbolt){
		if (RemBurnTime(currentStage) < stage.at(currentStage).expbolt.anticipation){
			SetThrusterLevel(stage.at(currentStage).expbolt.threxp_h, 1);
		}
	}


	if (booster.at(currentBooster).expbolt.wExpbolt){
		if (BoosterRemBurnTime(currentBooster) < booster.at(currentBooster).expbolt.anticipation){
			SetThrusterLevel(booster.at(currentBooster).expbolt.threxp_h, 1);
		}
	}

	MET += simdtime;
	return;
}

double Multistage2026::CalculateFullMass(){
	double FM = 0;
	for (int i = 0; i < nStages; i++){
		FM += stage.at(i).emptymass;
		FM += stage.at(i).fuelmass;
		if (stage.at(i).wInter) {
			FM += stage.at(i).interstage.emptymass;
		}
	}
	for (int j = 0; j < nPayloads; j++){
		FM += payload.at(j).mass;
	}
	for (int q = 0; q < nBoosters; q++){
		FM += booster.at(q).fuelmass * booster.at(q).N;
		FM += booster.at(q).emptymass * booster.at(q).N;
	}
	if (hasFairing){
		FM += fairing.emptymass;
	}
	if (wAdapter){
		FM += Adapter.emptymass;
	}
	if (wLes){
		FM += Les.emptymass;
	}

	return FM;
}

//returns hours mins and secs of a time (positive and negative)
VECTOR3 Multistage2026::hms(double time){

	VECTOR3 met = _V(0, 0, 0);

	if (time == 0){
		met = _V(0, 0, 0);
	} else {
		time = abs(time + 0.5 * (time / abs(time) - 1));
		met.x = floor(time / 3600) - 0.5 * (time / abs(time) - 1);
		met.y = floor((time - met.x * 3600 * (time / abs(time))) / 60) - 0.5 * (time / abs(time) - 1);
		met.z = floor(time - met.x * 3600 * (time / abs(time)) - met.y * 60 * (time / abs(time)));
	}

	return met;
}

double Multistage2026::GetProperNforCGTE(double time){
	double n;
	double Thrust = stage.at(0).thrust;
	double BoosterFlow = 0;
	double BoosterFuelMassBurnt = 0;
	for (int i = 0; i < nBoosters; i++) {
		if ((booster.at(i).burndelay < time) && (time < (booster.at(i).burndelay + booster.at(i).burntime))) {
			Thrust += booster.at(i).thrust * booster.at(i).N;
			BoosterFlow = ((booster.at(i).fuelmass * booster.at(i).N) / booster.at(i).burntime);
			BoosterFuelMassBurnt = BoosterFlow * (time - booster.at(i).burndelay);
		}
	}
	double mass = CalculateFullMass();
	double FirstStageFlow = stage.at(0).fuelmass / stage.at(0).burntime;

	//	Thrust*=0.99; //account for Drag

	double substr = time * (FirstStageFlow)+BoosterFuelMassBurnt; //Grande approssimazione
	mass -= substr;
	n = Thrust / (mass * g0);

	return n;
}

bool Multistage2026::CGTE(double psi0) {
	double t0, v0, x0, y0;
	double deltaPsi0, z0, C, psi, z, v, deltaT, deltax, deltay, x, y;
	//t0=3;

	double Thrust = stage.at(0).thrust;
	double BoostersFlow = 0;
	for (int i = 0; i < nBoosters; i++) {
		if (booster.at(i).burndelay == 0) {  //accounting for boosters not immediatly ignited
			Thrust += booster.at(i).thrust * booster.at(i).N;
		}
	}
	double mass = CalculateFullMass();//GetMassAtStage(0,false);

	x0 = 0;

	y0 = Misc.COG;
	t0 = 0;
	v0 = 0;
	double dtt = 0.1;
	while ((y0 < altsteps[1]) && (y0 > 0)){
		double acceleration = GetProperNforCGTE(t0) * g0;
		y0 += 0.5 * (acceleration - g0) * dtt * dtt + v0 * dtt;
		t0 += dtt;
		v0 += (acceleration - g0) * dtt;

	}


	//	 gt_pitch_table[gt_step].Gtpitch=psi0;

	double hvel, vvel, vel, absacc, hacc, vacc, modspost, normx, normy;
	hvel = 0;
	vvel = v0;
	vel = v0;
	deltaT = 1;

	while ((t0 < 500) && (psi0 < 80 * RAD)) {

		/// OFFICIAL METHOD

		/// OFFICIAL METHOD FINISHES HERE!!

		 //// MY METHOD

		double grel = mu / ((rt + y0) * (rt + y0)) - (vel * vel / (rt + y0));
		double n = GetProperNforCGTE(t0);
		absacc = n * g0;
		hacc = absacc * sin(psi0);
		vacc = absacc * cos(psi0) - grel;
		x = x0 + hvel * deltaT + 0.5 * hacc * deltaT * deltaT;
		y = y0 + vvel * deltaT + 0.5 * vacc * deltaT * deltaT;
		hvel += hacc * deltaT;
		vvel += vacc * deltaT;
		//v=v0+absacc*deltaT;
		vel = sqrt(hvel * hvel + vvel * vvel);
		t0 += deltaT;
		deltax = x - x0;
		deltay = y - y0;
		x0 = x;
		y0 = y;

		v0 = vel;
		modspost = sqrt(deltax * deltax + deltay * deltay);
		normx = deltax / modspost;
		normy = deltay / modspost;
		psi = 0.5 * PI - atan2(normy, normx);
		if (psi > psi0) {
			psi0 = psi;
		}
		if (y0 > altsteps.at(3)) {
			return true;
		}
	}


	return false;
}

void Multistage2026::CheckForAdditionalThrust(int pns){
	if (GetAttachmentStatus(live_a.at(pns))) {
		OBJHANDLE live = GetAttachmentStatus(live_a.at(pns));
		VESSEL4* v;
		v = (VESSEL4*)oapiGetVesselInterface(live);
		VECTOR3 TotalThrustVecPL = _V(0, 0, 0);
		v->GetThrustVector(TotalThrustVecPL);
		TotalThrustVecPL = mul(RotationMatrix(payload.at(pns).Rotation), TotalThrustVecPL);
		AddForce(TotalThrustVecPL, payload.at(pns).off.at(0));
	}
}

void Multistage2026::CheckForFX(int fxtype, double param){

	switch (fxtype) {
		case FXMACH:
			if ((param > FX_Mach.mach_min) && (param < FX_Mach.mach_max))
			{
				if (!FX_Mach.added)
				{
					FX_Mach.added = true;
					for (int nmach = 0; nmach < FX_Mach.nmach; nmach++) {
						PARTICLESTREAMSPEC Pss9 = GetProperPS(FX_Mach.pstream).Pss;
						FX_Mach.ps_h.at(nmach) = AddParticleStream(&Pss9, FX_Mach.off.at(nmach), FX_Mach.dir, &lvl);
					}
				}
			} else {
				if (FX_Mach.added == true) {
					for (int nmach = 0; nmach < FX_Mach.nmach; nmach++)
					{
						DelExhaustStream(FX_Mach.ps_h.at(nmach));
					}
					FX_Mach.added = false;
				}
			}
			break;
		case FXVENT:
			for (int fv = 1; fv <= FX_Vent.nVent; fv++)
			{
				if (param < FX_Vent.time_fin.at(fv))
				{
					if (!FX_Vent.added.at(fv))
					{
					PARTICLESTREAMSPEC Pss10 = GetProperPS(FX_Vent.pstream).Pss;
						FX_Vent.ps_h.at(fv) = AddParticleStream(&Pss10, FX_Vent.off.at(fv), FX_Vent.dir.at(fv), &lvl);
						FX_Vent.added.at(fv) = true;
						oapiWriteLogV("Venting Effect Added @: %.3f,%.3f,%.3f dir: %.3f,%.3f,%.3f", FX_Vent.off.at(fv).x, FX_Vent.off.at(fv).y, FX_Vent.off.at(fv).z, FX_Vent.dir.at(fv).x, FX_Vent.dir.at(fv).y, FX_Vent.dir.at(fv).z);
					}
				} else {
					if (FX_Vent.added.at(fv) == true){
						DelExhaustStream(FX_Vent.ps_h.at(fv));
						FX_Vent.added.at(fv) = false;
					}
				}
			}
			break;
	}
}

void Multistage2026::EvaluateComplexFlight(){

	srand((unsigned)time(nullptr));

	for (int i = 0; i < nStages; i++){
		for (int q = 0; q < stage.at(i).nEngines; q++){
			int amplitude = rand() % 1500;
			stage.at(i).engine_amp.at(q) = (double)amplitude / 100000;
			int transval = rand() % 180;
			stage.at(i).engine_phase.at(q) = (double)transval * RAD;
			int frequency = rand() % 60;
			stage.at(i).freq.at(q) = 30 + (double)frequency;
			oapiWriteLogV("%s Complex Flight-> Stage %i Engine %i Amplitude %.3f phase %.3f frequency %.3f", GetName(), i + 1, q + 1, stage.at(i).engine_amp.at(q), stage.at(i).engine_phase.at(q), stage.at(i).freq.at(q));
		}
	}

	for (int j = 0; j < nBoosters; j++){
		for (int z = 0; z < booster.at(j).N; z++){
			int amplitude = rand() % 1500;
			booster.at(j).engine_amp.at(z) = (double)amplitude / 100000;
			int transval = rand() % 180;
			booster.at(j).engine_phase.at(z) = (double)transval * RAD;
			int frequency = rand() % 60;
			booster.at(j).freq.at(z) = 30 + (double)frequency;
			oapiWriteLogV("%s Complex Flight-> Booster %i Engine %i Amplitude %.3f phase %.3f frequency %.3f", GetName(), j + 1, z + 1, booster.at(j).engine_amp.at(z), booster.at(j).engine_phase.at(z), booster.at(j).freq.at(z));
		}
	}
	return;
}

void Multistage2026::FailuresEvaluation(){
	srand((unsigned)time(nullptr));
	int check = rand() % 1000;
	if (check < 10 * failureProbability){
		timeOfFailure = rand() % 300;
	}

	oapiWriteLogV("%s: Failures->Probabilty: %i Number Extracted: %i Time Of Failure: %i", GetName(), failureProbability, check / 10, timeOfFailure);
}

void Multistage2026::boom(){
	
	if ((wRamp) || (HangarMode)){
		DelAttachment(AttToRamp);
	}

	VESSELSTATUS2 vs;
	memset(&vs, 0, sizeof(vs));
	vs.version = 2;
	GetStatusEx(&vs);

	VECTOR3 ofs = _V(0, 0, 0);
	VECTOR3 rofs, rvel = { vs.rvel.x, vs.rvel.y, vs.rvel.z };
	VECTOR3 vel = { 0,0,0 };
	Local2Rel(ofs, vs.rpos);
	GlobalRot(vel, rofs);
	vs.rvel.x = rvel.x + rofs.x;
	vs.rvel.y = rvel.y + rofs.y;
	vs.rvel.z = rvel.z + rofs.z;

	OBJHANDLE hwreck;
	hwreck = oapiCreateVesselEx("wreck", "boom", &vs);
	oapiSetFocusObject(hwreck);
	oapiCameraScaleDist(20);
	if (wRamp){
		oapiDeleteVessel(hramp);
	}
	oapiDeleteVessel(GetHandle());
	return;
}

void Multistage2026::Failure(){

	srand((unsigned)time(nullptr));
	int check = rand() % 1000;
	if ((currentStage == 0) && (check < 250)){
		boom();
	} else {

		int engineout = rand() % stage.at(currentStage).nEngines;

		SetThrusterResource(stage.at(currentStage).th_main_h.at(engineout), nullptr);
	}
}

bool Multistage2026::CheckForFailure(double met){

	if ((floor(met) == timeOfFailure) && (!failed)){
		oapiWriteLogV("%s: FAILURE FAILURE FAILURE @%.0f", GetName(), met);
		failed = true;
		return true;
	} else {
		return false;
	}
}

int Multistage2026::WriteTelemetryFile(int initline){

	FILEHANDLE TlmFile;

	std::string filenmbuff = std::format("Multistage2026\\Telemetry\\%s_%.2f_TLM.txt", GetName(), oapiGetSysMJD());

	std::string buffer;

	if (initline == 0) {
		TlmFile = oapiOpenFile(filenmbuff.c_str(), FILE_OUT, CONFIG);
		buffer = "<--!Multistage 2015 Automatically Generated Telemetry File!-->";
		oapiWriteLine(TlmFile, const_cast<char *>(buffer.c_str()));
		buffer = "MET,Altitude,Speed,Pitch,Thrust,Mass,V-Speed,Acceleration";
		oapiWriteLine(TlmFile, const_cast<char *>(buffer.c_str()));
	} else {
		TlmFile = oapiOpenFile(filenmbuff.c_str(), FILE_APP, ROOT);
	}


	for (int i = initline; i < tlmidx; i++){
		buffer = std::format("{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f}", tlmAlt.at(i).x, tlmAlt.at(i).y, tlmSpeed.at(i).y, tlmPitch.at(i).y, tlmThrust.at(i).y, tlmMass.at(i).y, tlmVv.at(i).y, tlmAcc.at(i).y);
		oapiWriteLine(TlmFile, const_cast<char *>(buffer.c_str()));
	}
	if (initline == 0) {
		oapiCloseFile(TlmFile, FILE_OUT);
	} else {
		oapiCloseFile(TlmFile, FILE_APP);
	}
	return tlmidx - 1;
}

void Multistage2026::Telemetry(){

	if ((updtlm >= 1) && (MET < TLMSECS - 1)){

		VECTOR3 ThrustVec;
		GetThrustVector(ThrustVec);

		tlmAlt.at(tlmidx).x = MET;
		tlmSpeed.at(tlmidx).x = MET;
		tlmPitch.at(tlmidx).x = MET;
		tlmThrust.at(tlmidx).x = MET;
		tlmMass.at(tlmidx).x = MET;
		tlmVv.at(tlmidx).x = MET;
		tlmAcc.at(tlmidx).x = MET;

		tlmAlt.at(tlmidx).y = GetAltitude();
		tlmSpeed.at(tlmidx).y = GetOS();
		tlmPitch.at(tlmidx).y = GetPitch() * DEG;
		tlmThrust.at(tlmidx).y = length(ThrustVec);
		tlmMass.at(tlmidx).y = GetMass();
		tlmVv.at(tlmidx).y = GetVPerp();
		tlmAcc.at(tlmidx).y = getabsacc();

		tlmidx++;
		updtlm = 0;
	}
	updtlm += oapiGetSimStep();

	if (Misc.telemetry) {
		writetlmTimer += oapiGetSimStep();
		if (writetlmTimer >= 60){
			writetlmTimer = 0;
			tlmnlines = WriteTelemetryFile(tlmnlines);
		}
	}
	return;
}

void Multistage2026::CalculateAltSteps(double planetmass){
	std::array<double, 4> altref{100, 350, 1400, 35000};
	double earthmass = 5.973698968 * pow(10.0, 24);//1000000000000
	for (int i = 0; i < 4; i++){
		altsteps.at(i) = altref.at(i) * planetmass / earthmass;
	}
	return;
}

void Multistage2026::SetNewAltSteps(double newstep1, double newstep2, double newstep3, double newstep4){
	altsteps.at(0) = newstep1;
	altsteps.at(1) = newstep2;
	altsteps.at(2) = newstep3;
	altsteps.at(3) = newstep4;

	return;
}

void Multistage2026::ToggleComplexFlight(){
	if (Complex){
		Complex = false;
	} else {
		Complex = true;
		EvaluateComplexFlight();
	}
	
	return;
}
//****************************CALLBACKS******************************************************************************
void Multistage2026::clbkSetClassCaps(FILEHANDLE cfg){

	initGlobalVars();			// First of all let's initialize everything!

	SetSurfaceFrictionCoeff(0.7, 0.7);  // to avoid bad vibrations while throttling up at launch
	SetCW(0.2, 0.5, 1.5, 1.5);		// gotten from Vinka
	EnableTransponder(true);
	InitNavRadios(4);
	SetCameraOffset(_V(0, 0, 100));	//Empirical

	SetVisibilityLimit(0, 0.002);
	SetAlbedoRGB(_V((float)255 / 255, (float)255 / 255, (float)255 / 255));
	SetGravityGradientDamping(0);

	SetMaxWheelbrakeForce(200000);

	//Default ParticleStreams Definitions:

	// DEF CONTRAIL
	std::string transfer;
	transfer = "Contrail";

	Particle.at(13).ParticleName = transfer;
	Particle.at(13).Pss.flags = 0;
	Particle.at(13).Pss.srcsize = 8;
	Particle.at(13).Pss.srcrate = 5;
	Particle.at(13).Pss.v0 = 150;
	Particle.at(13).Pss.srcspread = 0.3;
	Particle.at(13).Pss.lifetime = 8;
	Particle.at(13).Pss.growthrate = 4;
	Particle.at(13).Pss.atmslowdown = 3;
	Particle.at(13).Pss.ltype = PARTICLESTREAMSPEC::EMISSIVE;
	Particle.at(13).Pss.levelmap = PARTICLESTREAMSPEC::LVL_PSQRT;
	Particle.at(13).Pss.lmin = 0;
	Particle.at(13).Pss.lmax = 0.5;
	Particle.at(13).Pss.atmsmap = PARTICLESTREAMSPEC::ATM_PLOG;
	Particle.at(13).Pss.amin = 1e-6;
	Particle.at(13).Pss.amax = 0.1;
	Particle.at(13).Pss.tex = oapiRegisterParticleTexture(const_cast<char*>("Contrail1"));

	//DEF EXHAUST
	transfer = "Exhaust";
	
	Particle.at(14).ParticleName = transfer;
	Particle.at(14).Pss.flags = 0;
	Particle.at(14).Pss.srcsize = 4;
	Particle.at(14).Pss.srcrate = 20;
	Particle.at(14).Pss.v0 = 150; //250
	Particle.at(14).Pss.srcspread = 0.1;
	Particle.at(14).Pss.lifetime = 0.3; //0.6
	Particle.at(14).Pss.growthrate = 12; //20
	Particle.at(14).Pss.atmslowdown = 2;
	Particle.at(14).Pss.ltype = PARTICLESTREAMSPEC::EMISSIVE;
	Particle.at(14).Pss.levelmap = PARTICLESTREAMSPEC::LVL_PSQRT;
	Particle.at(14).Pss.lmin = 0;
	Particle.at(14).Pss.lmax = 0.5;
	Particle.at(14).Pss.atmsmap = PARTICLESTREAMSPEC::ATM_PLOG;
	Particle.at(14).Pss.amin = 1e-6;
	Particle.at(14).Pss.amax = 0.1;
	Particle.at(14).Pss.tex = oapiRegisterParticleTexture(const_cast<char*>("Contrail3"));

	//DEF EXHAUST
	transfer = "Clear";
	Particle.at(15).ParticleName = transfer;
	Particle.at(15).Pss.flags = 0;
	Particle.at(15).Pss.srcsize = 0;
	Particle.at(15).Pss.srcrate = 0;
	Particle.at(15).Pss.v0 = 0; //250
	Particle.at(15).Pss.srcspread = 0;
	Particle.at(15).Pss.lifetime = 0; //0.6
	Particle.at(15).Pss.growthrate = 0; //20
	Particle.at(15).Pss.atmslowdown = 0;
	Particle.at(15).Pss.ltype = PARTICLESTREAMSPEC::DIFFUSE;
	Particle.at(15).Pss.levelmap = PARTICLESTREAMSPEC::LVL_FLAT;
	Particle.at(15).Pss.lmin = 0;
	Particle.at(15).Pss.lmax = 0;
	Particle.at(15).Pss.atmsmap = PARTICLESTREAMSPEC::ATM_FLAT;
	Particle.at(15).Pss.amin = 0;
	Particle.at(15).Pss.amax = 0;

}

void Multistage2026::clbkLoadStateEx(FILEHANDLE scn, void* vs){

	oapiWriteLogV("Multistage Version: %i", GetMSVersion());

	oapiWriteLog(const_cast<char*>("Load State Started"));
	std::string line;
	char *cp_line;

	double batt_trans = 0;
	bool loadedbatts = false;
	stepsloaded = false;

	while (oapiReadScenario_nextline(scn, cp_line)){
		line = cp_line;

		if (line.rfind("CONFIG_FILE", 0) == 0){
			fileini = line.substr(11);
		} else if (line.rfind("MET", 0) == 0){
			MET = std::stod(line.substr(3));
		} else if (line.rfind("GNC_RUN", 0) == 0){
			Gnc_running = std::stoi(line.substr(7));
			if (Gnc_running == 1){
				APstat = true;
			}
		} else if (line.rfind("BATTERY", 0) == 0){
			batt_trans = std::stod(line.substr(7));
			loadedbatts = true;
		} else if (line.rfind("STAGE_IGNITION_TIME", 0) == 0){
			stage_ignition_time = std::stod(line.substr(19));
		} else if (line.rfind("FAILURE_PROB", 0) == 0){
			failureProbability = std::stoi(line.substr(12));
		} else if (line.rfind("GNC_AUTO_JETTISON", 0) == 0){
			int AJVal = std::stoi(line.substr(17));
			if(AJVal == 0){
				AJdisabled = true;
			}
		} else if (line.rfind("GUIDANCE_FILE", 0) == 0){
			guidancefile = std::stoi(line.substr(13));
			parseGuidanceFile(guidancefile);

			if(Gnc_running != 1){
				VinkaCheckInitialMet();
			}
		} else if (line.rfind("TELEMETRY_FILE", 0) == 0){
			tlmfile = line.substr(14);
			parseTelemetryFile(tlmfile);
			wReftlm = true;
		} else if (line.rfind("CURRENT_BOOSTER", 0) == 0){
			currentBooster = std::stoi(line.substr(15));
			currentBooster -= 1;
		} else if (line.rfind("CURRENT_INTERSTAGE", 0) == 0){
			currentInterstage = std::stoi(line.substr(18));
			currentInterstage -= 1;
		} else if (line.rfind("CURRENT_STAGE", 0) == 0){
			currentStage = std::stoi(line.substr(13));
			currentStage -= 1;
			if (currentStage > 0){
				currentBooster = 11;
			}
		} else if (line.rfind("STAGE_STATE", 0) == 0){
			stage.at(currentStage).StageState = std::stoi(line.substr(11));
			if(stage.at(currentStage).StageState == STAGE_IGNITED){
				stage.at(currentStage).Ignited = true;
			}
		} else if (line.rfind("CURRENT_PAYLOAD", 0) == 0){
			currentPayload = std::stoi(line.substr(15));
			currentPayload -= 1;
		} else if (line.rfind("FAIRING", 0)){
			wFairing = std::stoi(line.substr(7));
			if ((wFairing < 0) || (wFairing > 1)){
				wFairing = 0;
			}
		} else if (line.rfind("CONFIGURATION", 0) == 0){
			Configuration = std::stoi(line.substr(13));
			if ((Configuration < 0) || (Configuration > 1)){
				Configuration = 0;
			}
		} else if (line.rfind("COMPLEX", 0) == 0){
			Complex = true;
		} else if (line.rfind("HANGAR", 0)){
			HangarMode = true;
		} else if (line.rfind("CRAWLER", 0)){
			wCrawler = true;
		} else if (line.rfind("CAMERA", 0) == 0){
			
			std::istringstream iss(line.substr(6));
			iss >> CamDLat >> CamDLng;

			if ((CamDLat == 0) && (CamDLng == 0)){
				CamDLat = 0.01; CamDLng = 0.03;
			}

			wCamera = true;
		} else if (line.rfind("GROWING_PARTICLES", 0) == 0){
			GrowingParticles = true;
		} else if (line.rfind("DENY_IGNITION", 0) == 0){
			stage.at(currentStage).DenyIgnition = true;
		} else if (line.rfind("ALT_STEPS", 0) == 0){
			std::istringstream iss2(line.substr(9));
			iss2 >> altsteps.at(0) >> altsteps.at(1) >> altsteps.at(2) >> altsteps.at(3);
			stepsloaded = true;
		} else if (line.rfind("PEG_PITCH_LIMIT", 0) == 0){
			PegPitchLimit = std::stod(line.substr(15));
			PegPitchLimit *= RAD;
		} else if (line.rfind("PEG_MC_INTERVAL", 0) == 0){
			PegMajorCycleInterval = std::stod(line.substr(15));
		} else if (line.rfind("RAMP", 0) == 0){
			wRamp = true;
		} else if (line.rfind("ATTMSPAD", 0) == 0){
			MsPadZ.z = std::stod(line.substr(8));
			AttToMSPad = true;
		} else {
			ParseScenarioLineEx(cp_line, vs);
		}
	}

	std::string tempFile = std::format("{}\\{}", OrbiterRoot, fileini);
	oapiWriteLogV("%s: Config File: %s", GetName(), tempFile.c_str());
	parseinifile(tempFile);

	if ((currentInterstage > currentStage) || (currentInterstage > nInterstages) || (currentInterstage >= stage.at(currentStage).IntIncremental)){
		stage.at(currentStage).wInter = false;
	}

	if ((wFairing == 1) && (hasFairing == false)){
		wFairing = 0;
	}

	if (Configuration == 0){
					//if only configuration is defined, reset everything
		if (hasFairing == true){
			wFairing = 1;
		}
		currentStage = 0;
		currentPayload = 0;
		currentBooster = 0;
		currentInterstage = 0;
	}


	UpdateOffsets();

	VehicleSetup();

	LoadMeshes();

	if (Gnc_running == 1) {
		InitializeDelays();
	}

	if (loadedbatts){
		stage.at(currentStage).batteries.CurrentCharge = batt_trans;
	}



	loadedCurrentBooster = currentBooster;
	loadedCurrentInterstage = currentInterstage;
	loadedCurrentStage = currentStage;
	loadedCurrentPayload = currentPayload;;
	loadedwFairing = wFairing;
	loadedConfiguration = Configuration;
	loadedMET = MET;
	loadedGrowing = GrowingParticles;

	oapiWriteLog(const_cast<char*>("Load State Terminated"));
}


int Multistage2026::clbkConsumeBufferedKey(DWORD key, bool down, char* kstate) {

	if (!down) return 0;

	if (KEYDOWN(kstate, OAPI_KEY_J) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {

		if (currentBooster < nBoosters) {
			Jettison(TBOOSTER, currentBooster);
		} else {
			if (currentStage < nStages - 1) {
				if (stage.at(currentStage).wInter == true) {
					Jettison(TINTERSTAGE, currentStage);
				} else { Jettison(TSTAGE, currentStage); }

			} else if ((currentStage == nStages - 1) && (stage.at(currentStage).wInter)) {
				Jettison(TINTERSTAGE, currentStage);
			} else {

				if ((currentPayload < nPayloads) && (wFairing == 0)) {
					Jettison(TPAYLOAD, currentPayload);
				} else {
					return 0;
				}
			}
		}

		return 1;
	}
	if (KEYDOWN(kstate, OAPI_KEY_F) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {
		if (wLes) {
			Jettison(TLES, 0);
		} else {
			if (wFairing == 1){
				Jettison(TFAIRING, 0);
			} else {
				return 0;
			}
		}
		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_P) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {
		if (!APstat){
			APstat = true; 
			return 0;
		} else if (APstat) {
			APstat = false; 
			return 0;
		} else {
			return 0;
		}
		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_SPACE) && (KEYMOD_CONTROL(kstate)) && (!KEYMOD_ALT(kstate))) {
		if ((!DeveloperMode) && (!killDMD)){
			DeveloperMode = true;
			note = oapiCreateAnnotation(true, 1, _V(1, 0, 0));
			oapiAnnotationSetPos(note, 0.65, 0.25, 0.975, 0.75);
			oapiAnnotationSetSize(note, 0.75);
			std::string TXT = std::format("{}: \nDEVELOPER MODE ON \n\n[SPACEBAR] to reload .ini file \n\n[CTRL]+[SPACEBAR] to close Developer Mode", GetName());
			oapiAnnotationSetText(note, const_cast<char *>(TXT.c_str()));
			CreateDMD();
		} else {
			killDMD = true;
			DeveloperMode = false;
			if (note) {
				oapiDelAnnotation(note);
			}
		}
		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_SPACE) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_ALT(kstate))) {
		if (DeveloperMode){
			ResetVehicle(hangaranims, wRamp);
		}
		return 1;
	}


	if (KEYDOWN(kstate, OAPI_KEY_L) && (KEYMOD_CONTROL(kstate)) && (!KEYMOD_ALT(kstate) && (!KEYMOD_SHIFT(kstate)))){
		if (HangarMode){
			char kstate[256];
			for (int i = 0; i < 256; i++) kstate[i] = 0x00;
			kstate[OAPI_KEY_L] = 0x80;
			kstate[OAPI_KEY_LCONTROL] = 0x80;
			vhangar->SendBufferedKey(OAPI_KEY_L, true, kstate);
			return 1;
		}
	}
	if (KEYDOWN(kstate, OAPI_KEY_D) && (KEYMOD_CONTROL(kstate)) && (!KEYMOD_ALT(kstate) && (!KEYMOD_SHIFT(kstate)))){
		if (HangarMode){
			char kstate[256];
			for (int i = 0; i < 256; i++) kstate[i] = 0x00;
			kstate[OAPI_KEY_D] = 0x80;
			kstate[OAPI_KEY_LCONTROL] = 0x80;
			vhangar->SendBufferedKey(OAPI_KEY_D, true, kstate);

			if (!AttToMSPad) {
				OBJHANDLE hPad = oapiGetObjectByName(const_cast<char*>("MS_Pad"));
				if (oapiIsVessel(hPad)){
					AttachToMSPad(hPad);
					AttToMSPad = true;
				}
			}
			return 1;
		}
	}

	return 0;
}

void Multistage2026::AttachToMSPad(OBJHANDLE hPad){
	VESSEL4* vPad;
	vPad = (VESSEL4*)oapiGetVesselInterface(hPad);
	padramp = vPad->CreateAttachment(false, _V(0, 0, 0), _V(0, 1, 0), _V(0, 0, 1), "Pad", false);

	AttToRamp = CreateAttachment(true, MsPadZ, _V(0, 0, -1), _V(0, 1, 0), "Pad", false);

	vPad->AttachChild(GetHandle(), padramp, AttToRamp);
}

int Multistage2026::clbkConsumeDirectKey(char* kstate){
	if (HangarMode){
		vhangar->clbkConsumeDirectKey(kstate);
		if (AttToMSPad) {
			VECTOR3 pos, dir, rot;
			GetAttachmentParams(AttToRamp, pos, dir, rot);
			const double delta = 0.1;
			if (KEYDOWN(kstate, OAPI_KEY_UP) && (!KEYMOD_CONTROL(kstate)) && (KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {
				pos.z -= delta;
				SetAttachmentParams(AttToRamp, pos, dir, rot);
			}
			if (KEYDOWN(kstate, OAPI_KEY_DOWN) && (!KEYMOD_CONTROL(kstate)) && (KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {
				pos.z += delta;
				SetAttachmentParams(AttToRamp, pos, dir, rot);
			}
		}
	}
	return 0;
}
