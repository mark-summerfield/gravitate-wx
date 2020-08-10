// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "mainwindow.hpp"

#include "images/about32.xpm"
#include "images/gravitate32.xpm"
#include "images/help32.xpm"
#include "images/new32.xpm"
#include "images/options32.xpm"
#include "images/quit32.xpm"


MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "Gravitate", wxDefaultPosition,
              wxDefaultSize, wxDEFAULT_FRAME_STYLE |
                             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
    SetMinSize(wxSize(240, 300));
    SetTitle(wxTheApp->GetAppName());
    SetIcon(gravitate32_xpm);
    makeWidgets();

    CreateStatusBar();
    SetStatusText("Click a tile to start playing...");
    //Bind(wxEVT_MENU, &MainWindow::onAbout, this, wxID_ABOUT);
    //Bind(wxEVT_MENU, &MainWindow::onExit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::onClose, this);
}


void MainWindow::makeWidgets() {
    panel = new wxPanel(this);
    auto toolbar = CreateToolBar();
    auto newTool = toolbar->AddTool(
        wxID_NEW, "New", new32_xpm, "New game");
    auto optionsTool = toolbar->AddTool(
        wxID_PREFERENCES, "Options...", options32_xpm,
        "View or change options");
    auto aboutTool = toolbar->AddTool(
        wxID_ABOUT, "About", about32_xpm,
        "About " + wxTheApp->GetAppName());
    auto helpTool = toolbar->AddTool(
        wxID_HELP, "Help", help32_xpm, "How to play");
    auto quitTool = toolbar->AddTool(
        wxID_EXIT, "Quit", quit32_xpm, "Quit the game");
    // TODO
    toolbar->Realize();
    // TODO
}


void MainWindow::onExit(wxCommandEvent& WXUNUSED(event)) {
    Close(true);
}


void MainWindow::onClose(wxCloseEvent& WXUNUSED(event)) {
    using namespace std;
    cout << "onClose save settings & high score" << endl;
    Destroy();
}


void MainWindow::onAbout(wxCommandEvent& WXUNUSED(event)) {
    // TODO replace with proper about box name, version, wx version,
    // license etc.
    wxMessageBox("The body of Gravitate's About", "Gravitate — About",
                 wxOK | wxICON_INFORMATION);
}
