#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <random>
#include <unordered_map>
#include <vector>


using ColorMap = std::unordered_map<std::string, std::string>;
using ColorVector = std::vector<std::string>;
using TileRow = std::vector<std::string>;
using TileGrid = std::vector<TileRow>;
using Randomizer = std::default_random_engine;


wxDECLARE_EVENT(SCORE_EVENT, wxCommandEvent);
wxDECLARE_EVENT(GAME_OVER_EVENT, wxCommandEvent);


class BoardWidget : public wxWindow {
public:
    static const ColorMap& colorMap();

    explicit BoardWidget(wxWindow* parent);

    void newGame();

private:
    void announceScore();
    void announceGameOver(const wxString&);
    ColorVector getColors(int maxColors, Randomizer&);
    void draw();

    void onPaint(wxPaintEvent&);
    void onChar(wxKeyEvent&);
    void onClick(wxMouseEvent&);

    int score;
    bool gameOver;
    bool drawing;
    wxPoint selected;
    TileGrid tiles;
};
