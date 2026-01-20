#include "Multistage2026.hpp"
#include "DevModeCtrl.hpp"

DevModeDlg::DevModeDlg(Multistage2026 *_Ms26){

    Ms26 = _Ms26;

    hDlg = nullptr;

    TestingParticle = false;

    sliderlvl = 1;

    hideFairing = false;

    hideHalfFairing = false;

    Graph::InitGDI();

    Curve = new Graph();

}

DevModeDlg::~DevModeDlg(){
    Ms26 = nullptr;
    hDlg=nullptr;

    delete Curve;
    Graph::FreeGDI();
}


void DevModeDlg::Open(HINSTANCE hInstance){
	
	hInst = hInstance;
	hDlg = oapiOpenDialogEx(hInst, IDD_DIALOG1,DlgProcHook, 0, this);	
}

int DevModeDlg::Closed()
{
	hDlg=NULL;
	Ms15->DestroyDMD();
	Ms15=0;
	
	return TRUE;
}
