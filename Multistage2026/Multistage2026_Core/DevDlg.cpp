#include "DevDlg.hpp"
#include "resource.h"

TabDialog::TabDialog(int id, int idtab) : TabbedDialog(IDD_DIALOG1, IDC_TAB1){

}

TabDialog::~TabDialog(){
	Close();
}

void TabDialog::Update(double simt){

}

int TabDialog::OnInitDialog(WPARAM wParam){
	return 0;
}

int TabDialog::Closed(){
	return 1;
}