#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/spinctrl.h>

#include "mainwindow.hpp"


void onOptions(MainWindow* parent);


class OptionsWindow : public wxDialog {
public:
    explicit OptionsWindow(wxWindow* parent);

private:
    void makeWidgets();
    void makeLayout();
    void onOk(wxCommandEvent&);

    wxPanel* panel;
    wxStaticText* columnsLabel;
    wxSpinCtrl* columnsSpinCtrl;
    wxStaticText* rowsLabel;
    wxSpinCtrl* rowsSpinCtrl;
    wxStaticText* maxColorsLabel;
    wxSpinCtrl* maxColorsSpinCtrl;
    wxStaticText* delayMsLabel;
    wxSpinCtrl* delayMsSpinCtrl;
    wxButton* okButton;
    wxStaticText* padLabel;
    wxButton* cancelButton;
};
