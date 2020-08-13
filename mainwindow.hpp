#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardwidget.hpp"
#include "constants.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class MainWindow : public wxFrame {
public:
    explicit MainWindow();

    void setTemporaryStatusMessage(const wxString& message,
                                   int timeoutMs=TIMEOUT);

private:
    void makeWidgets();
    void makeToolBar();
    void makeStatusBar();
    void makeLayout();
    void makeBindings();
    void setPositionAndSize();
    void showScores(int);
    void saveConfig();

    void onChar(wxKeyEvent&);
    void onClose(wxCloseEvent&);
    void onNew(wxCommandEvent&);
    void onGameOver(wxCommandEvent&);

    wxTimer startupTimer;
    wxTimer statusTimer;
    wxPanel* panel;
    BoardWidget *board;
    bool starting;
};
