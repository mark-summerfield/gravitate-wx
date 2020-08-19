// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardwidget.hpp"

#include <wx/config.h>
#include <wx/dcclient.h>
#include <wx/utils.h>

#include <algorithm>
#include <cmath>
#include <chrono>
#include <functional>
#include <memory>


wxDEFINE_EVENT(SCORE_EVENT, wxCommandEvent);
wxDEFINE_EVENT(GAME_OVER_EVENT, wxCommandEvent);


const auto BACKGROUND_COLOR = wxColour("#FFFEE0");


bool operator==(const TilePos& a, const TilePos& b) {
    return a.x == b.x && a.y == b.y;
}

namespace std {
    template<> struct hash<TilePos> {
        size_t operator()(const TilePos& xy) const noexcept {
            return std::hash<int>{}(xy.x) ^ (std::hash<int>{}(xy.y) << 1);
        }
    };
}


BoardWidget::BoardWidget(wxWindow* parent)
        : wxWindow(parent, wxID_ANY), score(0), gameOver(true),
          drawing(false), dimming(false), columns(COLUMNS_DEFAULT),
          rows(ROWS_DEFAULT), delayMs(DELAY_MS_DEFAULT),
          selectedX(INVALID_POS), selectedY(INVALID_POS) {
    SetDoubleBuffered(true);
    Bind(wxEVT_LEFT_DOWN, &BoardWidget::onClick, this);
    Bind(wxEVT_CHAR_HOOK, &BoardWidget::onChar, this);
    Bind(wxEVT_PAINT, &BoardWidget::onPaint, this);
    Bind(wxEVT_SIZE, [&](wxSizeEvent&) { draw(); });
}


