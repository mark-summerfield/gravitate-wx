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
const auto BACKGROUND_COLOR = wxColour(0xFFFFFEE0);


struct ColorPair {
    wxColour light;
    wxColour dark;
};


struct TileSize {
    double width;
    double height;
};


struct Point {
    Point(int x_=INVALID_POS, int y_=INVALID_POS) : x(x_), y(y_) {}

    bool isValid() const { return x != INVALID_POS && y != INVALID_POS; }

    int x;
    int y;
};


bool operator==(const Point& a, const Point& b);


namespace std {
    template<> struct hash<Point> {
        size_t operator()(const Point& xy) const noexcept {
            return std::hash<int>{}(xy.x) ^ (std::hash<int>{}(xy.y) << 1);
        }
    };
}


using ColorMap = std::unordered_map<wxUint32, wxUint32>;
using ColorVector = std::vector<wxColour>;
using Coords = double[COORDS_LEN][2];
using PointMap = std::unordered_map<Point, Point>;
using PointSet = std::unordered_set<Point>;
using Randomizer = std::default_random_engine;
using Ripple = std::vector<int>;
using TileRow = std::vector<wxColour>;
using TileGrid = std::vector<TileRow>;


ColorVector getColors(int, Randomizer&);
ColorPair getColorPair(const wxColour&, bool);
size_t colorCount();
const ColorMap& colorMap();
Ripple rippledRange(int limit, Randomizer& randomizer);
