#include "Multistage2026.hpp"
#include <commctrl.h>
#include <format>
#include <stdio.h>
#include "DevModeCtrl.hpp"

DevModeDlg::DevModeDlg(Multistage2026 *_Ms26){

	CompleteFileIniName[0] = '\0';

	DMD_nBoosters = 0;

	DMD_nInterstages = 0;

	DMD_nParticles = 0;

	DMD_nPayloads = 0;

	DMD_nStages = 0;

	DMD_nTextures = 0;



    Ms26 = _Ms26;

    hDlg = nullptr;

    TestingParticle = false;

    sliderlvl = 1;

    hideFairing = false;

    hideHalfFairing = false;

    Graph::InitGDI();

    Curve = std::make_unique<Graph>();

}

DevModeDlg::~DevModeDlg(){
    Ms26 = nullptr;
    hDlg=nullptr;

    Graph::FreeGDI();
}


void DevModeDlg::Open(HINSTANCE hInstance){
	
	hInst = hInstance;
	hDlg = oapiOpenDialogEx(hInst, IDD_DIALOG1,DlgProcHook, 0, this);	
}

int DevModeDlg::Closed(){
	hDlg = nullptr;
	//Ms26->DestroyDMD();
	Ms26 = 0;
	
	return true;
}

double DevModeDlg::GetDlgItemDouble(HWND hDlg, int ctrl, int cchMax){
    std::vector<char> buff(cchMax);
    GetDlgItemTextA(hDlg, ctrl, buff.data(), cchMax);

    double out{};
    sscanf_s(buff.data(), "%lf", &out);
    return out;
}

VECTOR3 DevModeDlg::GetDlgItemVector3(HWND hDlg, int Idd_ctrlx, int Idd_ctrly, int Idd_ctrlz, int cchMax){
	VECTOR3 out;

	std::vector<char> buff(cchMax);

	GetDlgItemText(hDlg, Idd_ctrlx, buff.data(), cchMax);

	sscanf_s(buff.data(), "%lf", &out.x);

	GetDlgItemText(hDlg, Idd_ctrly, buff.data(), cchMax);

	sscanf_s(buff.data(), "%lf", &out.y);

	GetDlgItemText(hDlg, Idd_ctrlz, buff.data(), cchMax);

	sscanf_s(buff.data(), "%lf", &out.z);

	return out;
}

VECTOR4F DevModeDlg::GetDlgItemVector4(HWND hDlg, int Idd_ctrlx, int Idd_ctrly, int Idd_ctrlz, int Idd_ctrlt, int cchMax){

	VECTOR4F out;

	std::vector<char> buff(cchMax);

	GetDlgItemText(hDlg, Idd_ctrlx, buff.data(), cchMax);

	sscanf_s(buff.data(), "%lf", &out.x);

	GetDlgItemText(hDlg,Idd_ctrly,buff.data(), cchMax);

	sscanf_s(buff.data(), "%lf", &out.y);

	GetDlgItemText(hDlg, Idd_ctrlz, buff.data(), cchMax);

	sscanf_s(buff.data(), "%lf", &out.z);

	GetDlgItemText(hDlg, Idd_ctrlt, buff.data(), cchMax);

	sscanf_s(buff.data(), "%lf", &out.t);

	return out;
}

void DevModeDlg::RefreshGraph (std::unique_ptr<Graph> graph, int GraphId){
	
    HWND hCtrl = GetDlgItem (hChild.at(CD_CURVE), GraphId);

	InvalidateRect(hCtrl, nullptr, true);
    
    InvalidateRect(hChild.at(CD_CURVE), nullptr, true);

	UpdateWindow(hCtrl);

	RECT rect;
	
    HDC hDC = GetDC(hCtrl);

	GetClientRect (hCtrl, &rect);

	graph->Refresh (hDC, rect.right-rect.left, rect.bottom-rect.top);

	ReleaseDC (hCtrl, hDC);
}

void DevModeDlg::InitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,bool InitPrms= true, ItemType Expand=Empty){
	if(InitPrms){
        InitParams();
    }

	LONG style = GetWindowLong(GetDlgItem(hWnd,IDC_TREE1),GWL_STYLE);
	SetWindowLong(GetDlgItem(hWnd,IDC_TREE1),GWL_STYLE, style|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS);
	TreeView_DeleteAllItems(GetDlgItem(hWnd,IDC_TREE1));

	TVINSERTSTRUCT insertstruct = {};
		
		
	insertstruct.hInsertAfter=TVI_ROOT;
	insertstruct.hParent=TVI_ROOT;
	insertstruct.item.mask=TVIF_TEXT;
	insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		
	insertstruct.item.pszText=(LPSTR)TEXT("Miscellaneous\0");
	insertstruct.item.cchTextMax=14;
	HTREEITEM hrootMisc=TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
	insertstruct.item.pszText=(LPSTR)TEXT("Textures\0");
	insertstruct.item.cchTextMax=9;
	HTREEITEM hrootTextures=TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
	insertstruct.item.pszText=(LPSTR)TEXT("Particle Streams\0");
	insertstruct.item.cchTextMax=17;
	HTREEITEM hrootParticles = TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
	insertstruct.item.pszText=(LPSTR)TEXT("Boosters\0");
	insertstruct.item.cchTextMax=9;
	HTREEITEM hrootBoosters = TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
	insertstruct.item.pszText=(LPSTR)TEXT("Stages\0");
	insertstruct.item.cchTextMax=7;
	HTREEITEM hrootStages = TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
	insertstruct.item.pszText=(LPSTR)TEXT("LES\0");
	insertstruct.item.cchTextMax=4;
	HTREEITEM hrootLes = TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
	insertstruct.item.pszText=(LPSTR)TEXT("Fairing\0");
    insertstruct.item.cchTextMax=8;
	HTREEITEM hrootFairing = TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
	insertstruct.item.pszText=(LPSTR)TEXT("FX\0");
    insertstruct.item.cchTextMax=3;
	HTREEITEM hrootFX = TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
	insertstruct.item.pszText=(LPSTR)TEXT("Adapter\0");
	insertstruct.item.cchTextMax=8;
	HTREEITEM hrootAdapter = TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
	insertstruct.item.pszText=(LPSTR)TEXT("Payloads\0");
	insertstruct.item.cchTextMax=9;
	HTREEITEM hrootPayloads = TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);		
	insertstruct.item.pszText=(LPSTR)TEXT("Scenario Creation\0");
	insertstruct.item.cchTextMax=19;
	HTREEITEM hrootScenario = TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
		
	for(UINT i=0;i<DMD_nBoosters;i++){
		insertstruct.hParent=hrootBoosters;
		char text[256];
		sprintf(text,"Booster %i\0",i+1);
		insertstruct.item.pszText=(LPSTR)text;
		insertstruct.item.cchTextMax=11;
		TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
    }
		

	for(UINT i=0;i<DMD_nStages;i++){
		insertstruct.hParent=hrootStages;
		char text[256];
		sprintf(text,"Stage %i\0",i+1);
		insertstruct.item.pszText=(LPSTR)text;
		insertstruct.item.cchTextMax=10;
		TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
	}

	for(UINT i=0;i<DMD_nParticles;i++){
		insertstruct.hParent=hrootParticles;
		char text[256];
		sprintf(text,"Particle %i\0",i+1);
		insertstruct.item.pszText=(LPSTR)text;
        insertstruct.item.cchTextMax=12;
		TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
    }

	for(UINT i=0;i<DMD_nPayloads;i++){
		insertstruct.hParent=hrootPayloads;
		char text[256];
		sprintf(text,"Payload %i\0",i+1);
		insertstruct.item.pszText=(LPSTR)text;
		insertstruct.item.cchTextMax=12;
		TreeView_InsertItem(GetDlgItem(hWnd,IDC_TREE1),&insertstruct);
    }

	switch(Expand){
		case Payload:
			{
				TreeView_Expand(GetDlgItem(hWnd,IDC_TREE1),hrootPayloads,TVE_EXPAND);
				TreeView_Select(GetDlgItem(hWnd,IDC_TREE1),hrootPayloads,TVGN_CARET);
				break;
			}
		case Fairing:
			{
				TreeView_Select(GetDlgItem(hWnd,IDC_TREE1),hrootFairing,TVGN_CARET);
				break;
			}
		case Particle:
			{
				TreeView_Expand(GetDlgItem(hWnd,IDC_TREE1),hrootParticles,TVE_EXPAND);
				TreeView_Select(GetDlgItem(hWnd,IDC_TREE1),hrootParticles,TVGN_CARET);
				break;
			}
		case Booster:
			{
				TreeView_Expand(GetDlgItem(hWnd,IDC_TREE1),hrootBoosters,TVE_EXPAND);
				TreeView_Select(GetDlgItem(hWnd,IDC_TREE1),hrootBoosters,TVGN_CARET);
				break;
			}
		case Stage:
			{
				TreeView_Expand(GetDlgItem(hWnd,IDC_TREE1),hrootStages,TVE_EXPAND);
				TreeView_Select(GetDlgItem(hWnd,IDC_TREE1),hrootStages,TVGN_CARET);
				break;
			}
		}

	//FAIRING HIDDEN RESTORATION

	UINT half = Ms26->fairing.N;
    half += 1;
	half = half / 2;
						
	if(hideFairing){
		for(UINT NF = 1; NF < Ms26->fairing.N + 1; NF++){
			Ms26->SetMeshVisibilityMode(Ms26->fairing.msh_idh[NF], MESHVIS_NEVER);
		}
				
		SendDlgItemMessage(hChild[CD_FAIRING],IDC_FAIR_HIDE,BM_SETCHECK,BST_CHECKED,0);
	}
				

    if(hideHalfFairing){
		for(UINT NF=1;NF<=half;NF++){
			Ms26->SetMeshVisibilityMode(Ms26->fairing.msh_idh[NF], MESHVIS_NEVER);
		}
	    SendDlgItemMessage(hChild[CD_FAIRING],IDC_FAIR_HIDEHALF,BM_SETCHECK,BST_CHECKED,0);
	}
    
}

BOOL DevModeDlg::DlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	

	switch (uMsg) {
		case WM_INITDIALOG: 
			{
				InitDialog(hWnd,uMsg,wParam,lParam);
				SetDlgItemText(hWnd,IDC_FILENAME,CompleteFileIniName);
				SendDlgItemMessage(hWnd,IDC_FILENAME,EM_SETREADONLY,TRUE,0);
				
				hChild[CD_VIEW]= CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_VIEWTXT),hDlg,DlgProcViewTxt,LPARAM(this));
				hChild[CD_FAIRING]= CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_FAIRING),hDlg,DlgProcFairing,LPARAM(this));
				hChild[CD_PLD]= CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_PAYLOADS),hDlg,DlgProcPld,LPARAM(this));
				hChild[CD_PARTICLE] = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_PARTICLE),hDlg,DlgProcParticle,LPARAM(this));
				hChild[CD_MISC] = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_MISC),hDlg,DlgProcMisc,LPARAM(this));
				hChild[CD_TEX] = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_TEX),hDlg,DlgProcTex,LPARAM(this));
				hChild[CD_BOOSTER] = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_BOOSTERS),hDlg,DlgProcBooster,LPARAM(this));
				hChild[CD_CURVE] = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_BOOSTER_CURVE),hDlg,DlgProcCurve,LPARAM(this));
				hChild[CD_STAGE] = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_STAGES),hDlg,DlgProcStage,LPARAM(this));
				hChild[CD_ULLAGE] = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_STAGE_ULLAGE),hDlg,DlgProcUllage,LPARAM(this));
				hChild[CD_INTERSTAGE] = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_INTERSTAGES),hDlg,DlgProcInterstage,LPARAM(this));
				hChild[CD_LES] = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_LES),hDlg,DlgProcLes,LPARAM(this));
				hChild[CD_ADAPTER] = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_ADAPTER),hDlg,DlgProcAdapter,LPARAM(this));
				hChild[CD_FX] = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_FX),hDlg,DlgProcFX,LPARAM(this));
				hChild[CD_SCENARIO] = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_SCENARIO),hDlg,DlgProcScenario,LPARAM(this));



				ShowWindow(GetDlgItem(hWnd,IDC_SAVEINI),SW_HIDE);
				ShowWindow(GetDlgItem(hWnd,IDC_ADDGENERAL),SW_HIDE);
				ShowWindow(GetDlgItem(hWnd,IDC_SAVEGENERAL),SW_HIDE);

				std::string DestFile = std::format("{}\\Config\\Multistage2015\\Backups\\{}_Backup_{:.4f}.ini", Ms26->OrbiterRoot, Ms26->GetName(), oapiGetSysMJD());
				bool backupFile = CopyFile((LPCSTR)CompleteFileIniName, (LPCSTR)DestFile.c_str(),false);
				if(backupFile){
					oapiWriteLogV("DMD: Ini File Backup Created-> File Name: %s", DestFile.c_str());
				} else {
					oapiWriteLog(const_cast<char *>("DMD: Unable to Backup the ini file, Config\\Multistage2015\\Backups\\ folder may not be present"));
				}
				return false;
			} 
		
		
		break;
			case WM_DESTROY:
			oapiCloseDialog (hWnd);
		return true;

		case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			
			oapiCloseDialog (hWnd);

			char kstate[256];
			for(int i=0;i<256;i++) kstate[i]=0x00;
			kstate[OAPI_KEY_SPACE]=0x80;
			kstate[OAPI_KEY_LCONTROL]=0x80;
			Ms26->SendBufferedKey(OAPI_KEY_SPACE,true,kstate);
			
			
				
			return TRUE;
		/*case IDOK:
			//SendDlgItemMessage(hChild[0],IDC_PLD_LIVE,BM_SETCHECK,BST_CHECKED,0);
			return TRUE;*/
		case IDC_VIEWLOG:
			{
				for(UINT i=0;i<15;i++)
				{
					ShowWindow(hChild[i],SW_HIDE);
				}
				ShowWindow(GetDlgItem(hWnd,IDC_SAVEINI),SW_HIDE);
				ShowWindow(GetDlgItem(hWnd,IDC_ADDGENERAL),SW_HIDE);
				ShowWindow(GetDlgItem(hWnd,IDC_SAVEGENERAL),SW_HIDE);

				SetWindowPos(hChild[CD_VIEW],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_VIEW],SW_SHOW);
				SendMessage(GetDlgItem(hChild[CD_VIEW],IDC_VIEWFILE),EM_SETREADONLY,TRUE,0);
				std::string line;
				std::string tmpbuffer;
				std::ifstream logfile ("Orbiter.log");
				if(logfile.is_open()){
					while (getline(logfile,line)){
						tmpbuffer+=line;
						tmpbuffer+="\r\n";
					}
					logfile.close();
					SetWindowText(GetDlgItem(hChild[CD_VIEW],IDC_VIEWFILE),(LPCSTR)tmpbuffer.c_str());
				}
				return false;
			}
		case IDC_VIEWINI:
			{
				for(UINT i = 0; i  <15; i++){
					ShowWindow(hChild[i],SW_HIDE);
				}
				ShowWindow(GetDlgItem(hWnd,IDC_ADDGENERAL),SW_HIDE);
				ShowWindow(GetDlgItem(hWnd,IDC_SAVEGENERAL),SW_HIDE);

				SetWindowPos(hChild[CD_VIEW],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_VIEW],SW_SHOW);
				ShowWindow(GetDlgItem(hWnd,IDC_SAVEINI),SW_SHOW);
				SendMessage(GetDlgItem(hChild[CD_VIEW],IDC_VIEWFILE),EM_SETREADONLY,false,0);
				std::string filename(Ms26->fileini);
				std::string line;
				std::string tmpbuffer;
				std::ifstream inifile (filename);
				if(inifile.is_open()){
					while (getline(inifile,line)){
						
						std::string::size_type pos2=0;
						while((pos2=line.find("\r",pos2))!=std::string::npos){
							line.erase(pos2,1);
						}
						std::string::size_type pos3=0;
						while((pos3=line.find("\n",pos3))!=std::string::npos){
							line.erase(pos3,1);
						}
						std::string::size_type pos=0;
						while((pos=line.find("\r\n",pos))!=std::string::npos)
						{
							line.erase(pos,2);
						}
						tmpbuffer+=line;
						tmpbuffer+="\r\n";
					}
					inifile.close();
					SetWindowText(GetDlgItem(hChild[CD_VIEW],IDC_VIEWFILE),(LPCSTR)tmpbuffer.c_str());
				}
				
				return false;
			}
		case IDC_SAVEINI:
			{
				char *buff;
				int size=GetWindowTextLength(GetDlgItem(hChild[CD_VIEW],IDC_VIEWFILE));
				buff=new char[size];
				GetWindowText(GetDlgItem(hChild[CD_VIEW],IDC_VIEWFILE),buff,size);
				std::string filename(Ms26->fileini);
				std::ofstream inifile(filename);
				if(inifile.is_open()){
					std::string str(buff);
					std::string::size_type pos2=0;
						while((pos2=str.find("\r",pos2))!=std::string::npos)
						{
							str.erase(pos2,1);
						}
				inifile.write(str.c_str(),size);
				inifile.close();
				}
				delete[] buff;
			return false;
			}
		case IDC_RESET:
			Ms26->ResetVehicle(Ms26->hangaranims,Ms26->wRamp);
			this->InitDialog(hWnd,uMsg,wParam,lParam);
			return false;
		case IDC_ADDGENERAL:
			{
			AddItem(Viewing);
				break;
			}
		case IDC_SAVEGENERAL:
			{
			SaveItems(Viewing);
				break;
			}
		}break;
		case WM_NOTIFY:
			
			Notify( hWnd,  uMsg,  wParam,  lParam);
		
			return true;
			break;

	}
	return oapiDefDialogProc (hWnd, uMsg, wParam, lParam);
}


std::string DevModeDlg::PickFileName(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool NoExtension) {
    OPENFILENAME ofn;
    char szFile[260] = {0};
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn) == TRUE) {
        std::string fullPath(szFile);
        
        if (NoExtension && ofn.nFileExtension != 0) {
            
            return fullPath.substr(0, ofn.nFileExtension - 1);
        }
        return fullPath;
    }

    return "";
}

UINT DevModeDlg::BoosterCurveNpts(BOOSTER bst){
	UINT n = 0;
	for(UINT i = 0; i < 10; i++){
		if(bst.curve[i].x!=9000000){
			n++;
		}
	}
	return n;
}

void DevModeDlg::SaveInterstage(UINT stageid){
					
	LRESULT check=SendDlgItemMessage(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_WINTER,BM_GETCHECK,0,0);
	char buff[128];
	if(check==BST_CHECKED){
		DMD_stage[stageid].wInter=TRUE;
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_MESHNAME,DMD_stage[stageid].interstage.meshname.data(),128);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_OFFX,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.off.x);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_OFFY,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.off.y);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_OFFZ,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.off.z);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_HEIGHT,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.height);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_DIAM,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.diameter);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_EMPTYMASS,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.emptymass);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_SEPARATIONDELAY,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.separation_delay);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_MODULE,DMD_stage[stageid].interstage.module.data(),128);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_SPDX,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.speed.x);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_SPDY,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.speed.y);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_SPDZ,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.speed.z);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_RSPDX,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.rot_speed.x);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_RSPDY,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.rot_speed.y);
		GetDlgItemText(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_RSPDZ,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].interstage.rot_speed.z);
		} else {
			DMD_stage[stageid].wInter=false;
			DMD_stage[stageid].interstage=INTERSTAGE();
		}

}

bool DevModeDlg::SaveScenarioFile(bool defname){
	FILEHANDLE scn;
	char filebuff[128];
	char valbuff[256];
	char shipname[256];
	char stringbuff[256];
	if(defname){
		sprintf(filebuff,"Multistage2026\\%s - %.4f",Ms26->GetName(),oapiGetSysMJD());
	} else {
		char filebuffname[128];
		GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_FILENAME,filebuffname,128);
		sprintf(filebuff,"Multistage2026\\");
		strcat(filebuff,filebuffname);
	}
	scn=oapiOpenFile(filebuff,FILE_OUT,SCENARIOS);
	oapiWriteLine(scn,const_cast<char *>("BEGIN_DESC"));
	oapiWriteLine(scn,const_cast<char *>("Multistage 2026 Scenario Automatically Generated"));
	oapiWriteLine(scn,const_cast<char *>("END_DESC"));
	oapiWriteLine(scn,const_cast<char *>(""));
	oapiWriteLine(scn,const_cast<char *>("BEGIN_ENVIRONMENT"));
	oapiWriteLine(scn,const_cast<char *>("  System Sol"));
	LRESULT getcheck;
	getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_NOW,BM_GETCHECK,0,0);
	if(getcheck==BST_UNCHECKED){
		double date;
		GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_DATE,valbuff,256);
		sscanf(valbuff,"%lf",&date);
		sprintf(valbuff,"  Date MJD %.6f",date);
		oapiWriteLine(scn,valbuff);
	}
	oapiWriteLine(scn,const_cast<char *>("END_ENVIRONMENT"));
	oapiWriteLine(scn,const_cast<char *>(""));
	oapiWriteLine(scn,const_cast<char *>("BEGIN_FOCUS"));
	GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_SHIPNAME,shipname,256);
	sprintf(valbuff,"  Ship %s",shipname);
	oapiWriteLine(scn,valbuff);
	oapiWriteLine(scn,const_cast<char *>("END_FOCUS"));
	oapiWriteLine(scn,const_cast<char *>(""));
	oapiWriteLine(scn,const_cast<char *>("BEGIN_CAMERA"));
	sprintf(valbuff,"  TARGET %s",shipname);
	oapiWriteLine(scn,valbuff);
	oapiWriteLine(scn,const_cast<char *>("  MODE Extern"));
	oapiWriteLine(scn,const_cast<char *>("  POS 2.89 0.74 -110.96"));
	oapiWriteLine(scn,const_cast<char *>("  TRACKMODE TargetRelative"));
	oapiWriteLine(scn,const_cast<char *>("  FOV 60.00"));
	oapiWriteLine(scn,const_cast<char *>("END_CAMERA"));
	oapiWriteLine(scn,const_cast<char *>(""));
	oapiWriteLine(scn,const_cast<char *>("BEGIN_SHIPS"));
	sprintf(valbuff,"%s:Multistage2015",shipname);
	oapiWriteLine(scn,valbuff);
	oapiWriteLine(scn,const_cast<char *>("  STATUS Landed Earth"));
	GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_LAT,valbuff,256);
	double lat,lng;
	sscanf(valbuff,"%lf",&lat);
	GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_LNG,valbuff,256);
	sscanf(valbuff,"%lf",&lng);
	sprintf(valbuff,"  POS %.7f %.7f",lng,lat);
	oapiWriteLine(scn,valbuff);
	GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_HEADING,valbuff,265);
	double hdg;
	sscanf(valbuff,"%lf",&hdg);
	sprintf(valbuff,"  HEADING %.2f",hdg);
	oapiWriteLine(scn,valbuff);
	oapiWriteLine(scn,const_cast<char *>("  FUEL 1.000"));
	oapiWriteLine(scn,const_cast<char *>("  CONFIGURATION 0"));
	GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_CONFIGFILE,stringbuff,256);
	sprintf(valbuff,"  CONFIG_FILE %s",stringbuff);
	oapiWriteLine(scn,valbuff);
	getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_WGNCFILE,BM_GETCHECK,0,0);
	if(getcheck==BST_CHECKED){
		GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_GNCFILE,stringbuff,256);
		sprintf(valbuff,"  GUIDANCE_FILE %s",stringbuff);
		oapiWriteLine(scn,valbuff);
	}
	getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_WTLMFILE,BM_GETCHECK,0,0);
	if(getcheck==BST_CHECKED){
		GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_TLMFILE,stringbuff,256);
		sprintf(valbuff,"  TELEMETRY_FILE %s",stringbuff);
		oapiWriteLine(scn,valbuff);
	}
	getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_COMPLEX,BM_GETCHECK,0,0);
	if(getcheck==BST_CHECKED)
	{
		oapiWriteLine(scn,const_cast<char *>("  COMPLEX"));
	}
	getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_GROWINGPART,BM_GETCHECK,0,0);
	if(getcheck==BST_CHECKED)
	{
		oapiWriteLine(scn,const_cast<char *>("  GROWING_PARTICLES"));
	}
	getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_FAILURES,BM_GETCHECK,0,0);
	if(getcheck==BST_CHECKED)
	{
		GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_PROBABILITY,valbuff,256);
		UINT probs;
		sscanf(valbuff,"%i",&probs);
		sprintf(valbuff,"  FAILURE_PROB %i",probs);
		oapiWriteLine(scn,valbuff);
	}
	getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_CAMERA,BM_GETCHECK,0,0);
	if(getcheck==BST_CHECKED)
	{
		oapiWriteLine(scn,const_cast<char *>("  CAMERA"));
	}
	getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_HANGAR,BM_GETCHECK,0,0);
	if(getcheck==BST_CHECKED)
	{
		oapiWriteLine(scn,const_cast<char *>("  HANGAR"));
	}
	getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_CRAWLER,BM_GETCHECK,0,0);
	if(getcheck==BST_CHECKED)
	{
		oapiWriteLine(scn,const_cast<char *>("  CRAWLER"));
	}
	getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_PEGDEFAULT,BM_GETCHECK,0,0);
	if(getcheck==BST_UNCHECKED)
	{
		double altsteps[4];
		GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_ALTSTEPS0,valbuff,256);
		sscanf(valbuff,"%lf",&altsteps[0]);
		GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_ALTSTEPS1,valbuff,256);
		sscanf(valbuff,"%lf",&altsteps[1]);
		GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_ALTSTEPS2,valbuff,256);
		sscanf(valbuff,"%lf",&altsteps[2]);
		GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_ALTSTEPS3,valbuff,256);
		sscanf(valbuff,"%lf",&altsteps[3]);
		sprintf(valbuff,"  ALT_STEPS %.1f,%.1f,%.1f,%.1f",altsteps[0],altsteps[1],altsteps[2],altsteps[3]);
		oapiWriteLine(scn,valbuff);

		GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_PEGPITCHLIMIT,valbuff,256);
		double pegpitchlimit;
		sscanf(valbuff,"%lf",&pegpitchlimit);
		sprintf(valbuff,"  PEG_PITCH_LIMIT %.3f",pegpitchlimit);
		oapiWriteLine(scn,valbuff);
		
		GetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_PEGMCINTERVAL,valbuff,256);
		double pegmcinterval;
		sscanf(valbuff,"%lf",&pegmcinterval);
		sprintf(valbuff,"  PEG_MC_INTERVAL %.3f",pegmcinterval);
		oapiWriteLine(scn,valbuff);
	}
	oapiWriteLine(scn,const_cast<char *>("END"));
	oapiCloseFile(scn,FILE_OUT);
	return true;
}

