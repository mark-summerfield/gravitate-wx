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
#include <vector>


using ColorNameMap = std::unordered_map<std::string, std::string>;
using ColorVector = std::vector<wxColour>;
using TileRow = std::vector<wxColour>;
using TileGrid = std::vector<TileRow>;
using Randomizer = std::default_random_engine;
using Coords = std::vector<wxPoint>;


wxDECLARE_EVENT(SCORE_EVENT, wxCommandEvent);
wxDECLARE_EVENT(GAME_OVER_EVENT, wxCommandEvent);


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
    wxSize tileSize() const;
    ColorPair getColorPair(const wxColour& color) const;
    void drawTile(wxGraphicsContext* gc, int x, int y, int width,
                  int height, int edge, int edge2);
    void drawSegments(wxGraphicsContext* gc, int edge,
                      const ColorPair& colorPair, int x1, int y1, int x2,
                      int y2);
    void drawSegment(wxGraphicsContext* gc, int edge,
                      const wxColour& color, const Coords& coords);
    void drawFocus(wxGraphicsContext* gc, int x1, int y1, int edge,
                   int width, int height);

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
    wxPoint selected;
    TileGrid tiles;
};
