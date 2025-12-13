#include "Multistage2026.hpp"
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
			delete payloadrotatex[i].at(j);
			delete payloadrotatey[i].at(j);
			delete payloadrotatez[i].at(j);
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
		if (hasFairing == TRUE) {
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
double Multistage2026::GetMassAtStage(int MassStage, bool empty = TRUE) {
	double Mass = 0;

	int add = 0;
	if (empty == TRUE) { add = 1; }
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