BOOL DevModeDlg::ScenarioProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg)
	{
		

	case WM_INITDIALOG:
		{
			SetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_SHIPNAME,(LPCSTR)Ms26->GetName());
			SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_DATE,EM_SETREADONLY,TRUE,0);
			SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_NOW,BM_SETCHECK,TRUE,0);
			SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_CONFIGFILE,EM_SETREADONLY,TRUE,0);
			SetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_CONFIGFILE,(LPCSTR)Ms26->fileini.c_str());
			SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_GNCFILE,EM_SETREADONLY,TRUE,0);
			SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_TLMFILE,EM_SETREADONLY,TRUE,0);
			SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_PROBABILITY,EM_SETREADONLY,TRUE,0);

			int PegVars[6]={ IDC_SCN_ALTSTEPS0, IDC_SCN_ALTSTEPS1, IDC_SCN_ALTSTEPS2, IDC_SCN_ALTSTEPS3, 
				IDC_SCN_PEGPITCHLIMIT, IDC_SCN_PEGMCINTERVAL};
			SendMultipleDlgItemsMessage(hChild[CD_SCENARIO],PegVars,EM_SETREADONLY,TRUE,0,6);
			SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_PEGDEFAULT,BM_SETCHECK,TRUE,0);
			SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_FNDEFAULT,BM_SETCHECK,TRUE,0);
			SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_FILENAME,EM_SETREADONLY,TRUE,0);
			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				LRESULT getcheck;

			case IDC_SCN_NOW:
				{
					getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_NOW,BM_GETCHECK,0,0);	
					if(getcheck==BST_CHECKED)
					{
						SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_DATE,EM_SETREADONLY,TRUE,0);
						SetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_DATE,(LPCSTR)"");
					}else{
						SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_DATE,EM_SETREADONLY,false,0);
					}
					break;
				}
			case IDC_SCN_WGNCFILE:
				{
					getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_WGNCFILE,BM_GETCHECK,0,0);
					if(getcheck==BST_CHECKED)
					{
						SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_GNCFILE,EM_SETREADONLY,false,0);
					}else{
						SetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_GNCFILE,(LPCSTR)"");
						SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_GNCFILE,EM_SETREADONLY,TRUE,0);
					}
					break;
				}
			case IDC_SCN_WTLMFILE:
				{
					getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_WTLMFILE,BM_GETCHECK,0,0);
					if(getcheck==BST_CHECKED)
					{
						SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_TLMFILE,EM_SETREADONLY,false,0);
					}else{
						SetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_TLMFILE,(LPCSTR)"");
						SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_TLMFILE,EM_SETREADONLY,TRUE,0);
					}
					break;
				}
			case IDC_SCN_FAILURES:
				{
					getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_FAILURES,BM_GETCHECK,0,0);
					if(getcheck==BST_CHECKED)
					{
						SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_PROBABILITY,EM_SETREADONLY,false,0);
					}else{
						SetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_PROBABILITY,(LPCSTR)"");
						SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_PROBABILITY,EM_SETREADONLY,TRUE,0);
					}
					break;
				}
			case IDC_SCN_PEGDEFAULT:
				{
					int PegVars[6]={ IDC_SCN_ALTSTEPS0, IDC_SCN_ALTSTEPS1, IDC_SCN_ALTSTEPS2, IDC_SCN_ALTSTEPS3, 
			IDC_SCN_PEGPITCHLIMIT, IDC_SCN_PEGMCINTERVAL};
					getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_PEGDEFAULT,BM_GETCHECK,0,0);
					if(getcheck==BST_CHECKED)
					{
						std::string empty[6];
						for(UINT i=0;i<6;i++)
						{
						empty[i].clear();
						}
						SetMultipleDlgItemsText(hChild[CD_SCENARIO],PegVars,empty,6);
						SendMultipleDlgItemsMessage(hChild[CD_SCENARIO],PegVars,EM_SETREADONLY,TRUE,0,6);
					}else{
						SendMultipleDlgItemsMessage(hChild[CD_SCENARIO],PegVars,EM_SETREADONLY,false,0,6);
					}
					break;
				}
			case IDC_SCN_FNDEFAULT:
				{
					getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_FNDEFAULT,BM_GETCHECK,0,0);
					if(getcheck==BST_CHECKED)
					{
						SetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_FILENAME,(LPCSTR)"");
						SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_FILENAME,EM_SETREADONLY,TRUE,0);
					}else{
						SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_FILENAME,EM_SETREADONLY,false,0);
					}
					break;
				}
			case IDC_SCN_GETCURRPOS:
				{
					double lng,lat,rad,hdg;
					Ms26->GetEquPos(lng,lat,rad);
					hdg=Ms26->GetHeading();
					char posbuff[256];
					sprintf(posbuff,"%.8f",lat*DEG);
					SetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_LAT,(LPCSTR)posbuff);
					sprintf(posbuff,"%.8f",lng*DEG);
					SetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_LNG,(LPCSTR)posbuff);
					sprintf(posbuff,"%.3f",hdg*DEG);
					SetDlgItemText(hChild[CD_SCENARIO],IDC_SCN_HEADING,(LPCSTR)posbuff);

					break;
				}
			case IDC_SCN_SAVESCN:
				{
					getcheck=SendDlgItemMessage(hChild[CD_SCENARIO],IDC_SCN_FNDEFAULT,BM_GETCHECK,0,0);
					if(getcheck==BST_CHECKED)
					{
						SaveScenarioFile(TRUE);
					}else{
						SaveScenarioFile(false);
					}
					break;
				}
			}
			break;
		}
	}
	return oapiDefDialogProc(hWnd,uMsg,wParam,lParam);
}

BOOL DevModeDlg::FXProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			int FX_Combos[2]={	IDC_FXVENT_CB,IDC_FXMACH_CB	};
			SendMultipleDlgItemsMessage(hChild[CD_FX],FX_Combos,CB_INSERTSTRING,0,(LPARAM)TEXT("--"),2);
			for(UINT i=1;i<=5;i++)
			{
				char buff[10];
				sprintf(buff,"%i",i);
				SendMultipleDlgItemsMessage(hChild[CD_FX],FX_Combos,CB_INSERTSTRING,i,(LPARAM)buff,2);
			}
			break;
		}
	case WM_COMMAND:
		{
			int VentVecs[35]={
						IDC_FXVENT_OFF1X,IDC_FXVENT_OFF1Y,IDC_FXVENT_OFF1Z,IDC_FXVENT_TF1,IDC_FXVENT_DIR1X,IDC_FXVENT_DIR1Y,IDC_FXVENT_DIR1Z,
						IDC_FXVENT_OFF2X,IDC_FXVENT_OFF2Y,IDC_FXVENT_OFF2Z,IDC_FXVENT_TF2,IDC_FXVENT_DIR2X,IDC_FXVENT_DIR2Y,IDC_FXVENT_DIR2Z,
						IDC_FXVENT_OFF3X,IDC_FXVENT_OFF3Y,IDC_FXVENT_OFF3Z,IDC_FXVENT_TF3,IDC_FXVENT_DIR3X,IDC_FXVENT_DIR3Y,IDC_FXVENT_DIR3Z,
						IDC_FXVENT_OFF4X,IDC_FXVENT_OFF4Y,IDC_FXVENT_OFF4Z,IDC_FXVENT_TF4,IDC_FXVENT_DIR4X,IDC_FXVENT_DIR4Y,IDC_FXVENT_DIR4Z,
						IDC_FXVENT_OFF5X,IDC_FXVENT_OFF5Y,IDC_FXVENT_OFF5Z,IDC_FXVENT_TF5,IDC_FXVENT_DIR5X,IDC_FXVENT_DIR5Y,IDC_FXVENT_DIR5Z,
						};
					int MachVecs[15]={
				IDC_FXMACH_OFF1X,IDC_FXMACH_OFF1Y,IDC_FXMACH_OFF1Z,
				IDC_FXMACH_OFF2X,IDC_FXMACH_OFF2Y,IDC_FXMACH_OFF2Z,
				IDC_FXMACH_OFF3X,IDC_FXMACH_OFF3Y,IDC_FXMACH_OFF3Z,
				IDC_FXMACH_OFF4X,IDC_FXMACH_OFF4Y,IDC_FXMACH_OFF4Z,
				IDC_FXMACH_OFF5X,IDC_FXMACH_OFF5Y,IDC_FXMACH_OFF5Z,
			};
			
					int LaunchEdits[7]={
						IDC_FXLAUNCH_N,IDC_FXLAUNCH_HEIGHT,IDC_FXLAUNCH_DISTANCE,IDC_FXLAUNCH_ANGLE,IDC_FXLAUNCH_CUTOFF,IDC_FXLAUNCH_PS1,IDC_FXLAUNCH_PS2
					};
			switch(LOWORD(wParam))
			{
			case IDC_FXVENT_WVENT:
				{
					LRESULT getcheck;
					getcheck=SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_WVENT,BM_GETCHECK,0,0);
						std::string VentTxt[35];
//						char buff[128];

						for(UINT i=0;i<35;i++)
						{
							VentTxt[i].clear();
						}
					if(getcheck == BST_CHECKED)
					{
						SendMultipleDlgItemsMessage(hChild[CD_FX],VentVecs,EM_SETREADONLY,false,0,35);
						SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_CB,CB_SETCURSEL,0,0);
						EnableWindow(GetDlgItem(hChild[CD_FX],IDC_FXVENT_CB),TRUE);
						SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_PSTREAM,EM_SETREADONLY,false,0);
					}else{
						SendMultipleDlgItemsMessage(hChild[CD_FX],VentVecs,EM_SETREADONLY,TRUE,0,35);
						SetMultipleDlgItemsText(hChild[CD_FX],VentVecs,VentTxt,35);
						SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_CB,CB_SETCURSEL,0,0);
						EnableWindow(GetDlgItem(hChild[CD_FX],IDC_FXVENT_CB),false);
						SetDlgItemText(hChild[CD_FX],IDC_FXVENT_PSTREAM,VentTxt[0].c_str());
						SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_PSTREAM,EM_SETREADONLY,TRUE,0);
					}
					break;
				}
			case IDC_FXVENT_CB:
				{
					switch(HIWORD(wParam))
					{
					case CBN_SELCHANGE:
						{
							UINT check=SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_CB,CB_GETCURSEL,0,0);
							SendMultipleDlgItemsMessage(hChild[CD_FX],VentVecs,EM_SETREADONLY,TRUE,0,35);
							SendMultipleDlgItemsMessage(hChild[CD_FX],VentVecs,EM_SETREADONLY,false,0,7*check);
							break;
						}
					}
					break;
				}
			case IDC_FXMACH_WMACH:
				{
					LRESULT getcheck;
					getcheck=SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_WMACH,BM_GETCHECK,0,0);
					std::string MachTxt[15];
					for(UINT k=0;k<15;k++)
					{
						MachTxt[k].clear();
					}
					
					if(getcheck == BST_CHECKED)
					{
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_PSTREAM,EM_SETREADONLY,false,0);
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_MAX,EM_SETREADONLY,false,0);
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_MIN,EM_SETREADONLY,false,0);
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_DIRX,EM_SETREADONLY,false,0);
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_DIRY,EM_SETREADONLY,false,0);
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_DIRZ,EM_SETREADONLY,false,0);
						SendMultipleDlgItemsMessage(hChild[CD_FX],MachVecs,EM_SETREADONLY,false,0,15);
						EnableWindow(GetDlgItem(hChild[CD_FX],IDC_FXMACH_CB),TRUE);
					}else{
						SetDlgItemText(hChild[CD_FX],IDC_FXMACH_PSTREAM,MachTxt[0].c_str());
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_PSTREAM,EM_SETREADONLY,TRUE,0);
						SetDlgItemText(hChild[CD_FX],IDC_FXMACH_MAX,MachTxt[0].c_str());
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_MAX,EM_SETREADONLY,TRUE,0);
						SetDlgItemText(hChild[CD_FX],IDC_FXMACH_MIN,MachTxt[0].c_str());
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_MIN,EM_SETREADONLY,TRUE,0);
						SetDlgItemText(hChild[CD_FX],IDC_FXMACH_DIRX,MachTxt[0].c_str());
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_DIRX,EM_SETREADONLY,TRUE,0);
						SetDlgItemText(hChild[CD_FX],IDC_FXMACH_DIRY,MachTxt[0].c_str());
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_DIRY,EM_SETREADONLY,TRUE,0);
						SetDlgItemText(hChild[CD_FX],IDC_FXMACH_DIRZ,MachTxt[0].c_str());
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_DIRZ,EM_SETREADONLY,TRUE,0);
						SendMultipleDlgItemsMessage(hChild[CD_FX],MachVecs,EM_SETREADONLY,TRUE,0,15);
						SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_CB,CB_SETCURSEL,0,0);
						EnableWindow(GetDlgItem(hChild[CD_FX],IDC_FXMACH_CB),false);
						SetMultipleDlgItemsText(hChild[CD_FX],MachVecs,MachTxt,15);
					}
					break;
				}
			case IDC_FXMACH_CB:
				{
					switch(HIWORD(wParam))
					{
					case CBN_SELCHANGE:
						{
							UINT check = SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_CB,CB_GETCURSEL,0,0);
							SendMultipleDlgItemsMessage(hChild[CD_FX],MachVecs,EM_SETREADONLY,TRUE,0,15);
							SendMultipleDlgItemsMessage(hChild[CD_FX],MachVecs,EM_SETREADONLY,false,0,(check)*3);
							break;
						}
					}
					break;
				}


				case IDC_FXLAUNCH_WLAUNCH:
				{
					LRESULT getcheck;
					getcheck=SendDlgItemMessage(hChild[CD_FX],IDC_FXLAUNCH_WLAUNCH,BM_GETCHECK,0,0);
						std::string LaunchTxt[7];
//						char buff[128];

						for(UINT i=0;i<7;i++)
						{
							LaunchTxt[i].clear();
						}
					if(getcheck == BST_CHECKED)
					{
						SendMultipleDlgItemsMessage(hChild[CD_FX],LaunchEdits,EM_SETREADONLY,false,0,7);
					}else{
						SendMultipleDlgItemsMessage(hChild[CD_FX],LaunchEdits,EM_SETREADONLY,TRUE,0,7);
						SetMultipleDlgItemsText(hChild[CD_FX],LaunchEdits,LaunchTxt,7);
					}
					break;
				}



			case IDC_FX_STOREVALS:
				{
					LRESULT getcheck=SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_WVENT,BM_GETCHECK,0,0);
					if(getcheck == BST_CHECKED)
					{
						DMD_wVent=TRUE;
						GetDlgItemText(hChild[CD_FX],IDC_FXVENT_PSTREAM,DMD_FX_Vent.pstream.data(),128);
						DMD_FX_Vent.nVent=SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_CB,CB_GETCURSEL,0,0);
						DMD_FX_Vent.off[0]=_V(0,0,0);
						DMD_FX_Vent.off[1]=GetDlgItemVector3(hChild[CD_FX],IDC_FXVENT_OFF1X,IDC_FXVENT_OFF1Y,IDC_FXVENT_OFF1Z,128);
						DMD_FX_Vent.off[2]=GetDlgItemVector3(hChild[CD_FX],IDC_FXVENT_OFF2X,IDC_FXVENT_OFF2Y,IDC_FXVENT_OFF2Z,128);
						DMD_FX_Vent.off[3]=GetDlgItemVector3(hChild[CD_FX],IDC_FXVENT_OFF3X,IDC_FXVENT_OFF3Y,IDC_FXVENT_OFF3Z,128);
						DMD_FX_Vent.off[4]=GetDlgItemVector3(hChild[CD_FX],IDC_FXVENT_OFF4X,IDC_FXVENT_OFF4Y,IDC_FXVENT_OFF4Z,128);
						DMD_FX_Vent.off[5]=GetDlgItemVector3(hChild[CD_FX],IDC_FXVENT_OFF5X,IDC_FXVENT_OFF5Y,IDC_FXVENT_OFF5Z,128);
						
						DMD_FX_Vent.time_fin[0]=0;
						DMD_FX_Vent.time_fin[1]=GetDlgItemDouble(hChild[CD_FX],IDC_FXVENT_TF1,128);
						DMD_FX_Vent.time_fin[2]=GetDlgItemDouble(hChild[CD_FX],IDC_FXVENT_TF2,128);
						DMD_FX_Vent.time_fin[3]=GetDlgItemDouble(hChild[CD_FX],IDC_FXVENT_TF3,128);
						DMD_FX_Vent.time_fin[4]=GetDlgItemDouble(hChild[CD_FX],IDC_FXVENT_TF4,128);
						DMD_FX_Vent.time_fin[5]=GetDlgItemDouble(hChild[CD_FX],IDC_FXVENT_TF5,128);
						
						DMD_FX_Vent.dir[0]=_V(0,0,0);
						DMD_FX_Vent.dir[1]=GetDlgItemVector3(hChild[CD_FX],IDC_FXVENT_DIR1X,IDC_FXVENT_DIR1Y,IDC_FXVENT_DIR1Z,128);
						DMD_FX_Vent.dir[2]=GetDlgItemVector3(hChild[CD_FX],IDC_FXVENT_DIR2X,IDC_FXVENT_DIR2Y,IDC_FXVENT_DIR2Z,128);
						DMD_FX_Vent.dir[3]=GetDlgItemVector3(hChild[CD_FX],IDC_FXVENT_DIR3X,IDC_FXVENT_DIR3Y,IDC_FXVENT_DIR3Z,128);
						DMD_FX_Vent.dir[4]=GetDlgItemVector3(hChild[CD_FX],IDC_FXVENT_DIR4X,IDC_FXVENT_DIR4Y,IDC_FXVENT_DIR4Z,128);
						DMD_FX_Vent.dir[5]=GetDlgItemVector3(hChild[CD_FX],IDC_FXVENT_DIR5X,IDC_FXVENT_DIR5Y,IDC_FXVENT_DIR5Z,128);
						
						/*for(UINT i=1;i<=DMD_FX_Vent.nVent;i++)
						{
							
						}*/
					}else{
						DMD_wVent=false;
						DMD_FX_Vent=FX_VENT();
					}

					getcheck=SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_WMACH,BM_GETCHECK,0,0);
					if(getcheck == BST_CHECKED)
					{
						DMD_wMach=TRUE;
						GetDlgItemText(hChild[CD_FX],IDC_FXMACH_PSTREAM,DMD_FX_Mach.pstream.data(),128);
						DMD_FX_Mach.nmach=SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_CB,CB_GETCURSEL,0,0);
						DMD_FX_Mach.mach_max=GetDlgItemDouble(hChild[CD_FX],IDC_FXMACH_MAX,128);
						DMD_FX_Mach.mach_min=GetDlgItemDouble(hChild[CD_FX],IDC_FXMACH_MIN,128);
						DMD_FX_Mach.dir=GetDlgItemVector3(hChild[CD_FX],IDC_FXMACH_DIRX,IDC_FXMACH_DIRY,IDC_FXMACH_DIRZ,128);

						DMD_FX_Mach.off[0]=GetDlgItemVector3(hChild[CD_FX],IDC_FXMACH_OFF1X,IDC_FXMACH_OFF1Y,IDC_FXMACH_OFF1Z,128);
						DMD_FX_Mach.off[1]=GetDlgItemVector3(hChild[CD_FX],IDC_FXMACH_OFF2X,IDC_FXMACH_OFF2Y,IDC_FXMACH_OFF2Z,128);
						DMD_FX_Mach.off[2]=GetDlgItemVector3(hChild[CD_FX],IDC_FXMACH_OFF3X,IDC_FXMACH_OFF3Y,IDC_FXMACH_OFF3Z,128);
						DMD_FX_Mach.off[3]=GetDlgItemVector3(hChild[CD_FX],IDC_FXMACH_OFF4X,IDC_FXMACH_OFF4Y,IDC_FXMACH_OFF4Z,128);
						DMD_FX_Mach.off[4]=GetDlgItemVector3(hChild[CD_FX],IDC_FXMACH_OFF5X,IDC_FXMACH_OFF5Y,IDC_FXMACH_OFF5Z,128);
						
					}else{
						DMD_wMach=false;
						DMD_FX_Mach=FX_MACH();

					}
					getcheck=SendDlgItemMessage(hChild[CD_FX],IDC_FXLAUNCH_WLAUNCH,BM_GETCHECK,0,0);
					if(getcheck == BST_CHECKED)
					{
						DMD_wFXLaunch=TRUE;
						GetDlgItemText(hChild[CD_FX],IDC_FXLAUNCH_PS1,const_cast<char *>(DMD_FX_Launch.Ps1.c_str()),128);
						GetDlgItemText(hChild[CD_FX],IDC_FXLAUNCH_PS2,const_cast<char *>(DMD_FX_Launch.Ps2.c_str()),128);
						DMD_FX_Launch.N=GetDlgItemInt(hChild[CD_FX],IDC_FXLAUNCH_N,FALSE,false);
						DMD_FX_Launch.H=GetDlgItemDouble(hChild[CD_FX],IDC_FXLAUNCH_HEIGHT,128);
						DMD_FX_Launch.Distance=GetDlgItemDouble(hChild[CD_FX],IDC_FXLAUNCH_DISTANCE,128);
						DMD_FX_Launch.Angle=GetDlgItemDouble(hChild[CD_FX],IDC_FXLAUNCH_ANGLE,128);
						DMD_FX_Launch.CutoffAltitude=GetDlgItemDouble(hChild[CD_FX],IDC_FXLAUNCH_CUTOFF,128);
					} else {
						DMD_wFXLaunch=false;
						DMD_FX_Launch=FX_LAUNCH();
					}
					WriteIni.WriteFX_Vent(DMD_FX_Vent,DMD_wVent);
					WriteIni.WriteFX_Mach(DMD_FX_Mach,DMD_wMach);
					WriteIni.WriteFX_Launch(DMD_FX_Launch,DMD_wFXLaunch);
					break;
				}

			}
			break;
		}
	}
	return oapiDefDialogProc(hWnd,uMsg,wParam,lParam);
}

BOOL DevModeDlg::AdapterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){ 
	switch (uMsg){
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_ADAPTER_WADAPTER:
				{
					LRESULT getcheck;
					getcheck=SendDlgItemMessage(hChild[CD_ADAPTER],IDC_ADAPTER_WADAPTER,BM_GETCHECK,0,0);
					int AdaptRead[7]={
					IDC_ADAPTER_MESHNAME, IDC_ADAPTER_OFFX, IDC_ADAPTER_OFFY, IDC_ADAPTER_OFFZ,
					IDC_ADAPTER_HEIGHT, IDC_ADAPTER_DIAM, IDC_ADAPTER_EMPTYMASS
					};
					std::string AdaptTxt[7];
					if(getcheck == BST_CHECKED)
					{
						EnableWindow(GetDlgItem(hChild[CD_ADAPTER],IDC_ADAPTER_PICKMESH),TRUE);
						EnableWindow(GetDlgItem(hChild[CD_ADAPTER],IDC_ADAPTER_STOREVALS),TRUE);
						
						SendMultipleDlgItemsMessage(hChild[CD_ADAPTER],AdaptRead,EM_SETREADONLY,FALSE,0,7);
						
					}else{
						EnableWindow(GetDlgItem(hChild[CD_ADAPTER],IDC_ADAPTER_PICKMESH),FALSE);
						EnableWindow(GetDlgItem(hChild[CD_ADAPTER],IDC_ADAPTER_STOREVALS),FALSE);
						for(UINT i=0;i<7;i++)
						{
							AdaptTxt[i].clear();
						}
						SendMultipleDlgItemsMessage(hChild[CD_ADAPTER],AdaptRead,EM_SETREADONLY,TRUE,0,7);
						SetMultipleDlgItemsText(hChild[CD_ADAPTER],AdaptRead,AdaptTxt,7);
					}
					break;
				}
			case IDC_ADAPTER_PICKMESH:
				{
					char FileNameChar[MAX_PATH];
					sprintf(FileNameChar,"%s", PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
					char dir[MAX_PATH];
					int len=GetCurrentDirectory(MAX_PATH,dir);
					char Msh[9]={"\\Meshes\0"};
					strcat(dir,Msh);
					len+=8;
					char FinalFileName[260];
					int prog=0;
					for(int i=len;i<260;i++)
					{
					FinalFileName[prog]=FileNameChar[i];
					prog++;
					}
					SetWindowText(GetDlgItem(hChild[CD_ADAPTER],IDC_ADAPTER_MESHNAME),(LPCSTR)FinalFileName);
					break;
				}
			case IDC_ADAPTER_STOREVALS:
				{
				LRESULT getcheck=SendDlgItemMessage(hChild[CD_ADAPTER],IDC_ADAPTER_WADAPTER,BM_GETCHECK,0,0);
				if(getcheck == BST_CHECKED)
					{
						DMD_wAdapter=TRUE;
						GetDlgItemText(hChild[CD_ADAPTER],IDC_ADAPTER_MESHNAME,DMD_Adapter.meshname.data(),128);
						DMD_Adapter.off=GetDlgItemVector3(hChild[CD_ADAPTER],IDC_ADAPTER_OFFX,IDC_ADAPTER_OFFY,IDC_ADAPTER_OFFZ,128);
						DMD_Adapter.height=GetDlgItemDouble(hChild[CD_ADAPTER],IDC_ADAPTER_HEIGHT,128);
						DMD_Adapter.diameter=GetDlgItemDouble(hChild[CD_ADAPTER],IDC_ADAPTER_DIAM,128);
						DMD_Adapter.emptymass=GetDlgItemDouble(hChild[CD_ADAPTER],IDC_ADAPTER_EMPTYMASS,128);
					}else{
						DMD_wAdapter=FALSE;
						DMD_Adapter=ADAPTER();
					}

					WriteIni.WriteAdapter(DMD_Adapter,DMD_wAdapter);
					break;
				}
			}
			break;
		}
	}
	return oapiDefDialogProc(hWnd,uMsg,wParam,lParam);
}

BOOL DevModeDlg::InterstageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg){
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_INTERSTAGE_WINTER:
				{
					if(HIWORD(wParam)== BN_CLICKED)
					{
							int InterRead[15] = {
									IDC_INTERSTAGE_MESHNAME,IDC_INTERSTAGE_OFFX, IDC_INTERSTAGE_OFFY,IDC_INTERSTAGE_OFFZ,
									IDC_INTERSTAGE_HEIGHT, IDC_INTERSTAGE_DIAM , IDC_INTERSTAGE_EMPTYMASS , 
									IDC_INTERSTAGE_SEPARATIONDELAY, 
									IDC_INTERSTAGE_MODULE , IDC_INTERSTAGE_SPDX, IDC_INTERSTAGE_SPDY, IDC_INTERSTAGE_SPDZ,
									IDC_INTERSTAGE_RSPDX , IDC_INTERSTAGE_RSPDY , IDC_INTERSTAGE_RSPDZ
									};

						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_WINTER,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							SendMultipleDlgItemsMessage(hChild[CD_INTERSTAGE],InterRead,EM_SETREADONLY,FALSE,0,15);
						}else{
							std::string intertext[15];
							for(UINT i=0;i<15;i++)
							{
								intertext[i].clear();
							}
							SendMultipleDlgItemsMessage(hChild[CD_INTERSTAGE],InterRead,EM_SETREADONLY,TRUE,0,15);
							SetMultipleDlgItemsText(hChild[CD_INTERSTAGE],InterRead,intertext,15);
						}
					}
					break;
				}
			case IDC_INTERSTAGE_CLOSE:
				{
					ShowWindow(hChild[CD_INTERSTAGE],SW_HIDE);
					break;
				}
			case IDC_INTERSTAGE_SAVE:
				{
					UINT stageid=GetDlgItemInt(hChild[CD_STAGE],IDC_STAGE_ID,FALSE,FALSE);
					SaveInterstage(stageid);
					break;
				}
			case IDC_INTERSTAGE_PICKMESH:
				{
					char FileNameChar[MAX_PATH];
					sprintf(FileNameChar,"%s", PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
					char dir[MAX_PATH];
					int len=GetCurrentDirectory(MAX_PATH,dir);
					char Msh[9]={"\\Meshes\0"};
					strcat(dir,Msh);
					len+=8;
					char FinalFileName[260];
					int prog=0;
					for(int i=len;i<260;i++)
					{
					FinalFileName[prog]=FileNameChar[i];
					prog++;
					}
					SetWindowText(GetDlgItem(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_MESHNAME),(LPCSTR)FinalFileName);
					break;
				}
			case IDC_INTERSTAGE_PICKMODULE:
				{
					char FileNameChar[MAX_PATH];
					sprintf(FileNameChar,"%s", PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
					char dir[MAX_PATH];
					int len=GetCurrentDirectory(MAX_PATH,dir);
					char Msh[17]={"\\Config\\Vessels\0"};
					//char Msh[9]={"\\Config\0"};
					strcat(dir,Msh);
					//len+=8;
					len+=16;

					char FinalFileName[260];
					int prog=0;
					for(int i=len;i<260;i++)
					{
					FinalFileName[prog]=FileNameChar[i];
					prog++;
					}
					SetWindowText(GetDlgItem(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_MODULE),(LPCSTR)FinalFileName);
					break;
				}
			
			}
			break;
		}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}

