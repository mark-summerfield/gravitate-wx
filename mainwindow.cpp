// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "mainwindow.hpp"


MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "Gravitate", wxDefaultPosition,
              wxDefaultSize, wxDEFAULT_FRAME_STYLE |
                             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
    SetMinSize(wxSize(240, 300));
    SetTitle(wxTheApp->GetAppName());
    CreateStatusBar();
    SetStatusText("Click a tile to start playing...");
    //Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    //Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnClose, this);
}

void MainWindow::OnExit(wxCommandEvent& WXUNUSED(event)) {
    Close(true);
}

void MainWindow::OnClose(wxCloseEvent& WXUNUSED(event)) {
    using namespace std;
    cout << "OnClose save settings & high score" << endl;
    Destroy();
}

void MainWindow::OnAbout(wxCommandEvent& WXUNUSED(event)) {
    wxMessageBox("The body of Gravitate's About", "Gravitate — About",
                 wxOK | wxICON_INFORMATION);
}
