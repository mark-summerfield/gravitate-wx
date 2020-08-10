#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class MainWindow : public wxFrame {
public:
    MainWindow();

private:
    void readOptions();
    void saveOptions();
    void makeWidgets();
    void makeLayout();
    void makeBindings();

    void onNew(wxCommandEvent& event);
    void onOptions(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);
    void onHelp(wxCommandEvent& event);
    void onExit(wxCommandEvent& event);
    void onClose(wxCloseEvent& event);

    wxPanel* panel;
    // TODO pointer to helpDialog
    wxToolBarToolBase *newTool;
    wxToolBarToolBase *optionsTool;
    wxToolBarToolBase *aboutTool;
    wxToolBarToolBase *helpTool;
    wxToolBarToolBase *quitTool;
};