void DevModeDlg::SaveUllage(UINT stageid){
	LRESULT check=SendDlgItemMessage(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEC,BM_GETCHECK,0,0);
	char buff[128];
	if(check==BST_CHECKED){
		DMD_stage[stageid].ullage.wUllage=TRUE;
					
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGETHRUST,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.thrust);
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEANT,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.anticipation);
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEOVERLAP,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.overlap);
		DMD_stage[stageid].ullage.N=GetDlgItemInt(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEN,FALSE,FALSE);
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEANGLE,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.angle);
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEDIAM,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.diameter);
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGELENGTH,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.length);
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEDIRX,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.dir.x);
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEDIRY,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.dir.y);
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEDIRZ,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.dir.z);
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEPOSX,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.pos.x);
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEPOSY,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.pos.y);
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEPOSZ,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.pos.z);
		GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_ULLAGERECTFACTOR,buff,128);
		sscanf(buff,"%lf",&DMD_stage[stageid].ullage.rectfactor);

		} else {
			DMD_stage[stageid].ullage.wUllage=FALSE;
			DMD_stage[stageid].ullage=ULLAGE();
		}

/// EXPBOLTS
		check=SendDlgItemMessage(hChild[CD_ULLAGE],IDC_STAGE_WEXPB,BM_GETCHECK,0,0);
		if(check==BST_CHECKED){
			DMD_stage[stageid].expbolt.wExpbolt=TRUE;
			GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_EXPBPS,DMD_stage[stageid].expbolt.pstream.data(),128);
			GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_EXPBX,buff,128);
			sscanf(buff,"%lf",&DMD_stage[stageid].expbolt.pos.x);
			GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_EXPBY,buff,128);
			sscanf(buff,"%lf",&DMD_stage[stageid].expbolt.pos.y);
			GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_EXPBZ,buff,128);
			sscanf(buff,"%lf",&DMD_stage[stageid].expbolt.pos.z);
			GetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_EXPBANT,buff,128);
			sscanf(buff,"%lf",&DMD_stage[stageid].expbolt.anticipation);
		} else {
			DMD_stage[stageid].expbolt=EXPBOLT();
			DMD_stage[stageid].expbolt.wExpbolt=FALSE;
		}

}

BOOL DevModeDlg::UllageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_STAGE_ULLAGECLOSE:
				{
					ShowWindow(hChild[CD_ULLAGE],SW_HIDE);
					break;
				}
			case IDC_STAGE_ULLAGESAVE:
				{
					
					UINT stageid=GetDlgItemInt(hChild[CD_STAGE],IDC_STAGE_ID,FALSE,FALSE);
					SaveUllage(stageid);
					break;
				}
			case IDC_STAGE_ULLAGEC:
				{
				if(HIWORD(wParam)== BN_CLICKED)
					{
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEC,BM_GETCHECK,0,0);
						int ReadOnly[14]={
						IDC_STAGE_ULLAGETHRUST, IDC_STAGE_ULLAGEANT, IDC_STAGE_ULLAGEOVERLAP, IDC_STAGE_ULLAGEN,
						IDC_STAGE_ULLAGEANGLE, IDC_STAGE_ULLAGEDIAM, IDC_STAGE_ULLAGELENGTH, 
						IDC_STAGE_ULLAGEDIRX, IDC_STAGE_ULLAGEDIRY, IDC_STAGE_ULLAGEDIRZ,
						IDC_STAGE_ULLAGEPOSX, IDC_STAGE_ULLAGEPOSY, IDC_STAGE_ULLAGEPOSZ,
						IDC_STAGE_ULLAGERECTFACTOR
						};
						
						if(getcheck==BST_CHECKED)
						{
						SendMultipleDlgItemsMessage(hChild[CD_ULLAGE],ReadOnly,EM_SETREADONLY,FALSE,0,14);
						}else{
						SendMultipleDlgItemsMessage(hChild[CD_ULLAGE],ReadOnly,EM_SETREADONLY,TRUE,0,14);
						}
					}
					break;
				}
				case IDC_STAGE_WEXPB:
				{
					if(HIWORD(wParam) == BN_CLICKED)
					{
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_ULLAGE],IDC_STAGE_WEXPB,BM_GETCHECK,0,0);
						int ReadOnly[5]={ IDC_STAGE_EXPBX, IDC_STAGE_EXPBY, IDC_STAGE_EXPBZ,IDC_STAGE_EXPBPS,IDC_STAGE_EXPBANT	};
						if(getcheck == BST_CHECKED)
						{
							SendMultipleDlgItemsMessage(hChild[CD_ULLAGE],ReadOnly,EM_SETREADONLY,FALSE,0,5);
						}else{
							std::string emptybuff[5];
							for(UINT i=0;i<5;i++)
							{
								emptybuff[i].clear();
							}
							SendMultipleDlgItemsMessage(hChild[CD_ULLAGE],ReadOnly,EM_SETREADONLY,TRUE,0,5);
							SetMultipleDlgItemsText(hChild[CD_ULLAGE],ReadOnly,emptybuff,5);
						}
					}
					break;
				}
			}
			break;
		}
	}
	return oapiDefDialogProc (hWnd, uMsg, wParam, lParam);
}

BOOL DevModeDlg::StageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT stageid=GetDlgItemInt(hChild[CD_STAGE],IDC_STAGE_ID,FALSE,FALSE);
	switch(uMsg){
	case WM_INITDIALOG:
		{
			SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_ENGN,CB_INSERTSTRING,0,(LPARAM)TEXT("--"));
			for(UINT i=1;i<=5;i++)
			{
				char buff[28];
				sprintf(buff,"%i",i);
				SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_ENGN,CB_INSERTSTRING,i,(LPARAM)buff);
			}
			
			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam)){
			case IDC_STAGE_SHOWULLAGE:
				{
					SetWindowPos(hChild[CD_ULLAGE],HWND_TOP,0,0,0,0,SWP_NOSIZE);
					ShowWindow(hChild[CD_ULLAGE],SW_SHOW);
					break;
				}
			case IDC_STAGE_DELSTAGE:
				{
					DelItem(Stage,stageid);
					break;
				}
			case IDC_STAGE_ADDSTAGE:
				{
					AddItem(Stage);
					break;
				}
			case IDC_STAGE_BATTERYCHECK:
				{
					if(HIWORD(wParam)== BN_CLICKED)
					{
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_BATTERYCHECK,BM_GETCHECK,0,0);
						if(getcheck==BST_CHECKED)
						{
							SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_BATTERY,EM_SETREADONLY,FALSE,0);
						}else{
							SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_BATTERY,EM_SETREADONLY,TRUE,0);
						}
					}
					break;
				}
			case IDC_STAGE_PTDEF:
				{
					if(HIWORD(wParam) == BN_CLICKED)
					{
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_PTDEF,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_PITCHTHRUST,EM_SETREADONLY,TRUE,0);
							std::string emptybuff;
							emptybuff.clear();
							SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_PITCHTHRUST,emptybuff.c_str());
						}else{
							SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_PITCHTHRUST,EM_SETREADONLY,FALSE,0);
						}
					}
					break;
				}
				case IDC_STAGE_RTDEF:
				{
					if(HIWORD(wParam) == BN_CLICKED)
					{
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_RTDEF,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_ROLLTHRUST,EM_SETREADONLY,TRUE,0);
							std::string emptybuff;
							emptybuff.clear();
							SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ROLLTHRUST,emptybuff.c_str());
						}else{
							SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_ROLLTHRUST,EM_SETREADONLY,FALSE,0);
						}
					}
					break;
				}
				case IDC_STAGE_YTDEF:
				{
					if(HIWORD(wParam) == BN_CLICKED)
					{
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_YTDEF,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_YAWTHRUST,EM_SETREADONLY,TRUE,0);
							std::string emptybuff;
							emptybuff.clear();
							SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_YAWTHRUST,emptybuff.c_str());
						}else{
							SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_YAWTHRUST,EM_SETREADONLY,FALSE,0);
						}
					}
					break;
				}
				case IDC_STAGE_LTDEF:
				{
					if(HIWORD(wParam) == BN_CLICKED)
					{
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_LTDEF,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_LINEARTHRUST,EM_SETREADONLY,TRUE,0);
							std::string emptybuff;
							emptybuff.clear();
							SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_LINEARTHRUST,emptybuff.c_str());
						}else{
							SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_LINEARTHRUST,EM_SETREADONLY,FALSE,0);
						}
					}
					break;
				}
				case IDC_STAGE_PARTPACKCHECK:
				{
					if(HIWORD(wParam) == BN_CLICKED)
					{
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_PARTPACKCHECK,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_PARTPACKTOENG,EM_SETREADONLY,FALSE,0);
							
						}else{
							SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_PARTPACKTOENG,EM_SETREADONLY,TRUE,0);
							std::string emptybuff;
							emptybuff.clear();
							SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_PARTPACKTOENG,emptybuff.c_str());
						}
					}
					break;
				}
				case IDC_STAGE_ENGN:
				{
					int ReadStage[20]={
						IDC_STAGE_ENG1X,IDC_STAGE_ENG1Y,IDC_STAGE_ENG1Z,IDC_STAGE_ENG1SCALE,
						IDC_STAGE_ENG2X,IDC_STAGE_ENG2Y,IDC_STAGE_ENG2Z,IDC_STAGE_ENG2SCALE,
						IDC_STAGE_ENG3X,IDC_STAGE_ENG3Y,IDC_STAGE_ENG3Z,IDC_STAGE_ENG3SCALE,
						IDC_STAGE_ENG4X,IDC_STAGE_ENG4Y,IDC_STAGE_ENG4Z,IDC_STAGE_ENG4SCALE,
						IDC_STAGE_ENG5X,IDC_STAGE_ENG5Y,IDC_STAGE_ENG5Z,IDC_STAGE_ENG5SCALE,
					};
					switch(HIWORD(wParam)){
						case CBN_SELCHANGE:
							{
								UINT check=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_ENGN,CB_GETCURSEL,0,0);
								SendMultipleDlgItemsMessage(hChild[CD_STAGE],ReadStage,EM_SETREADONLY,TRUE,0,20);
								SendMultipleDlgItemsMessage(hChild[CD_STAGE],ReadStage,EM_SETREADONLY,FALSE,0,check*4);
								/*
								switch(check){
								case 0:
								
									break;
								
								}*/
							}
					}
					break;
				}
				
				case IDC_STAGE_PICK:
				{
					

				char FileNameChar[MAX_PATH];
				sprintf(FileNameChar,"%s", PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
			
				char dir[MAX_PATH];
			
				int len=GetCurrentDirectory(MAX_PATH,dir);
				char Msh[9]={"\\Meshes\0"};
				strcat(dir,Msh);
				len+=8;
			
			
				char FinalFileName[260];
				int prog=0;
				for(int i=len;i<260;i++)
					{
						FinalFileName[prog]=FileNameChar[i];
						prog++;
					}

				SetWindowText(GetDlgItem(hChild[CD_STAGE],IDC_STAGE_MESHNAME),(LPCSTR)FinalFileName);
			
					break;
				}

				case IDC_STAGE_PICKMODULE:
				{
			
				char FileNameChar[MAX_PATH];
				sprintf_s(FileNameChar,PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
			
				char dir[MAX_PATH];
			
				int len=GetCurrentDirectory(MAX_PATH,dir);
				char Msh[17]={"\\Config\\Vessels\0"};
				strcat(dir,Msh);
				len+=16;
			
			
				char FinalFileName[260];
				int prog=0;
				for(int i=len;i<260;i++)
					{
						FinalFileName[prog]=FileNameChar[i];
						prog++;
					}
					SetWindowText(GetDlgItem(hChild[CD_STAGE],IDC_STAGE_MODULE),(LPCSTR)FinalFileName);
					break;
				}
				case IDC_STAGE_SHOWINTERSTAGE:
					{
						SetWindowPos(hChild[CD_INTERSTAGE],HWND_TOP,0,0,0,0,SWP_NOSIZE);
						ShowWindow(hChild[CD_INTERSTAGE],SW_SHOW);
						break;
					}
				case IDC_STAGE_STOREVALS:
					{
						UINT stageid=GetDlgItemInt(hChild[CD_STAGE],IDC_STAGE_ID,FALSE,FALSE);
//						char buff[128];
						GetDlgItemText(hChild[CD_STAGE],IDC_STAGE_MESHNAME,DMD_stage[stageid].meshname.data(),128);
						DMD_stage[stageid].off=GetDlgItemVector3(hChild[CD_STAGE],IDC_STAGE_OFFX,IDC_STAGE_OFFY,IDC_STAGE_OFFZ,128);
						DMD_stage[stageid].height=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_HEIGHT,128);
						DMD_stage[stageid].diameter=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_DIAM,128);
						DMD_stage[stageid].thrust=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_THRUST,128);
						DMD_stage[stageid].emptymass=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_EMPTYMASS,128);
						DMD_stage[stageid].fuelmass=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_FUELMASS,128);
						DMD_stage[stageid].burntime=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_BURNTIME,128);
						DMD_stage[stageid].ignite_delay=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_IGNITEDELAY,128);
						
						LRESULT getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_BATTERYCHECK,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							DMD_stage[stageid].batteries.wBatts=TRUE;
							DMD_stage[stageid].batteries.MaxCharge=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_BATTERY,128)*3600;
						}else{
							DMD_stage[stageid].batteries.wBatts=FALSE;
							DMD_stage[stageid].batteries.MaxCharge=12*3600;
						}
						getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_REIGNITABLE,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							DMD_stage[stageid].reignitable=FALSE;
						}else{
							DMD_stage[stageid].reignitable=TRUE;
						}

						DMD_stage[stageid].eng_diameter=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_ENGDIAM,128);
						GetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGTEX,DMD_stage[stageid].eng_tex.data(),128);
						GetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGPS1,DMD_stage[stageid].eng_pstream1.data(),128);
						GetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGPS2,DMD_stage[stageid].eng_pstream2.data(),128);
						
						getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_PARTPACKCHECK,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							DMD_stage[stageid].ParticlesPacked=TRUE;
							DMD_stage[stageid].ParticlesPackedToEngine=GetDlgItemInt(hChild[CD_STAGE],IDC_STAGE_PARTPACKTOENG,FALSE,TRUE);

						}else{
							DMD_stage[stageid].ParticlesPacked=FALSE;
						}

						DMD_stage[stageid].eng_dir=GetDlgItemVector3(hChild[CD_STAGE],IDC_STAGE_ENGDIRX,IDC_STAGE_ENGDIRY,IDC_STAGE_ENGDIRZ,128);

						DMD_stage[stageid].nEngines=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_ENGN,CB_GETCURSEL,0,0);
						switch(DMD_stage[stageid].nEngines)
						{
						case 1:
							{
								DMD_stage[stageid].engV4[0]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG1X,IDC_STAGE_ENG1Y,IDC_STAGE_ENG1Z,IDC_STAGE_ENG1SCALE,128);
								break;
							}
						case 2:
							{
								DMD_stage[stageid].engV4[0]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG1X,IDC_STAGE_ENG1Y,IDC_STAGE_ENG1Z,IDC_STAGE_ENG1SCALE,128);
								DMD_stage[stageid].engV4[1]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG2X,IDC_STAGE_ENG2Y,IDC_STAGE_ENG2Z,IDC_STAGE_ENG2SCALE,128);
								break;
							}
						case 3:
							{
								DMD_stage[stageid].engV4[0]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG1X,IDC_STAGE_ENG1Y,IDC_STAGE_ENG1Z,IDC_STAGE_ENG1SCALE,128);
								DMD_stage[stageid].engV4[1]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG2X,IDC_STAGE_ENG2Y,IDC_STAGE_ENG2Z,IDC_STAGE_ENG2SCALE,128);
								DMD_stage[stageid].engV4[2]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG3X,IDC_STAGE_ENG3Y,IDC_STAGE_ENG3Z,IDC_STAGE_ENG3SCALE,128);
								break;
							}
						case 4:
							{
								DMD_stage[stageid].engV4[0]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG1X,IDC_STAGE_ENG1Y,IDC_STAGE_ENG1Z,IDC_STAGE_ENG1SCALE,128);
								DMD_stage[stageid].engV4[1]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG2X,IDC_STAGE_ENG2Y,IDC_STAGE_ENG2Z,IDC_STAGE_ENG2SCALE,128);
								DMD_stage[stageid].engV4[2]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG3X,IDC_STAGE_ENG3Y,IDC_STAGE_ENG3Z,IDC_STAGE_ENG3SCALE,128);
								DMD_stage[stageid].engV4[3]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG4X,IDC_STAGE_ENG4Y,IDC_STAGE_ENG4Z,IDC_STAGE_ENG4SCALE,128);
								break;
							}
						case 5:
							{
								DMD_stage[stageid].engV4[0]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG1X,IDC_STAGE_ENG1Y,IDC_STAGE_ENG1Z,IDC_STAGE_ENG1SCALE,128);
								DMD_stage[stageid].engV4[1]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG2X,IDC_STAGE_ENG2Y,IDC_STAGE_ENG2Z,IDC_STAGE_ENG2SCALE,128);
								DMD_stage[stageid].engV4[2]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG3X,IDC_STAGE_ENG3Y,IDC_STAGE_ENG3Z,IDC_STAGE_ENG3SCALE,128);
								DMD_stage[stageid].engV4[3]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG4X,IDC_STAGE_ENG4Y,IDC_STAGE_ENG4Z,IDC_STAGE_ENG4SCALE,128);
								DMD_stage[stageid].engV4[4]=GetDlgItemVector4(hChild[CD_STAGE],IDC_STAGE_ENG5X,IDC_STAGE_ENG5Y,IDC_STAGE_ENG5Z,IDC_STAGE_ENG5SCALE,128);
								break;
							}
						}

						getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_PTDEF,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							DMD_stage[stageid].defpitch=TRUE;
							DMD_stage[stageid].pitchthrust=0;
						}else{
							DMD_stage[stageid].defpitch=FALSE;
							DMD_stage[stageid].pitchthrust=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_PITCHTHRUST,128);
						}
						getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_RTDEF,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							DMD_stage[stageid].defroll=TRUE;
							DMD_stage[stageid].rollthrust=0;
						}else{
							DMD_stage[stageid].defroll=FALSE;
							DMD_stage[stageid].rollthrust=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_ROLLTHRUST,128);
						}
						getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_YTDEF,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							DMD_stage[stageid].defyaw=TRUE;
							DMD_stage[stageid].yawthrust=0;
						}else{
							DMD_stage[stageid].defyaw=FALSE;
							DMD_stage[stageid].yawthrust=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_YAWTHRUST,128);
						}
						getcheck=SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_LTDEF,BM_GETCHECK,0,0);
						if(getcheck == BST_CHECKED)
						{
							DMD_stage[stageid].linearthrust=0;
						}else{
							DMD_stage[stageid].linearthrust=GetDlgItemDouble(hChild[CD_STAGE],IDC_STAGE_LINEARTHRUST,128);
						}

						GetDlgItemText(hChild[CD_STAGE],IDC_STAGE_MODULE,DMD_stage[stageid].module.data(),128);
						DMD_stage[stageid].speed=GetDlgItemVector3(hChild[CD_STAGE],IDC_STAGE_SPDX,IDC_STAGE_SPDY,IDC_STAGE_SPDZ,128);
						DMD_stage[stageid].rot_speed=GetDlgItemVector3(hChild[CD_STAGE],IDC_STAGE_RSPDX,IDC_STAGE_RSPDY,IDC_STAGE_RSPDZ,128);

						SaveInterstage(stageid);
						SaveUllage(stageid);

						WriteIni.WriteStage(DMD_stage[stageid],stageid);
						break;
					}





			}
			break;
		}
	}
	return oapiDefDialogProc (hWnd, uMsg, wParam, lParam);
}

BOOL DevModeDlg::LesProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg){
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_LES_WLES:
			{
				LRESULT getcheck;
				getcheck=SendDlgItemMessage(hChild[CD_LES],IDC_LES_WLES,BM_GETCHECK,0,0);
				int LesRead[14]={ 
					IDC_LES_MESHNAME , IDC_LES_OFFX, IDC_LES_OFFY , IDC_LES_OFFZ , 
					IDC_LES_HEIGHT , IDC_LES_DIAM , IDC_LES_EMPTYMASS , IDC_LES_MODULE ,
					IDC_LES_SPDX , IDC_LES_SPDY , IDC_LES_SPDZ ,
					IDC_LES_RSPDX , IDC_LES_RSPDY , IDC_LES_RSPDZ
					};
				std::string LesTxt[14];
				
				if(getcheck == BST_CHECKED)
				{
					SendMultipleDlgItemsMessage(hChild[CD_LES],LesRead,EM_SETREADONLY,FALSE,0,14);
					EnableWindow(GetDlgItem(hChild[CD_LES],IDC_LES_PICKMESH),TRUE);
					EnableWindow(GetDlgItem(hChild[CD_LES],IDC_LES_PICKMODULE),TRUE);
					EnableWindow(GetDlgItem(hChild[CD_LES],IDC_LES_STOREVALS),TRUE);
					
				}else{
					SendMultipleDlgItemsMessage(hChild[CD_LES],LesRead,EM_SETREADONLY,TRUE,0,14);
					EnableWindow(GetDlgItem(hChild[CD_LES],IDC_LES_PICKMESH),FALSE);
					EnableWindow(GetDlgItem(hChild[CD_LES],IDC_LES_PICKMODULE),FALSE);
					//EnableWindow(GetDlgItem(hChild[CD_LES],IDC_LES_STOREVALS),FALSE);
					for(UINT i=0;i<14;i++)
					{
						LesTxt[i].clear();
					}
					SetMultipleDlgItemsText(hChild[CD_LES],LesRead,LesTxt,14);
				}
				break;
			}
		case IDC_LES_PICKMESH:
			{
				char FileNameChar[MAX_PATH];
				sprintf(FileNameChar,"%s", PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
				char dir[MAX_PATH];
				int len=GetCurrentDirectory(MAX_PATH,dir);
				char Msh[9]={"\\Meshes\0"};
				strcat(dir,Msh);
				len+=8;
				char FinalFileName[260];
				int prog=0;
				for(int i=len;i<260;i++)
				{
				FinalFileName[prog]=FileNameChar[i];
				prog++;
				}
				SetWindowText(GetDlgItem(hChild[CD_LES],IDC_LES_MESHNAME),(LPCSTR)FinalFileName);
				break;
			}
		case IDC_LES_PICKMODULE:
			{
				char FileNameChar[MAX_PATH];
				sprintf_s(FileNameChar,PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
			
				char dir[MAX_PATH];
			
				int len=GetCurrentDirectory(MAX_PATH,dir);
				char Msh[17]={"\\Config\\Vessels\0"};
				strcat(dir,Msh);
				len+=16;
			
			
				char FinalFileName[260];
				int prog=0;
				for(int i=len;i<260;i++)
				{
					FinalFileName[prog]=FileNameChar[i];
					prog++;
				}
			
					SetWindowText(GetDlgItem(hChild[CD_LES],IDC_LES_MODULE),(LPCSTR)FinalFileName);
				break;
			}
		case IDC_LES_STOREVALS:
			{
				LRESULT getcheck=SendDlgItemMessage(hChild[CD_LES],IDC_LES_WLES,BM_GETCHECK,0,0);
				if(getcheck == BST_CHECKED)
				{
					DMD_wLes=TRUE;
					GetDlgItemText(hChild[CD_LES],IDC_LES_MESHNAME,DMD_Les.meshname.data(),128);
					DMD_Les.off=GetDlgItemVector3(hChild[CD_LES],IDC_LES_OFFX,IDC_LES_OFFY,IDC_LES_OFFZ,128);
					DMD_Les.height=GetDlgItemDouble(hChild[CD_LES],IDC_LES_HEIGHT,128);
					DMD_Les.diameter=GetDlgItemDouble(hChild[CD_LES],IDC_LES_DIAM,128);
					DMD_Les.emptymass=GetDlgItemDouble(hChild[CD_LES],IDC_LES_EMPTYMASS,128);
					GetDlgItemText(hChild[CD_LES],IDC_LES_MODULE,DMD_Les.module.data(),128);
					DMD_Les.speed=GetDlgItemVector3(hChild[CD_LES],IDC_LES_SPDX,IDC_LES_SPDY,IDC_LES_SPDZ,128);
					DMD_Les.rot_speed=GetDlgItemVector3(hChild[CD_LES],IDC_LES_RSPDX, IDC_LES_RSPDY, IDC_LES_RSPDZ,128);
				}else{
					DMD_wLes=FALSE;
					DMD_Les=LES();
				}

				WriteIni.WriteLes(DMD_Les,DMD_wLes);
				break;
			}
		}
	}
	return oapiDefDialogProc (hWnd, uMsg, wParam, lParam);
}

BOOL DevModeDlg::CurveProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg){
		case WM_PAINT:
		{
		RefreshGraph(std::move(Curve),IDC_BOOSTER_CURVEGRAPH);
		break;
		}
		case WM_INITDIALOG:
		{
			char buff[MAX_PATH];
			for(UINT i=0;i<10;i++)
			{
			sprintf(buff,"%i",i+1);				
			SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVE_N,CB_INSERTSTRING,i,(LPARAM)buff);
			}
			
			break;
		}
		case WM_COMMAND:
			{
				switch(LOWORD(wParam)){
				case IDC_CURVE_CLOSE:
					{
						ShowWindow(hChild[CD_CURVE],SW_HIDE);
						break;
					}
				case IDC_CURVE_N:
					{
						switch(HIWORD(wParam)){
						case CBN_SELCHANGE:
							{
								UINT check=SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVE_N,CB_GETCURSEL,0,0);
								check+=1;
								BoosterCurvePointActive(check);
								break;
							}
						}
						break;
					}
				case IDC_CURVE_UPDATE:
					{
						UINT check=SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVE_N,CB_GETCURSEL,0,0);
						check+=1;
						BoosterDrawGraph(check);
						InvalidateRect(hChild[CD_CURVE],NULL,TRUE);
						//RefreshGraph(Curve,IDC_BOOSTER_CURVEGRAPH);
						break;
					}
				case IDC_CURVE_SAVE:
					{
						UINT id=GetDlgItemInt(hChild[CD_BOOSTER],IDC_BOOSTER_ID,FALSE,FALSE);
						BoosterCurveReadPoints();
						UINT npts=SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVE_N,CB_GETCURSEL,0,0);
						for(UINT i=0;i<=npts;i++)
						{
							DMD_booster[id].curve[i].x=point[i].x;
							DMD_booster[id].curve[i].y=point[i].y;
							DMD_booster[id].curve[i].z=0;
						}
						for(UINT j=npts+1;j<10;j++)
						{
							DMD_booster[id].curve[j].x=9000000;
							DMD_booster[id].curve[j].y=100;
							DMD_booster[id].curve[j].z=0;
						}
						
						break;
					}
				}
				break;
			}
	}
	
	return oapiDefDialogProc (hWnd, uMsg, wParam, lParam);
}

