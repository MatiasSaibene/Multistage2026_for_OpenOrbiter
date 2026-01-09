#pragma once
#ifndef __DEVDLG_HPP_
#define __DEVDLG_HPP_

#include "../../samples/Common/Dialog/TabDlg.h"

class TabDialog : public TabbedDialog{

    public:

        TabDialog(int id, int idtab);
        ~TabDialog();

        void Update(double simt);

        int OnInitDialog(WPARAM wParam);

        int Closed();
};

#endif //!__DEVDLG_HPP_