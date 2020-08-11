#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "board.hpp"
#include "constants.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class MainWindow : public wxFrame {
public:
    MainWindow();

private:
    void saveConfig();
    void makeWidgets();
    void makeStatusBar();
    void makeLayout();
    void makeBindings();
    void setPositionAndSize();

    void onNew(wxCommandEvent& event);
    void onOptions(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);
    void onHelp(wxCommandEvent& event);
    void onExit(wxCommandEvent& event);
    void onClose(wxCloseEvent& event);

    wxPanel* panel;
    Board *board;
    // TODO pointer to helpDialog
};