BOOL DevModeDlg::BoosterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT boosterid=0;
	switch (uMsg){
	case WM_INITDIALOG:
		{
			SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_ENGN,CB_INSERTSTRING,0,(LPARAM)TEXT("1"));
			SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_ENGN,CB_INSERTSTRING,1,(LPARAM)TEXT("2"));
			SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_ENGN,CB_INSERTSTRING,2,(LPARAM)TEXT("3"));
			SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_ENGN,CB_INSERTSTRING,3,(LPARAM)TEXT("4"));

			

			break;
		}

	case WM_COMMAND:
		{
			switch(LOWORD(wParam)){
			case IDC_BOOSTER_SHOWCURVE:
				{
					SetWindowPos(hChild[CD_CURVE],HWND_TOP,0,0,0,0,SWP_NOSIZE);
					ShowWindow(hChild[CD_CURVE],SW_SHOW);
					
					UINT check=SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVE_N,CB_GETCURSEL,0,0);
						check+=1;
						BoosterDrawGraph(check);
						InvalidateRect(hChild[CD_CURVE],NULL,TRUE);
					break;
				}
			case IDC_BOOSTER_ENGN:
				{
						switch(HIWORD(wParam)){
						case CBN_SELCHANGE:
							{
								UINT check=SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_ENGN,CB_GETCURSEL,0,0);
								check+=1;
								switch(check){
								case 0:
									{
										int ReadOnly[12]={
											IDC_BOOSTER_ENG1X,IDC_BOOSTER_ENG1Y,IDC_BOOSTER_ENG1Z,
											IDC_BOOSTER_ENG2X,IDC_BOOSTER_ENG2Y,IDC_BOOSTER_ENG2Z,
											IDC_BOOSTER_ENG3X,IDC_BOOSTER_ENG3Y,IDC_BOOSTER_ENG3Z,
											IDC_BOOSTER_ENG4X,IDC_BOOSTER_ENG4Y,IDC_BOOSTER_ENG4Z
										};
										SendMultipleDlgItemsMessage(hChild[CD_BOOSTER],ReadOnly,EM_SETREADONLY,TRUE,0,12);
										std::string empty[12];
										for(UINT i=0;i<12;i++)
										{
											empty[i].clear();
										}
										SetMultipleDlgItemsText(hChild[CD_BOOSTER],ReadOnly,empty,12);
										break;
									}
								case 1:
									{
										int ReadOnly[9]={
											IDC_BOOSTER_ENG2X,IDC_BOOSTER_ENG2Y,IDC_BOOSTER_ENG2Z,
											IDC_BOOSTER_ENG3X,IDC_BOOSTER_ENG3Y,IDC_BOOSTER_ENG3Z,
											IDC_BOOSTER_ENG4X,IDC_BOOSTER_ENG4Y,IDC_BOOSTER_ENG4Z
										};
										int NotReadOnly[3]={
											IDC_BOOSTER_ENG1X,IDC_BOOSTER_ENG1Y,IDC_BOOSTER_ENG1Z
										};
										SendMultipleDlgItemsMessage(hChild[CD_BOOSTER],ReadOnly,EM_SETREADONLY,TRUE,0,9);
										SendMultipleDlgItemsMessage(hChild[CD_BOOSTER],NotReadOnly,EM_SETREADONLY,FALSE,0,3);
										std::string empty[9];
										for(UINT i=0;i<9;i++)
										{
											empty[i].clear();
										}
										SetMultipleDlgItemsText(hChild[CD_BOOSTER],ReadOnly,empty,9);
										break;
									}
								case 2:
									{
										int ReadOnly[6]={
											IDC_BOOSTER_ENG3X,IDC_BOOSTER_ENG3Y,IDC_BOOSTER_ENG3Z,
											IDC_BOOSTER_ENG4X,IDC_BOOSTER_ENG4Y,IDC_BOOSTER_ENG4Z
										};
										int NotReadOnly[6]={
											IDC_BOOSTER_ENG1X,IDC_BOOSTER_ENG1Y,IDC_BOOSTER_ENG1Z,
											IDC_BOOSTER_ENG2X,IDC_BOOSTER_ENG2Y,IDC_BOOSTER_ENG2Z
										};
										SendMultipleDlgItemsMessage(hChild[CD_BOOSTER],ReadOnly,EM_SETREADONLY,TRUE,0,6);
										SendMultipleDlgItemsMessage(hChild[CD_BOOSTER],NotReadOnly,EM_SETREADONLY,FALSE,0,6);
										std::string empty[6];
										for(UINT i=0;i<6;i++)
										{
											empty[i].clear();
										}
										SetMultipleDlgItemsText(hChild[CD_BOOSTER],ReadOnly,empty,6);
										break;
									}
								case 3:
									{										
										int ReadOnly[3]={
											IDC_BOOSTER_ENG4X,IDC_BOOSTER_ENG4Y,IDC_BOOSTER_ENG4Z
										};
										int NotReadOnly[9]={
											IDC_BOOSTER_ENG3X,IDC_BOOSTER_ENG3Y,IDC_BOOSTER_ENG3Z,
											IDC_BOOSTER_ENG1X,IDC_BOOSTER_ENG1Y,IDC_BOOSTER_ENG1Z,
											IDC_BOOSTER_ENG2X,IDC_BOOSTER_ENG2Y,IDC_BOOSTER_ENG2Z
										};
										SendMultipleDlgItemsMessage(hChild[CD_BOOSTER],ReadOnly,EM_SETREADONLY,TRUE,0,3);
										SendMultipleDlgItemsMessage(hChild[CD_BOOSTER],NotReadOnly,EM_SETREADONLY,FALSE,0,9);
										std::string empty[3];
										for(UINT i=0;i<3;i++)
										{
											empty[i].clear();
										}
										SetMultipleDlgItemsText(hChild[CD_BOOSTER],ReadOnly,empty,3);
										break;
									}
								case 4:
									{
										int NotReadOnly[12]={
											IDC_BOOSTER_ENG4X,IDC_BOOSTER_ENG4Y,IDC_BOOSTER_ENG4Z,
											IDC_BOOSTER_ENG3X,IDC_BOOSTER_ENG3Y,IDC_BOOSTER_ENG3Z,
											IDC_BOOSTER_ENG1X,IDC_BOOSTER_ENG1Y,IDC_BOOSTER_ENG1Z,
											IDC_BOOSTER_ENG2X,IDC_BOOSTER_ENG2Y,IDC_BOOSTER_ENG2Z
										};
										
										SendMultipleDlgItemsMessage(hChild[CD_BOOSTER],NotReadOnly,EM_SETREADONLY,FALSE,0,12);
										break;
									}

								}
								break;
							}
						}
					break;
				}
			case IDC_BOOSTER_PICKMODULE:
				{
					boosterid=GetDlgItemInt(hChild[CD_BOOSTER],IDC_BOOSTER_ID,FALSE,FALSE);
			char FileNameChar[MAX_PATH];
			sprintf_s(FileNameChar,PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
			
			char dir[MAX_PATH];
			
			int len=GetCurrentDirectory(MAX_PATH,dir);
			char Msh[17]={"\\Config\\Vessels\0"};
			strcat(dir,Msh);
			len+=16;
			
			
			char FinalFileName[260];
			int prog=0;
			for(int i=len;i<260;i++)
			{
				FinalFileName[prog]=FileNameChar[i];
				prog++;
			}
			
				SetWindowText(GetDlgItem(hChild[CD_BOOSTER],IDC_BOOSTER_MODULE),(LPCSTR)FinalFileName);
					break;
				}
			case IDC_BOOSTER_PICK:
				{
					boosterid=GetDlgItemInt(hChild[CD_BOOSTER],IDC_BOOSTER_ID,FALSE,FALSE);
			char FileNameChar[MAX_PATH];
			sprintf(FileNameChar,"%s", PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
			
			char dir[MAX_PATH];
			
			int len=GetCurrentDirectory(MAX_PATH,dir);
			char Msh[9]={"\\Meshes\0"};
			strcat(dir,Msh);
			len+=8;
			
			
			char FinalFileName[260];
			int prog=0;
			for(int i=len;i<260;i++)
			{
				FinalFileName[prog]=FileNameChar[i];
				prog++;
			}

				SetWindowText(GetDlgItem(hChild[CD_BOOSTER],IDC_BOOSTER_MESHNAME),(LPCSTR)FinalFileName);
			
					break;
				}
			case IDC_BOOSTER_DELBOOSTER:
				{
					boosterid=GetDlgItemInt(hChild[CD_BOOSTER],IDC_BOOSTER_ID,FALSE,FALSE);
					DelItem(Booster,boosterid);
					break;
				}
			case IDC_BOOSTER_ADDBOOSTER:
				{
					AddItem(Booster);
					break;
				}

			case IDC_BOOSTER_WEXPB:
				{
					if(HIWORD(wParam)== BN_CLICKED)
					{
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_WEXPB,BM_GETCHECK,0,0);
						if(getcheck==BST_CHECKED)
						{
							SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBPS,EM_SETREADONLY,FALSE,0);
							SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBANT,EM_SETREADONLY,FALSE,0);
							SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBX,EM_SETREADONLY,FALSE,0);
							SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBY,EM_SETREADONLY,FALSE,0);
							SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBZ,EM_SETREADONLY,FALSE,0);
						}else{
							std::string emptybuff;
							emptybuff.clear();
							SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBPS,emptybuff.c_str());
							SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBANT,emptybuff.c_str());
							SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBX,emptybuff.c_str());
							SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBY,emptybuff.c_str());
							SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBZ,emptybuff.c_str());
							SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBPS,EM_SETREADONLY,TRUE,0);
							SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBANT,EM_SETREADONLY,TRUE,0);
							SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBX,EM_SETREADONLY,TRUE,0);
							SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBY,EM_SETREADONLY,TRUE,0);
							SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBZ,EM_SETREADONLY,TRUE,0);						
						}
					}
				
					
					break;
				}

			case IDC_BOOSTER_STOREVALS:
				{
					boosterid=GetDlgItemInt(hChild[CD_BOOSTER],IDC_BOOSTER_ID,FALSE,FALSE);
					char buff[128];
					DMD_booster[boosterid].N=GetDlgItemInt(hChild[CD_BOOSTER],IDC_BOOSTER_N,FALSE,FALSE);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ANGLE,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].angle);
					DMD_booster[boosterid].angle*=RAD;
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_MESHNAME,DMD_booster[boosterid].meshname.data(),128);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_OFFX,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].off.x);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_OFFY,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].off.y);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_OFFZ,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].off.z);

					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_HEIGHT,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].height);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_DIAM,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].diameter);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_THRUST,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].thrust);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EMPTYMASS,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].emptymass);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_FUELMASS,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].fuelmass);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_BURNTIME,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].burntime);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_BURNDELAY,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].burndelay);

					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENGDIAM,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].eng_diameter);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENGTEX,DMD_booster[boosterid].eng_tex.data(),128);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENGPS1,DMD_booster[boosterid].eng_pstream1.data(),128);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENGPS2,DMD_booster[boosterid].eng_pstream2.data(),128);

					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENGDIRX,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].eng_dir.x);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENGDIRY,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].eng_dir.y);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENGDIRZ,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].eng_dir.z);
					
					DMD_booster[boosterid].nEngines=SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_ENGN,CB_GETCURSEL,0,0);
					DMD_booster[boosterid].nEngines+=1;

					switch(DMD_booster[boosterid].nEngines)
					{
					case 0:
						{
							break;
						}
					case 1:
						{
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG1X,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[0].x);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG1Y,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[0].y);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG1Z,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[0].z);
							break;
						}
					case 2:
						{
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG1X,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[0].x);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG1Y,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[0].y);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG1Z,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[0].z);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG2X,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[1].x);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG2Y,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[1].y);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG2Z,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[1].z);
							break;
						}
					case 3:
						{
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG1X,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[0].x);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG1Y,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[0].y);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG1Z,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[0].z);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG2X,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[1].x);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG2Y,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[1].y);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG2Z,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[1].z);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG3X,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[2].x);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG3Y,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[2].y);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG3Z,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[2].z);
							break;
						}
					case 4:
						{
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG1X,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[0].x);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG1Y,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[0].y);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG1Z,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[0].z);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG2X,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[1].x);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG2Y,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[1].y);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG2Z,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[1].z);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG3X,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[2].x);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG3Y,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[2].y);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG3Z,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[2].z);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG4X,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[3].x);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG4Y,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[3].y);
							GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_ENG4Z,buff,128);
							sscanf(buff,"%lf",&DMD_booster[boosterid].eng[3].z);
							break;
						}
					}
					
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_MODULE,DMD_booster[boosterid].module.data(),256);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_SPDX,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].speed.x);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_SPDY,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].speed.y);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_SPDZ,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].speed.z);

					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_RSPDX,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].rot_speed.x);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_RSPDY,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].rot_speed.y);
					GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_RSPDZ,buff,128);
					sscanf(buff,"%lf",&DMD_booster[boosterid].rot_speed.z);

					LRESULT getcheck;
					getcheck=SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_WEXPB,BM_GETCHECK,0,0);
					if(getcheck==BST_CHECKED)
					{
						GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBPS,DMD_booster[boosterid].expbolt.pstream.data(),128);
						GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBANT,buff,128);
						sscanf(buff,"%lf",&DMD_booster[boosterid].expbolt.anticipation);
						GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBX,buff,128);
						sscanf(buff,"%lf",&DMD_booster[boosterid].expbolt.pos.x);
						GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBY,buff,128);
						sscanf(buff,"%lf",&DMD_booster[boosterid].expbolt.pos.y);
						GetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBZ,buff,128);
						sscanf(buff,"%lf",&DMD_booster[boosterid].expbolt.pos.z);
					}

					WriteIni.WriteBooster(DMD_booster[boosterid],boosterid);

					break;
				}
			}
		}
	}
	return oapiDefDialogProc(hWnd,uMsg,wParam,lParam);
}

BOOL DevModeDlg::TexProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	char buff[256];

	switch(uMsg)
	{
	case WM_COMMAND:
		{
		switch (LOWORD(wParam)) {
		case IDC_TEX_STOREVALS:
			{
				for(UINT i=0;i<16;i++)
				{
					buff[0] = '\0';
					GetDlgItemText(hChild[CD_TEX],IDC_TEX_TEX1+i,buff,256);
					if(buff[0]!='\0'){
						DMD_tex.TextureName[i] = buff;
					} else {
						DMD_nTextures=i;
						break;
					}
				}
				/////sprintf(oapiDebugString(),"nTex: %i",DMD_nTextures);
				WriteIni.WriteTex(DMD_tex,DMD_nTextures);
				break;
			}
		}
		break;
		}
	}
	return oapiDefDialogProc(hWnd,uMsg,wParam,lParam);
}


