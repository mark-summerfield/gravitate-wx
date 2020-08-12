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
    explicit MainWindow();

private:
    void makeWidgets();
    void makeToolBar();
    void makeStatusBar();
    void makeLayout();
    void makeBindings();
    void setPositionAndSize();
    void showUpdatedScore();
    void saveConfig();

    void onChar(wxKeyEvent&);
    void onClose(wxCloseEvent&);

    wxTimer startupTimer;
    wxTimer statusTimer;
    wxPanel* panel;
    Board *board;
    int score;
};
