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
#include <unordered_set>
#include <vector>


const int INVALID_POS = -1;


struct ColorPair;
struct TileSize;

struct Point {
    Point(int x_=INVALID_POS, int y_=INVALID_POS) : x(x_), y(y_) {}

    int x;
    int y;

    bool isValid() const { return x != INVALID_POS && y != INVALID_POS; }
};


using ColorVector = std::vector<wxColour>;
using Coords = double[COORDS_LEN][2];
using PointMap = std::unordered_map<Point, Point>;
using PointSet = std::unordered_set<Point>;
using Randomizer = std::default_random_engine;
using Ripple = std::vector<int>;
using TileRow = std::vector<wxColour>;
using TileGrid = std::vector<TileRow>;


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
    void deleteTile(const Point point);
    bool isLegal(const Point point, const wxColour& color);
    void dimAdjoining(const Point point, const wxColour& color);
    void populateAdjoining(const Point point, const wxColour& color,
                           PointSet& adjoining);
    void deleteAdjoining(const PointSet adjoining);
    void closeTilesUp(size_t count);
    void moveTiles();
    bool moveIsPossible(const Point point, PointMap& moves);
    PointSet getEmptyNeighbours(const Point point);
    Point nearestToMiddle(const Point point, const PointSet& empties,
                          bool* move);
    bool isSquare(const Point& point);
    void checkGameOver();
    Ripple rippledRange(int limit);

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
    Point selected;
    TileGrid tiles;
    wxTimer timer;
    Randomizer randomizer;
};
