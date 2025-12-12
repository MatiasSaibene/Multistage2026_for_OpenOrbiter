#include "MS_Camera.hpp"
#include <format>
#define ORBITER_MODULE

MS_Camera::MS_Camera(OBJHANDLE hObj, int fmodel) : VESSEL4(hObj, fmodel){

    penwhite = oapiCreatePen(1, 3, 0xFFFFFF);

    RefHeight = 0.0;

    v = nullptr;

    Distance = 0.0;

    Polar = 0.0;

    Azimuth = 0.0;

    Aperture = 0.0;

    hMS = nullptr;

}

MS_Camera::~MS_Camera(){
	oapiReleasePen(penwhite);
}

void MS_Camera::clbkSetClassCaps(FILEHANDLE cfg) {

	SetEmptyMass(1000);
	SetClipRadius(0.01);
	RefHeight = 50;

    SetCameraDefaultDirection(_V(0, 0, 1));
	SetCameraRotationRange(2 * PI, 2 * PI, 2 * PI, 2 * PI);

	double ro = 20;

    SetTouchdownPoints(_V(cos(30 * RAD) * ro, -20, -sin(30 * RAD) * ro), _V(0, -20, 1 * ro), _V(-cos(30 * RAD) * ro, -20, -sin(30 * RAD) * ro));
	Distance = 0;
	Polar = 0;
	Azimuth = 0;
	Aperture = 0;

}

void MS_Camera::clbkLoadStateEx(FILEHANDLE scn, void *vs){

    char * cp_line;
    std::string line;

    while(oapiReadScenario_nextline(scn, cp_line)){
        
        line = cp_line;

        if(line.rfind("TARGET", 0) == 0){
            RefVessel = line.substr(6);

            std::string logbuff = std::format("MS_Camera: Target Vessel Loaded->{}", RefVessel);

            oapiWriteLog(const_cast<char *>(logbuff.c_str()));
        } else {
            ParseScenarioLineEx(const_cast<char *>(line.c_str()), vs);
        }

    }
    
}

void MS_Camera::clbkSaveState(FILEHANDLE scn){

    SaveDefaultState(scn);

    oapiWriteScenario_string(scn, const_cast<char *>("TARGET"), const_cast<char *>(RefVessel.c_str()));

}

int MS_Camera::clbkConsumeBufferedKey(DWORD key, bool down, char* kstate)
{
	if (!down) return 0;

	if (KEYDOWN(kstate, OAPI_KEY_UP) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_ALT(kstate) && (!KEYMOD_SHIFT(kstate))))
	{
		RefHeight -= 50;
		if (RefHeight < 50) { RefHeight = 50; }
		return 1;
	}
	if (KEYDOWN(kstate, OAPI_KEY_DOWN) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_ALT(kstate) && (!KEYMOD_SHIFT(kstate))))
	{
		RefHeight += 50;
		if (RefHeight > 200) { RefHeight = 200; }
		return 1;
	}
	if (KEYDOWN(kstate, OAPI_KEY_R) && (KEYMOD_CONTROL(kstate)) && (!KEYMOD_ALT(kstate) && (!KEYMOD_SHIFT(kstate))))
	{
		oapiOpenInputBox(const_cast<char*>("Reference Vessel"), InputNewRefVessel, 0, 35, (void*)this);
		return 1;
	}

	return 0;
}

void MS_Camera::clbkPostStep(double simt, double simdt, double mjd)
{

	if(Distance > 10e6){
        oapiDeleteVessel(GetHandle(), oapiGetVesselByName(const_cast<char *>(RefVessel.c_str())));
    }

	if(oapiGetFocusObject() == GetHandle()){

		oapiCameraAttach(GetHandle(), 0);

		hMS = oapiGetVesselByName(const_cast<char *>(RefVessel.c_str()));

		if (oapiIsVessel(hMS)){
			v = (VESSEL4*)oapiGetVesselInterface(hMS);

			VECTOR3 rpos, gpos;
			GetRelativePos(hMS, rpos);
			GetGlobalPos(gpos);
			Distance = length(rpos);
			Aperture = 2 * atan2(RefHeight, Distance);
			if (Aperture <= 0.1 * RAD){
                Aperture = 0.1 * RAD;
            }
			oapiCameraSetAperture(Aperture);

			VECTOR3 rposloc;
			Global2Local((gpos - rpos), rposloc);
			normalise(rposloc);
			Polar = atan2(-rposloc.x, rposloc.z);
			Azimuth = acos(sqrt(rposloc.x * rposloc.x + rposloc.z * rposloc.z));
			oapiCameraSetCockpitDir(Polar, Azimuth, false);
			//oapiSetHUDMode(HUD_DOCKING);
		} else {
            return;
        }
	
    }

	return;
}