BOOL DevModeDlg::MiscProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		{
		switch (LOWORD(wParam)) {
		case IDC_MISC_STOREVALS:
			{
				char buff[128];
				GetDlgItemText(hChild[CD_MISC],IDC_MISC_COG,buff,128);
				sscanf(buff,"%lf",&DMD_Misc.COG);
				GetDlgItemText(hChild[CD_MISC],IDC_MISC_VERTICALANGLE,buff,128);
				sscanf(buff,"%lf",&DMD_Misc.VerticalAngle);
				DMD_Misc.VerticalAngle*=RAD;

				LRESULT GetCheck;
				GetCheck=SendDlgItemMessage(hChild[CD_MISC],IDC_MISC_GNCDEBUG,BM_GETCHECK,0,0);
				if(GetCheck==BST_CHECKED)
				{
					DMD_Misc.GNC_Debug=1;
				}else{
					DMD_Misc.GNC_Debug=0;
				}

				GetCheck=SendDlgItemMessage(hChild[CD_MISC],IDC_MISC_TELEMETRY,BM_GETCHECK,0,0);
				if(GetCheck==BST_CHECKED)
				{
					DMD_Misc.telemetry=TRUE;
				}else{
					DMD_Misc.telemetry=FALSE;
				}

				GetCheck=SendDlgItemMessage(hChild[CD_MISC],IDC_MISC_THRUSTREALPOS,BM_GETCHECK,0,0);
				if(GetCheck==BST_CHECKED)
				{
					DMD_Misc.thrustrealpos=TRUE;
				}else{
					DMD_Misc.thrustrealpos=FALSE;
				}
				
				int result=SendDlgItemMessage(hChild[CD_MISC],IDC_MISC_FOCUS,CB_GETCURSEL,0,0);
				DMD_Misc.Focus=result+1;

				GetCheck=SendDlgItemMessage(hChild[CD_MISC],IDC_MISC_WPADMODULE,BM_GETCHECK,0,0);
				if(GetCheck==BST_CHECKED){
					char PadModule_aux[256];
					GetDlgItemText(hChild[CD_MISC],IDC_MISC_PADMODULE, PadModule_aux,256);
					DMD_Misc.PadModule = PadModule_aux;
				} else {
					DMD_Misc.PadModule = "EmptyModule";
				}

				WriteIni.WriteMisc(DMD_Misc);

				break;
			}
		
		}
	case IDC_MISC_WPADMODULE:
		{
			if(HIWORD(wParam)== BN_CLICKED)
			{
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_MISC],IDC_MISC_WPADMODULE,BM_GETCHECK,0,0);
						if(getcheck==BST_CHECKED)
						{
							SendDlgItemMessage(hChild[CD_MISC],IDC_MISC_PADMODULE,EM_SETREADONLY,FALSE,0);
						}else{
							SendDlgItemMessage(hChild[CD_MISC],IDC_MISC_PADMODULE,EM_SETREADONLY,TRUE,0);
							SetDlgItemText(hChild[CD_MISC],IDC_MISC_PADMODULE,(LPCSTR)" ");
						}
			}
			break;
		}
		break;
		}
	}
	return oapiDefDialogProc(hWnd,uMsg,wParam,lParam);
}
BOOL DevModeDlg::ParticleProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT partid=0;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
		
		//	int check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LTYPE,CB_FINDSTRING,-1,(LPARAM)TEXT("EMISSIVE"));
			//if(check==CB_ERR){
				SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_LTYPE,CB_INSERTSTRING,0,(LPARAM)TEXT("EMISSIVE"));
			//}
			//check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LTYPE,CB_FINDSTRING,-1,(LPARAM)TEXT("DIFFUSE"));
			//if(check==CB_ERR){
				SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_LTYPE,CB_INSERTSTRING,1,(LPARAM)TEXT("DIFFUSE"));
			//}
			
			//check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("LVL_FLAT"));
			//if(check==CB_ERR){
				SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_LEVELMAP,CB_INSERTSTRING,0,(LPARAM)TEXT("LVL_FLAT"));
			//}
			//check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("LVL_LIN"));
			//if(check==CB_ERR){
				SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_LEVELMAP,CB_INSERTSTRING,1,(LPARAM)TEXT("LVL_LIN"));
			//}
			//check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("LVL_SQRT"));
			//if(check==CB_ERR){
				SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_LEVELMAP,CB_INSERTSTRING,2,(LPARAM)TEXT("LVL_SQRT"));
			//}
			//check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("LVL_PLIN"));
			//if(check==CB_ERR){
				SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_LEVELMAP,CB_INSERTSTRING,3,(LPARAM)TEXT("LVL_PLIN"));
			//}
			//check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("LVL_PSQRT"));
			//if(check==CB_ERR){
				SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_LEVELMAP,CB_INSERTSTRING,4,(LPARAM)TEXT("LVL_PSQRT"));
			//}
			//check=SendDlgItemMessage(hChild[iddialog],IDC_PART_ATMSMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("ATM_FLAT"));
			//if(check==CB_ERR){
				SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_ATMSMAP,CB_INSERTSTRING,0,(LPARAM)TEXT("ATM_FLAT"));
			//}
			//check=SendDlgItemMessage(hChild[iddialog],IDC_PART_ATMSMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("ATM_PLIN"));
			//if(check==CB_ERR){
				SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_ATMSMAP,CB_INSERTSTRING,1,(LPARAM)TEXT("ATM_PLIN"));
			//}
			//check=SendDlgItemMessage(hChild[iddialog],IDC_PART_ATMSMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("ATM_PLOG"));
			//if(check==CB_ERR){
				SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_ATMSMAP,CB_INSERTSTRING,2,(LPARAM)TEXT("ATM_PLOG"));
			//}

			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_PART_ENABLEGROWING:
				{
					if(HIWORD(wParam)== BN_CLICKED)
					{
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_ENABLEGROWING,BM_GETCHECK,0,0);
						if(getcheck==BST_CHECKED)
						{
						SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_GFS,EM_SETREADONLY,FALSE,0);
						SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_GFR,EM_SETREADONLY,FALSE,0);
						}else{
						SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_GFS,EM_SETREADONLY,TRUE,0);
						SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_GFR,EM_SETREADONLY,TRUE,0);
						}
					}
					break;
				}
			case IDC_PART_TESTPART:
				{
					if(!TestingParticle){
					TestingParticle=TRUE;
					SetDlgItemText(hChild[CD_PARTICLE],IDC_PART_TESTPART,TEXT("STOP TEST"));
					

					
					char buff[128];
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_SRCSIZE,buff,128);
					sscanf(buff,"%lf",&test_particle.srcsize);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_SRCRATE,buff,128);
					sscanf(buff,"%lf",&test_particle.srcrate);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_V0,buff,128);					
					sscanf(buff,"%lf",&test_particle.v0);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_LIFETIME,buff,128);					
					sscanf(buff,"%lf",&test_particle.lifetime);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_GROWTHRATE,buff,128);					
					sscanf(buff,"%lf",&test_particle.growthrate);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_ATMSLOWDOWN,buff,128);					
					sscanf(buff,"%lf",&test_particle.atmslowdown);

					int result=SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_LTYPE,CB_GETCURSEL,0,0);
					if(result==0){
						test_particle.ltype=PARTICLESTREAMSPEC::EMISSIVE;
					}else{
						test_particle.ltype=PARTICLESTREAMSPEC::DIFFUSE;
					}


					result=SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_LEVELMAP,CB_GETCURSEL,0,0);
					if(result==0){
						test_particle.levelmap=PARTICLESTREAMSPEC::LVL_FLAT;
					}else if(result==1){
						test_particle.levelmap=PARTICLESTREAMSPEC::LVL_LIN;
					}else if(result==2){
						test_particle.levelmap=PARTICLESTREAMSPEC::LVL_SQRT;
					}else if(result==3){
						test_particle.levelmap=PARTICLESTREAMSPEC::LVL_PLIN;
					}else if(result==4){
						test_particle.levelmap=PARTICLESTREAMSPEC::LVL_PSQRT;
					}else{
						test_particle.levelmap=PARTICLESTREAMSPEC::LVL_FLAT;
					}


					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_LMIN,buff,128);					
					sscanf(buff,"%lg",&test_particle.lmin);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_LMAX,buff,128);					
					sscanf(buff,"%lf",&test_particle.lmax);

					result=SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_ATMSMAP,CB_GETCURSEL,0,0);
					if(result==0)
					{
						test_particle.atmsmap=PARTICLESTREAMSPEC::ATM_FLAT;
					}else if(result==1)
					{
						test_particle.atmsmap=PARTICLESTREAMSPEC::ATM_PLIN;
					}else if(result==2)
					{
						test_particle.atmsmap=PARTICLESTREAMSPEC::ATM_PLOG;
					}else{
						test_particle.atmsmap=PARTICLESTREAMSPEC::ATM_FLAT;
					}

					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_AMIN,buff,128);
					sscanf(buff,"%lg",&test_particle.amin);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_AMAX,buff,128);
					sscanf(buff,"%lf",&test_particle.amax);

					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_TEX,buff,128);
					test_particle.tex=oapiRegisterExhaustTexture(buff);
					
					//char testt[4096];
					//sprintf(testt,"contrail");
					test_part_h=Ms26->AddParticleStream(&test_particle,_V(75,0,10),_V(0,0,-1),&sliderlvl);
					ShowWindow(GetDlgItem(hChild[CD_PARTICLE],IDC_PART_SLIDERLEVEL),SW_SHOW);
					SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_SLIDERLEVEL,TBM_SETRANGEMIN,TRUE,0);
					SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_SLIDERLEVEL,TBM_SETRANGEMAX,TRUE,100);
					
					/*oapiWriteLogV("size: %.3f",test_particle.srcsize);
					oapiWriteLogV("rate: %.3f",test_particle.srcrate);
					oapiWriteLogV("V0: %.3f",test_particle.v0);
					oapiWriteLogV("Spread: %.3f",test_particle.srcspread);
					oapiWriteLogV("lifetime: %.3f",test_particle.lifetime);
					oapiWriteLogV("growthrate: %.3f",test_particle.growthrate);
					oapiWriteLogV("Atmslowdown: %.3f",test_particle.atmslowdown);
					oapiWriteLogV("lmin:%.3f",test_particle.lmin);
					oapiWriteLogV("lmax:%.3f",test_particle.lmax);
					oapiWriteLogV("amin:%.3f",test_particle.amin);
					oapiWriteLogV("amax:%.4f",test_particle.amax);
					*/

					}else{
						TestingParticle=FALSE;
						Ms26->DelExhaustStream(test_part_h);
						ShowWindow(GetDlgItem(hChild[CD_PARTICLE],IDC_PART_SLIDERLEVEL),SW_HIDE);
						SetDlgItemText(hChild[CD_PARTICLE],IDC_PART_TESTPART,TEXT("TEST \n\nThis Particle Stream Definition"));
					}

					break;
				}
				case IDC_PART_DELPART:
					{
						partid=GetDlgItemInt(hChild[CD_PARTICLE],IDC_PART_ID,FALSE,FALSE);
						DelItem(Particle,partid);
						break;
					}
				case IDC_PART_ADDPART:
					{
						AddItem(Particle);
						break;
					}
				case IDC_PART_STOREVALS:
					{
						partid=GetDlgItemInt(hChild[CD_PARTICLE],IDC_PART_ID,FALSE,FALSE);
						//sprintf(oapiDebugString(),"PARTID: %i",partid);
						///////////////////////////////////////////////////////////////////////
					char particleName[256];
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_NAME,particleName,256);					
					DMD_Particle[partid].ParticleName = particleName;

						char buff[128];
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_SRCSIZE,buff,128);
					sscanf(buff,"%lf",&DMD_Particle[partid].Pss.srcsize);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_SRCRATE,buff,128);
					sscanf(buff,"%lf",&DMD_Particle[partid].Pss.srcrate);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_V0,buff,128);					
					sscanf(buff,"%lf",&DMD_Particle[partid].Pss.v0);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_LIFETIME,buff,128);					
					sscanf(buff,"%lf",&DMD_Particle[partid].Pss.lifetime);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_GROWTHRATE,buff,128);					
					sscanf(buff,"%lf",&DMD_Particle[partid].Pss.growthrate);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_ATMSLOWDOWN,buff,128);					
					sscanf(buff,"%lf",&DMD_Particle[partid].Pss.atmslowdown);

					int result=SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_LTYPE,CB_GETCURSEL,0,0);
					if(result==0){
						DMD_Particle[partid].Pss.ltype=PARTICLESTREAMSPEC::EMISSIVE;
					}else{
						DMD_Particle[partid].Pss.ltype=PARTICLESTREAMSPEC::DIFFUSE;
					}


					result=SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_LEVELMAP,CB_GETCURSEL,0,0);
					if(result==0){
						DMD_Particle[partid].Pss.levelmap=PARTICLESTREAMSPEC::LVL_FLAT;
					}else if(result==1){
						DMD_Particle[partid].Pss.levelmap=PARTICLESTREAMSPEC::LVL_LIN;
					}else if(result==2){
						DMD_Particle[partid].Pss.levelmap=PARTICLESTREAMSPEC::LVL_SQRT;
					}else if(result==3){
						DMD_Particle[partid].Pss.levelmap=PARTICLESTREAMSPEC::LVL_PLIN;
					}else if(result==4){
						DMD_Particle[partid].Pss.levelmap=PARTICLESTREAMSPEC::LVL_PSQRT;
					}else{
						DMD_Particle[partid].Pss.levelmap=PARTICLESTREAMSPEC::LVL_FLAT;
					}


					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_LMIN,buff,128);					
					sscanf(buff,"%lg",&DMD_Particle[partid].Pss.lmin);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_LMAX,buff,128);					
					sscanf(buff,"%lf",&DMD_Particle[partid].Pss.lmax);

					result=SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_ATMSMAP,CB_GETCURSEL,0,0);
					if(result==0)
					{
						DMD_Particle[partid].Pss.atmsmap=PARTICLESTREAMSPEC::ATM_FLAT;
					}else if(result==1)
					{
						DMD_Particle[partid].Pss.atmsmap=PARTICLESTREAMSPEC::ATM_PLIN;
					}else if(result==2)
					{
						DMD_Particle[partid].Pss.atmsmap=PARTICLESTREAMSPEC::ATM_PLOG;
					}else{
						DMD_Particle[partid].Pss.atmsmap=PARTICLESTREAMSPEC::ATM_FLAT;
					}

					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_AMIN,buff,128);
					sscanf(buff,"%lg",&DMD_Particle[partid].Pss.amin);
					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_AMAX,buff,128);
					sscanf(buff,"%lf",&DMD_Particle[partid].Pss.amax);

					GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_TEX,buff,128);
					DMD_Particle[partid].Pss.tex=oapiRegisterExhaustTexture(buff);
					DMD_Particle[partid].TexName = buff;

					LRESULT getcheck;
					getcheck=SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_ENABLEGROWING,BM_GETCHECK,0,0);
					if(getcheck==BST_CHECKED)
						{
							GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_GFS,buff,128);
							sscanf(buff,"%lf",&DMD_Particle[partid].GrowFactor_size);
							GetDlgItemText(hChild[CD_PARTICLE],IDC_PART_GFR,buff,128);
							sscanf(buff,"%lf",&DMD_Particle[partid].GrowFactor_rate);
							DMD_Particle[partid].Growing=TRUE;
					}else{
						DMD_Particle[partid].GrowFactor_rate=0;
						DMD_Particle[partid].GrowFactor_size=0;
						DMD_Particle[partid].Growing=FALSE;						
						}
					
					
						//////////////////////////////////////////////////////////////////////////
					WriteIni.WriteParticle(DMD_Particle[partid],partid);
						break;
					}
			
			}
			break;
		}
		case WM_HSCROLL:
			{
				switch (LOWORD(wParam)) { 
					
				case TB_ENDTRACK: 
					{
						
							DWORD currpos=SendDlgItemMessage(hChild[CD_PARTICLE],IDC_PART_SLIDERLEVEL,TBM_GETPOS,0,0);
							sliderlvl=(double)currpos/100;
						//	sprintf(oapiDebugString(),"slider: %.3f",sliderlvl);
					
					break;
					}
				}
				break;
			}
	}
	return oapiDefDialogProc(hWnd,uMsg,wParam,lParam);
}
BOOL DevModeDlg::ViewTxtProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return oapiDefDialogProc (hWnd, uMsg, wParam, lParam);
}
BOOL DevModeDlg::FairingProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch(uMsg)
	{
	case WM_COMMAND:
		{
			switch(LOWORD(wParam)){
			case IDC_FAIR_PICKMESH:
				{
				char FileNameChar[MAX_PATH];
				sprintf(FileNameChar,"%s", PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
				char dir[MAX_PATH];
				int len=GetCurrentDirectory(MAX_PATH,dir);
				char Msh[9]={"\\Meshes\0"};
				strcat(dir,Msh);
				len+=8;
				char FinalFileName[260];
				int prog=0;
				for(int i=len;i<260;i++)
				{
				FinalFileName[prog]=FileNameChar[i];
				prog++;
				}
				SetWindowText(GetDlgItem(hChild[CD_FAIRING],IDC_FAIR_MESHNAME),(LPCSTR)FinalFileName);
				
				break;
				}
			case IDC_FAIR_HIDE:
				{
					if(HIWORD(wParam)== BN_CLICKED)
					{
						
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_FAIRING],IDC_FAIR_HIDE,BM_GETCHECK,0,0);
						if(getcheck==BST_CHECKED){
							for(UINT NF=1;NF<Ms26->fairing.N+1;NF++)
							{
							Ms26->SetMeshVisibilityMode(Ms26->fairing.msh_idh[NF],MESHVIS_NEVER);
							hideFairing=TRUE;
							}
							
						}else{
							for(UINT NF=1;NF<Ms26->fairing.N+1;NF++)
							{
							Ms26->SetMeshVisibilityMode(Ms26->fairing.msh_idh[NF],MESHVIS_EXTERNAL);
							hideFairing=FALSE;
							}
						}
					}
				break;
				}
			case IDC_FAIR_HIDEHALF:
				{
					if(HIWORD(wParam)== BN_CLICKED)
					{
						LRESULT getcheck;
						getcheck=SendDlgItemMessage(hChild[CD_FAIRING],IDC_FAIR_HIDEHALF,BM_GETCHECK,0,0);
							
						UINT half=Ms26->fairing.N;
						half+=1;
						half=half/2;
						
						if(getcheck==BST_CHECKED){
							for(UINT NF=1;NF<=half;NF++)
							{
							Ms26->SetMeshVisibilityMode(Ms26->fairing.msh_idh[NF],MESHVIS_NEVER);
							hideHalfFairing=TRUE;
							}
							
						}else{
							for(UINT NF=1;NF<=half;NF++)
							{
							Ms26->SetMeshVisibilityMode(Ms26->fairing.msh_idh[NF],MESHVIS_EXTERNAL);
							hideHalfFairing=FALSE;
							}
						}
					}
				break;
				}
			case IDC_FAIR_PICKMODULE:
			{
			
			char FileNameChar[MAX_PATH];
			sprintf_s(FileNameChar,PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
			
			char dir[MAX_PATH];
			
			int len=GetCurrentDirectory(MAX_PATH,dir);
			char Msh[17]={"\\Config\\Vessels\0"};
			strcat(dir,Msh);
			len+=16;
			
			
			char FinalFileName[260];
			int prog=0;
			for(int i=len;i<260;i++)
			{
				FinalFileName[prog]=FileNameChar[i];
				prog++;
			}
			
				SetWindowText(GetDlgItem(hChild[CD_FAIRING],IDC_FAIR_MODULE),(LPCSTR)FinalFileName);
				break;
			}
			case IDC_FAIR_WFAIR:
			{
				LRESULT getcheck=SendDlgItemMessage(hChild[CD_FAIRING],IDC_FAIR_WFAIR,BM_GETCHECK,0,0);
				int FairRead[16]={ IDC_FAIR_MESHNAME , IDC_FAIR_OFFX, IDC_FAIR_OFFY, IDC_FAIR_OFFZ,
					IDC_FAIR_N, IDC_FAIR_ANGLE, IDC_FAIR_HEIGHT, IDC_FAIR_DIAM, IDC_FAIR_EMPTYMASS,
					IDC_FAIR_MODULE, IDC_FAIR_SPDX, IDC_FAIR_SPDY, IDC_FAIR_SPDZ, 
					IDC_FAIR_RSPDX, IDC_FAIR_RSPDY, IDC_FAIR_RSPDZ
				};
				if(getcheck == BST_CHECKED)
				{
					SendMultipleDlgItemsMessage(hChild[CD_FAIRING],FairRead,EM_SETREADONLY,FALSE,0,16);
					EnableWindow(GetDlgItem(hChild[CD_FAIRING],IDC_FAIR_PICKMESH),TRUE);
					EnableWindow(GetDlgItem(hChild[CD_FAIRING],IDC_FAIR_PICKMODULE),TRUE);
					EnableWindow(GetDlgItem(hChild[CD_FAIRING],IDC_FAIR_HIDE),TRUE);
					EnableWindow(GetDlgItem(hChild[CD_FAIRING],IDC_FAIR_HIDEHALF),TRUE);
					DMD_wFairing=1;
				}else{
					SendMultipleDlgItemsMessage(hChild[CD_FAIRING],FairRead,EM_SETREADONLY,TRUE,0,16);
					EnableWindow(GetDlgItem(hChild[CD_FAIRING],IDC_FAIR_PICKMESH),FALSE);
					EnableWindow(GetDlgItem(hChild[CD_FAIRING],IDC_FAIR_PICKMODULE),FALSE);
					EnableWindow(GetDlgItem(hChild[CD_FAIRING],IDC_FAIR_HIDE),FALSE);
					EnableWindow(GetDlgItem(hChild[CD_FAIRING],IDC_FAIR_HIDEHALF),FALSE);
					std::string fairtxt[16];
					for(UINT i=0;i<16;i++)
					{
						fairtxt[i].clear();
					}
					SetMultipleDlgItemsText(hChild[CD_FAIRING],FairRead,fairtxt,16);
					DMD_wFairing=0;
				}
				break;
			}
			case IDC_FAIR_STOREVALS:
			{
				char buff[128];
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_MESHNAME,(LPSTR)DMD_fairing.meshname.data(),128);
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_OFFX,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.off.x);
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_OFFY,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.off.y);
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_OFFZ,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.off.z);
				
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_N,(LPSTR)buff,128);
				sscanf(buff,"%i",&DMD_fairing.N);
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_ANGLE,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.angle);

				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_HEIGHT,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.height);
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_DIAM,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.diameter);
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_EMPTYMASS,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.emptymass);
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_MODULE,(LPSTR)DMD_fairing.module.data(),128);
				
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_SPDX,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.speed.x);
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_SPDY,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.speed.y);
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_SPDZ,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.speed.z);

				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_RSPDX,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.rot_speed.x);
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_RSPDY,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.rot_speed.y);
				GetDlgItemText(hChild[CD_FAIRING],IDC_FAIR_RSPDZ,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_fairing.rot_speed.z);

				bool wFair=FALSE;
				if(DMD_wFairing==1){wFair=TRUE;}
				WriteIni.WriteFairing(DMD_fairing,wFair);
				break;
			}
			}
			break;
		}
	}
	return oapiDefDialogProc(hWnd,uMsg,wParam,lParam);
}
BOOL DevModeDlg::PldProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT pldid=0;
	
	switch(uMsg)
	{
	case WM_COMMAND:
		{
		switch (LOWORD(wParam)) {
		case IDC_PLD_PICK:
			{
			pldid=GetDlgItemInt(hChild[CD_PLD],IDC_PLD_ID,FALSE,FALSE);
			char FileNameChar[MAX_PATH];
			sprintf(FileNameChar,"%s", PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
			
			char dir[MAX_PATH];
			
			int len=GetCurrentDirectory(MAX_PATH,dir);
			char Msh[9]={"\\Meshes\0"};
			strcat(dir,Msh);
			len+=8;
			
			
			char FinalFileName[260];
			int prog=0;
			for(int i=len;i<260;i++)
			{
				FinalFileName[prog]=FileNameChar[i];
				prog++;
			}



		
				SetWindowText(GetDlgItem(hChild[CD_PLD],IDC_PLD_MESHNAME),(LPCSTR)FinalFileName);
			
				break;
			}

		case IDC_PLD_PICKMODULE:
			{
			pldid=GetDlgItemInt(hChild[CD_PLD],IDC_PLD_ID,FALSE,FALSE);
			char FileNameChar[MAX_PATH];
			sprintf_s(FileNameChar,PickFileName(hWnd,uMsg,wParam,lParam, true).c_str());
			
			char dir[MAX_PATH];
			
			int len=GetCurrentDirectory(MAX_PATH,dir);
			char Msh[17]={"\\Config\\Vessels\0"};
			strcat(dir,Msh);
			len+=16;
			
			
			char FinalFileName[260];
			int prog=0;
			for(int i=len;i<260;i++)
			{
				FinalFileName[prog]=FileNameChar[i];
				prog++;
			}
			
				SetWindowText(GetDlgItem(hChild[CD_PLD],IDC_PLD_MODULE),(LPCSTR)FinalFileName);
				break;
			}
	/*	case IDC_PLD_UPDMSH:
			{
				pldid=GetDlgItemInt(hChild[CD_PLD],IDC_PLD_ID,FALSE,FALSE);
				for(int i=0;i<Ms26->payload[pldid].nMeshes;i++)
				{
				Ms26->ShiftMesh(Ms26->payload[pldid].msh_idh[i],-);
				}
				break;
			}*/
		case IDC_PLD_STOREVALS:
			{
				pldid=GetDlgItemInt(hChild[CD_PLD],IDC_PLD_ID,FALSE,FALSE);

				

				char buff[128];
				LRESULT getcheck;
				getcheck=SendDlgItemMessage(hChild[CD_PLD],IDC_PLD_MULTIMSH,BM_GETCHECK,0,0);
				if(getcheck==BST_CHECKED)
				{
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_MULTIMSHNAME,(LPSTR)DMD_payload[pldid].meshname.data(),128);
				char multioff[256];
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_MULTIOFF,(LPSTR)multioff,128);
				DMD_payload[pldid].MultiOffset = multioff;
				}else{
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_MESHNAME,(LPSTR)DMD_payload[pldid].meshname.data(),128);
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_OFFX,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].off[0].x);
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_OFFY,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].off[0].y);
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_OFFZ,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].off[0].z);
				DMD_payload[pldid].MultiOffset = std::format("({:.3f},{:.3f},{:.3f})",DMD_payload[pldid].off[0].x,DMD_payload[pldid].off[0].y,DMD_payload[pldid].off[0].z);
				
				}
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_MODULE,(LPSTR)DMD_payload[pldid].module.data(),128);
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_NAME,(LPSTR)DMD_payload[pldid].name.data(),128);
				
				

				GetDlgItemText(hChild[CD_PLD],IDC_PLD_ROTX,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].Rotation.x);
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_ROTY,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].Rotation.y);
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_ROTZ,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].Rotation.z);


				GetDlgItemText(hChild[CD_PLD],IDC_PLD_HEIGHT,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].height);
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_DIAM,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].diameter);
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_MASS,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].mass);
				
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_SPDX,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].speed.x);
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_SPDY,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].speed.y);
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_SPDZ,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].speed.z);

				GetDlgItemText(hChild[CD_PLD],IDC_PLD_RSPDX,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].rot_speed.x);
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_RSPDY,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].rot_speed.y);
				GetDlgItemText(hChild[CD_PLD],IDC_PLD_RSPDZ,(LPSTR)buff,128);
				sscanf(buff,"%lf",&DMD_payload[pldid].rot_speed.z);
				
				getcheck=SendDlgItemMessage(hChild[CD_PLD],IDC_PLD_LIVE,BM_GETCHECK,0,0);
				if(getcheck==BST_CHECKED)
				{
				DMD_payload[pldid].live=TRUE;
				}else{
				DMD_payload[pldid].live=FALSE;
				}
				getcheck=SendDlgItemMessage(hChild[CD_PLD],IDC_PLD_RENDER,BM_GETCHECK,0,0);
				if(getcheck==BST_CHECKED)
				{
				DMD_payload[pldid].render=1;
				}else{
				DMD_payload[pldid].render=0;
				}
				
				WriteIni.WritePayload(DMD_payload[pldid],pldid);
				break;
			}
		case IDC_PLD_MULTIMSH:
			{
				if(HIWORD(wParam)== BN_CLICKED)
				{
				LRESULT getcheck;
				getcheck=SendDlgItemMessage(hChild[CD_PLD],IDC_PLD_MULTIMSH,BM_GETCHECK,0,0);
				if(getcheck==BST_CHECKED){
				ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_OFFX),SW_HIDE);
				ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_OFFY),SW_HIDE);
				ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_OFFZ),SW_HIDE);
				ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_MESHNAME),SW_HIDE);
				ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_MULTIOFF),SW_SHOW);
				ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_MULTIMSHNAME),SW_SHOW);
				}else{
				ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_OFFX),SW_SHOW);
				ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_OFFY),SW_SHOW);
				ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_OFFZ),SW_SHOW);
				ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_MESHNAME),SW_SHOW);
				ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_MULTIOFF),SW_HIDE);
				ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_MULTIMSHNAME),SW_HIDE);
				}
				}
				break;
			}
		case IDC_PLD_LIVE:
			{
				if(HIWORD(wParam)== BN_CLICKED)
				{
				LRESULT getcheck;
				getcheck=SendDlgItemMessage(hChild[CD_PLD],IDC_PLD_LIVE,BM_GETCHECK,0,0);
				if(getcheck==BST_CHECKED){
				EnableWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_MESHNAME),FALSE);
				EnableWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_MULTIMSHNAME),FALSE);
				EnableWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_UPDMSH),FALSE);
				}else{
				EnableWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_MESHNAME),TRUE);
				EnableWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_MULTIMSHNAME),TRUE);
				EnableWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_UPDMSH),TRUE);
				}
				}
				break;
			}
		case IDC_PLD_DELPLD:
			{
				pldid=GetDlgItemInt(hChild[CD_PLD],IDC_PLD_ID,FALSE,FALSE);
				DelItem(Payload,pldid);
				
				break;
			}
		case IDC_PLD_ADDPLD:
			{
				AddItem(Payload);
				break;
			}
		}

		break;
	}
	}
	return oapiDefDialogProc (hWnd, uMsg, wParam, lParam);
}

void DevModeDlg::InitParams()
{
	for(int i=0;i<10;i++)
	{
		DMD_stage[i]=Ms26->stage[i];
		DMD_booster[i]=Ms26->booster[i];
		DMD_payload[i]=Ms26->payload[i];
	}
	DMD_fairing=Ms26->fairing;
	DMD_Misc=Ms26->Misc;
	DMD_tex=Ms26->tex;
	DMD_Les=Ms26->Les;
	
	for(int j=0;j<16;j++)
	{
	DMD_Particle[j]=Ms26->Particle[j];
	}

	DMD_nStages=Ms26->nStages;
	DMD_nBoosters=Ms26->nBoosters;
	DMD_nInterstages=Ms26->nInterstages;
	DMD_nPayloads=Ms26->nPayloads;
	DMD_nParticles=Ms26->nParticles;
	DMD_nTextures=Ms26->nTextures;
	DMD_wFairing=Ms26->wFairing;
	DMD_wAdapter=Ms26->wAdapter;
	DMD_wFXLaunch=Ms26->wLaunchFX;
	if(DMD_wAdapter)
	{
		DMD_Adapter=Ms26->Adapter;
	}else{
		DMD_Adapter=ADAPTER();
	}
	DMD_wLes=Ms26->wLes;
	DMD_wMach=Ms26->wMach;
	DMD_wVent=Ms26->wVent;
	DMD_FX_Vent=Ms26->FX_Vent;
	DMD_FX_Mach=Ms26->FX_Mach;
	DMD_FX_Launch=Ms26->FX_Launch;
	ReverseOffsets();
}


void DevModeDlg::BoosterCurvePointActive(UINT npts)
{
	switch(npts){
			case 0:
				{
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX1,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY1,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX2,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY2,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX3,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY3,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX4,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY4,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX5,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY5,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX6,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY6,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX10,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY10,EM_SETREADONLY,TRUE,0);

					break;
				}
			case 1:
				{
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX1,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY1,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX2,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY2,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX3,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY3,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX4,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY4,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX5,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY5,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX6,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY6,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX10,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY10,EM_SETREADONLY,TRUE,0);

					break;
				}
			case 2:
				{
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX1,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY1,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX2,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY2,EM_SETREADONLY,FALSE,0);

					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX3,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY3,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX4,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY4,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX5,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY5,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX6,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY6,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX10,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY10,EM_SETREADONLY,TRUE,0);
					break;
				}
			case 3:
				{
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX1,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY1,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX2,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY2,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX3,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY3,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX4,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY4,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX5,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY5,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX6,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY6,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX10,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY10,EM_SETREADONLY,TRUE,0);
					break;
				}
			case 4:
				{
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX1,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY1,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX2,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY2,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX3,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY3,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX4,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY4,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX5,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY5,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX6,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY6,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX10,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY10,EM_SETREADONLY,TRUE,0);
					
					break;
				}
			case 5:
				{
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX1,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY1,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX2,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY2,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX3,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY3,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX4,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY4,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX5,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY5,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX6,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY6,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX10,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY10,EM_SETREADONLY,TRUE,0);

					break;
				}
			case 6:
				{
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX1,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY1,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX2,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY2,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX3,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY3,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX4,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY4,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX5,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY5,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX6,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY6,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY7,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX10,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY10,EM_SETREADONLY,TRUE,0);
	
					break;
				}
			case 7:
				{
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX1,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY1,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX2,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY2,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX3,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY3,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX4,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY4,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX5,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY5,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX6,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY6,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX7,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY7,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY8,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX10,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY10,EM_SETREADONLY,TRUE,0);

					break;
				}
			case 8:
				{
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX1,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY1,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX2,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY2,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX3,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY3,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX4,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY4,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX5,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY5,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX6,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY6,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX7,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY7,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX8,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY8,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY9,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX10,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY10,EM_SETREADONLY,TRUE,0);

					break;
				}
			case 9:
				{
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX1,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY1,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX2,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY2,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX3,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY3,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX4,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY4,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX5,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY5,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX6,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY6,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX7,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY7,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX8,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY8,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX9,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY9,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX10,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY10,EM_SETREADONLY,TRUE,0);

					break;
				}
			case 10:
				{
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX1,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY1,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX2,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY2,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX3,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY3,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX4,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY4,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX5,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY5,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX6,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY6,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX7,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY7,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX8,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY8,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX9,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY9,EM_SETREADONLY,FALSE,0);
					
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEX10,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVEY10,EM_SETREADONLY,FALSE,0);
					
					break;
				}
			}
}

void DevModeDlg::PopulateBoosters(UINT iddialog, UINT id)
{
			char buff[256];
			std::string emptybuff;
			emptybuff.clear();
			sprintf(buff,"%i",id-1);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ID,buff);
			sprintf(buff,"%i",DMD_booster[id-1].N);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_N,buff);
			sprintf(buff,"%.1f",DMD_booster[id-1].angle);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ANGLE,buff);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_MESHNAME,DMD_booster[id-1].meshname.data());
			sprintf(buff,"%.3f",DMD_booster[id-1].off.x);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_OFFX,buff);
			sprintf(buff,"%.3f",DMD_booster[id-1].off.y);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_OFFY,buff);
			sprintf(buff,"%.3f",DMD_booster[id-1].off.z);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_OFFZ,buff);

			sprintf(buff,"%.3f",DMD_booster[id-1].height);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_HEIGHT,buff);
			sprintf(buff,"%.3f",DMD_booster[id-1].diameter);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_DIAM,buff);
			sprintf(buff,"%.3f",DMD_booster[id-1].thrust);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_THRUST,buff);
			
			double resultingISP=((DMD_booster[id-1].thrust*DMD_booster[id-1].N)*DMD_booster[id-1].burntime/(DMD_booster[id-1].fuelmass*DMD_booster[id-1].N))/G;
			sprintf(buff,"%.3f",resultingISP);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_RESISP,buff);

			sprintf(buff,"%.1f",DMD_booster[id-1].emptymass);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_EMPTYMASS,buff);
			sprintf(buff,"%.1f",DMD_booster[id-1].fuelmass);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_FUELMASS,buff);
			sprintf(buff,"%.1f",DMD_booster[id-1].burntime);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_BURNTIME,buff);
			sprintf(buff,"%.1f",DMD_booster[id-1].burndelay);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_BURNDELAY,buff);

			sprintf(buff,"%.3f",DMD_booster[id-1].eng_diameter);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENGDIAM,buff);
			if(DMD_booster[id-1].eng_tex[0]=='0'){
				SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENGTEX,emptybuff.c_str());}else{
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENGTEX,DMD_booster[id-1].eng_tex.data());}
			if(DMD_booster[id-1].eng_pstream1[0]=='0'){
				SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENGPS1,emptybuff.c_str());}else{
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENGPS1,DMD_booster[id-1].eng_pstream1.data());}
			if(DMD_booster[id-1].eng_pstream2[0]=='0'){
				SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENGPS2,emptybuff.c_str());}else{
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENGPS2,DMD_booster[id-1].eng_pstream2.data());}

			sprintf(buff,"%.3f",DMD_booster[id-1].eng_dir.x);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENGDIRX,buff);
			sprintf(buff,"%.3f",DMD_booster[id-1].eng_dir.y);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENGDIRY,buff);
			sprintf(buff,"%.3f",DMD_booster[id-1].eng_dir.z);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENGDIRZ,buff);

			SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENGN,CB_SETCURSEL,DMD_booster[id-1].nEngines-1,0);
			switch(DMD_booster[id-1].nEngines){
			case 1:
				{
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG1X,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG1Y,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG1Z,EM_SETREADONLY,FALSE,0);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[0].x);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG1X,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[0].y);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG1Y,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[0].z);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG1Z,buff);

					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG2X,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG2Y,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG2Z,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG3X,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG3Y,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG3Z,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG4X,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG4Y,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG4Z,EM_SETREADONLY,TRUE,0);
					break;
				}
			case 2:
				{
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG1X,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG1Y,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG1Z,EM_SETREADONLY,FALSE,0);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[0].x);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG1X,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[0].y);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG1Y,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[0].z);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG1Z,buff);

					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG2X,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG2Y,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG2Z,EM_SETREADONLY,FALSE,0);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[1].x);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG2X,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[1].y);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG2Y,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[1].z);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG2Z,buff);
					
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG3X,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG3Y,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG3Z,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG4X,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG4Y,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG4Z,EM_SETREADONLY,TRUE,0);
					break;
				}
			case 3:
				{
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG1X,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG1Y,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG1Z,EM_SETREADONLY,FALSE,0);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[0].x);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG1X,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[0].y);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG1Y,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[0].z);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG1Z,buff);

					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG2X,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG2Y,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG2Z,EM_SETREADONLY,FALSE,0);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[1].x);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG2X,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[1].y);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG2Y,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[1].z);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG2Z,buff);
					
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG3X,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG3Y,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG3Z,EM_SETREADONLY,FALSE,0);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[2].x);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG3X,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[2].y);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG3Y,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[2].z);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG3Z,buff);
					

					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG4X,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG4Y,EM_SETREADONLY,TRUE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG4Z,EM_SETREADONLY,TRUE,0);
					break;
				}
			case 4:
				{
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG1X,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG1Y,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG1Z,EM_SETREADONLY,FALSE,0);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[0].x);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG1X,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[0].y);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG1Y,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[0].z);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG1Z,buff);

					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG2X,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG2Y,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG2Z,EM_SETREADONLY,FALSE,0);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[1].x);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG2X,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[1].y);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG2Y,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[1].z);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG2Z,buff);
					
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG3X,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG3Y,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG3Z,EM_SETREADONLY,FALSE,0);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[2].x);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG3X,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[2].y);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG3Y,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[2].z);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG3Z,buff);
					
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG4X,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG4Y,EM_SETREADONLY,FALSE,0);
					SendDlgItemMessage(hChild[iddialog],IDC_BOOSTER_ENG4Z,EM_SETREADONLY,FALSE,0);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[3].x);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG4X,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[3].y);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG4Y,buff);
					sprintf(buff,"%.3f",DMD_booster[id-1].eng[3].z);
					SetDlgItemText(hChild[iddialog],IDC_BOOSTER_ENG4Z,buff);
					
					break;
				}

			}

			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_MODULE,DMD_booster[id-1].module.data());
			sprintf(buff,"%.3f",DMD_booster[id-1].speed.x);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_SPDX,buff);
			sprintf(buff,"%.3f",DMD_booster[id-1].speed.y);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_SPDY,buff);
			sprintf(buff,"%.3f",DMD_booster[id-1].speed.z);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_SPDZ,buff);

			sprintf(buff,"%.3f",DMD_booster[id-1].rot_speed.x);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_RSPDX,buff);
			sprintf(buff,"%.3f",DMD_booster[id-1].rot_speed.y);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_RSPDY,buff);
			sprintf(buff,"%.3f",DMD_booster[id-1].rot_speed.z);
			SetDlgItemText(hChild[iddialog],IDC_BOOSTER_RSPDZ,buff);


			if(DMD_booster[id-1].expbolt.wExpbolt)
			{
				SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_WEXPB,BM_SETCHECK,BST_CHECKED,0);
				SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBPS,DMD_booster[id-1].expbolt.pstream.data());
				sprintf(buff,"%.3f",DMD_booster[id-1].expbolt.anticipation);
				SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBANT,buff);
				sprintf(buff,"%.3f",DMD_booster[id-1].expbolt.pos.x);
				SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBX,buff);
				sprintf(buff,"%.3f",DMD_booster[id-1].expbolt.pos.y);
				SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBY,buff);
				sprintf(buff,"%.3f",DMD_booster[id-1].expbolt.pos.z);
				SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBZ,buff);
				SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBPS,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBANT,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBX,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBY,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBZ,EM_SETREADONLY,FALSE,0);

			}else{
				SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_WEXPB,BM_SETCHECK,BST_UNCHECKED,0);
				SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBPS,emptybuff.c_str());
				SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBANT,emptybuff.c_str());
				SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBX,emptybuff.c_str());
				SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBY,emptybuff.c_str());
				SetDlgItemText(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBZ,emptybuff.c_str());
				SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBPS,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBANT,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBX,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBY,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[CD_BOOSTER],IDC_BOOSTER_EXPBZ,EM_SETREADONLY,TRUE,0);
			}

			UINT npts=BoosterCurveNpts(DMD_booster[id-1]);
			SendDlgItemMessage(hChild[CD_CURVE],IDC_CURVE_N,CB_SETCURSEL,npts-1,0);

			BoosterCurvePointActive(npts);

			switch(npts){
			case 0:
				{

					break;
				}
			case 1:
				{
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX1,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY1,buff);
					

					break;
				}
			case 2:
				{
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX1,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY1,buff);
					
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX2,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY2,buff);
					
					break;
				}
			case 3:
				{
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX1,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY1,buff);
				
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX2,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY2,buff);
					
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX3,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY3,buff);
					
					break;
				}
			case 4:
				{
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX1,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY1,buff);
					
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX2,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY2,buff);
					
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX3,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY3,buff);
				
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX4,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY4,buff);
					
					break;
				}
			case 5:
				{
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX1,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY1,buff);
					
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX2,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY2,buff);
			
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX3,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY3,buff);
					
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX4,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY4,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[4].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX5,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[4].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY5,buff);

					break;
				}
			case 6:
				{
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX1,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY1,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX2,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY2,buff);
					
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX3,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY3,buff);
	
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX4,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY4,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[4].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX5,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[4].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY5,buff);
	
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[5].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX6,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[5].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY6,buff);

	
					break;
				}
			case 7:
				{
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX1,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY1,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX2,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY2,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX3,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY3,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX4,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY4,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[4].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX5,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[4].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY5,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[5].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX6,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[5].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY6,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[6].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX7,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[6].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY7,buff);

					break;
				}
			case 8:
				{
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX1,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY1,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX2,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY2,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX3,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY3,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX4,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY4,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[4].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX5,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[4].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY5,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[5].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX6,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[5].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY6,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[6].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX7,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[6].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY7,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[7].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX8,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[7].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY8,buff);

					break;
				}
			case 9:
				{
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX1,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY1,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX2,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY2,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX3,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY3,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX4,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY4,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[4].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX5,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[4].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY5,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[5].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX6,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[5].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY6,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[6].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX7,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[6].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY7,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[7].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX8,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[7].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY8,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[8].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX9,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[8].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY9,buff);

					break;
				}
			case 10:
				{
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX1,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[0].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY1,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX2,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[1].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY2,buff);
					
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX3,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[2].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY3,buff);
	
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX4,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[3].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY4,buff);
		
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[4].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX5,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[4].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY5,buff);
					
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[5].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX6,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[5].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY6,buff);
					
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[6].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX7,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[6].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY7,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[7].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX8,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[7].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY8,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[8].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX9,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[8].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY9,buff);

					sprintf(buff,"%.1f",DMD_booster[id-1].curve[9].x);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEX10,buff);
					sprintf(buff,"%.1f",DMD_booster[id-1].curve[9].y);
					SetDlgItemText(hChild[CD_CURVE],IDC_CURVEY10,buff);

					break;
				}
			}

			

