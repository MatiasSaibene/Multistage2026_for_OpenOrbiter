#include "MS_Crawler.hpp"
#include <array>
#include <format>
#define ORBITER_MODULE

//Creation
MS_Crawler::MS_Crawler(OBJHANDLE hObj, int fmodel) : VESSEL4(hObj, fmodel) {

	Rot1 = { nullptr };

	Rot2 = { nullptr };

	Rot3 = { nullptr };

	Rot4 = { nullptr };

	Tr1 = { nullptr };

	Tr2 = { nullptr };

	Tr3 = { nullptr };

	Tr4 = { nullptr };

	animCing = {};

	lights = {nullptr};

	Initials = {};

    FWD = false;

	BWD = false;

	LFT = false;

	RGT = false;

	Speed = 0.0;

	LengthCrawl = 0.0;

	rt = 0.0;

	earth_circ = 0.0;

	each_deg = 0.0;

	AngSpeed = 0.0;

	TangentSpd = 0.0;

    StateLifter = 0.0;

	SpeedLifter = 0.0;

	Lifter = nullptr;

	Pistons = nullptr;

	GoingUp = false;

	GoingDown = false;

    col_d = { 0 };
	col_s = { 0 };
	col_a = { 0 };
	col_white = { 0 };

    hPad = nullptr;

	vPad = nullptr;

	vsPad = { 0 };

	AttPadCrawler = nullptr;

	AttPadRocket = nullptr;

	AttCrawlerPad = nullptr;

	msh = nullptr;

	PadDetached = false;

    wCustomPadModule = false;

	NoPad = false;

	AnimLifter = 0;

	CameraOff = _V(0, 0, 0);


}

MS_Crawler::~MS_Crawler(){

}

