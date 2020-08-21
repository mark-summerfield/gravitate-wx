#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "constants.hpp"
#include "boardutil.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/graphics.h>


wxDECLARE_EVENT(SCORE_EVENT, wxCommandEvent);
wxDECLARE_EVENT(GAME_OVER_EVENT, wxCommandEvent);


class BoardWidget : public wxWindow {
public:
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
    CheckPair checkTiles();

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
