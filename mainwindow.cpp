// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "mainwindow.hpp"

#include "images/about32.xpm"
#include "images/gravitate32.xpm"
#include "images/help32.xpm"
#include "images/new32.xpm"
#include "images/options32.xpm"
#include "images/quit32.xpm"

#include <wx/config.h>


const long FRAME_STYLE = wxDEFAULT_FRAME_STYLE | wxDEFAULT_DIALOG_STYLE |
                         wxRESIZE_BORDER;


MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, wxTheApp->GetAppName(), wxDefaultPosition,
              wxDefaultSize, FRAME_STYLE),
      columns(COLUMNS_DEFAULT), rows(ROWS_DEFAULT),
      maxColors(MAX_COLORS_DEFAULT), delayMs(DELAY_MS_DEFAULT),
      highScore(HIGH_SCORE_DEFAULT) {
    SetMinSize(wxSize(240, 300));
    SetTitle(wxTheApp->GetAppName());
    SetIcon(gravitate32_xpm);
    makeWidgets();
    makeStatusBar();
    makeBindings();
    setPositionAndSize();
    wxCommandEvent start;
    onNew(start);
    // TODO board->SetFocus();
}


void MainWindow::makeWidgets() {
    panel = new wxPanel(this);
    auto toolbar = CreateToolBar();
    toolbar->AddTool(wxID_NEW, "New", new32_xpm, "New game (n)");
    toolbar->AddTool(wxID_PREFERENCES, "Options... (o)", options32_xpm,
                     "View or change options");
    toolbar->AddStretchableSpace();
    toolbar->AddTool(wxID_ABOUT, "About (a)", about32_xpm,
                     "About " + wxTheApp->GetAppName());
    toolbar->AddTool(wxID_HELP, "Help (F1)", help32_xpm, "How to play");
    toolbar->AddStretchableSpace();
    toolbar->AddTool(wxID_EXIT, "Quit (q)", quit32_xpm, "Quit the game");
    toolbar->Realize();
    loadConfig(); // Needed for board
    // TODO create Board
}


void MainWindow::makeStatusBar() {
    const int STATUS_FIELDS = 2;
    const long STYLE_FLAG = wxSTB_DEFAULT_STYLE & ~wxSTB_SIZEGRIP;
    auto statusBar = CreateStatusBar(STATUS_FIELDS, STYLE_FLAG);
    const int widths[STATUS_FIELDS] = {-3, -1};
    statusBar->SetStatusWidths(STATUS_FIELDS, widths);
    SetStatusText("Click a tile to start playing...");
    // TODO updateScore();
    // TODO wx.CallLater(TIMEOUT, clearStatus);
}


void MainWindow::makeLayout() {
    // TODO
}


void MainWindow::makeBindings() {
    Bind(wxEVT_TOOL, &MainWindow::onNew, this, wxID_NEW);
    Bind(wxEVT_TOOL, &MainWindow::onOptions, this, wxID_PREFERENCES);
    Bind(wxEVT_TOOL, &MainWindow::onAbout, this, wxID_ABOUT);
    Bind(wxEVT_TOOL, &MainWindow::onHelp, this, wxID_HELP);
    Bind(wxEVT_TOOL, &MainWindow::onExit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::onClose, this);
}


void MainWindow::setPositionAndSize() {
    std::cout << "setPositionAndSize" << std::endl;
}


void MainWindow::onAbout(wxCommandEvent& WXUNUSED(event)) {
    // TODO replace with proper about box name, version, wx version,
    // license etc.
    wxMessageBox("The body of Gravitate's About",
                 wxString::FromUTF8("Gravitate — About"),
                 wxOK | wxICON_INFORMATION);
}


void MainWindow::onNew(wxCommandEvent& WXUNUSED(event)) {
    std::cout << "onNew" << std::endl;
}


void MainWindow::onOptions(wxCommandEvent& WXUNUSED(event)) {
    std::cout << "onOptions" << std::endl;
}


void MainWindow::onHelp(wxCommandEvent& WXUNUSED(event)) {
    std::cout << "onHelp" << std::endl;
}


void MainWindow::onExit(wxCommandEvent& WXUNUSED(event)) {
    Close(true);
}


void MainWindow::onClose(wxCloseEvent& WXUNUSED(event)) {
    saveConfig();
    Destroy();
}


void MainWindow::saveConfig() {
    auto config = new wxConfig(wxTheApp->GetAppName());
    // config->Write(COLUMNS, ?);
    // config->Write(ROWS, ?);
    // config->Write(MAX_COLORS, ?);
    // config->Write(DELAY_MS, ?);
    // config->Write(HIGH_SCORE, ?);
    delete config;
    std::cout << "saveConfig" << std::endl;
}


void MainWindow::loadConfig() {
    auto config = new wxConfig(wxTheApp->GetAppName());
    config->Read(COLUMNS, &columns, COLUMNS_DEFAULT);
    config->Read(ROWS, &rows, ROWS_DEFAULT);
    config->Read(MAX_COLORS, &maxColors, MAX_COLORS_DEFAULT);
    config->Read(DELAY_MS, &delayMs, DELAY_MS_DEFAULT);
    config->Read(HIGH_SCORE, &highScore, HIGH_SCORE_DEFAULT);
    delete config;
}