void MS_Crawler::DefineAnimations()
{
	VECTOR3 Translate1 = _V(0, 0, 4);
	VECTOR3 Rotate1 = _V(0, 3.48, 22.71);
	double Angle1 = 22.4 * RAD;
	VECTOR3 Translate2 = _V(0, -sin(Angle1) * 4.5, 4.5 * cos(Angle1));
	VECTOR3 Rotate2 = _V(0, 1.66, 26.73);
	double Angle2 = 154.3 * RAD;
	VECTOR3 Translate3 = _V(0, 0, -12.05);
	VECTOR3 Rotate3 = _V(0, 1.66, 14.68);
	double Angle3 = 160.9 * RAD;

    VECTOR3 Translate4 = _V(0, sin(Angle1) * 4.25, cos(Angle1) * 4.25);
	VECTOR3 Rotate4 = _V(0, 3.48, 18.71);
	double Angle4 = 22.4 * RAD;

    static constexpr std::array<UINT, NANIM> ElementsCing1 = {
        481, 479, 436, 435, 456, 423, 421, 459,
        466, 468, 467, 420, 422, 428, 425, 424,
        426, 427, 469, 463, 462, 465, 458, 464,
        461, 457, 460, 455, 453, 454, 440, 441,
        451, 452, 450, 449, 444, 446, 443, 438,
        447, 445, 442, 448, 439, 473, 474, 472,
        470, 476, 478, 477, 475, 471, 480, 432,
        429, 437, 431, 430, 434, 433, 482, 483
    };

    static constexpr std::array<UINT, NANIM> ElementsCing2 = {
        417, 415, 372, 371, 392, 359, 357, 395,
        402, 404, 403, 356, 358, 364, 361, 360,
        362, 363, 405, 399, 398, 401, 394, 400,
        397, 393, 396, 391, 389, 390, 376, 377,
        387, 388, 386, 385, 380, 382, 379, 374,
        383, 381, 378, 384, 375, 409, 410, 408,
        406, 412, 414, 413, 411, 407, 416, 368,
        365, 373, 367, 366, 370, 369, 418, 419
    };

    static constexpr std::array<UINT, NANIM> ElementsCing3 = {
        353	,351	,308	,307	,328	,295	,293	,331	,338	,340	,339	,292	,294	,300	,297	,296	,298	,299	,341	,335	,334	,337	,330	,336	,333	,329	,332	,327	,325	,326	,312	,313	,323	,324	,322	,321	,316	,318	,315	,310	,319	,317	,314	,320	,311	,345	,346	,344	,342	,348	,350	,349	,347	,343	,352	,304	,301	,309	,303	,302	,306	,305	,354	,355
    };

    static constexpr std::array<UINT, NANIM> ElementsCing4 = {
        289	,   287	,244	,243	,264	,231	,229	,267	,274	,276	,275	,228	,230	,236	,233	,232	,234	,235	,277	,271	,270	,273	,266	,272	,269	,265	,268	,263	,261	,262	,248	,249	,259	,260	,258	,257	,252	,254	,251	,246	,255	,253	,250	,256	,247	,281	,282	,280	,278	,284	,286	,285	,283	,279	,288	,240	,237	,245	,239	,238	,242	,241	,290	,291
    };

    static constexpr std::array<UINT, NANIM> ElementsCing5 = {
        225	,223	,180	,179	,200	,167	,165	,203	,210	,212	,211	,164	,166	,172	,169	,168	,170	,171	,213	,207	,206	,209	,202	,208	,205	,201	,204	,199	,197	,198	,184	,185	,195	,196	,194	,193	,188	,190	,187	,182	,191	,189	,186	,192	,183	,217	,218	,216	,214	,220	,222	,221	,219	,215	,224	,176	,173	,181	,175	,174	,178	,177	,226	,227
    };

    static constexpr std::array<UINT, NANIM> ElementsCing6 = {
        161	,159	,116	,115	,136	,103	,101	,139	,146	,148	,147	,100	,102	,108	,105	,104	,106	,107	,149	,143	,142	,145	,138	,144	,141	,137	,140	,135	,133	,134	,120	,121	,131	,132	,130	,129	,124	,126	,123	,118	,127	,125	,122	,128	,119	,153	,154	,152	,150	,156	,158	,157	,155	,151	,160	,112	,109	,117	,111	,110	,114	,113	,162	,163
    };

    static constexpr std::array<UINT, NANIM> ElementsCing7 = {
        97	,95	,563	,562	,72	,550	,548	,75	,82	,84	,83	,547	,549	,555	,552	,551	,553	,554	,85	,79	,78	,81	,74	,80	,77	,73	,76	,582	,580	,581	,567	,568	,578	,579	,577	,576	,571	,573	,570	,565	,574	,572	,569	,575	,566	,89	,90	,88	,86	,92	,94	,93	,91	,87	,96	,559	,556	,564	,558	,557	,561	,560	,98	,99
    };

    static constexpr std::array<UINT, NANIM> ElementsCing8 = {
        544	,542	,499	,498	,519	,486	,484	,522	,529	,531	,530	,583	,485	,491	,488	,487	,489	,490	,532	,526	,525	,528	,521	,527	,524	,520	,523	,518	,516	,517	,503	,504	,514	,515	,513	,512	,507	,509	,506	,501	,510	,508	,505	,511	,502	,536	,537	,535	,533	,539	,541	,540	,538	,534	,543	,495	,492	,500	,494	,493	,497	,496	,545	,546
    };

    static std::array<std::array<UINT, NCING>, NANIM>ElementsCing{};

    for(int i = 0; i < NANIM; i++){
        ElementsCing.at(i).at(0) = ElementsCing1.at(i);
        ElementsCing.at(i).at(1) = ElementsCing2.at(i);
        ElementsCing.at(i).at(2) = ElementsCing3.at(i);
        ElementsCing.at(i).at(3) = ElementsCing4.at(i);
        ElementsCing.at(i).at(4) = ElementsCing5.at(i);
        ElementsCing.at(i).at(5) = ElementsCing6.at(i);
        ElementsCing.at(i).at(6) = ElementsCing7.at(i);
        ElementsCing.at(i).at(7) = ElementsCing8.at(i);
    }

    static constexpr std::array<double, NANIM> Beginnings = {
        0	,0.014320309	,0.028640619	,0.042960928	,0.057281237	,0.071601546	,0.085921856	,0.100242165	,0.114562474	,0.130126531	,0.14444684	,0.15876715	,0.173087459	,0.187407768	,0.201728078	,0.216048387	,0.230368696	,0.244689005	,0.259009315	,0.273664093	,0.288318871	,0.302973649	,0.317628427	,0.332283205	,0.346937983	,0.361592761	,0.376247539	,0.393503511	,0.410759484	,0.428015457	,0.44527143	,0.462527402	,0.479783375	,0.497039348	,0.51429532	,0.531551293	,0.548807266	,0.566063238	,0.583319211	,0.600575184	,0.617831157	,0.635087129	,0.652343102	,0.669599075	,0.686855047	,0.70411102	,0.721366993	,0.738140264	,0.754913534	,0.771686805	,0.788460076	,0.805233347	,0.822006618	,0.838779889	,0.85555316	,0.869873469	,0.884193778	,0.898514087	,0.912834397	,0.927154706	,0.941475015	,0.955795325	,0.970115634	,0.984435943
    };

    for(int j = 0; j < NCING; j++){
        for(int i = 0; i < NANIM; i++){
            StateCing.at(i).at(j) = Beginnings.at(i);
            Initials.at(i).at(j) = Beginnings.at(i);
        }
    }

    for(int j = 0; j < NCING; j++){
        for(int i = 0; i < NANIM; i++){
            if(j < 4){
                Rotate1 = _V(0, 3.48, 22.71);
				Rotate2 = _V(0, 1.66, 26.73);
				Rotate3 = _V(0, 1.66, 14.68);
				Rotate4 = _V(0, 3.48, 18.71);
            } else {
                double deltaZ = 41.919094;
				Rotate1 = _V(0, 3.48, 22.71 - deltaZ);
				Rotate2 = _V(0, 1.66, 26.73 - deltaZ);
				Rotate3 = _V(0, 1.66, 14.68 - deltaZ);
				Rotate4 = _V(0, 3.48, 18.71 - deltaZ);
            }

            Tr1.at(i).at(j) = new MGROUP_TRANSLATE(0, &ElementsCing.at(i).at(j), 1, Translate1);
			Rot1.at(i).at(j) = new MGROUP_ROTATE(0, &ElementsCing.at(i).at(j), 1, Rotate1, _V(1, 0, 0), Angle1);
			Tr2.at(i).at(j) = new MGROUP_TRANSLATE(0, &ElementsCing.at(i).at(j), 1, Translate2);
			Rot2.at(i).at(j) = new MGROUP_ROTATE(0, &ElementsCing.at(i).at(j), 1, Rotate2, _V(1, 0, 0), Angle2);
			Tr3.at(i).at(j) = new MGROUP_TRANSLATE(0, &ElementsCing.at(i).at(j), 1, Translate3);
			Rot3.at(i).at(j) = new MGROUP_ROTATE(0, &ElementsCing.at(i).at(j), 1, Rotate3, _V(1, 0, 0), Angle3);
			Tr4.at(i).at(j) = new MGROUP_TRANSLATE(0, &ElementsCing.at(i).at(j), 1, Translate4);
			Rot4.at(i).at(j) = new MGROUP_ROTATE(0, &ElementsCing.at(i).at(j), 1, Rotate4, _V(1, 0, 0), Angle4);

            animCing.at(i).at(j) = CreateAnimation(Beginnings.at(i));
			AddAnimationComponent(animCing.at(i).at(j), 0, 0.113683, Tr1.at(i).at(j));
			AddAnimationComponent(animCing.at(i).at(j), 0.113683, 0.129128, Rot1.at(i).at(j));
			AddAnimationComponent(animCing.at(i).at(j), 0.129128, 0.257022, Tr2.at(i).at(j));
			AddAnimationComponent(animCing.at(i).at(j), 0.257022, 0.37336, Rot2.at(i).at(j));
			AddAnimationComponent(animCing.at(i).at(j), 0.37336, 0.723505, Tr3.at(i).at(j));
			AddAnimationComponent(animCing.at(i).at(j), 0.723505, 0.856662, Rot3.at(i).at(j));
			AddAnimationComponent(animCing.at(i).at(j), 0.856662, 0.984555, Tr4.at(i).at(j));
			AddAnimationComponent(animCing.at(i).at(j), 0.984555, 1, Rot4.at(i).at(j));
        }
    }

    static std::array<UINT, 64> LifterGrp = {
        668,669,666,663,664,661,670,673,675,0,667,665,662,660,674,672,671,1,601,604,600,607,58,628,642,630,629,643,632,633,631,627,611,610,609,608,613,614,615,616,612,636,640,635,634,638,637,622,617,621,626,620,625,619,624,618,623,641,639,59,605,603,602,606 
    };

    double HeightLifter = 2;

    Lifter = new MGROUP_TRANSLATE(0, LifterGrp.data(), 64, _V(0, HeightLifter, 0));

    AnimLifter = CreateAnimation(0);

    AddAnimationComponent(AnimLifter, 0, 1, Lifter);


    static std::array<UINT, 16> Pistoni = {
        14,15,16,17,11,12,13,25,21,22,23,24,10,18,19,20
    };

    Pistons = new MGROUP_SCALE(0, Pistoni.data(), 16, _V(0, 2.77444, 0), _V(1, (2.63016 + HeightLifter) / 2.63016, 1));

    AddAnimationComponent(AnimLifter, 0, 1, Pistons);

    return;
}

