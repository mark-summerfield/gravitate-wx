// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardwidget.hpp"
#include "constants.hpp"

#include <wx/config.h>

#include <algorithm>
#include <chrono>
#include <memory>


wxDEFINE_EVENT(SCORE_EVENT, wxCommandEvent);
wxDEFINE_EVENT(GAME_OVER_EVENT, wxCommandEvent);


const wxString BACKGROUND("#FFFEE0");


BoardWidget::BoardWidget(wxWindow* parent)
        : wxWindow(parent, wxID_ANY), score(0), gameOver(true),
          drawing(false) {
    SetDoubleBuffered(true);
    Bind(wxEVT_LEFT_DOWN, &BoardWidget::onClick, this);
    Bind(wxEVT_CHAR_HOOK, &BoardWidget::onChar, this);
    Bind(wxEVT_PAINT, &BoardWidget::onPaint, this);
    Bind(wxEVT_SIZE, [&](wxSizeEvent&) { draw(); });
}


void BoardWidget::newGame() {
    gameOver = false;
    score = 0;
    selected.x = selected.y = 0;
    const auto seed = std::chrono::system_clock::now().time_since_epoch()
        .count();
    Randomizer randomizer(seed);
    std::unique_ptr<wxConfig> config(new wxConfig(wxTheApp->GetAppName()));
    int maxColors;
    config->Read(MAX_COLORS, &maxColors, MAX_COLORS_DEFAULT);
    const auto colors = getColors(maxColors, randomizer);
    int columns;
    config->Read(COLUMNS, &columns, COLUMNS_DEFAULT);
    int rows;
    config->Read(ROWS, &rows, ROWS_DEFAULT);
    std::uniform_int_distribution<int> distribution(0, maxColors - 1);
    tiles.clear();
    for (int x = 0; x < columns; ++x) {
        tiles.push_back(TileRow());
        for (int y = 0; y < rows; ++y)
            tiles[x].push_back(colors[distribution(randomizer)]);
    }
    announceScore();
    draw();
}


ColorVector BoardWidget::getColors(int maxColors, Randomizer &randomizer) {
    auto colourMap = colorMap();
    ColorVector colors;
    for (auto it = colourMap.cbegin(); it != colourMap.cend(); ++it)
        colors.push_back(it->first);
    std::shuffle(colors.begin(), colors.end(), randomizer);
    colors.resize(maxColors);
    return colors;
}


const ColorMap& BoardWidget::colorMap() {
    static ColorMap colors;
    if (colors.empty())
        colors = {
            {"#000080", "#9999F9"},
            {"#008000", "#99F999"},
            {"#008080", "#99F9F9"},
            {"#800000", "#F99999"},
            {"#800080", "#F999F9"},
            {"#808000", "#F9F999"},
            {"#808080", "#F9F9F9"},
        };
    return colors;
}


void BoardWidget::announceScore() {
    wxCommandEvent event(SCORE_EVENT, GetId());
    event.SetEventObject(this);
    event.SetInt(score);
    ProcessWindowEvent(event);
}


// outcome is LOST or WON: call this from checkGameOver()
void BoardWidget::announceGameOver(const wxString& outcome) {
    wxCommandEvent event(GAME_OVER_EVENT, GetId());
    event.SetEventObject(this);
    event.SetInt(score);
    event.SetString(outcome);
    ProcessWindowEvent(event);
}


void BoardWidget::draw() {
    std::cout << "draw" << std::endl;
}


void BoardWidget::onClick(wxMouseEvent&) {
    std::cout << "onClick" << std::endl;
}


void BoardWidget::onChar(wxKeyEvent& event) {
    std::cout << "onChar" << std::endl;
    event.Skip();
}


void BoardWidget::onPaint(wxPaintEvent&) {
    std::cout << "onPaint" << std::endl;
}
