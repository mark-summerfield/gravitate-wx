#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "constants.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/graphics.h>

#include <random>
#include <unordered_map>
#include <vector>


using ColorNameMap = std::unordered_map<std::string, std::string>;
using ColorVector = std::vector<wxColour>;
using TileRow = std::vector<wxColour>;
using TileGrid = std::vector<TileRow>;
using Randomizer = std::default_random_engine;
using Coords = double[COORDS_LEN][2];


wxDECLARE_EVENT(SCORE_EVENT, wxCommandEvent);
wxDECLARE_EVENT(GAME_OVER_EVENT, wxCommandEvent);


struct TileSize {
    double width;
    double height;
};


struct ColorPair {
    wxColour light;
    wxColour dark;
};


class BoardWidget : public wxWindow {
public:
    static const ColorNameMap& colorNameMap();

    explicit BoardWidget(wxWindow* parent);

    void newGame();

private:
    void announceScore();
    void announceGameOver(const wxString&);
    ColorVector getColors(int maxColors, Randomizer&);
    void draw(int delayMs=0, bool force=false);
    TileSize tileSize() const;
    ColorPair getColorPair(const wxColour& color) const;
    void drawTile(wxGraphicsContext* gc, int x, int y, double width,
                  double height, double edge, double edge2);
    void drawSegments(wxGraphicsContext* gc, double edge,
                      const ColorPair& colorPair, double x1, double y1,
                      double x2, double y2);
    void drawSegment(wxGraphicsContext* gc, const wxColour& color,
                     const Coords& coords);
    void drawFocus(wxGraphicsContext* gc, double x1, double y1, double edge,
                   double width, double height);

    void onPaint(wxPaintEvent&);
    void onChar(wxKeyEvent&);
    void onClick(wxMouseEvent&);

    wxSize DoGetBestClientSize() const;

    int score;
    bool gameOver;
    bool drawing;
    bool dimming;
    int columns;
    int rows;
    int selectedX;
    int selectedY;
    TileGrid tiles;
};