void MS_Crawler::clbkSetClassCaps(FILEHANDLE cfg){
    SetEmptyMass(30000);
	SetClipRadius(0.01);
	double ro = 20;

    SetTouchdownPoints(_V(cos(30 * RAD) * ro, 0.01, -sin(30 * RAD) * ro), _V(0, 0.01, 1 * ro), _V(-cos(30 * RAD) * ro, 0.01, -sin(30 * RAD) * ro));
	msh = oapiLoadMeshGlobal("Multistage2026\\MS_Crawler\\MS_Crawler");
	VECTOR3 ofs = _V(0, 0, 0);
	SetMeshVisibilityMode(AddMesh(msh, &ofs), MESHVIS_ALWAYS);

    CameraOff = _V(-20.56, 8, 25.13);
	SetCameraOffset(CameraOff);
	SetCameraRotationRange(PI, PI, PI, PI);
	SetSurfaceFrictionCoeff(0.1, 0.99);

    FWD = false;
	BWD = false;
	LFT = false;
	RGT = false;

	GoingUp = false;
	GoingDown = false;
	PadDetached = false;

    oapiReadItem_float(cfg, const_cast<char *>("Speed"), Speed);
    LengthCrawl = 34.915447;
	rt = 6371010;
	earth_circ = rt * 2 * PI;
	each_deg = earth_circ / 360;

    oapiReadItem_float(cfg, const_cast<char*>("AngSpeed"), AngSpeed);
	AngSpeed *= RAD;

    double radius = 20.7931;
	TangentSpd = AngSpeed * radius;
	StateLifter = 1;
	SpeedLifter = 0.1;

    DefineAnimations();

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

    VECTOR3 dir;
	dir = _V(0, sin(60 * RAD), -cos(60 * RAD));
	const double att = 0.0001;
	lights.at(0) = AddSpotLight(_V(0, 10, 20), dir, 200, att, att, att, 45 * RAD, 60 * RAD, col_d, col_s, col_a);

	dir = _V(0, sin(60 * RAD), cos(60 * RAD));
	lights.at(1) = AddSpotLight(_V(0, 10, -20), dir, 200, att, att, att, 45 * RAD, 60 * RAD, col_d, col_s, col_a);

	dir = _V(0, -sin(15 * RAD), cos(15 * RAD));
	lights.at(2) = AddSpotLight(_V(0, 10, 40), dir, 200, att, att, att, 60 * RAD, 80 * RAD, col_d, col_s, col_a);

	dir = _V(0, -sin(15 * RAD), -cos(15 * RAD));
	lights.at(3) = AddSpotLight(_V(0, 10, -40), dir, 200, att, att, att, 60 * RAD, 80 * RAD, col_d, col_s, col_a);

    for(int i = 0; i < 4; i++){
        lights.at(i)->Activate(false);
    }

    char buffer[256] = {};
    oapiReadItem_string(cfg, const_cast<char*>("PAD_MESH"), buffer);
    PadMesh = buffer;

    PadModule.clear();

    wCustomPadModule = false;

    char buffer2[256] = {};
    oapiReadItem_string(cfg, const_cast<char*>("PAD_MODULE"), buffer);
    PadModule = buffer2;

    if(!PadModule.empty()){
        wCustomPadModule = true;
    }

    if(PadModule != "NOPAD"){
        NoPad = true;
    } else {
        NoPad = false;
    }

}

