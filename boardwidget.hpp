#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/graphics.h>

#include <random>
#include <unordered_map>
#include <utility>
#include <vector>


using ColorMap = std::unordered_map<std::string, std::string>;
using ColorVector = std::vector<std::string>;
using ColorPair = std::pair<wxColour, wxColour>;
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
    void draw(int delayMs=0, bool force=false);
    wxSize tileSize() const;
    void drawTile(wxGraphicsContext* gc, int x, int y, int width,
                  int height, int edge, int edge2);
//    ColorPair getColorPair(const std::string& color) const;

    void onPaint(wxPaintEvent&);
    void onChar(wxKeyEvent&);
    void onClick(wxMouseEvent&);

    int score;
    bool gameOver;
    bool drawing;
    int columns;
    int rows;
    wxPoint selected;
    TileGrid tiles;
};