int MS_Camera::clbkGeneric(int msgid = 0, int prm = 0, void* context = nullptr){

	if (msgid == VMSG_USER){
		if (prm == 1){
			RefVessel = (char*)context;
			return 1;
		}
		return 0;
	}
	return 0;
}

bool MS_Camera::InputNewRefVessel(void* id, char* str, void* usrdata){

	MS_Camera *Cam = (MS_Camera*)(usrdata);

	Cam->RefVessel = str;

	if (oapiIsVessel(oapiGetVesselByName(str))){
		return true;
	} else {
		return false;
	}
}

bool MS_Camera::clbkDrawHUD(int mode, const HUDPAINTSPEC* hps, oapi::Sketchpad* skp){

	if (mode != HUD_NONE){
		//VESSEL3::clbkDrawHUD (mode, hps, skp);
		int cx = hps->CX, cy = hps->CY;
		DWORD w;
		DWORD h;
		DWORD bpp;
		oapiGetViewportSize(&w, &h, &bpp);
		int ww = (int)w;
		int hh = (int)h;

		skp->SetPen(penwhite);
		skp->Line((0.5 * ww), (0.1 * hh), (0.5 * ww), (0.45 * hh));
		skp->Line((0.5 * ww), (0.55 * hh), (0.5 * ww), (0.9 * hh));
		skp->Line((0.5 * ww), (0.49 * hh), (0.5 * ww), (0.51 * hh));
		skp->Line((0.1 * ww), (0.5 * hh), (0.45 * ww), (0.5 * hh));
		skp->Line((0.55 * ww), (0.5 * hh), (0.9 * ww), (0.5 * hh));
		skp->Line((0.49 * ww), (0.5 * hh), (0.51 * ww), (0.5 * hh));

		//skp->Rectangle(0.1*ww,0.1*hh,0.35*ww,0.1*hh+60);
		char hudbuff[256];
		skp->SetTextColor(0xFFFFFF);
		int len = 0;
		if (oapiIsVessel(hMS)){
			len = sprintf_s(hudbuff, "Altitude	: %03.3f km", v->GetAltitude() / 1000);
			skp->Text(0.1 * ww, 0.1 * hh, hudbuff, len);
			len = sprintf_s(hudbuff, "Distance	: %03.3f km", Distance / 1000);
			skp->Text(0.1 * ww, 0.1 * hh + 20, hudbuff, len);
			len = sprintf_s(hudbuff, "Velocity	: %04.0f m/s", v->GetGroundspeed());
			skp->Text(0.1 * ww, 0.1 * hh + 40, hudbuff, len);

			len = sprintf_s(hudbuff, "Polar   : %03.1f", Polar * DEG);
			skp->Text(0.1 * ww, 0.1 * hh + 80, hudbuff, len);
			len = sprintf_s(hudbuff, "Azimuth : %03.1f", Azimuth * DEG);
			skp->Text(0.1 * ww, 0.1 * hh + 100, hudbuff, len);
			len = sprintf_s(hudbuff, "Aperture: %03.2f", Aperture * DEG);
			skp->Text(0.1 * ww, 0.1 * hh + 120, hudbuff, len);
			double Met;
			v->clbkGeneric(VMSG_USER, 1, &Met);
			len = sprintf_s(hudbuff, "MET: %03.2f s", Met);
			skp->Text(0.1 * ww, 0.1 * hh + 160, hudbuff, len);
		}

	}
	return true;
}