void MS_Crawler::clbkLoadStateEx(FILEHANDLE scn, void *vs){

    std::string line;
    char *cp_line;

    while(oapiReadScenario_nextline(scn, cp_line)){
        
        line = cp_line;

        if(line.rfind("LIFTER", 0) == 0){
            StateLifter = std::stod(line.substr(6));
        } else if(line.rfind("NOATTPAD", 0) == 0){
            PadDetached = true;
        } else {
            ParseScenarioLineEx(const_cast<char *>(line.c_str()), vs);
        }
    }

}

void MS_Crawler::clbkSaveState(FILEHANDLE scn){

    SaveDefaultState(scn);

    std::string savevalbuff = std::format("{:.3f}", StateLifter);
    oapiWriteScenario_string(scn, const_cast<char *>("LIFTER"), const_cast<char *>(savevalbuff.c_str()));

    if(PadDetached){
        oapiWriteScenario_string(scn, const_cast<char *>("NOATTPAD"), const_cast<char *>(""));
    }

}

int MS_Crawler::clbkConsumeBufferedKey(DWORD key, bool down, char* kstate){

	if (KEYDOWN(kstate, OAPI_KEY_UP) && (KEYMOD_CONTROL(kstate)) && (KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))){
		if (GoingDown){
			GoingDown = false;
		} else {
			GoingUp = true;
		}

		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_DOWN) && (KEYMOD_CONTROL(kstate)) && (KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {
		if (GoingUp){
			GoingUp = false;
		} else {
			GoingDown = true;
		}

		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_G) && (KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {
		
        if(oapiIsVessel(hPad)){
			VECTOR3 rpos;
			GetRelativePos(hPad, rpos);
			if (length(rpos) < 5){
                AttachChild(hPad, AttCrawlerPad, AttPadCrawler);
            }
		}

		return 1;
	}

	if(KEYDOWN(kstate, OAPI_KEY_D) && (KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))){

		if (StateLifter < 0.1){
			if (GetAttachmentStatus(AttCrawlerPad) != NULL){
				DetachChild(AttCrawlerPad);
				PadDetached = true;
			}
		}

		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_UP) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {

		if ((BWD) || (RGT) || (LFT)) {
			BWD = false;
			FWD = false;
			LFT = false;
			RGT = false;
		} else {
			FWD = true;
			BWD = false;
			LFT = false;
			RGT = false;
		}

		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_DOWN) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {

		if ((FWD) || (RGT) || (LFT)) {
			BWD = false;
			FWD = false;
			LFT = false;
			RGT = false;
		} else {
			FWD = false;
			BWD = true;
			LFT = false;
			RGT = false;
		}

		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_LEFT) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {

		if ((RGT) || (FWD) || (BWD)) {
			BWD = false;
			FWD = false;
			LFT = false;
			RGT = false;
		} else {
			FWD = false;
			BWD = false;
			LFT = true;
			RGT = false;
		}

		return 1;
	}
	if (KEYDOWN(kstate, OAPI_KEY_RIGHT) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {

		if ((LFT) || (FWD) || (BWD)) {
			BWD = false;
			FWD = false;
			LFT = false;
			RGT = false;
		} else {
			FWD = false;
			BWD = false;
			LFT = false;
			RGT = true;
		}

		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_L) && (KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {
		for (int i = 0; i < 4; i++)
		{
			lights.at(i)->Activate(!lights.at(i)->IsActive());
		}
		return 1;
	}
	return 0;
}

void MS_Crawler::MoveAround(bool FWD, bool BWD, bool LFT, bool RGT)
{
	if(FWD){

		VESSELSTATUS2 vs2;
		memset(&vs2, 0, sizeof(vs2));
		vs2.version = 2;
		GetStatusEx(&vs2);

		double d_lat = (Speed * oapiGetSimStep() * cos(vs2.surf_hdg)) / each_deg;
		double d_lng = (Speed * oapiGetSimStep() * sin(vs2.surf_hdg)) / each_deg;
		vs2.surf_lat += d_lat * RAD;
		vs2.surf_lng += d_lng * RAD;

		DefSetStateEx(&vs2);
		return;
	}

	if (BWD){
		VESSELSTATUS2 vs2;
		memset(&vs2, 0, sizeof(vs2));
		vs2.version = 2;
		GetStatusEx(&vs2);

		double d_lat = (Speed * oapiGetSimStep() * cos(vs2.surf_hdg)) / each_deg;
		double d_lng = (Speed * oapiGetSimStep() * sin(vs2.surf_hdg)) / each_deg;
		vs2.surf_lat -= d_lat * RAD;
		vs2.surf_lng -= d_lng * RAD;

		DefSetStateEx(&vs2);
		return;
	}

	if (LFT){
		VESSELSTATUS2 vs2;
		memset(&vs2, 0, sizeof(vs2));
		vs2.version = 2;
		GetStatusEx(&vs2);
		double d_hdg = AngSpeed * oapiGetSimStep();
		vs2.surf_hdg -= d_hdg;

		if (vs2.surf_hdg < 0) {
            vs2.surf_hdg += 2 * PI;
        }

		DefSetStateEx(&vs2);
		return;
	}


	if (RGT){
		VESSELSTATUS2 vs2;
		memset(&vs2, 0, sizeof(vs2));
		vs2.version = 2;
		GetStatusEx(&vs2);
		double d_hdg = AngSpeed * oapiGetSimStep();
		vs2.surf_hdg += d_hdg;

		if (vs2.surf_hdg > 2 * PI){
            vs2.surf_hdg -= 2 * PI;
        }

		DefSetStateEx(&vs2);
		return;
	}

	return;
}

void MS_Crawler::UpdateAnimations(bool Reset = false)
{
	if(!Reset){
		double d_anim = 0;
		if ((FWD) || (BWD)){
			d_anim = (Speed / LengthCrawl) * oapiGetSimStep();
		} else if ((LFT) || (RGT)) {
			d_anim = (TangentSpd / LengthCrawl) * oapiGetSimStep();
		}
		d_anim = d_anim / oapiGetTimeAcceleration();

		if (FWD){
			for (int j = 0; j < NCING; j++){
	
				for (int i = 0; i < NANIM; i++)
				{
				    StateCing.at(i).at(j) += d_anim;
				}
			}
		}

		if (BWD){
			for (int j = 0; j < NCING; j++)
			{
				for (int i = 0; i < NANIM; i++)
				{
					StateCing.at(i).at(j) -= d_anim;
				}
			}
		}

		if (LFT){
			for (int j = 0; j < 2; j++){
				for (int i = 0; i < NANIM; i++)
				{
					StateCing.at(i).at(j) += d_anim;
					StateCing.at(i).at(j + 4) += d_anim;
				}
			}


			for (int j = 2; j < 4; j++){
				for (int i = 0; i < NANIM; i++)
				{
					StateCing.at(i).at(j) -= d_anim;
					StateCing.at(i).at(j + 4) -= d_anim;
				}
			}
		}

		if (RGT) {
			for (int j = 0; j < 2; j++){
				for (int i = 0; i < NANIM; i++)
				{
					StateCing.at(i).at(j) -= d_anim;
					StateCing.at(i).at(j + 4) -= d_anim;
				}
			}

			for (int j = 2; j < 4; j++){
				for (int i = 0; i < NANIM; i++)
				{
					StateCing.at(i).at(j) += d_anim;
					StateCing.at(i).at(j + 4) += d_anim;
				}
			}
		}

		for (int j = 0; j < NCING; j++){
			for (int i = 0; i < NANIM; i++){
				if (StateCing.at(i).at(j) > 1) { StateCing.at(i).at(j) = 0; }
				if (StateCing.at(i).at(j) < 0) { StateCing.at(i).at(j) = 1; }

			}
		}
	} else {
		for (int j = 0; j < NCING; j++){
			for (int i = 0; i < NANIM; i++){
				StateCing.at(i).at(j) = Initials.at(i).at(j);
			}
		}
	}


	for (int j = 0; j < NCING; j++){
		for (int i = 0; i < NANIM; i++){
			SetAnimation(animCing.at(i).at(j), StateCing.at(i).at(j));
		}
	}

	return;
}

void MS_Crawler::UpdateLifter(){
	if (GoingUp){
		StateLifter += SpeedLifter * oapiGetSimStep();

		if (StateLifter > 1) { StateLifter = 1; GoingUp = false; }
	}
	else if (GoingDown) {
		StateLifter -= SpeedLifter * oapiGetSimStep();

		if (StateLifter < 0) { StateLifter = 0; GoingDown = false; }
	}

	CameraOff.y = 8 + 2 * StateLifter;
	SetCameraOffset(CameraOff);
	SetAnimation(AnimLifter, StateLifter);
	VECTOR3 attpos = _V(0, 9.405 + 2 * StateLifter, 0);
	VECTOR3 dir, rot, pos;

	if (AttCrawlerPad) {
		GetAttachmentParams(AttCrawlerPad, pos, dir, rot);
		SetAttachmentParams(AttCrawlerPad, attpos, dir, rot);
	}

	return;
}

void MS_Crawler::clbkPreStep(double simt, double simdt, double mjd)
{
	MoveAround(FWD, BWD, LFT, RGT);
	if ((FWD) || (BWD) || (LFT) || (RGT))
	{
		if (oapiGetTimeAcceleration() < 10) {
			UpdateAnimations(false);
		}
		else {
			UpdateAnimations(true);
		}
	}
	if ((GoingUp) || (GoingDown))
	{
		UpdateLifter();
	}

}

void MS_Crawler::clbkPostCreation(){
	
    hPad = oapiGetVesselByName(const_cast<char*>("MS_Pad"));
	if (!NoPad){
		memset(&vsPad, 0, sizeof(vsPad));
		vsPad.version = 2;
		GetStatusEx(&vsPad);


		if (!oapiIsVessel(hPad)){
			if (!wCustomPadModule){
				hPad = oapiCreateVesselEx("MS_Pad", "EmptyModule", &vsPad);
			} else {
				hPad = oapiCreateVesselEx("MS_Pad", PadModule.c_str(), &vsPad);
			}
		}


		vPad = (VESSEL4*)oapiGetVesselInterface(hPad);
		VECTOR3 ofs = _V(0, 0, 0);

		if (!wCustomPadModule){
			MESHHANDLE msPad = oapiLoadMeshGlobal(PadMesh.c_str());
			UINT msh_idx = vPad->AddMesh(msPad, &ofs);
			vPad->SetMeshVisibilityMode(msh_idx, MESHVIS_ALWAYS);
			vPad->SetCameraOffset(_V(0, 120, 20));
			vPad->SetEmptyMass(30000);
			vPad->SetClipRadius(0.01);
			double ro = 20;
			vPad->SetTouchdownPoints(_V(cos(30 * RAD) * ro, 0, -sin(30 * RAD) * ro), _V(0, 0, 1 * ro), _V(-cos(30 * RAD) * ro, 0, -sin(30 * RAD) * ro));
		}

		AttPadCrawler = vPad->CreateAttachment(true, _V(0, 9.405, 0), _V(0, -1, 0), _V(0, 0, 1), "PadCrawl", false);
		AttCrawlerPad = CreateAttachment(false, _V(0, 9.4050, 0), _V(0, 1, 0), _V(0, 0, 1), "PadCrawl", false);
		if (!PadDetached) { AttachChild(hPad, AttCrawlerPad, AttPadCrawler); }

	}

	UpdateLifter();

	return;
}



//Creation
DLLCLBK void InitModule(HINSTANCE hModule){

}
DLLCLBK void ExitModule(HINSTANCE hModule){

}
DLLCLBK VESSEL* ovcInit(OBJHANDLE hvessel, int flightmodel){
    return new MS_Crawler(hvessel, flightmodel);
}

DLLCLBK void ovcExit(VESSEL* vessel){
    if (vessel)delete(MS_Crawler*)vessel;
}