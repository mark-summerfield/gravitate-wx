// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "actions.hpp"
#include "mainwindow.hpp"

#include "images/about32.xpm"
#include "images/gravitate32.xpm"
#include "images/help32.xpm"
#include "images/new32.xpm"
#include "images/options32.xpm"
#include "images/quit32.xpm"

#include <wx/config.h>

#include <memory>


MainWindow::MainWindow()
        : wxFrame(nullptr, wxID_ANY, wxTheApp->GetAppName(),
                  wxDefaultPosition, wxDefaultSize, FRAME_STYLE),
          starting(true) {
    SetMinSize(wxSize(240, 300));
    SetTitle(wxTheApp->GetAppName());
    SetIcon(gravitate32_xpm);
    makeWidgets();
    makeLayout();
    makeBindings();
    setPositionAndSize();
    startupTimer.Bind( // Call after MainWindow is fully constructed
        wxEVT_TIMER, [&](wxTimerEvent&) {
            wxCommandEvent event; MainWindow::onNew(event); });
    startupTimer.StartOnce(50);
}


void MainWindow::setTemporaryStatusMessage(const wxString& message,
                                           int timeoutMs) {
    statusTimer.Stop();
    SetStatusText(message);
    statusTimer.Bind(wxEVT_TIMER,
                     [&](wxTimerEvent&) { SetStatusText(""); });
    statusTimer.StartOnce(timeoutMs);
}


void MainWindow::makeWidgets() {
    panel = new wxPanel(this);
    makeToolBar();
    board = new BoardWidget(panel);
    makeStatusBar();
}


void MainWindow::makeToolBar() {
    auto toolbar = CreateToolBar();
    toolbar->AddTool(wxID_NEW, "New", new32_xpm, "New game (n)");
    toolbar->AddSeparator();
    toolbar->AddTool(wxID_PREFERENCES, "Options... (o)", options32_xpm,
                     "View or change options");
    toolbar->AddSeparator();
    toolbar->AddStretchableSpace();
    toolbar->AddSeparator();
    toolbar->AddTool(wxID_ABOUT, "About (a)", about32_xpm,
                     "About " + wxTheApp->GetAppName());
    toolbar->AddSeparator();
    toolbar->AddTool(wxID_HELP, "Help (F1)", help32_xpm, "How to play");
    toolbar->AddSeparator();
    toolbar->AddStretchableSpace();
    toolbar->AddSeparator();
    toolbar->AddTool(wxID_EXIT, "Quit (q)", quit32_xpm, "Quit the game");
    toolbar->Realize();
}


void MainWindow::makeStatusBar() {
    const int STATUS_FIELDS = 2;
    const long STYLE_FLAG = wxSTB_DEFAULT_STYLE & ~wxSTB_SIZEGRIP;
    auto statusBar = CreateStatusBar(STATUS_FIELDS, STYLE_FLAG);
    const int widths[STATUS_FIELDS] = {-3, -1};
    statusBar->SetStatusWidths(STATUS_FIELDS, widths);
    showScores(0);
    setTemporaryStatusMessage("Click a tile to start playing...");
}


void MainWindow::makeLayout() {
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(board, 1, wxALL | wxEXPAND, PAD);
    panel->SetSizerAndFit(sizer);
}


void MainWindow::makeBindings() {
    Bind(wxEVT_CHAR_HOOK, &MainWindow::onChar, this);
    Bind(wxEVT_TOOL, &MainWindow::onNew, this, wxID_NEW);
    Bind(wxEVT_TOOL, [&](wxCommandEvent&) { Actions::onOptions(this); },
         wxID_PREFERENCES);
    Bind(wxEVT_TOOL, [&](wxCommandEvent&) { Actions::onAbout(this); },
         wxID_ABOUT);
    Bind(wxEVT_TOOL, [&](wxCommandEvent&) { Actions::onHelp(this); },
         wxID_HELP);
    Bind(wxEVT_TOOL, [&](wxCommandEvent&) { Close(true); }, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::onClose, this);
    Bind(SCORE_EVENT, [&](wxCommandEvent& event) {
         showScores(event.GetInt()); });
    Bind(GAME_OVER_EVENT, &MainWindow::onGameOver, this);
}


void MainWindow::setPositionAndSize() {
    std::unique_ptr<wxConfig> config(new wxConfig(wxTheApp->GetAppName()));
    int x;
    config->Read(WINDOW_X, &x, -1);
    int y;
    config->Read(WINDOW_Y, &y, -1);
    if (x > -1 && y > -1)
        SetPosition(wxPoint(x, y));
    int width;
    config->Read(WINDOW_WIDTH, &width, 380);
    int height;
    config->Read(WINDOW_HEIGHT, &height, 440);
    SetClientSize(width, height);
}


void MainWindow::showScores(int score) {
    std::unique_ptr<wxConfig> config(new wxConfig(wxTheApp->GetAppName()));
    int highScore;
    config->Read(HIGH_SCORE, &highScore, HIGH_SCORE_DEFAULT);
    // TODO use 1,000s separator if poss.
    SetStatusText(wxString::Format("%d/%d", score, highScore), 1);
}


void MainWindow::onChar(wxKeyEvent& event) {
    if (event.GetKeyCode() == WXK_F1)
        Actions::onHelp(this);
    else
        switch (event.GetUnicodeKey()) {
            case 'A': Actions::onAbout(this); break;
            case 'H': Actions::onHelp(this); break;
            case 'N': { wxCommandEvent event; onNew(event); break; }
            case 'O': Actions::onOptions(this); break;
            case 'Q': Close(true); break;
            default: event.Skip();
        }
}


void MainWindow::onClose(wxCloseEvent&) {
    saveConfig();
    Destroy();
}


void MainWindow::saveConfig() {
    std::unique_ptr<wxConfig> config(new wxConfig(wxTheApp->GetAppName()));
    const auto pos = GetPosition();
    config->Write(WINDOW_X, pos.x);
    config->Write(WINDOW_Y, pos.y);
    const auto size = GetClientSize();
    config->Write(WINDOW_WIDTH, size.GetWidth());
    config->Write(WINDOW_HEIGHT, size.GetHeight());
}


void MainWindow::onNew(wxCommandEvent&) {
    if (starting)
        starting = false;
    else
        SetStatusText("");
    board->newGame();
    board->SetFocus();
}


void MainWindow::onGameOver(wxCommandEvent& event) {
    const int score = event.GetInt();
    std::unique_ptr<wxConfig> config(new wxConfig(wxTheApp->GetAppName()));
    int highScore;
    config->Read(HIGH_SCORE, &highScore, HIGH_SCORE_DEFAULT);
    wxString outcome;
    if (event.GetString() == WON) {
        outcome = "You won";
        if (score > highScore) {
            outcome += " with a new highscore";
            config->Write(HIGH_SCORE, score);
        }
        outcome += '!';
    }
    else
        outcome = "Game Over.";
    showScores(score);
    SetStatusText(outcome + " Click New to play again...");
}
