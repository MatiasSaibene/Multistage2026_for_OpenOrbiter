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