BoosterDrawGraph(npts);





}
void DevModeDlg::BoosterCurveReadPoints()
{
		Curve->ClearPoints();
	//POINT point[10];
	char buff[128];
	double trans=0;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEX1,buff,128);
	sscanf(buff,"%lf",&trans);
	point[0].x=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEY1,buff,128);
	sscanf(buff,"%lf",&trans);
	point[0].y=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEX2,buff,128);
	sscanf(buff,"%lf",&trans);
	point[1].x=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEY2,buff,128);
	sscanf(buff,"%lf",&trans);
	point[1].y=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEX3,buff,128);
	sscanf(buff,"%lf",&trans);
	point[2].x=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEY3,buff,128);
	sscanf(buff,"%lf",&trans);
	point[2].y=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEX4,buff,128);
	sscanf(buff,"%lf",&trans);
	point[3].x=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEY4,buff,128);
	sscanf(buff,"%lf",&trans);
	point[3].y=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEX5,buff,128);
	sscanf(buff,"%lf",&trans);
	point[4].x=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEY5,buff,128);
	sscanf(buff,"%lf",&trans);
	point[4].y=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEX6,buff,128);
	sscanf(buff,"%lf",&trans);
	point[5].x=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEY6,buff,128);
	sscanf(buff,"%lf",&trans);
	point[5].y=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEX7,buff,128);
	sscanf(buff,"%lf",&trans);
	point[6].x=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEY7,buff,128);
	sscanf(buff,"%lf",&trans);
	point[6].y=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEX8,buff,128);
	sscanf(buff,"%lf",&trans);
	point[7].x=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEY8,buff,128);
	sscanf(buff,"%lf",&trans);
	point[7].y=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEX9,buff,128);
	sscanf(buff,"%lf",&trans);
	point[8].x=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEY9,buff,128);
	sscanf(buff,"%lf",&trans);
	point[8].y=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEX10,buff,128);
	sscanf(buff,"%lf",&trans);
	point[9].x=(int)trans;
	GetDlgItemText(hChild[CD_CURVE],IDC_CURVEY10,buff,128);
	sscanf(buff,"%lf",&trans);
	point[9].y=(int)trans;

}
void DevModeDlg::BoosterDrawGraph(UINT npts)
{
	BoosterCurveReadPoints();

for(UINT i=0;i<npts;i++)
{
	Curve->AppendDataXY(point[i]);
	//oapiWriteLogV("x:%i y:%i",point[i].x,point[i].y);
}

}

