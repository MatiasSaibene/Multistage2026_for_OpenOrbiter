#include "Multistage2026.hpp"
#include <array>
#include <filesystem>
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
	psg[nPsg].psh[2] = psh;
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

		exhaustN.at(currentStage).at(i) = AddExhaust(stage.at(currentStage).th_main_h.at(i), 10 * stage.at(currentStage).eng_diameter * stage.at(currentStage).engV4.at(i).t, stage.at(currentStage).eng_diameter * stage.at(currentStage).engV4.at(i).t, stage.at(currentStage).eng.at(i), operator*(stage.at(currentStage).eng_dir, -1), GetProperExhaustTexture(stage.at(currentStage).eng_tex.data()));

		if (!stage.at(currentStage).ParticlesPacked) {
			if (stage.at(currentStage).wps1) {
				PARTICLESTREAMSPEC Pss1 = GetProperPS(stage.at(currentStage).eng_pstream1.data()).Pss;
				AddExhaustStreamGrowing(stage.at(currentStage).th_main_h.at(i), stage.at(currentStage).eng.at(i), &Pss1, GetProperPS(stage.at(currentStage).eng_pstream1.data()).Growing, GetProperPS(stage.at(currentStage).eng_pstream1.data()).GrowFactor_size, GetProperPS(stage.at(currentStage).eng_pstream1.data()).GrowFactor_rate, true, false, currentStage, i);

				oapiWriteLogV("%s: Stage n.%i Engine Exhaust Stream Added: %s to engine n.%i", GetName(), currentStage + 1, stage.at(currentStage).eng_pstream1.c_str(), i + 1);
			}
			if (stage.at(currentStage).wps2) {
				PARTICLESTREAMSPEC Pss2 = GetProperPS(stage.at(currentStage).eng_pstream2.data()).Pss;
				AddExhaustStreamGrowing(stage.at(currentStage).th_main_h.at(i), stage.at(currentStage).eng.at(i), &Pss2, GetProperPS(stage.at(currentStage).eng_pstream2.data()).Growing, GetProperPS(stage.at(currentStage).eng_pstream2.data()).GrowFactor_size, GetProperPS(stage.at(currentStage).eng_pstream2.data()).GrowFactor_rate, true, false, currentStage, i);

				oapiWriteLogV("%s: Stage n.%i Engine Exhaust Stream Added: %s to engine n.%i", GetName(), currentStage + 1, stage.at(currentStage).eng_pstream2.c_str(), i + 1);
			}
		}
		oapiWriteLogV("%s: Stage n. %i Engines Exhaust Added--> number of engines: %i , diameter: %.3f, position x: %.3f y: %.3f z: %.3f", GetName(), currentStage + 1, stage.at(currentStage).nEngines, stage.at(currentStage).eng_diameter * stage.at(currentStage).engV4.at(i).t, stage.at(currentStage).eng.at(i).x, stage.at(currentStage).eng.at(i).y, stage.at(currentStage).eng.at(i).z);

	}

	if (stage.at(currentStage).ParticlesPacked)
	{
		std::array<PARTICLESTREAMSPEC, 2> partpacked;
		partpacked.at(0) = GetProperPS(stage.at(currentStage).eng_pstream1.data()).Pss;
		partpacked[1] = GetProperPS(stage.at(currentStage).eng_pstream2.data()).Pss;


		int engine = abs(stage.at(currentStage).ParticlesPackedToEngine) - 1;

		VECTOR3 thdir; std::array<VECTOR3, 2> Pos;
		GetThrusterDir(stage.at(currentStage).th_main_h.at(engine), thdir);
		thdir.x *= -1;
		thdir.y *= -1;
		thdir.z *= -1;
		if (stage.at(currentStage).ParticlesPackedToEngine > 0) {
			Pos.at(0) = stage.at(currentStage).eng.at(engine);
			Pos[1] = stage.at(currentStage).eng.at(engine);
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
			AddExhaustStreamGrowing(stage.at(currentStage).th_main_h.at(engine), Pos.at(0), &partpacked.at(0), GetProperPS(stage.at(currentStage).eng_pstream1.data()).Growing, GetProperPS(stage.at(currentStage).eng_pstream1.data()).GrowFactor_size, GetProperPS(stage.at(currentStage).eng_pstream1.data()).GrowFactor_rate, true, false, currentStage, engine);
			oapiWriteLogV("%s: Stage n.%i Engine Packed Exhaust Stream Added: %s to engine n.%i", GetName(), currentStage + 1, stage.at(currentStage).eng_pstream1.c_str(), engine + 1);
		}
		if (stage.at(currentStage).wps2) {
			AddExhaustStreamGrowing(stage.at(currentStage).th_main_h.at(engine), Pos[1], &partpacked.at(1), GetProperPS(stage.at(currentStage).eng_pstream2.data()).Growing, GetProperPS(stage.at(currentStage).eng_pstream2.data()).GrowFactor_size, GetProperPS(stage.at(currentStage).eng_pstream2.data()).GrowFactor_rate, true, false, currentStage, engine);
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

		live_a.at(pns) = CreateAttachment(false, payload.at(pns).off[0], direction, normal, "MS2015", false);

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