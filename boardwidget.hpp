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
#include <unordered_set>
#include <vector>


struct ColorPair;
struct TilePos;
struct TileSize;


using ColorVector = std::vector<wxColour>;
using TileRow = std::vector<wxColour>;
using TileGrid = std::vector<TileRow>;
using Randomizer = std::default_random_engine;
using Coords = double[COORDS_LEN][2];
using Adjoining = std::unordered_set<TilePos>;


wxDECLARE_EVENT(SCORE_EVENT, wxCommandEvent);
wxDECLARE_EVENT(GAME_OVER_EVENT, wxCommandEvent);


class BoardWidget : public wxWindow {
public:
    static size_t colorCount();

    explicit BoardWidget(wxWindow* parent);

    void newGame();

private:
    void announceScore();
    void announceGameOver(const wxString&);
    ColorVector getColors(Randomizer&);
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
    void deleteTile(int x, int y);
    bool isLegal(int x, int y, const wxColour& color);
    void dimAdjoining(int x, int y, const wxColour& color);
    void populateAdjoining(int x, int y, const wxColour& color,
                           Adjoining& adjoining);
    void deleteAdjoining(const Adjoining& adjoining);

    void onPaint(wxPaintEvent&);
    void onChar(wxKeyEvent&);
    void onClick(wxMouseEvent&);
    void onMoveKey(int code);

    wxSize DoGetBestClientSize() const wxOVERRIDE;

    int score;
    bool gameOver;
    bool drawing;
    int columns;
    int rows;
    int maxColors;
    int delayMs;
    int selectedX;
    int selectedY;
    TileGrid tiles;
    wxTimer timer;
};
