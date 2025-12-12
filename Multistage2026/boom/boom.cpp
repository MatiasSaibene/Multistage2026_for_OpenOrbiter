#include "boom.hpp"
#define ORBITER_MODULE

//Class creation
boom::boom(OBJHANDLE hObj, int fmodel) : VESSEL4(hObj, fmodel){

    ph_h = nullptr;

    th_h = nullptr;

    thg_h = nullptr;

    msh_h = nullptr;

    msh_idh = 0;

    MyID2 = 0;

    deltat1 = 0.0;

}

boom::~boom(){

}


void boom::clbkSetClassCaps(FILEHANDLE cfg)
{
	deltat1 = 0;

	// int i;
	 //THRUSTER_HANDLE **grps;
	 //int *grpc;

	 //Base stuff
	SetSize(5);
	SetEmptyMass(100);
	SetPMI(_V(1, 1, 1));
	SetCrossSections(_V(0.1, 0.1, 0.1));
	SetRotDrag(_V(0, 0, 0));
	SetTouchdownPoints(_V(0, 0, 1), _V(-1, 0, -1), _V(1, 0, -1));
	SetSurfaceFrictionCoeff(0.07, 0.3);
	SetCW(0.09, 0.09, 2, 1.4);
	EnableTransponder(true);
	InitNavRadios(4);
	SetCameraOffset(_V(0, 0, 0));

	//SetVisibilityLimit(0,0.002);
	SetAlbedoRGB(_V((float)255 / 255, (float)255 / 255, (float)255 / 255));
	//SetGravityGradientDamping(0);
	SetWingAspect(0);
	SetWingEffectiveness(0);
	SetMaxWheelbrakeForce(200000);
	//SetBankMomentScale(0);
	//SetPitchMomentScale(0);


    SURFHANDLE texfire = oapiRegisterExhaustTexture(const_cast<char*>("Multistage2015\\Contrail2"));
	static PARTICLESTREAMSPEC fire = {
		0, //flag
		30, //size
		20, //rate
		0, //velocity
		0, //velocity distribution
		10, //lifetime
		45, //growthrate
		0, //atmslowdown
		PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_LIN, 0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1,
		texfire
	};

    SURFHANDLE JSexplosivebolt = oapiRegisterExhaustTexture(const_cast<char*>("Multistage2015\\Explosivebolt"));
	static PARTICLESTREAMSPEC stagingfireps = {

		0,		// flag
		3,	// size
		50,		// rate
		50,	    // velocity
		20.,    // velocity distribution
		10,		// lifetime
		4,	// growthrate
		0,    // atmslowdown 
		PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_LIN, 0, 1.0,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1,
		JSexplosivebolt
	};
	SURFHANDLE explosion = oapiRegisterExhaustTexture(const_cast<char*>("Multistage2015\\explosion"));
	static PARTICLESTREAMSPEC def = {

		0,		// flag
		2,	// size
		0.5,		// rate
		5,	    // velocity
		0.5,    // velocity distribution
		10,		// lifetime
		30,	// growthrate
		0,    // atmslowdown 
		PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_LIN, 0, 1.0,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1,
		explosion
	};

    ph_h = CreatePropellantResource(1);

    th_h = CreateThruster(_V(0, 0, 0), _V(0, 0, 1), 1, ph_h, 3, 0, 101400);

    AddExhaustStream(th_h, &stagingfireps);
	AddExhaustStream(th_h, &fire);
	AddExhaustStream(th_h, &def);

}


void boom::clbkPostStep(double simt, double simdt, double mjd) {

	deltat1 += oapiGetSimStep();

	SetThrusterLevel(th_h, 1);

	if (deltat1 > 20) {
		oapiDeleteVessel(GetHandle());
	}

}

void boom::clbkLoadStateEx(FILEHANDLE scn, void* vs)
{
	char* line;

	while (oapiReadScenario_nextline(scn, line)) {
		ParseScenarioLineEx(line, vs);
	}

}

void boom::clbkSaveState(FILEHANDLE scn)
{
	SaveDefaultState(scn);
}

void boom::clbkPostCreation() {
	
    oapiWriteLog(const_cast<char*>("About To Explode"));

}

//Creation
DLLCLBK void InitModule(HINSTANCE hModule){

}

DLLCLBK void ExitModule(HINSTANCE hModule){

}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hvessel, int flightmodel){
    return new boom(hvessel, flightmodel);
}

DLLCLBK void ovcExit(VESSEL* vessel){
    if (vessel)delete(boom*)vessel;
}