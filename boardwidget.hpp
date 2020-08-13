#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <unordered_map>


wxDECLARE_EVENT(SCORE_EVENT, wxCommandEvent);
wxDECLARE_EVENT(GAME_OVER_EVENT, wxCommandEvent);

using ColorMap = std::unordered_map<std::string, std::string>;


class BoardWidget : public wxWindow {
public:
    static const ColorMap& colorMap();

    explicit BoardWidget(wxWindow* parent);

    void newGame();

private:
    void announceScore();
    void announceGameOver(const wxString&);

    int score;
    bool gameOver;
    bool drawing;
    int columns;
    int rows;
    int maxColors;
    int delayMs;
};