void BoardWidget::newGame() {
    gameOver = false;
    score = 0;
    selectedX = selectedY = INVALID_POS;
    const auto seed = std::chrono::system_clock::now().time_since_epoch()
        .count();
    Randomizer randomizer(seed);
    std::unique_ptr<wxConfig> config(new wxConfig(wxTheApp->GetAppName()));
    int maxColors;
    config->Read(MAX_COLORS, &maxColors, MAX_COLORS_DEFAULT);
    const auto colors = getColors(maxColors, randomizer);
    config->Read(COLUMNS, &columns, COLUMNS_DEFAULT);
    config->Read(ROWS, &rows, ROWS_DEFAULT);
    config->Read(DELAY_MS, &delayMs, DELAY_MS_DEFAULT);
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
        colors = { // key=dark, value=light
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
    return {rect.width / static_cast<double>(columns),
            rect.height / static_cast<double>(rows)};
}


void BoardWidget::onChar(wxKeyEvent& event) {
    if (gameOver || drawing) {
        event.Skip();
        return;
    }
    auto key = event.GetKeyCode();
    if (key == WXK_LEFT || key == WXK_RIGHT || key == WXK_UP ||
            key == WXK_DOWN)
        onMoveKey(key);
    else if (key == WXK_SPACE) {
        if (selectedX != INVALID_POS && selectedY != INVALID_POS)
            deleteTile(selectedX, selectedY);
    }
    else
        event.Skip();
}


void BoardWidget::onMoveKey(int code) {
    if (selectedX == INVALID_POS || selectedY == INVALID_POS) {
        selectedX = columns / 2;
        selectedY = rows / 2;
    }
    else {
        int x = selectedX;
        int y = selectedY;
        if (code == WXK_LEFT)
            --x;
        else if (code == WXK_RIGHT)
            ++x;
        else if (code == WXK_UP)
            --y;
        else if (code == WXK_DOWN)
            ++y;
        if (0 <= x && x < columns && 0 <= y && y < rows &&
                tiles[x][y] != wxNullColour) {
            selectedX = x;
            selectedY = y;
        }
    }
    draw();
}


void BoardWidget::onClick(wxMouseEvent& event) {
    if (gameOver || drawing) {
        event.Skip();
        return;
    }
    const auto size = tileSize();
    const int x = static_cast<int>(event.GetX() / round(size.width));
    const int y = static_cast<int>(event.GetY() / round(size.height));
    if (selectedX != INVALID_POS || selectedY != INVALID_POS) {
        selectedX = selectedY = INVALID_POS;
        draw();
    }
    deleteTile(x, y);
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
    auto nameMap = colorNameMap();
    const auto hexColor = color.GetAsString(wxC2S_HTML_SYNTAX);
    ColorPair colorPair;
    if (dimming) {
        // TODO dim
        colorPair.light = color;
        colorPair.dark = wxColour(nameMap[std::string(hexColor)]);
    }
    else if (gameOver) {
        // TODO grey out
        colorPair.light = wxColour(nameMap[std::string(hexColor)]);
        colorPair.dark = wxColour(nameMap[std::string(hexColor)]);
    }
    else {
        colorPair.light = wxColour(nameMap[std::string(hexColor)]);
if (!colorPair.light.IsOk()) {
    std::cout << "light = " << color.GetAsString(wxC2S_HTML_SYNTAX) <<
        "\n";
    std::cout << "alpha = " << int(color.Alpha()) << " dimming=" <<
        dimming << " hexColor=" << hexColor << "\n";
}
        colorPair.dark = color;
    }
    return colorPair;
}


void BoardWidget::drawTile(wxGraphicsContext* gc, int x, int y,
                           double width, double height, double edge,
                           double edge2) {
    const double x1 = x * width;
    const double y1 = y * height;
    const auto color = tiles[x][y];
    if (color == wxNullColour) {
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
        if (selectedX == x && selectedY == y)
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


void BoardWidget::deleteTile(int x, int y) {
    auto color = tiles[x][y];
    if (color == wxNullColour || !isLegal(x, y, color))
        return;
    dimming = true;
    dimAdjoining(x, y, color);
    dimming = false;
}


bool BoardWidget::isLegal(int x, int y, const wxColour& color) {
    // A legal click is on a colored tile that is adjacent to another
    // tile of the same color.
    if (x > 0 && tiles[x - 1][y] == color)
        return true;
    if (x + 1 < columns && tiles[x + 1][y] == color)
        return true;
    if (y > 0 && tiles[x][y - 1] == color)
        return true;
    if (y + 1 < rows && tiles[x][y + 1] == color)
        return true;
    return false;
}


void BoardWidget::dimAdjoining(int x, int y, const wxColour& color) {
    Adjoining adjoining;
    populateAdjoining(x, y, color, adjoining);
    auto nameMap = colorNameMap();
    for (auto it = adjoining.cbegin(); it != adjoining.cend(); ++it) {
        const int x = (*it).x;
        const int y = (*it).y;
        const auto hexColor = tiles[x][y].GetAsString(wxC2S_HTML_SYNTAX);
        // TODO dim
        tiles[x][y] = wxColour(nameMap[std::string(hexColor)]);
    }
    draw(5);
}


void BoardWidget::populateAdjoining(int x, int y, const wxColour& color,
                                    Adjoining& adjoining) {
    if (x < 0 || x >= columns || y < 0 || y >= rows)
        return; // Fallen off an edge
    if (tiles[x][y] != color)
        return; // Color doesn't match
    const auto tilePos = TilePos(x, y);
    auto it = adjoining.find(tilePos);
    if (it != adjoining.end())
        return; // Already done (C++20 supports .contains())
    adjoining.insert(tilePos);
    populateAdjoining(x - 1, y, color, adjoining);
    populateAdjoining(x + 1, y, color, adjoining);
    populateAdjoining(x, y - 1, color, adjoining);
    populateAdjoining(x, y + 1, color, adjoining);
}

// do score in closeTilesUp() (count is adjoining.size())
//  score += static_cast<int>(round(sqrt(static_cast<double>(columns) * rows)) +
//                  pow(count, maxColors / 2));