void DevModeDlg::PopulateStages(UINT iddialog, UINT id)
{
	
	char buff[128],buffempty[128];
	for(UINT i=0;i<128;i++)
	{
		buffempty[i]='\0';
	}
	sprintf(buff,"%i",id-1);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ID,buff);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_MESHNAME,DMD_stage[id-1].meshname.data());
	sprintf(buff,"%.3f",DMD_stage[id-1].off.x);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_OFFX,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].off.y);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_OFFY,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].off.z);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_OFFZ,buff);

	sprintf(buff,"%.3f",DMD_stage[id-1].height);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_HEIGHT,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].diameter);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_DIAM,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].thrust);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_THRUST,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].emptymass);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_EMPTYMASS,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].fuelmass);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_FUELMASS,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].burntime);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_BURNTIME,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].ignite_delay);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_IGNITEDELAY,buff);
	if(DMD_stage[id-1].batteries.wBatts)
	{
		SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_BATTERYCHECK,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_BATTERY,EM_SETREADONLY,FALSE,0);
		sprintf(buff,"%.1f",DMD_stage[id-1].batteries.MaxCharge/3600);
	}else{
		SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_BATTERYCHECK,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_BATTERY,EM_SETREADONLY,TRUE,0);
		strcpy(buff,buffempty);
	}
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_BATTERY,buff);
	if(DMD_stage[id-1].reignitable)
	{
		SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_REIGNITABLE,BM_SETCHECK,BST_UNCHECKED,0);
	}else{
		SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_REIGNITABLE,BM_SETCHECK,BST_CHECKED,0);
	}

	double resultingISP=((DMD_stage[id-1].thrust)*DMD_stage[id-1].burntime/(DMD_stage[id-1].fuelmass))/G;
			sprintf(buff,"%.3f",resultingISP);
			SetDlgItemText(hChild[iddialog],IDC_STAGE_RESISP,buff);

	if(DMD_stage[id-1].defpitch)
	{
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_PITCHTHRUST,EM_SETREADONLY,TRUE,0);
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_PTDEF,BM_SETCHECK,BST_CHECKED,0);
		SetDlgItemText(hChild[iddialog],IDC_STAGE_PITCHTHRUST,buffempty);
	}else{
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_PITCHTHRUST,EM_SETREADONLY,FALSE,0);
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_PTDEF,BM_SETCHECK,BST_UNCHECKED,0);
		sprintf(buff,"%.3f",DMD_stage[id-1].pitchthrust/2);
		SetDlgItemText(hChild[iddialog],IDC_STAGE_PITCHTHRUST,buff);
	}
	
	
	if(DMD_stage[id-1].defroll)
	{
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_ROLLTHRUST,EM_SETREADONLY,TRUE,0);
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_RTDEF,BM_SETCHECK,BST_CHECKED,0);
		SetDlgItemText(hChild[iddialog],IDC_STAGE_ROLLTHRUST,buffempty);
	}else{
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_ROLLTHRUST,EM_SETREADONLY,FALSE,0);
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_RTDEF,BM_SETCHECK,BST_UNCHECKED,0);
		sprintf(buff,"%.3f",DMD_stage[id-1].rollthrust/2);
		SetDlgItemText(hChild[iddialog],IDC_STAGE_ROLLTHRUST,buff);
	}
	
	
	if(DMD_stage[id-1].defyaw)
	{
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_YAWTHRUST,EM_SETREADONLY,TRUE,0);
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_YTDEF,BM_SETCHECK,BST_CHECKED,0);
		SetDlgItemText(hChild[iddialog],IDC_STAGE_YAWTHRUST,buffempty);
	}else{
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_YAWTHRUST,EM_SETREADONLY,FALSE,0);
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_YTDEF,BM_SETCHECK,BST_UNCHECKED,0);
		sprintf(buff,"%.3f",DMD_stage[id-1].yawthrust/2);
		SetDlgItemText(hChild[iddialog],IDC_STAGE_YAWTHRUST,buff);
	}

	if(DMD_stage[id-1].linearthrust>0)
	{
		sprintf(buff,"%.3f",DMD_stage[id-1].linearthrust);
		SetDlgItemText(hChild[iddialog],IDC_STAGE_LINEARTHRUST,buff);
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_LINEARTHRUST,EM_SETREADONLY,FALSE,0);
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_LTDEF,BM_SETCHECK,BST_UNCHECKED,0);
	}else{
		SetDlgItemText(hChild[iddialog],IDC_STAGE_LINEARTHRUST,TEXT("0"));
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_LINEARTHRUST,EM_SETREADONLY,TRUE,0);
		SendDlgItemMessage(hChild[iddialog],IDC_STAGE_LTDEF,BM_SETCHECK,BST_CHECKED,0);
	}
	
	sprintf(buff,"%.3f",DMD_stage[id-1].eng_diameter);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGDIAM,buff);
	if(DMD_stage[id-1].eng_tex[0]!='0'){
		SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGTEX,DMD_stage[id-1].eng_tex.data());
	}else{SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGTEX,buffempty);}
	if(DMD_stage[id-1].eng_pstream1[0]!='0'){
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGPS1,DMD_stage[id-1].eng_pstream1.data());
	}else{SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGPS1,buffempty);}
	if(DMD_stage[id-1].eng_pstream2[0]!='0'){
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGPS2,DMD_stage[id-1].eng_pstream2.data());
	}else{SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGPS2,buffempty);}
	if(DMD_stage[id-1].ParticlesPacked)
	{
		SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_PARTPACKCHECK,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_PARTPACKTOENG,EM_SETREADONLY,FALSE,0);
		sprintf(buff,"%i",DMD_stage[id-1].ParticlesPackedToEngine);
		SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_PARTPACKTOENG,buff);
	}else{
		SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_PARTPACKCHECK,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_PARTPACKTOENG,EM_SETREADONLY,TRUE,0);
		SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_PARTPACKTOENG,buffempty);
	}

	sprintf(buff,"%.3f",DMD_stage[id-1].eng_dir.x);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGDIRX,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].eng_dir.y);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGDIRY,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].eng_dir.z);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_ENGDIRZ,buff);
	
	SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_ENGN,CB_SETCURSEL,DMD_stage[id-1].nEngines,0);

	StageActiveEngines(DMD_stage[id-1].nEngines);

	std::string engx[5],engy[5],engz[5],engt[5];
	for(UINT i=0;i<DMD_stage[id-1].nEngines;i++)
	{
		sprintf(buff,"%.3f",DMD_stage[id-1].engV4[i].x);
		engx[i]=std::string(buff);
		sprintf(buff,"%.3f",DMD_stage[id-1].engV4[i].y);
		engy[i]=std::string(buff);
		sprintf(buff,"%.3f",DMD_stage[id-1].engV4[i].z);
		engz[i]=std::string(buff);
		sprintf(buff,"%.3f",DMD_stage[id-1].engV4[i].t);
		engt[i]=std::string(buff);
	}
	for(UINT j=DMD_stage[id-1].nEngines;j<5;j++)
	{
		engx[j].clear();
		engy[j].clear();
		engz[j].clear();
		engt[j].clear();
	}
	int SetTextx[5]={IDC_STAGE_ENG1X,IDC_STAGE_ENG2X,IDC_STAGE_ENG3X,IDC_STAGE_ENG4X,IDC_STAGE_ENG5X};
	int SetTexty[5]={IDC_STAGE_ENG1Y,IDC_STAGE_ENG2Y,IDC_STAGE_ENG3Y,IDC_STAGE_ENG4Y,IDC_STAGE_ENG5Y};
	int SetTextz[5]={IDC_STAGE_ENG1Z,IDC_STAGE_ENG2Z,IDC_STAGE_ENG3Z,IDC_STAGE_ENG4Z,IDC_STAGE_ENG5Z};
	int SetTextt[5]={IDC_STAGE_ENG1SCALE,IDC_STAGE_ENG2SCALE,IDC_STAGE_ENG3SCALE,IDC_STAGE_ENG4SCALE,IDC_STAGE_ENG5SCALE};
	SetMultipleDlgItemsText(hChild[CD_STAGE],SetTextx,engx,5);
	SetMultipleDlgItemsText(hChild[CD_STAGE],SetTexty,engy,5);
	SetMultipleDlgItemsText(hChild[CD_STAGE],SetTextz,engz,5);
	SetMultipleDlgItemsText(hChild[CD_STAGE],SetTextt,engt,5);
	
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_MODULE,DMD_stage[id-1].module.data());
	sprintf(buff,"%.3f",DMD_stage[id-1].speed.x);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_SPDX,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].speed.y);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_SPDY,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].speed.z);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_SPDZ,buff);

	sprintf(buff,"%.3f",DMD_stage[id-1].rot_speed.x);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_RSPDX,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].rot_speed.y);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_RSPDY,buff);
	sprintf(buff,"%.3f",DMD_stage[id-1].rot_speed.z);
	SetDlgItemText(hChild[CD_STAGE],IDC_STAGE_RSPDZ,buff);

	
			if(DMD_stage[id-1].expbolt.wExpbolt)
			{
				SendDlgItemMessage(hChild[CD_ULLAGE],IDC_STAGE_WEXPB,BM_SETCHECK,BST_CHECKED,0);
				SetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_EXPBPS,DMD_stage[id-1].expbolt.pstream.data());
				sprintf(buff,"%.3f",DMD_stage[id-1].expbolt.anticipation);
				SetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_EXPBANT,buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].expbolt.pos.x);
				SetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_EXPBX,buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].expbolt.pos.y);
				SetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_EXPBY,buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].expbolt.pos.z);
				SetDlgItemText(hChild[CD_ULLAGE],IDC_STAGE_EXPBZ,buff);
				
				int Read[5]={IDC_STAGE_EXPBPS,IDC_STAGE_EXPBANT,IDC_STAGE_EXPBX,IDC_STAGE_EXPBY,IDC_STAGE_EXPBZ};
				SendMultipleDlgItemsMessage(hChild[CD_ULLAGE],Read,EM_SETREADONLY,FALSE,0,5);
			}else{
				SendDlgItemMessage(hChild[CD_ULLAGE],IDC_STAGE_WEXPB,BM_SETCHECK,BST_UNCHECKED,0);
				int Read[5]={IDC_STAGE_EXPBPS,IDC_STAGE_EXPBANT,IDC_STAGE_EXPBX,IDC_STAGE_EXPBY,IDC_STAGE_EXPBZ};
				std::string empty[5];
				for(UINT i=0;i<5;i++){empty[i].clear();}
				SetMultipleDlgItemsText(hChild[CD_ULLAGE],Read,empty,5);
				SendMultipleDlgItemsMessage(hChild[CD_ULLAGE],Read,EM_SETREADONLY,TRUE,0,5);
			}
			if(!DMD_stage[id-1].ullage.wUllage)
			{
				SendDlgItemMessage(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEC,BM_SETCHECK,BST_UNCHECKED,0);
				int ReadOnly[14]={
					IDC_STAGE_ULLAGETHRUST, IDC_STAGE_ULLAGEANT, IDC_STAGE_ULLAGEOVERLAP, IDC_STAGE_ULLAGEN,
					IDC_STAGE_ULLAGEANGLE, IDC_STAGE_ULLAGEDIAM, IDC_STAGE_ULLAGELENGTH, 
					IDC_STAGE_ULLAGEDIRX, IDC_STAGE_ULLAGEDIRY, IDC_STAGE_ULLAGEDIRZ,
					IDC_STAGE_ULLAGEPOSX, IDC_STAGE_ULLAGEPOSY, IDC_STAGE_ULLAGEPOSZ,
					IDC_STAGE_ULLAGERECTFACTOR
				};
				SendMultipleDlgItemsMessage(hChild[CD_ULLAGE],ReadOnly,EM_SETREADONLY,TRUE,0,14);
				std::string empty[14];
				for(UINT i=0;i<14;i++)
				{
					empty[i].clear();
				}
				SetMultipleDlgItemsText(hChild[CD_ULLAGE],ReadOnly,empty,14);
			}else{

				SendDlgItemMessage(hChild[CD_ULLAGE],IDC_STAGE_ULLAGEC,BM_SETCHECK,BST_CHECKED,0);
				int ReadOnly[14]={
					IDC_STAGE_ULLAGETHRUST, IDC_STAGE_ULLAGEANT, IDC_STAGE_ULLAGEOVERLAP, IDC_STAGE_ULLAGEN,
					IDC_STAGE_ULLAGEANGLE, IDC_STAGE_ULLAGEDIAM, IDC_STAGE_ULLAGELENGTH, 
					IDC_STAGE_ULLAGEDIRX, IDC_STAGE_ULLAGEDIRY, IDC_STAGE_ULLAGEDIRZ,
					IDC_STAGE_ULLAGEPOSX, IDC_STAGE_ULLAGEPOSY, IDC_STAGE_ULLAGEPOSZ,
					IDC_STAGE_ULLAGERECTFACTOR
				};
				SendMultipleDlgItemsMessage(hChild[CD_ULLAGE],ReadOnly,EM_SETREADONLY,FALSE,0,14);
				std::string dlgtext[14];
				sprintf(buff,"%.1f",DMD_stage[id-1].ullage.thrust);
				dlgtext[0]=std::string(buff);
				sprintf(buff,"%.1f",DMD_stage[id-1].ullage.anticipation);
				dlgtext[1]=std::string(buff);
				sprintf(buff,"%.1f",DMD_stage[id-1].ullage.overlap);
				dlgtext[2]=std::string(buff);
				sprintf(buff,"%i",DMD_stage[id-1].ullage.N);
				dlgtext[3]=std::string(buff);
				sprintf(buff,"%.1f",DMD_stage[id-1].ullage.angle);
				dlgtext[4]=std::string(buff);
				sprintf(buff,"%.1f",DMD_stage[id-1].ullage.diameter);
				dlgtext[5]=std::string(buff);
				sprintf(buff,"%.1f",DMD_stage[id-1].ullage.length);
				dlgtext[6]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].ullage.dir.x);
				dlgtext[7]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].ullage.dir.y);
				dlgtext[8]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].ullage.dir.z);
				dlgtext[9]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].ullage.pos.x);
				dlgtext[10]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].ullage.pos.y);
				dlgtext[11]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].ullage.pos.z);
				dlgtext[12]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].ullage.rectfactor);
				dlgtext[13]=std::string(buff);
				SetMultipleDlgItemsText(hChild[CD_ULLAGE],ReadOnly,dlgtext,14);
			}

			int InterRead[15] = {
					IDC_INTERSTAGE_MESHNAME,IDC_INTERSTAGE_OFFX, IDC_INTERSTAGE_OFFY,IDC_INTERSTAGE_OFFZ,
					IDC_INTERSTAGE_HEIGHT, IDC_INTERSTAGE_DIAM , IDC_INTERSTAGE_EMPTYMASS , 
					IDC_INTERSTAGE_SEPARATIONDELAY, 
					IDC_INTERSTAGE_MODULE , IDC_INTERSTAGE_SPDX, IDC_INTERSTAGE_SPDY, IDC_INTERSTAGE_SPDZ,
					IDC_INTERSTAGE_RSPDX , IDC_INTERSTAGE_RSPDY , IDC_INTERSTAGE_RSPDZ
				};
			std::string intertext[15];
			if(DMD_stage[id-1].wInter)
			{
				SendDlgItemMessage(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_WINTER,BM_SETCHECK,BST_CHECKED,0);
				SendMultipleDlgItemsMessage(hChild[CD_INTERSTAGE],InterRead,EM_SETREADONLY,FALSE,0,15);
				intertext[0]=std::string(DMD_stage[id-1].interstage.meshname);
				sprintf(buff,"%.3f",DMD_stage[id-1].interstage.off.x);
				intertext[1]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].interstage.off.y);
				intertext[2]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].interstage.off.z);
				intertext[3]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].interstage.height);
				intertext[4]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].interstage.diameter);
				intertext[5]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].interstage.emptymass);
				intertext[6]=std::string(buff);
				sprintf(buff,"%.1f",DMD_stage[id-1].interstage.separation_delay);
				intertext[7]=std::string(buff);
				intertext[8]=std::string(DMD_stage[id-1].interstage.module);
				sprintf(buff,"%.3f",DMD_stage[id-1].interstage.speed.x);
				intertext[9]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].interstage.speed.y);
				intertext[10]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].interstage.speed.z);
				intertext[11]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].interstage.rot_speed.x);
				intertext[12]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].interstage.rot_speed.y);
				intertext[13]=std::string(buff);
				sprintf(buff,"%.3f",DMD_stage[id-1].interstage.rot_speed.z);
				intertext[14]=std::string(buff);
				
				SetMultipleDlgItemsText(hChild[CD_INTERSTAGE],InterRead,intertext,15);


			}else{
				SendDlgItemMessage(hChild[CD_INTERSTAGE],IDC_INTERSTAGE_WINTER,BM_SETCHECK,BST_UNCHECKED,0);
				SendMultipleDlgItemsMessage(hChild[CD_INTERSTAGE],InterRead,EM_SETREADONLY,TRUE,0,15);
				for(UINT i=0;i<15;i++)
				{
					intertext[i].clear();
				}
				SetMultipleDlgItemsText(hChild[CD_INTERSTAGE],InterRead,intertext,15);

			}

}
void DevModeDlg::SetMultipleDlgItemsText(HWND hDlg, int *idc, std::string *str, UINT nInputs)
{
	for(UINT i=0;i<nInputs;i++)
	{
		SetDlgItemText(hDlg,idc[i],str[i].c_str());
	}
}
void DevModeDlg::SendMultipleDlgItemsMessage(HWND hDlg,int *idc, UINT Msg, WPARAM wParam, LPARAM lParam, UINT nInputs)
{
	for(UINT i=0;i<nInputs;i++)
	{
		SendDlgItemMessage(hDlg,idc[i],Msg,wParam,lParam);
	}
	
}
void DevModeDlg::StageActiveEngines(UINT nEngines)
{
	switch(nEngines)
	{
	case 0:
		{
			break;
		}
	case 1:
		{
			int NotRead[4]={IDC_STAGE_ENG1X,IDC_STAGE_ENG1Y,IDC_STAGE_ENG1Z,IDC_STAGE_ENG1SCALE};
			SendMultipleDlgItemsMessage(hChild[CD_STAGE],NotRead,EM_SETREADONLY,FALSE,0,4);
			int Read[16]={
				IDC_STAGE_ENG2X,IDC_STAGE_ENG2Y,IDC_STAGE_ENG2Z,IDC_STAGE_ENG2SCALE,
				IDC_STAGE_ENG3X,IDC_STAGE_ENG3Y,IDC_STAGE_ENG3Z,IDC_STAGE_ENG3SCALE,
				IDC_STAGE_ENG4X,IDC_STAGE_ENG4Y,IDC_STAGE_ENG4Z,IDC_STAGE_ENG4SCALE,
				IDC_STAGE_ENG5X,IDC_STAGE_ENG5Y,IDC_STAGE_ENG5Z,IDC_STAGE_ENG5SCALE
			};
			SendMultipleDlgItemsMessage(hChild[CD_STAGE],Read,EM_SETREADONLY,TRUE,0,16);
		//	SendDlgItemMessage(hChild[CD_STAGE],IDC_STAGE_ENG1X|IDC_STAGE_ENG2X,EM_SETREADONLY,FALSE,0);
			break;
		}
	case 2:
		{
			int NotRead[8]={
				IDC_STAGE_ENG1X,IDC_STAGE_ENG1Y,IDC_STAGE_ENG1Z,IDC_STAGE_ENG1SCALE,
				IDC_STAGE_ENG2X,IDC_STAGE_ENG2Y,IDC_STAGE_ENG2Z,IDC_STAGE_ENG2SCALE
			};
			SendMultipleDlgItemsMessage(hChild[CD_STAGE],NotRead,EM_SETREADONLY,FALSE,0,8);
			int Read[12]={
				IDC_STAGE_ENG3X,IDC_STAGE_ENG3Y,IDC_STAGE_ENG3Z,IDC_STAGE_ENG3SCALE,
				IDC_STAGE_ENG4X,IDC_STAGE_ENG4Y,IDC_STAGE_ENG4Z,IDC_STAGE_ENG4SCALE,
				IDC_STAGE_ENG5X,IDC_STAGE_ENG5Y,IDC_STAGE_ENG5Z,IDC_STAGE_ENG5SCALE
			};
			SendMultipleDlgItemsMessage(hChild[CD_STAGE],Read,EM_SETREADONLY,TRUE,0,12);
			break;
		}
	case 3:
		{
			int NotRead[12]={
				IDC_STAGE_ENG1X,IDC_STAGE_ENG1Y,IDC_STAGE_ENG1Z,IDC_STAGE_ENG1SCALE,
				IDC_STAGE_ENG2X,IDC_STAGE_ENG2Y,IDC_STAGE_ENG2Z,IDC_STAGE_ENG2SCALE,
				IDC_STAGE_ENG3X,IDC_STAGE_ENG3Y,IDC_STAGE_ENG3Z,IDC_STAGE_ENG3SCALE
			};
			SendMultipleDlgItemsMessage(hChild[CD_STAGE],NotRead,EM_SETREADONLY,FALSE,0,12);
			int Read[8]={
				IDC_STAGE_ENG4X,IDC_STAGE_ENG4Y,IDC_STAGE_ENG4Z,IDC_STAGE_ENG4SCALE,
				IDC_STAGE_ENG5X,IDC_STAGE_ENG5Y,IDC_STAGE_ENG5Z,IDC_STAGE_ENG5SCALE
			};
			SendMultipleDlgItemsMessage(hChild[CD_STAGE],Read,EM_SETREADONLY,TRUE,0,8);
			break;
		}
	case 4:
		{
				int NotRead[16]={
				IDC_STAGE_ENG1X,IDC_STAGE_ENG1Y,IDC_STAGE_ENG1Z,IDC_STAGE_ENG1SCALE,
				IDC_STAGE_ENG2X,IDC_STAGE_ENG2Y,IDC_STAGE_ENG2Z,IDC_STAGE_ENG2SCALE,
				IDC_STAGE_ENG3X,IDC_STAGE_ENG3Y,IDC_STAGE_ENG3Z,IDC_STAGE_ENG3SCALE,
				IDC_STAGE_ENG4X,IDC_STAGE_ENG4Y,IDC_STAGE_ENG4Z,IDC_STAGE_ENG4SCALE
			};
			SendMultipleDlgItemsMessage(hChild[CD_STAGE],NotRead,EM_SETREADONLY,FALSE,0,16);
			int Read[4]={
				IDC_STAGE_ENG5X,IDC_STAGE_ENG5Y,IDC_STAGE_ENG5Z,IDC_STAGE_ENG5SCALE
			};
			SendMultipleDlgItemsMessage(hChild[CD_STAGE],Read,EM_SETREADONLY,TRUE,0,4);
		
			break;
		}
	case 5:
		{
				int NotRead[20]={
				IDC_STAGE_ENG1X,IDC_STAGE_ENG1Y,IDC_STAGE_ENG1Z,IDC_STAGE_ENG1SCALE,
				IDC_STAGE_ENG2X,IDC_STAGE_ENG2Y,IDC_STAGE_ENG2Z,IDC_STAGE_ENG2SCALE,
				IDC_STAGE_ENG3X,IDC_STAGE_ENG3Y,IDC_STAGE_ENG3Z,IDC_STAGE_ENG3SCALE,
				IDC_STAGE_ENG4X,IDC_STAGE_ENG4Y,IDC_STAGE_ENG4Z,IDC_STAGE_ENG4SCALE,
				IDC_STAGE_ENG5X,IDC_STAGE_ENG5Y,IDC_STAGE_ENG5Z,IDC_STAGE_ENG5SCALE
			};
			SendMultipleDlgItemsMessage(hChild[CD_STAGE],NotRead,EM_SETREADONLY,FALSE,0,20);
		
			break;
		}


	}
}
void DevModeDlg::PopulateChildDialog(UINT iddialog, UINT id)
{
	
	char buff[256];
	switch(iddialog){
	case CD_PLD:
		{
	ShowWindow(GetDlgItem(hChild[CD_PLD],IDC_PLD_UPDMSH),SW_HIDE);
	sprintf(buff,"%i",id-1);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_ID),(LPCSTR)buff);

	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_MESHNAME),(LPCSTR)DMD_payload[id-1].meshname.data());
	sprintf(buff,"%.3f",DMD_payload[id-1].off[0].x);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_OFFX),(LPCSTR)buff);
	sprintf(buff,"%.3f",DMD_payload[id-1].off[0].y);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_OFFY),(LPCSTR)buff);
	sprintf(buff,"%.3f",DMD_payload[id-1].off[0].z);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_OFFZ),(LPCSTR)buff);
	
	//sprintf(DMD_payload[id-1].MultiOffset,"(%.3f,%.3f,%.3f)",DMD_payload[id-1].off[0].x,DMD_payload[id-1].off[0].y,DMD_payload[id-1].off[0].z);
	
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_MODULE),(LPCSTR)DMD_payload[id-1].module.data());
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_NAME),(LPCSTR)DMD_payload[id-1].name.data());

	sprintf(buff,"%.3f",DMD_payload[id-1].height);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_HEIGHT),(LPCSTR)buff);
	sprintf(buff,"%.3f",DMD_payload[id-1].mass);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_MASS),(LPCSTR)buff);
	sprintf(buff,"%.3f",DMD_payload[id-1].diameter);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_DIAM),(LPCSTR)buff);

	sprintf(buff,"%.3f",DMD_payload[id-1].Rotation.x);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_ROTX),(LPCSTR)buff);
	sprintf(buff,"%.3f",DMD_payload[id-1].Rotation.y);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_ROTY),(LPCSTR)buff);
	sprintf(buff,"%.3f",DMD_payload[id-1].Rotation.z);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_ROTZ),(LPCSTR)buff);


	sprintf(buff,"%.3f",DMD_payload[id-1].speed.x);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_SPDX),(LPCSTR)buff);
	sprintf(buff,"%.3f",DMD_payload[id-1].speed.y);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_SPDY),(LPCSTR)buff);
	sprintf(buff,"%.3f",DMD_payload[id-1].speed.z);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_SPDZ),(LPCSTR)buff);

	sprintf(buff,"%.3f",DMD_payload[id-1].rot_speed.x);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_RSPDX),(LPCSTR)buff);
	sprintf(buff,"%.3f",DMD_payload[id-1].rot_speed.y);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_RSPDY),(LPCSTR)buff);
	sprintf(buff,"%.3f",DMD_payload[id-1].rot_speed.z);
	SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_RSPDZ),(LPCSTR)buff);
	if(DMD_payload[id-1].nMeshes>1)
	{
		SendDlgItemMessage(hChild[iddialog],IDC_PLD_MULTIMSH,BM_SETCHECK,BST_CHECKED,0);
		ShowWindow(GetDlgItem(hChild[iddialog],IDC_PLD_OFFX),SW_HIDE);
		ShowWindow(GetDlgItem(hChild[iddialog],IDC_PLD_OFFY),SW_HIDE);
		ShowWindow(GetDlgItem(hChild[iddialog],IDC_PLD_OFFZ),SW_HIDE);
		ShowWindow(GetDlgItem(hChild[iddialog],IDC_PLD_MESHNAME),SW_HIDE);
		ShowWindow(GetDlgItem(hChild[iddialog],IDC_PLD_MULTIMSHNAME),SW_SHOW);
		std::string meshname(DMD_payload[id-1].meshname);
		char quotes[1]={'\"'};
	//	meshname.insert(0,quotes);
	//	meshname.insert(meshname.npos-10,quotes);
		meshname.erase(std::remove(meshname.begin(),meshname.end(),'\"'),meshname.end());
		SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_MULTIMSHNAME),(LPCSTR)meshname.c_str());
		ShowWindow(GetDlgItem(hChild[iddialog],IDC_PLD_MULTIOFF),SW_SHOW);
		char buffer[256]={"0"};
		char tempbuff[256]={"0"};
		for(int i=0;i<DMD_payload[id-1].nMeshes;i++)
		{
			sprintf(tempbuff,"(%.3f,%.3f,%.3f)",DMD_payload[id-1].off[i].x,DMD_payload[id-1].off[i].y,DMD_payload[id-1].off[i].z);
			if(i==0){
			strcpy(buffer,tempbuff);
			}else{
			char semicol[2]=";";
			strcat(buffer,semicol);
			strcat(buffer,tempbuff);}
		}
		
		SetWindowText(GetDlgItem(hChild[iddialog],IDC_PLD_MULTIOFF),(LPCSTR)buffer);
		
	}else{
		ShowWindow(GetDlgItem(hChild[iddialog],IDC_PLD_MULTIOFF),SW_HIDE);
		ShowWindow(GetDlgItem(hChild[iddialog],IDC_PLD_MULTIMSHNAME),SW_HIDE);
		SendDlgItemMessage(hChild[iddialog],IDC_PLD_MULTIMSH,BM_SETCHECK,BST_UNCHECKED,0);
		ShowWindow(GetDlgItem(hChild[iddialog],IDC_PLD_OFFX),SW_SHOW);
		ShowWindow(GetDlgItem(hChild[iddialog],IDC_PLD_OFFY),SW_SHOW);
		ShowWindow(GetDlgItem(hChild[iddialog],IDC_PLD_OFFZ),SW_SHOW);
		ShowWindow(GetDlgItem(hChild[iddialog],IDC_PLD_MESHNAME),SW_SHOW);
	}
	if(DMD_payload[id-1].render==1){
		SendDlgItemMessage(hChild[iddialog],IDC_PLD_RENDER,BM_SETCHECK,BST_CHECKED,0);
	}else{
		SendDlgItemMessage(hChild[iddialog],IDC_PLD_RENDER,BM_SETCHECK,BST_UNCHECKED,0);
	}
	if(DMD_payload[id-1].live){
		SendDlgItemMessage(hChild[iddialog],IDC_PLD_LIVE,BM_SETCHECK,BST_CHECKED,0);
		EnableWindow(GetDlgItem(hChild[iddialog],IDC_PLD_MESHNAME),FALSE);
		EnableWindow(GetDlgItem(hChild[iddialog],IDC_PLD_MULTIMSHNAME),FALSE);
		EnableWindow(GetDlgItem(hChild[iddialog],IDC_PLD_UPDMSH),FALSE);
			}else{
		SendDlgItemMessage(hChild[iddialog],IDC_PLD_LIVE,BM_SETCHECK,BST_UNCHECKED,0);
		EnableWindow(GetDlgItem(hChild[iddialog],IDC_PLD_MESHNAME),TRUE);
		EnableWindow(GetDlgItem(hChild[iddialog],IDC_PLD_MULTIMSHNAME),TRUE);
		EnableWindow(GetDlgItem(hChild[iddialog],IDC_PLD_UPDMSH),TRUE);
			}
		break;
		}


	case CD_FAIRING:
		{
			if(DMD_wFairing==1){




				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_MESHNAME,EM_SETREADONLY,FALSE,0);
				EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_PICKMESH),TRUE);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_OFFX,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_OFFY,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_OFFZ,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_N,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_ANGLE,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_HEIGHT,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_DIAM,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_EMPTYMASS,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_MODULE,EM_SETREADONLY,FALSE,0);
				EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_PICKMODULE),TRUE);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_SPDX,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_SPDY,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_SPDZ,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_RSPDX,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_RSPDY,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_RSPDZ,EM_SETREADONLY,FALSE,0);
				EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_ADDFAIR),FALSE);
			    EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_DELFAIR),TRUE);
				EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_STOREVALS),TRUE);
				EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_HIDE),TRUE);
				EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_HIDEHALF),TRUE);

				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_WFAIR,BM_SETCHECK,BST_CHECKED,0);



			


				UINT half=Ms26->fairing.N;
				half+=1;
				half=half/2;
						
				if(hideFairing){
							for(UINT NF=1;NF<Ms26->fairing.N+1;NF++)
							{
							Ms26->SetMeshVisibilityMode(Ms26->fairing.msh_idh[NF],MESHVIS_NEVER);
							}
				
							SendDlgItemMessage(hChild[iddialog],IDC_FAIR_HIDE,BM_SETCHECK,BST_CHECKED,0);
							}
				

				if(hideHalfFairing){
							for(UINT NF=1;NF<=half;NF++)
							{
							Ms26->SetMeshVisibilityMode(Ms26->fairing.msh_idh[NF],MESHVIS_NEVER);
							}
							SendDlgItemMessage(hChild[iddialog],IDC_FAIR_HIDEHALF,BM_SETCHECK,BST_CHECKED,0);
				}
							
					

			char buff[256];
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_MESHNAME),(LPCSTR)DMD_fairing.meshname.data());
			sprintf(buff,"%.3f",DMD_fairing.off.x);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_OFFX),(LPCSTR)buff);
			sprintf(buff,"%.3f",DMD_fairing.off.y);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_OFFY),(LPCSTR)buff);
			sprintf(buff,"%.3f",DMD_fairing.off.z);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_OFFZ),(LPCSTR)buff);
			sprintf(buff,"%i",DMD_fairing.N);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_N),(LPCSTR)buff);
			sprintf(buff,"%.3f",DMD_fairing.angle);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_ANGLE),(LPCSTR)buff);
			sprintf(buff,"%.3f",DMD_fairing.height);

			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_HEIGHT),(LPCSTR)buff);
			sprintf(buff,"%.3f",DMD_fairing.diameter);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_DIAM),(LPCSTR)buff);
			sprintf(buff,"%.1f",DMD_fairing.emptymass);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_EMPTYMASS),(LPCSTR)buff);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_MODULE),(LPCSTR)DMD_fairing.module.data());
			sprintf(buff,"%.1f",DMD_fairing.speed.x);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_SPDX),(LPCSTR)buff);
			sprintf(buff,"%.1f",DMD_fairing.speed.y);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_SPDY),(LPCSTR)buff);
			sprintf(buff,"%.1f",DMD_fairing.speed.z);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_SPDZ),(LPCSTR)buff);

			sprintf(buff,"%.1f",DMD_fairing.rot_speed.x);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_RSPDX),(LPCSTR)buff);
			sprintf(buff,"%.1f",DMD_fairing.rot_speed.y);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_RSPDY),(LPCSTR)buff);
			sprintf(buff,"%.1f",DMD_fairing.rot_speed.z);
			SetWindowText(GetDlgItem(hChild[iddialog],IDC_FAIR_RSPDZ),(LPCSTR)buff);
			
			}else{
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_MESHNAME,EM_SETREADONLY,TRUE,0);
				EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_PICKMESH),FALSE);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_OFFX,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_OFFY,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_OFFZ,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_N,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_ANGLE,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_HEIGHT,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_DIAM,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_EMPTYMASS,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_MODULE,EM_SETREADONLY,TRUE,0);
				EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_PICKMODULE),FALSE);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_SPDX,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_SPDY,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_SPDZ,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_RSPDX,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_RSPDY,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_RSPDZ,EM_SETREADONLY,TRUE,0);
				EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_ADDFAIR),TRUE);
			    EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_DELFAIR),FALSE);
				//EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_STOREVALS),FALSE);
				EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_HIDE),FALSE);
				EnableWindow(GetDlgItem(hChild[iddialog],IDC_FAIR_HIDEHALF),FALSE);
				
				SendDlgItemMessage(hChild[iddialog],IDC_FAIR_WFAIR,BM_SETCHECK,BST_UNCHECKED,0);

				int FairRead[16]={ IDC_FAIR_MESHNAME , IDC_FAIR_OFFX, IDC_FAIR_OFFY, IDC_FAIR_OFFZ,
					IDC_FAIR_N, IDC_FAIR_ANGLE, IDC_FAIR_HEIGHT, IDC_FAIR_DIAM, IDC_FAIR_EMPTYMASS,
					IDC_FAIR_MODULE, IDC_FAIR_SPDX, IDC_FAIR_SPDY, IDC_FAIR_SPDZ, 
					IDC_FAIR_RSPDX, IDC_FAIR_RSPDY, IDC_FAIR_RSPDZ
				};
				std::string fairtxt[16];
				for(UINT i=0;i<16;i++)
				{
					fairtxt[i].clear();
				}
				SetMultipleDlgItemsText(hChild[iddialog],FairRead,fairtxt,16);

			}
			break;
		}

	case CD_PARTICLE:
		{
			char buff[128];
			sprintf(buff,"%i",id-1);
			SetDlgItemText(hChild[iddialog],IDC_PART_ID,buff);
			SetDlgItemText(hChild[iddialog],IDC_PART_NAME,DMD_Particle[id-1].ParticleName.c_str());
			sprintf(buff,"%.3f",DMD_Particle[id-1].Pss.srcsize);
			SetDlgItemText(hChild[iddialog],IDC_PART_SRCSIZE,buff);
			sprintf(buff,"%.3f",DMD_Particle[id-1].Pss.srcrate);
			SetDlgItemText(hChild[iddialog],IDC_PART_SRCRATE,buff);
			sprintf(buff,"%.3f",DMD_Particle[id-1].Pss.srcspread);
			SetDlgItemText(hChild[iddialog],IDC_PART_SRCSPREAD,buff);
			sprintf(buff,"%.3f",DMD_Particle[id-1].Pss.v0);
			SetDlgItemText(hChild[iddialog],IDC_PART_V0,buff);
			sprintf(buff,"%.3f",DMD_Particle[id-1].Pss.lifetime);
			SetDlgItemText(hChild[iddialog],IDC_PART_LIFETIME,buff);
			sprintf(buff,"%.3f",DMD_Particle[id-1].Pss.growthrate);
			SetDlgItemText(hChild[iddialog],IDC_PART_GROWTHRATE,buff);
			sprintf(buff,"%.3f",DMD_Particle[id-1].Pss.atmslowdown);
			SetDlgItemText(hChild[iddialog],IDC_PART_ATMSLOWDOWN,buff);
			///////////////////////////*********************************************************************
			/*
			int check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LTYPE,CB_FINDSTRING,-1,(LPARAM)TEXT("EMISSIVE"));
			if(check==CB_ERR){
				SendDlgItemMessage(hChild[iddialog],IDC_PART_LTYPE,CB_INSERTSTRING,0,(LPARAM)TEXT("EMISSIVE"));
			}
			check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LTYPE,CB_FINDSTRING,-1,(LPARAM)TEXT("DIFFUSE"));
			if(check==CB_ERR){
				SendDlgItemMessage(hChild[iddialog],IDC_PART_LTYPE,CB_INSERTSTRING,1,(LPARAM)TEXT("DIFFUSE"));
			}
			
			check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("LVL_FLAT"));
			if(check==CB_ERR){
				SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_INSERTSTRING,0,(LPARAM)TEXT("LVL_FLAT"));
			}
			check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("LVL_LIN"));
			if(check==CB_ERR){
				SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_INSERTSTRING,1,(LPARAM)TEXT("LVL_LIN"));
			}
			check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("LVL_SQRT"));
			if(check==CB_ERR){
				SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_INSERTSTRING,2,(LPARAM)TEXT("LVL_SQRT"));
			}
			check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("LVL_PLIN"));
			if(check==CB_ERR){
				SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_INSERTSTRING,3,(LPARAM)TEXT("LVL_PLIN"));
			}
			check=SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("LVL_PSQRT"));
			if(check==CB_ERR){
				SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_INSERTSTRING,4,(LPARAM)TEXT("LVL_PSQRT"));
			}

				check=SendDlgItemMessage(hChild[iddialog],IDC_PART_ATMSMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("ATM_FLAT"));
			if(check==CB_ERR){
				SendDlgItemMessage(hChild[iddialog],IDC_PART_ATMSMAP,CB_INSERTSTRING,0,(LPARAM)TEXT("ATM_FLAT"));
			}
			check=SendDlgItemMessage(hChild[iddialog],IDC_PART_ATMSMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("ATM_PLIN"));
			if(check==CB_ERR){
				SendDlgItemMessage(hChild[iddialog],IDC_PART_ATMSMAP,CB_INSERTSTRING,1,(LPARAM)TEXT("ATM_PLIN"));
			}
			check=SendDlgItemMessage(hChild[iddialog],IDC_PART_ATMSMAP,CB_FINDSTRING,-1,(LPARAM)TEXT("ATM_PLOG"));
			if(check==CB_ERR){
				SendDlgItemMessage(hChild[iddialog],IDC_PART_ATMSMAP,CB_INSERTSTRING,2,(LPARAM)TEXT("ATM_PLOG"));
			}
			*/

			/////////////////////////////////////////////////////////////////////////////************

			if(DMD_Particle[id-1].Pss.ltype==PARTICLESTREAMSPEC::EMISSIVE)
			{
				SendDlgItemMessage(hChild[iddialog],IDC_PART_LTYPE,CB_SETCURSEL,0,0);
			}else if(DMD_Particle[id-1].Pss.ltype==PARTICLESTREAMSPEC::DIFFUSE)
			{
				SendDlgItemMessage(hChild[iddialog],IDC_PART_LTYPE,CB_SETCURSEL,1,0);
			}


			switch(DMD_Particle[id-1].Pss.levelmap){
			case PARTICLESTREAMSPEC::LVL_FLAT:
				{
					SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_SETCURSEL,0,0);
					break;
				}
			case PARTICLESTREAMSPEC::LVL_LIN:
				{
					SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_SETCURSEL,1,0);
					break;
				}
			case PARTICLESTREAMSPEC::LVL_SQRT:
				{
					SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_SETCURSEL,2,0);
					break;
				}
			case PARTICLESTREAMSPEC::LVL_PLIN:
				{
					SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_SETCURSEL,3,0);
					break;
				}
			case PARTICLESTREAMSPEC::LVL_PSQRT:
				{
					SendDlgItemMessage(hChild[iddialog],IDC_PART_LEVELMAP,CB_SETCURSEL,4,0);
					break;
				}
			}

			sprintf(buff,"%g",DMD_Particle[id-1].Pss.lmin);
			SetDlgItemText(hChild[iddialog],IDC_PART_LMIN,buff);
			sprintf(buff,"%.3f",DMD_Particle[id-1].Pss.lmax);
			SetDlgItemText(hChild[iddialog],IDC_PART_LMAX,buff);


		

			switch(DMD_Particle[id-1].Pss.atmsmap)
			{
			case PARTICLESTREAMSPEC::ATM_FLAT:
				{
					SendDlgItemMessage(hChild[iddialog],IDC_PART_ATMSMAP,CB_SETCURSEL,0,0);
					break;
				}
			case PARTICLESTREAMSPEC::ATM_PLIN:
				{
					SendDlgItemMessage(hChild[iddialog],IDC_PART_ATMSMAP,CB_SETCURSEL,1,0);
					break;
				}
			case PARTICLESTREAMSPEC::ATM_PLOG:
				{
					SendDlgItemMessage(hChild[iddialog],IDC_PART_ATMSMAP,CB_SETCURSEL,2,0);
					break;
				}
			}

			sprintf(buff,"%g",DMD_Particle[id-1].Pss.amin);
			SetDlgItemText(hChild[iddialog],IDC_PART_AMIN,buff);
			sprintf(buff,"%.3f",DMD_Particle[id-1].Pss.amax);
			SetDlgItemText(hChild[iddialog],IDC_PART_AMAX,buff);

			SetDlgItemText(hChild[iddialog],IDC_PART_TEX,DMD_Particle[id-1].TexName.c_str());

			if(DMD_Particle[id-1].Growing)
			{
				SendDlgItemMessage(hChild[iddialog],IDC_PART_ENABLEGROWING,BM_SETCHECK,BST_CHECKED,0);
				SendDlgItemMessage(hChild[iddialog],IDC_PART_GFS,EM_SETREADONLY,FALSE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_PART_GFR,EM_SETREADONLY,FALSE,0);
				sprintf(buff,"%.3f",DMD_Particle[id-1].GrowFactor_size);
				SetDlgItemText(hChild[iddialog],IDC_PART_GFS,buff);
				sprintf(buff,"%.3f",DMD_Particle[id-1].GrowFactor_rate);
				SetDlgItemText(hChild[iddialog],IDC_PART_GFR,buff);

			}else{
				std::string emptybuff;
				emptybuff.clear();
				SendDlgItemMessage(hChild[iddialog],IDC_PART_ENABLEGROWING,BM_SETCHECK,BST_UNCHECKED,0);
				SendDlgItemMessage(hChild[iddialog],IDC_PART_GFS,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[iddialog],IDC_PART_GFR,EM_SETREADONLY,TRUE,0);
				SetDlgItemText(hChild[iddialog],IDC_PART_GFS,emptybuff.c_str());
				SetDlgItemText(hChild[iddialog],IDC_PART_GFR,emptybuff.c_str());
			}
			ShowWindow(GetDlgItem(hChild[CD_PARTICLE],IDC_PART_SLIDERLEVEL),SW_HIDE);
			break;
		}
		case CD_MISC:
		{
			char buff[128];
			sprintf(buff,"%.3f",DMD_Misc.COG);
			SetDlgItemText(hChild[iddialog],IDC_MISC_COG,buff);
			sprintf(buff,"%.3f",DMD_Misc.VerticalAngle*DEG);
			SetDlgItemText(hChild[iddialog],IDC_MISC_VERTICALANGLE,buff);

			if(DMD_Misc.GNC_Debug!=0)
			{
					SendDlgItemMessage(hChild[iddialog],IDC_MISC_GNCDEBUG,BM_SETCHECK,BST_CHECKED,0);
			}else{
					SendDlgItemMessage(hChild[iddialog],IDC_MISC_GNCDEBUG,BM_SETCHECK,BST_UNCHECKED,0);
			}
			
			if(!DMD_Misc.telemetry)
			{
					SendDlgItemMessage(hChild[iddialog],IDC_MISC_TELEMETRY,BM_SETCHECK,BST_UNCHECKED,0);
			}else{
					SendDlgItemMessage(hChild[iddialog],IDC_MISC_TELEMETRY,BM_SETCHECK,BST_CHECKED,0);
			}
			
			if(!DMD_Misc.thrustrealpos)
			{
					SendDlgItemMessage(hChild[iddialog],IDC_MISC_THRUSTREALPOS,BM_SETCHECK,BST_UNCHECKED,0);
			}else{
					SendDlgItemMessage(hChild[iddialog],IDC_MISC_THRUSTREALPOS,BM_SETCHECK,BST_CHECKED,0);
			}

			for (UINT i=1;i<=DMD_nPayloads;i++)
			{
				//sprintf(buff,"Payload %i",i);
				//int check=SendDlgItemMessage(hChild[iddialog],IDC_MISC_FOCUS,CB_FINDSTRING,-1,(LPARAM)buff);
				int check=SendDlgItemMessage(hChild[iddialog],IDC_MISC_FOCUS,CB_FINDSTRING,-1,(LPARAM)DMD_payload[i-1].name.data());
				
				if(check==CB_ERR)
				{
					SendDlgItemMessage(hChild[iddialog],IDC_MISC_FOCUS,CB_INSERTSTRING,i-1,(LPARAM)DMD_payload[i-1].name.data());
				}
			}
			if(DMD_Misc.Focus!=0)
			{
				SendDlgItemMessage(hChild[iddialog],IDC_MISC_FOCUS,CB_SETCURSEL,(int)DMD_Misc.Focus-1,0);
			}else{
					SendDlgItemMessage(hChild[iddialog],IDC_MISC_FOCUS,CB_SETCURSEL,-1,0);
			}
			if(DMD_Misc.PadModule == "EmptyModule")
			{
				SendDlgItemMessage(hChild[iddialog],IDC_MISC_WPADMODULE,BM_SETCHECK,BST_UNCHECKED,0);
				SendDlgItemMessage(hChild[iddialog],IDC_MISC_PADMODULE,EM_SETREADONLY,TRUE,0);
			}else{
				SendDlgItemMessage(hChild[iddialog],IDC_MISC_WPADMODULE,BM_SETCHECK,BST_CHECKED,0);
				SetDlgItemText(hChild[iddialog],IDC_MISC_PADMODULE,(LPCSTR)DMD_Misc.PadModule.c_str());
				SendDlgItemMessage(hChild[iddialog],IDC_MISC_PADMODULE,EM_SETREADONLY,FALSE,0);
			}
		

			break;
		}
		case CD_TEX:
		{
			for(UINT i=0;i<DMD_nTextures;i++)
			{
				SetDlgItemText(hChild[iddialog],IDC_TEX_TEX1+i,DMD_tex.TextureName.at(i).c_str());
			}
			break;
		}
		case CD_BOOSTER:
		{
			PopulateBoosters(iddialog,id);
			break;
		}
		case CD_STAGE:
		{
			PopulateStages(iddialog,id);
			break;
		}
		case CD_LES:
		{
			int LesRead[14]={ 
			IDC_LES_MESHNAME , IDC_LES_OFFX, IDC_LES_OFFY , IDC_LES_OFFZ , 
			IDC_LES_HEIGHT , IDC_LES_DIAM , IDC_LES_EMPTYMASS , IDC_LES_MODULE ,
			IDC_LES_SPDX , IDC_LES_SPDY , IDC_LES_SPDZ ,
			IDC_LES_RSPDX , IDC_LES_RSPDY , IDC_LES_RSPDZ
			};
			std::string LesTxt[14];
			if(!DMD_wLes)
			{
				SendDlgItemMessage(hChild[CD_LES],IDC_LES_WLES,BM_SETCHECK,BST_UNCHECKED,0);
				SendMultipleDlgItemsMessage(hChild[CD_LES],LesRead,EM_SETREADONLY,TRUE,0,14);
				for(UINT i=0;i<14;i++)
				{
					LesTxt[i].clear();
				}
				SetMultipleDlgItemsText(hChild[CD_LES],LesRead,LesTxt,14);
				EnableWindow(GetDlgItem(hChild[CD_LES],IDC_LES_PICKMESH),FALSE);
				EnableWindow(GetDlgItem(hChild[CD_LES],IDC_LES_PICKMODULE),FALSE);
				EnableWindow(GetDlgItem(hChild[CD_LES],IDC_LES_STOREVALS),FALSE);
				
			}else{
				SendDlgItemMessage(hChild[CD_LES],IDC_LES_WLES,BM_SETCHECK,BST_CHECKED,0);
				SendMultipleDlgItemsMessage(hChild[CD_LES],LesRead,EM_SETREADONLY,FALSE,0,14);
				LesTxt[0]=std::string(DMD_Les.meshname);
				char buff[128];
				sprintf(buff,"%.3f",DMD_Les.off.x);
				LesTxt[1]=std::string(buff);
				sprintf(buff,"%.3f",DMD_Les.off.y);
				LesTxt[2]=std::string(buff);
				sprintf(buff,"%.3f",DMD_Les.off.z);
				LesTxt[3]=std::string(buff);
				sprintf(buff,"%.1f",DMD_Les.height);
				LesTxt[4]=std::string(buff);
				sprintf(buff,"%.1f",DMD_Les.diameter);
				LesTxt[5]=std::string(buff);
				sprintf(buff,"%.1f",DMD_Les.emptymass);
				LesTxt[6]=std::string(buff);
				LesTxt[7]=std::string(DMD_Les.module);
				sprintf(buff,"%.3f",DMD_Les.speed.x);
				LesTxt[8]=std::string(buff);
				sprintf(buff,"%.3f",DMD_Les.speed.y);
				LesTxt[9]=std::string(buff);
				sprintf(buff,"%.3f",DMD_Les.speed.z);
				LesTxt[10]=std::string(buff);
				sprintf(buff,"%.3f",DMD_Les.rot_speed.x);
				LesTxt[11]=std::string(buff);
				sprintf(buff,"%.3f",DMD_Les.rot_speed.y);
				LesTxt[12]=std::string(buff);
				sprintf(buff,"%.3f",DMD_Les.rot_speed.z);
				LesTxt[13]=std::string(buff);
			
				SetMultipleDlgItemsText(hChild[CD_LES],LesRead,LesTxt,14);
				EnableWindow(GetDlgItem(hChild[CD_LES],IDC_LES_PICKMESH),TRUE);
				EnableWindow(GetDlgItem(hChild[CD_LES],IDC_LES_PICKMODULE),TRUE);
				EnableWindow(GetDlgItem(hChild[CD_LES],IDC_LES_STOREVALS),TRUE);
				
			}
			break;
		}
		case CD_ADAPTER:
		{
			
			int AdaptRead[7]={
				IDC_ADAPTER_MESHNAME, IDC_ADAPTER_OFFX, IDC_ADAPTER_OFFY, IDC_ADAPTER_OFFZ,
				IDC_ADAPTER_HEIGHT, IDC_ADAPTER_DIAM, IDC_ADAPTER_EMPTYMASS
			};
			std::string AdaptTxt[7];
			if(!DMD_wAdapter)
			{
				
				SendDlgItemMessage(hChild[CD_ADAPTER],IDC_ADAPTER_WADAPTER,BM_SETCHECK,BST_UNCHECKED,0);
				for(UINT i=0;i<7;i++)
				{
					AdaptTxt[i].clear();
				}
				SendMultipleDlgItemsMessage(hChild[CD_ADAPTER],AdaptRead,EM_SETREADONLY,TRUE,0,7);
				SetMultipleDlgItemsText(hChild[CD_ADAPTER],AdaptRead,AdaptTxt,7);
				EnableWindow(GetDlgItem(hChild[CD_ADAPTER],IDC_ADAPTER_PICKMESH),FALSE);
				EnableWindow(GetDlgItem(hChild[CD_ADAPTER],IDC_ADAPTER_STOREVALS),FALSE);
			}else{
				
				SendDlgItemMessage(hChild[CD_ADAPTER],IDC_ADAPTER_WADAPTER,BM_SETCHECK,BST_CHECKED,0);
				SendMultipleDlgItemsMessage(hChild[CD_ADAPTER],AdaptRead,EM_SETREADONLY,FALSE,0,7);
				char buff[128];
				AdaptTxt[0]=std::string(DMD_Adapter.meshname);
				sprintf(buff,"%.3f",DMD_Adapter.off.x);
				AdaptTxt[1]=std::string(buff);
				sprintf(buff,"%.3f",DMD_Adapter.off.y);
				AdaptTxt[2]=std::string(buff);
				sprintf(buff,"%.3f",DMD_Adapter.off.z);
				AdaptTxt[3]=std::string(buff);
				sprintf(buff,"%.3f",DMD_Adapter.height);
				AdaptTxt[4]=std::string(buff);
				sprintf(buff,"%.3f",DMD_Adapter.diameter);
				AdaptTxt[5]=std::string(buff);
				sprintf(buff,"%.3f",DMD_Adapter.emptymass);
				AdaptTxt[6]=std::string(buff);

				SetMultipleDlgItemsText(hChild[CD_ADAPTER],AdaptRead,AdaptTxt,7);

				EnableWindow(GetDlgItem(hChild[CD_ADAPTER],IDC_ADAPTER_PICKMESH),TRUE);
				EnableWindow(GetDlgItem(hChild[CD_ADAPTER],IDC_ADAPTER_STOREVALS),TRUE);
				
			}

			


			break;
		}
		case CD_FX:
		{
			int VentVecs[35]={
				IDC_FXVENT_OFF1X,IDC_FXVENT_OFF1Y,IDC_FXVENT_OFF1Z,IDC_FXVENT_TF1,IDC_FXVENT_DIR1X,IDC_FXVENT_DIR1Y,IDC_FXVENT_DIR1Z,
				IDC_FXVENT_OFF2X,IDC_FXVENT_OFF2Y,IDC_FXVENT_OFF2Z,IDC_FXVENT_TF2,IDC_FXVENT_DIR2X,IDC_FXVENT_DIR2Y,IDC_FXVENT_DIR2Z,
				IDC_FXVENT_OFF3X,IDC_FXVENT_OFF3Y,IDC_FXVENT_OFF3Z,IDC_FXVENT_TF3,IDC_FXVENT_DIR3X,IDC_FXVENT_DIR3Y,IDC_FXVENT_DIR3Z,
				IDC_FXVENT_OFF4X,IDC_FXVENT_OFF4Y,IDC_FXVENT_OFF4Z,IDC_FXVENT_TF4,IDC_FXVENT_DIR4X,IDC_FXVENT_DIR4Y,IDC_FXVENT_DIR4Z,
				IDC_FXVENT_OFF5X,IDC_FXVENT_OFF5Y,IDC_FXVENT_OFF5Z,IDC_FXVENT_TF5,IDC_FXVENT_DIR5X,IDC_FXVENT_DIR5Y,IDC_FXVENT_DIR5Z,

			};
			std::string VentTxt[35];
			char buff[128];

			for(UINT i=0;i<35;i++)
			{
				VentTxt[i].clear();
			}

			if(DMD_wVent)
			{
				SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_WVENT,BM_SETCHECK,BST_CHECKED,0);
				EnableWindow(GetDlgItem(hChild[CD_FX],IDC_FXVENT_CB),TRUE);
				SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_CB,CB_SETCURSEL,DMD_FX_Vent.nVent,0);
				SendMultipleDlgItemsMessage(hChild[CD_FX],VentVecs,EM_SETREADONLY,TRUE,0,70);
				SendMultipleDlgItemsMessage(hChild[CD_FX],VentVecs,EM_SETREADONLY,FALSE,0,7*DMD_FX_Vent.nVent);
				SetMultipleDlgItemsText(hChild[CD_FX],VentVecs,VentTxt,70);
				SetDlgItemText(hChild[CD_FX],IDC_FXVENT_PSTREAM,DMD_FX_Vent.pstream.data());
				//string transstring[7];
				UINT counter=0;
				for(UINT i=0;i<DMD_FX_Vent.nVent;i++)
				{
					sprintf(buff,"%.3f",DMD_FX_Vent.off[i+1].x);
					VentTxt[7*i+counter]=std::string(buff);
					counter++;
					sprintf(buff,"%.3f",DMD_FX_Vent.off[i+1].y);
					VentTxt[7*i+counter]=std::string(buff);
					counter++;
					sprintf(buff,"%.3f",DMD_FX_Vent.off[i+1].z);
					VentTxt[7*i+counter]=std::string(buff);
					counter++;
					sprintf(buff,"%.1f",DMD_FX_Vent.time_fin[i+1]);
					VentTxt[7*i+counter]=std::string(buff);
					counter++;
					sprintf(buff,"%.3f",DMD_FX_Vent.dir[i+1].x);
					VentTxt[7*i+counter]=std::string(buff);
					counter++;
					sprintf(buff,"%.3f",DMD_FX_Vent.dir[i+1].y);
					VentTxt[7*i+counter]=std::string(buff);
					counter++;
					sprintf(buff,"%.3f",DMD_FX_Vent.dir[i+1].z);
					VentTxt[7*i+counter]=std::string(buff);
					counter=0;
				}
				SetMultipleDlgItemsText(hChild[CD_FX],VentVecs,VentTxt,7*DMD_FX_Vent.nVent);
			}else{
				SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_WVENT,BM_SETCHECK,BST_UNCHECKED,0);
				SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_CB,CB_SETCURSEL,0,0);
				EnableWindow(GetDlgItem(hChild[CD_FX],IDC_FXVENT_CB),FALSE);
				SendMultipleDlgItemsMessage(hChild[CD_FX],VentVecs,EM_SETREADONLY,TRUE,0,35);
				SetMultipleDlgItemsText(hChild[CD_FX],VentVecs,VentTxt,35);
				SendDlgItemMessage(hChild[CD_FX],IDC_FXVENT_PSTREAM,EM_SETREADONLY,TRUE,0);

			}


			int MachVecs[15]={
				IDC_FXMACH_OFF1X,IDC_FXMACH_OFF1Y,IDC_FXMACH_OFF1Z,
				IDC_FXMACH_OFF2X,IDC_FXMACH_OFF2Y,IDC_FXMACH_OFF2Z,
				IDC_FXMACH_OFF3X,IDC_FXMACH_OFF3Y,IDC_FXMACH_OFF3Z,
				IDC_FXMACH_OFF4X,IDC_FXMACH_OFF4Y,IDC_FXMACH_OFF4Z,
				IDC_FXMACH_OFF5X,IDC_FXMACH_OFF5Y,IDC_FXMACH_OFF5Z,
			};
			std::string MachTxt[15];
			for(UINT k=0;k<15;k++)
			{
				MachTxt[k].clear();
			}
			if(DMD_wMach)
			{
				SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_WMACH,BM_SETCHECK,BST_CHECKED,0);
				EnableWindow(GetDlgItem(hChild[CD_FX],IDC_FXMACH_CB),TRUE);
				SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_CB,CB_SETCURSEL,DMD_FX_Mach.nmach,0);
				SendMultipleDlgItemsMessage(hChild[CD_FX],MachVecs,EM_SETREADONLY,TRUE,0,15);
				SetMultipleDlgItemsText(hChild[CD_FX],MachVecs,MachTxt,15);
				SetDlgItemText(hChild[CD_FX],IDC_FXMACH_PSTREAM,DMD_FX_Mach.pstream.data());
				sprintf(buff,"%.1f",DMD_FX_Mach.mach_max);
				SetDlgItemText(hChild[CD_FX],IDC_FXMACH_MAX,buff);
				sprintf(buff,"%.1f",DMD_FX_Mach.mach_min);
				SetDlgItemText(hChild[CD_FX],IDC_FXMACH_MIN,buff);
				sprintf(buff,"%.3f",DMD_FX_Mach.dir.x);
				SetDlgItemText(hChild[CD_FX],IDC_FXMACH_DIRX,buff);
				sprintf(buff,"%.3f",DMD_FX_Mach.dir.y);
				SetDlgItemText(hChild[CD_FX],IDC_FXMACH_DIRY,buff);
				sprintf(buff,"%.3f",DMD_FX_Mach.dir.z);
				SetDlgItemText(hChild[CD_FX],IDC_FXMACH_DIRZ,buff);
				SendMultipleDlgItemsMessage(hChild[CD_FX],MachVecs,EM_SETREADONLY,FALSE,0,3*DMD_FX_Mach.nmach);
				UINT counter=0;
				for(UINT q=0;q<DMD_FX_Mach.nmach;q++)
				{
					sprintf(buff,"%.3f",DMD_FX_Mach.off[q].x);
					MachTxt[q*3+counter]=std::string(buff);
					counter++;
					sprintf(buff,"%.3f",DMD_FX_Mach.off[q].y);
					MachTxt[q*3+counter]=std::string(buff);
					counter++;
					sprintf(buff,"%.3f",DMD_FX_Mach.off[q].z);
					MachTxt[q*3+counter]=std::string(buff);
					counter=0;
				}
				SetMultipleDlgItemsText(hChild[CD_FX],MachVecs,MachTxt,3*DMD_FX_Mach.nmach);

			}else{
				SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_WMACH,BM_SETCHECK,BST_UNCHECKED,0);
				SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_CB,CB_SETCURSEL,0,0);
				EnableWindow(GetDlgItem(hChild[CD_FX],IDC_FXMACH_CB),FALSE);
				SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_PSTREAM,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_MAX,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_MIN,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_DIRX,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_DIRY,EM_SETREADONLY,TRUE,0);
				SendDlgItemMessage(hChild[CD_FX],IDC_FXMACH_DIRZ,EM_SETREADONLY,TRUE,0);
				SendMultipleDlgItemsMessage(hChild[CD_FX],MachVecs,EM_SETREADONLY,TRUE,0,15);
				SetMultipleDlgItemsText(hChild[CD_FX],MachVecs,MachTxt,15);
			}

			int LaunchEdits[7]={
				IDC_FXLAUNCH_N,IDC_FXLAUNCH_HEIGHT,IDC_FXLAUNCH_DISTANCE,IDC_FXLAUNCH_ANGLE,IDC_FXLAUNCH_CUTOFF,IDC_FXLAUNCH_PS1,IDC_FXLAUNCH_PS2
			};
			std::string LaunchTxt[7];
			for(UINT k=0;k<7;k++)
			{
				LaunchTxt[k].clear();
			}
			if(DMD_wFXLaunch)
			{
				SendDlgItemMessage(hChild[CD_FX],IDC_FXLAUNCH_WLAUNCH,BM_SETCHECK,BST_CHECKED,0);
				SendMultipleDlgItemsMessage(hChild[CD_FX],LaunchEdits,EM_SETREADONLY,FALSE,0,7);
				sprintf(buff,"%i",DMD_FX_Launch.N);
				SetDlgItemText(hChild[CD_FX],IDC_FXLAUNCH_N,buff);
				sprintf(buff,"%.1f",DMD_FX_Launch.H);
				SetDlgItemText(hChild[CD_FX],IDC_FXLAUNCH_HEIGHT,buff);
				sprintf(buff,"%.1f",DMD_FX_Launch.Distance);
				SetDlgItemText(hChild[CD_FX],IDC_FXLAUNCH_DISTANCE,buff);
				sprintf(buff,"%.1f",DMD_FX_Launch.Angle);
				SetDlgItemText(hChild[CD_FX],IDC_FXLAUNCH_ANGLE,buff);
				sprintf(buff,"%.1f",DMD_FX_Launch.CutoffAltitude);
				SetDlgItemText(hChild[CD_FX],IDC_FXLAUNCH_CUTOFF,buff);
				SetDlgItemText(hChild[CD_FX],IDC_FXLAUNCH_PS1,DMD_FX_Launch.Ps1.c_str());
				SetDlgItemText(hChild[CD_FX],IDC_FXLAUNCH_PS2,DMD_FX_Launch.Ps2.c_str());
			}else{
				SendDlgItemMessage(hChild[CD_FX],IDC_FXLAUNCH_WLAUNCH,BM_SETCHECK,BST_UNCHECKED,0);
				SendMultipleDlgItemsMessage(hChild[CD_FX],LaunchEdits,EM_SETREADONLY,TRUE,0,7);
				SetMultipleDlgItemsText(hChild[CD_FX],LaunchEdits,LaunchTxt,7);

			}


			/*if((!DMD_wVent)&&(!DMD_wMach))
			{
				EnableWindow(GetDlgItem(hChild[CD_FX],IDC_FX_STOREVALS),FALSE);
			}*/
			break;
		}
	}
}
void DevModeDlg::SaveItems(ItemType IT)
{
	switch(IT){
	case Payload:
		{
			for(UINT i=0;i<DMD_nPayloads;i++)
			{
				char buffer[256]={"0"};
				char tempbuff[256]={"0"};
				for(UINT j=0;j<DMD_payload[i].nMeshes;j++)
				{
					sprintf(tempbuff,"(%.3f,%.3f,%.3f)",DMD_payload[i].off[j].x,DMD_payload[i].off[j].y,DMD_payload[i].off[j].z);
				if(j==0){
					strcpy(buffer,tempbuff);
				}else{
					char semicol[2]=";";
					strcat(buffer,semicol);
					strcat(buffer,tempbuff);}
				}
				DMD_payload[i].MultiOffset = buffer;
			
				WriteIni.WritePayload(DMD_payload[i],i);
				
			}
			for(UINT j=DMD_nPayloads;j<10;j++)
			{
				WriteIni.WritePayload(DMD_payload[j],j,TRUE);
			}
			break;
		}
	case Particle:
		{
			for(UINT i=0;i<DMD_nParticles;i++)
			{
				WriteIni.WriteParticle(DMD_Particle[i],i);
			}
			for(UINT j=DMD_nParticles;j<16;j++)
			{
				WriteIni.WriteParticle(DMD_Particle[j],j,TRUE);
			}
			
			break;
		}
	case Stage:
		{
			for(UINT i=0;i<DMD_nStages;i++)
			{
				WriteIni.WriteStage(DMD_stage[i],i);
			}
			for(UINT j=DMD_nStages;j<10;j++)
			{
				WriteIni.WriteStage(DMD_stage[j],j,TRUE);
			}
			break;
		}
	case Booster:
		{
		for(UINT i=0;i<DMD_nBoosters;i++)
		{
			WriteIni.WriteBooster(DMD_booster[i],i);
		}
		for(UINT j=DMD_nBoosters;j<10;j++)
		{
			WriteIni.WriteBooster(DMD_booster[j],j,TRUE);
		}
			break;
		}
	}
	return;
}
void DevModeDlg::AddItem(ItemType IT)
{
	switch(IT){
	case Payload:
		{
			if(DMD_nPayloads<10){
			DMD_nPayloads+=1;
			DMD_payload[DMD_nPayloads-1]=PAYLOAD();
			}
			break;
		}
	case Fairing:
		{
			DMD_wFairing=1;
			DMD_fairing=FAIRING();
			break;
		}
	case Stage:
		{
			if(DMD_nStages<10)
			{
				DMD_nStages+=1;
				DMD_stage[DMD_nStages-1]=STAGE();
			}
			break;
		}
	case Particle:
		{
			if(DMD_nParticles<13){
			DMD_nParticles+=1;
			DMD_Particle[DMD_nParticles+1]=PARTICLE();
			}
			break;
		}
	case Texture:
		{
			DMD_nTextures+=1;
			//DMD_texture[DMD_nTextures-1]={'0'};
			break;
		}
	case Booster:
		{
			if(DMD_nBoosters<10)
			{
			DMD_nBoosters+=1;
			DMD_booster[DMD_nBoosters-1]=BOOSTER();
			}
			break;
		}
	case Interstage:
		{
			break;
		}
	case Empty:
		{
			return;
		}
	}
	InitDialog(hDlg,0,0,0,FALSE,IT);
	return;
}
void DevModeDlg::DelItem(ItemType IT, UINT idx)
{
	switch(IT){
	case Payload:
		{
			if(DMD_nPayloads>0)
			{
				DMD_payload[idx]=PAYLOAD();
				for(UINT i=idx;i<9;i++)
				{
					DMD_payload[i]=DMD_payload[i+1];
				}
				DMD_nPayloads-=1;
			}
			break;
		}
	case Fairing:
		{
			DMD_wFairing=0;
			break;
		}
	case Particle:
		{
			if(DMD_nParticles>0)
			{
				DMD_Particle[idx]=PARTICLE();
				for(UINT i=idx;i<15;i++)
				{
					DMD_Particle[i]=DMD_Particle[i+1];
				}
				DMD_nParticles-=1;
			}
			break;
		}
	case Booster:
		{
			if(DMD_nBoosters>0)
			{
				DMD_booster[idx]=BOOSTER();
				for(UINT i=idx;i<9;i++)
				{
					DMD_booster[i]=DMD_booster[i+1];
				}
				DMD_nBoosters-=1;
			}
			break;
		}
	case Stage:
		{
			if(DMD_nStages>0)
			{
				DMD_stage[idx]=STAGE();
				for(UINT i=idx;i<9;i++)
				{
					DMD_stage[i]=DMD_stage[i+1];
				}
				DMD_nStages-=1;
			}
			break;
		}
	}
	InitDialog(hDlg,0,0,0,FALSE,IT);
	
	return;
}
void DevModeDlg::removeSpaces(char* s)
{
    char* cpy = s;  // an alias to iterate through s without moving s
    char* temp = s;

    while (*cpy)
    {
        if (*cpy != ' ')
            *temp++ = *cpy;
        cpy++;
    }
    *temp = 0;

    
}
void DevModeDlg::Notify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
      {
        case IDC_TREE1:
			{
			
          if(((LPNMHDR)lParam)->code == TVN_SELCHANGED)
          {
			  	ShowWindow(GetDlgItem(hWnd,IDC_SAVEINI),SW_HIDE);
				ShowWindow(GetDlgItem(hWnd,IDC_ADDGENERAL),SW_HIDE);
				ShowWindow(GetDlgItem(hWnd,IDC_SAVEGENERAL),SW_HIDE);
				Viewing=Empty;
			  HTREEITEM Selected;
        //     Selected=(HTREEITEM)SendDlgItemMessage(hWnd,IDC_TREE1,TVM_GETNEXTITEM,TVGN_CARET,(LPARAM)Selected);
		//	TreeView_EnsureVisible(hWnd,Selected);
			TV_ITEM tvi;
			char text[256];
			//tvi.hItem=Selected;
			Selected=TreeView_GetSelection(GetDlgItem(hWnd,IDC_TREE1));
			tvi.hItem=Selected;
			tvi.mask=TVIF_TEXT;
			tvi.pszText=text;
			tvi.cchTextMax=256;
			TreeView_GetItem(GetDlgItem(hWnd,IDC_TREE1),&tvi);
			char item[256];
			UINT id=0;
			sscanf_s(text,"%s %i",&item,&id);
			//removeSpaces(item);
			for(int i=0;i<sizeof(item)/sizeof(char);i++)
			{
				item[i]=tolower(item[i]);
			}
			
		//	sprintf(oapiDebugString(),"item:%s id:%i",item,id);

			for(int q=0;q<15;q++)
			{
				ShowWindow(hChild[q],SW_HIDE);
			}
			UINT sizeN=sizeof(item)/sizeof(char);
			if((strncmp(item,"payload",sizeN)==0)&&(id>0))
			{
				SetWindowPos(hChild[CD_PLD],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_PLD],SW_SHOW);
				PopulateChildDialog(CD_PLD,id);
			}
			else if(strncmp(item,"payloads",sizeN)==0)
			{
				ShowWindow(GetDlgItem(hWnd,IDC_ADDGENERAL),SW_SHOW);
				SetDlgItemText(hWnd,IDC_ADDGENERAL,TEXT("ADD PAYLOAD"));
				ShowWindow(GetDlgItem(hWnd,IDC_SAVEGENERAL),SW_SHOW);
				SetDlgItemText(hWnd,IDC_SAVEGENERAL,TEXT("SAVE PAYLOADS"));
				Viewing=Payload;
			}
			else if(strncmp(item,"fairing",sizeN)==0)
			{
				SetWindowPos(hChild[CD_FAIRING],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_FAIRING],SW_SHOW);
				PopulateChildDialog(CD_FAIRING,0);
			}else if((strncmp(item,"particle",sizeN)==0)&&(id>0))
			{
				SetWindowPos(hChild[CD_PARTICLE],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_PARTICLE],SW_SHOW);
				PopulateChildDialog(CD_PARTICLE,id);
			}else if((strncmp(item,"particle",sizeN)==0)&&(id==0))
			{
				ShowWindow(GetDlgItem(hWnd,IDC_ADDGENERAL),SW_SHOW);
				SetDlgItemText(hWnd,IDC_ADDGENERAL,TEXT("ADD PARTICLE STREAM"));
				ShowWindow(GetDlgItem(hWnd,IDC_SAVEGENERAL),SW_SHOW);
				SetDlgItemText(hWnd,IDC_SAVEGENERAL,TEXT("SAVE PARTICLES"));
				Viewing=Particle;
			}else if(strncmp(item,"miscellaneous",sizeN)==0)
			{
				SetWindowPos(hChild[CD_MISC],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_MISC],SW_SHOW);
				PopulateChildDialog(CD_MISC,0);
			}else if(strncmp(item,"textures",sizeN)==0)
			{
				SetWindowPos(hChild[CD_TEX],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_TEX],SW_SHOW);
				PopulateChildDialog(CD_TEX,0);
			}else if((strncmp(item,"booster",sizeN)==0)&&(id>0))
			{
				SetWindowPos(hChild[CD_BOOSTER],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_BOOSTER],SW_SHOW);
				PopulateChildDialog(CD_BOOSTER,id);
			}else if(strncmp(item,"boosters",sizeN)==0)
			{
				ShowWindow(GetDlgItem(hWnd,IDC_ADDGENERAL),SW_SHOW);
				SetDlgItemText(hWnd,IDC_ADDGENERAL,TEXT("ADD BOOSTER"));
				ShowWindow(GetDlgItem(hWnd,IDC_SAVEGENERAL),SW_SHOW);
				SetDlgItemText(hWnd,IDC_SAVEGENERAL,TEXT("SAVE BOOSTERS"));
				Viewing=Booster;
			}else if((strncmp(item,"stage",sizeN)==0)&&(id>0))
			{
				SetWindowPos(hChild[CD_STAGE],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_STAGE],SW_SHOW);
				PopulateChildDialog(CD_STAGE,id);
			}else if(strncmp(item,"stages",sizeN)==0)
			{
				ShowWindow(GetDlgItem(hWnd,IDC_ADDGENERAL),SW_SHOW);
				SetDlgItemText(hWnd,IDC_ADDGENERAL,TEXT("ADD STAGE"));
				ShowWindow(GetDlgItem(hWnd,IDC_SAVEGENERAL),SW_SHOW);
				SetDlgItemText(hWnd,IDC_SAVEGENERAL,TEXT("SAVE STAGES"));
				Viewing=Stage;
			}else if(strncmp(item,"les",sizeN)==0)
			{
				SetWindowPos(hChild[CD_LES],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_LES],SW_SHOW);
				PopulateChildDialog(CD_LES,0);
			}else if(strncmp(item,"adapter",sizeN)==0)
			{
				SetWindowPos(hChild[CD_ADAPTER],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_ADAPTER],SW_SHOW);
				PopulateChildDialog(CD_ADAPTER,0);
			}else if(strncmp(item,"fx",sizeN)==0)
			{
				SetWindowPos(hChild[CD_FX],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_FX],SW_SHOW);
				PopulateChildDialog(CD_FX,0);
			}else if(strncmp(item,"scenario",sizeN)==0)
			{
			//	sprintf(oapiDebugString(),"SCENARIO");
				SetWindowPos(hChild[CD_SCENARIO],HWND_TOP,200,34,0,0,SWP_NOSIZE);
				ShowWindow(hChild[CD_SCENARIO],SW_SHOW);
				//PopulateChildDialog(CD_FX,0);
			}

		}
		  break;
			}
			
      }

}

 void DevModeDlg::ReverseOffsets()
 {
	double currentDelta=Ms26->currentDelta;
	
	int i;
	for(i=Ms26->currentStage;i<DMD_nStages;i++){
		DMD_stage[i].off.z+=currentDelta;
		if(DMD_stage[i].wInter==TRUE){
			DMD_stage[i].interstage.off.z+=currentDelta;
		}
	}
	int p;
	for(p=Ms26->currentPayload;p<DMD_nPayloads;p++){
		for(int s=0;s<DMD_payload[p].nMeshes;s++){
		DMD_payload[p].off[s].z+=currentDelta;
		}
	}
	
	int z;
	for(z=Ms26->currentBooster;z<DMD_nBoosters;z++){
		DMD_booster[z].off.z+=currentDelta;
	}

	if(DMD_wFairing==1){
		DMD_fairing.off.z+=currentDelta;
	}

	if(DMD_wAdapter==TRUE){
		DMD_Adapter.off.z+=currentDelta;
	}
	if(DMD_wLes==TRUE){
		DMD_Les.off.z+=currentDelta;
	}
	return;
}