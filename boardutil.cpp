// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardutil.hpp"

#include <algorithm>


bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}


ColorVector getColors(int maxColors, Randomizer &randomizer) {
    auto colors = colorMap();
    ColorVector result;
    for (auto it = colors.cbegin(); it != colors.cend(); ++it)
        result.push_back(wxColour(it->first));
    std::shuffle(result.begin(), result.end(), randomizer);
    result.resize(maxColors);
    return result;
}


ColorPair getColorPair(const wxColour& color, bool gameOver) {
    ColorPair colorPair;
    auto colors = colorMap();
    if (colors.find(color.GetRGBA()) == colors.end()) { // not found
        colorPair.light = color;                        // ∴ dimmed
        colorPair.dark = color.ChangeLightness(70);
    }
    else {
        colorPair.light = wxColour(colors[color.GetRGBA()]);
        colorPair.dark = color;
        if (gameOver) {
            colorPair.light = colorPair.light.ChangeLightness(85);
            colorPair.dark = colorPair.dark.ChangeLightness(85);
        }
    }
    return colorPair;
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


Ripple rippledRange(int limit, Randomizer& randomizer) {
    Ripple ripple;
    for (int i = 0; i < limit; ++i)
        ripple.push_back(i);
    std::shuffle(ripple.begin(), ripple.end(), randomizer);
    return ripple;
}
