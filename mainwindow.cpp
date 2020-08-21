// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "actions.hpp"
#include "constants.hpp"
#include "mainwindow.hpp"
#include "util.hpp"

#include <wx/artprov.h>
#include <wx/config.h>

#include <memory>


MainWindow::MainWindow()
        : wxFrame(nullptr, wxID_ANY, wxTheApp->GetAppName(),
                  wxDefaultPosition, wxDefaultSize, FRAME_STYLE),
          starting(true) {
    SetMinSize(wxSize(240, 300));
    SetTitle(wxTheApp->GetAppName());
    SetIcon(wxArtProvider::GetIcon(ICON_ID));
    makeWidgets();
    makeLayout();
    makeBindings();
    setPositionAndSize();
    startupTimer.Bind( // Only call after MainWindow is fully constructed
        wxEVT_TIMER, [&](wxTimerEvent&) {
            wxCommandEvent event; MainWindow::onNew(event); });
    startupTimer.StartOnce(50);
}


void MainWindow::setTemporaryStatusMessage(const wxString& message,
                                           int timeoutMs) {
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
    wxSize size(FromDIP(wxSize(32, 32)));
    auto toolbar = CreateToolBar();
    toolbar->AddTool(
        wxID_NEW, "New",
        wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR, size),
        "New game (n)");
    toolbar->AddSeparator();
    toolbar->AddTool(
        wxID_PREFERENCES, "Options",
        wxArtProvider::GetBitmap(OPTIONS_ID, wxART_TOOLBAR, size),
        "View or change options (o)");
    toolbar->AddSeparator();
    toolbar->AddStretchableSpace();
    toolbar->AddSeparator();
    toolbar->AddTool(
        wxID_ABOUT, "About",
        wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_TOOLBAR, size),
        "About (a)");
    toolbar->AddSeparator();
    toolbar->AddTool(
        wxID_HELP, "Help",
        wxArtProvider::GetBitmap(wxART_HELP, wxART_TOOLBAR, size),
        "How to play (F1)");
    toolbar->AddSeparator();
    toolbar->AddStretchableSpace();
    toolbar->AddSeparator();
    toolbar->AddTool(
        wxID_EXIT, "Quit",
        wxArtProvider::GetBitmap(wxART_QUIT, wxART_TOOLBAR, size),
        "Quit the game (q)");
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
    Bind(wxEVT_TOOL, [&](wxCommandEvent&) { onOptions(this); },
         wxID_PREFERENCES);
    Bind(wxEVT_TOOL, [&](wxCommandEvent&) { onAbout(this); }, wxID_ABOUT);
    Bind(wxEVT_TOOL, [&](wxCommandEvent&) { onHelp(this); }, wxID_HELP);
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
    SetStatusText(humanize(score) + '/' + humanize(highScore), 1);
}


void MainWindow::onChar(wxKeyEvent& event) {
    if (event.GetKeyCode() == WXK_F1)
        onHelp(this);
    else
        switch (event.GetUnicodeKey()) {
            case 'A': onAbout(this); break;
            case 'H': onHelp(this); break;
            case 'N': { wxCommandEvent event; onNew(event); break; }
            case 'O': onOptions(this); break;
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
