// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardutil.hpp"


bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}


size_t colorCount() { return colorMap().size(); }


const ColorMap& colorMap() {
    static ColorMap colors;
    if (colors.empty())
        colors = { // key=dark, value=light
        {0xFF800000, 0xFFF99999},
        {0xFF800000, 0xFFF99999},
        {0xFF008000, 0xFF99F999},
        {0xFF808000, 0xFFF9F999},
        {0xFF000080, 0xFF9999F9},
        {0xFF800080, 0xFFF999F9},
        {0xFF008080, 0xFF99F9F9},
        {0xFF808080, 0xFFF9F9F9},
        };
    return colors;
}
