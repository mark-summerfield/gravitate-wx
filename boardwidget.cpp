// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardwidget.hpp"

#include <wx/config.h>
#include <wx/dcclient.h>
#include <wx/utils.h>

#include <algorithm>
#include <chrono>
#include <memory>


wxDEFINE_EVENT(SCORE_EVENT, wxCommandEvent);
wxDEFINE_EVENT(GAME_OVER_EVENT, wxCommandEvent);


const auto BACKGROUND_COLOR = wxColour("#FFFEE0");
const auto EMPTY_COLOR = wxColour("#010101");


BoardWidget::BoardWidget(wxWindow* parent)
        : wxWindow(parent, wxID_ANY), score(0), gameOver(true),
          drawing(false), dimming(false), columns(COLUMNS_DEFAULT),
          rows(ROWS_DEFAULT) {
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
    config->Read(COLUMNS, &columns, COLUMNS_DEFAULT);
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
    auto colourMap = colorNameMap();
    ColorVector colors;
    for (auto it = colourMap.cbegin(); it != colourMap.cend(); ++it)
        colors.push_back(wxColour(it->first));
    std::shuffle(colors.begin(), colors.end(), randomizer);
    colors.resize(maxColors);
    return colors;
}


const ColorNameMap& BoardWidget::colorNameMap() {
    static ColorNameMap colors;
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


void BoardWidget::draw(int delayMs, bool force) {
    if (delayMs)
        wxMilliSleep(delayMs);
    Refresh();
    if (force)
        Update();
}


TileSize BoardWidget::tileSize() const {
    auto rect = GetRect();
    return {rect.width / double(columns), rect.height / double(rows)};
}


void BoardWidget::onClick(wxMouseEvent&) {
    std::cout << "onClick" << std::endl;
}


void BoardWidget::onChar(wxKeyEvent& event) {
    std::cout << "onChar" << std::endl;
    event.Skip();
}


wxSize BoardWidget::DoGetBestClientSize() const {
    auto rect = GetRect();
    int size = std::min(rect.width, rect.height);
    return wxSize(size, size);
}


void BoardWidget::onPaint(wxPaintEvent&) {
    if (tiles.empty())
        return;
    drawing = true;
    wxPaintDC dc(this);
    auto gc = wxGraphicsContext::Create(dc);
    if (gc) {
        const auto size = tileSize();
        const double edge = std::min(size.width, size.height) / 9.0;
        const double edge2 = edge * 2.0;
        for (int x = 0; x < columns; ++x)
            for (int y = 0; y < rows; ++y)
                drawTile(gc, x, y, size.width, size.height, edge, edge2);
        delete gc;
    }
    drawing = false;
}


ColorPair BoardWidget::getColorPair(const wxColour& color) const {
    ColorPair colorPair;
    if (dimming) {
        colorPair.light = color;
        colorPair.dark = color.ChangeLightness(60);
    }
    else {
        const auto hexColor = color.GetAsString(wxC2S_HTML_SYNTAX);
        auto nameMap = colorNameMap();
        const auto lightColor = nameMap[std::string(hexColor)];
        colorPair.light = wxColour(lightColor);
        colorPair.dark = color;
        if (gameOver) {
            colorPair.light = colorPair.light.ChangeLightness(85);
            colorPair.dark = colorPair.dark.ChangeLightness(85);
        }
    }
    return colorPair;
}


void BoardWidget::drawTile(wxGraphicsContext* gc, int x, int y,
                           double width, double height, double edge,
                           double edge2) {
    const double x1 = x * width;
    const double y1 = y * height;
    const auto color = tiles[x][y];
    if (color == EMPTY_COLOR) {
        gc->SetBrush(wxBrush(BACKGROUND_COLOR));
        gc->DrawRectangle(x1, y1, width, height);
    }
    else {
        const double x2 = x1 + width;
        const double y2 = y1 + height;
        const auto colorPair = getColorPair(color);
        drawSegments(gc, edge, colorPair, x1, y1, x2, y2);
        auto brush = gc->CreateLinearGradientBrush(
            x1, y1, x2, y2, colorPair.light, colorPair.dark);
        gc->SetBrush(brush);
        gc->DrawRectangle(x1 + edge, y1 + edge, width - edge2,
                          height - edge2);
        if (selected == wxPoint(x, y))
            drawFocus(gc, x1, y1, edge, width, height);
    }
}


void BoardWidget::drawSegments(wxGraphicsContext* gc, double edge,
                               const ColorPair& colorPair, double x1,
                               double y1, double x2, double y2) {
    drawSegment(gc, colorPair.light, {{x1, y1}, {x1 + edge, y1 + edge},
                {x2 - edge, y1 + edge}, {x2, y1}});
    drawSegment(gc, colorPair.light, {{x1, y1}, {x1, y2},
                {x1 + edge, y2 - edge}, {x1 + edge, y1 + edge}});
    drawSegment(gc, colorPair.dark, {{x2 - edge, y1 + edge}, {x2, y1},
                {x2, y2}, {x2 - edge, y2 - edge}});
    drawSegment(gc, colorPair.dark, {{x1, y2}, {x1 + edge, y2 - edge},
                {x2 - edge, y2 - edge}, {x2, y2}});
}


void BoardWidget::drawSegment(wxGraphicsContext* gc, const wxColour& color,
                              const Coords& coords) {
    auto path = gc->CreatePath();
    path.MoveToPoint(coords[0][0], coords[0][1]);
    for (int i = 1; i < COORDS_LEN; ++i)
        path.AddLineToPoint(coords[i][0], coords[i][1]);
    path.CloseSubpath();
    gc->SetBrush(wxBrush(color));
    gc->FillPath(path);
}


void BoardWidget::drawFocus(wxGraphicsContext* gc, double x1, double y1,
                            double edge, double width, double height) {
    gc->SetBrush(wxBrush());
    edge *= 4 / 3;
    const double edge2 = edge * 2;
    gc->SetPen(wxPen(*wxBLACK, 1, wxPENSTYLE_DOT));
    gc->DrawRectangle(x1 + edge, y1 + edge, width - edge2, height - edge2);
    gc->SetPen(wxPen());
}
