// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardwidget.hpp"

#include <wx/config.h>
#include <wx/dcclient.h>
#include <wx/utils.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <memory>


wxDEFINE_EVENT(SCORE_EVENT, wxCommandEvent);
wxDEFINE_EVENT(GAME_OVER_EVENT, wxCommandEvent);


BoardWidget::BoardWidget(wxWindow* parent)
        : wxWindow(parent, wxID_ANY), score(0), gameOver(true),
          userWon(false), drawing(false), columns(COLUMNS_DEFAULT),
          rows(ROWS_DEFAULT), maxColors(MAX_COLORS_DEFAULT),
          delayMs(DELAY_MS_DEFAULT) {
    SetDoubleBuffered(true);
    const auto seed = std::chrono::system_clock::now().time_since_epoch()
        .count();
    randomizer.seed(seed);
    Bind(wxEVT_LEFT_DOWN, &BoardWidget::onClick, this);
    Bind(wxEVT_CHAR_HOOK, &BoardWidget::onChar, this);
    Bind(wxEVT_PAINT, &BoardWidget::onPaint, this);
    Bind(wxEVT_SIZE, [&](wxSizeEvent&) { draw(); });
}


void BoardWidget::newGame() {
    gameOver = false;
    userWon = false;
    score = 0;
    selected.x = selected.y = INVALID_POS;
    std::unique_ptr<wxConfig> config(new wxConfig(wxTheApp->GetAppName()));
    config->Read(MAX_COLORS, &maxColors, MAX_COLORS_DEFAULT);
    config->Read(COLUMNS, &columns, COLUMNS_DEFAULT);
    config->Read(ROWS, &rows, ROWS_DEFAULT);
    config->Read(DELAY_MS, &delayMs, DELAY_MS_DEFAULT);
    const auto colors = getColors(randomizer);
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


ColorVector BoardWidget::getColors(Randomizer &randomizer) {
    auto colors = colorMap();
    ColorVector result;
    for (auto it = colors.cbegin(); it != colors.cend(); ++it)
        result.push_back(wxColour(it->first));
    std::shuffle(result.begin(), result.end(), randomizer);
    result.resize(maxColors);
    return result;
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
    const auto rect = GetRect();
    return {rect.width / static_cast<double>(columns),
            rect.height / static_cast<double>(rows)};
}


void BoardWidget::onChar(wxKeyEvent& event) {
    if (gameOver || drawing) {
        event.Skip();
        return;
    }
    const auto key = event.GetKeyCode();
    if (key == WXK_LEFT || key == WXK_RIGHT || key == WXK_UP ||
            key == WXK_DOWN)
        onMoveKey(key);
    else if (key == WXK_SPACE) {
        if (selected.isValid())
            deleteTile(selected);
    }
    else
        event.Skip();
}


void BoardWidget::onMoveKey(int code) {
    if (!selected.isValid()) {
        selected.x = columns / 2;
        selected.y = rows / 2;
    }
    else {
        int x = selected.x;
        int y = selected.y;
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
            selected.x = x;
            selected.y = y;
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
    if (selected.isValid()) {
        selected.x = selected.y = INVALID_POS;
        draw();
    }
    deleteTile(Point(x, y));
}


wxSize BoardWidget::DoGetBestClientSize() const {
    const auto rect = GetRect();
    const int size = std::min(rect.width, rect.height);
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
        if (userWon || gameOver)
            drawGameOver(gc);
        delete gc;
    }
    drawing = false;
}


void BoardWidget::drawGameOver(wxGraphicsContext *gc) {
    const wxString text(userWon ? "You Won!" : "Game Over");
    wxFont font(36, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL,
                wxFONTWEIGHT_BOLD);
    gc->SetFont(font, *wxWHITE);
    double width;
    double height;
    double descent;
    double leading;
    gc->GetTextExtent(text, &width, &height, &descent, &leading);
    const auto rect = GetRect();
    const auto x = (rect.GetWidth() - width) / 2;
    const auto y = (rect.GetHeight() / 2) - (height + descent + leading);
    gc->DrawText(text, x - 2, y - 3);
    gc->SetFont(font, *wxBLACK);
    gc->DrawText(text, x, y);
    wxColour color(userWon ? 0xFF0000BB : 0xFF009900);
    gc->SetFont(font, color);
    gc->DrawText(text, x - 1, y - 2);
}


ColorPair BoardWidget::getColorPair(const wxColour& color) const {
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
        if (selected.x == x && selected.y == y)
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


void BoardWidget::deleteTile(const Point point) {
    const auto color = tiles[point.x][point.y];
    if (color == wxNullColour || !isLegal(point, color))
        return;
    dimAdjoining(point, color);
}


bool BoardWidget::isLegal(const Point point, const wxColour& color) {
    // A legal click is on a colored tile that is adjacent to another
    // tile of the same color.
    const auto& x = point.x;
    const auto& y = point.y;
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


void BoardWidget::dimAdjoining(const Point point, const wxColour& color) {
    PointSet adjoining;
    populateAdjoining(point, color, adjoining);
    auto colors = colorMap();
    for (auto it = adjoining.cbegin(); it != adjoining.cend(); ++it) {
        const int x = (*it).x;
        const int y = (*it).y;
        tiles[x][y] = tiles[x][y].ChangeLightness(160);
    }
    draw(5);
    timer.Bind(wxEVT_TIMER,
               [=](wxTimerEvent&) { deleteAdjoining(adjoining); });
    timer.StartOnce(delayMs);
}


void BoardWidget::populateAdjoining(const Point point,
                                    const wxColour& color,
                                    PointSet& adjoining) {
    const auto& x = point.x;
    const auto& y = point.y;
    if (x < 0 || x >= columns || y < 0 || y >= rows)
        return; // Fallen off an edge
    if (tiles[x][y] != color)
        return; // Color doesn't match
    auto it = adjoining.find(point);
    if (it != adjoining.end())
        return; // Already done (C++20 supports .contains())
    adjoining.insert(point);
    populateAdjoining(Point(x - 1, y), color, adjoining);
    populateAdjoining(Point(x + 1, y), color, adjoining);
    populateAdjoining(Point(x, y - 1), color, adjoining);
    populateAdjoining(Point(x, y + 1), color, adjoining);
}


void BoardWidget::deleteAdjoining(const PointSet adjoining) {
    for (auto it = adjoining.cbegin(); it != adjoining.cend(); ++it)
        tiles[(*it).x][(*it).y] = wxNullColour;
    draw(5);
    const auto size = adjoining.size();
    timer.Bind(wxEVT_TIMER, [=](wxTimerEvent&) { closeTilesUp(size); });
    timer.StartOnce(delayMs);
}


void BoardWidget::closeTilesUp(size_t count) {
    moveTiles();
    if (selected.isValid() &&
            tiles[selected.x][selected.y] == wxNullColour) {
        selected.x = columns / 2;
        selected.y = rows / 2;
    }
    draw();
    score += static_cast<int>(
        std::round(std::sqrt(static_cast<double>(columns) * rows)) +
        std::pow(count, maxColors / 2));
    announceScore();
    checkGameOver();
}


void BoardWidget::moveTiles() {
    PointMap moves;
    bool moved = true;
    while (moved) {
        moved = false;
        for (int x: rippledRange(columns, randomizer))
            for (int y: rippledRange(rows, randomizer)) {
                if (tiles[x][y] != wxNullColour)
                    if (moveIsPossible(Point(x, y), moves)) {
                        moved = true;
                        break;
                    }
            }
    }
}


bool BoardWidget::moveIsPossible(const Point point, PointMap& moves) {
    const auto empties = getEmptyNeighbours(point);
    if (!empties.empty()) {
        bool move;
        const auto newPoint = nearestToMiddle(point, empties, &move);
        auto it = moves.find(newPoint);
        if (it != moves.end() && it->second == point)
            return false; // avoid endless loop
        if (move) {
            tiles[newPoint.x][newPoint.y] = tiles[point.x][point.y];
            tiles[point.x][point.y] = wxNullColour;
            draw(std::max(1, static_cast<int>(std::round(delayMs / 7))),
                 true);
            moves.insert({point, newPoint});
            return true;
        }
    }
    return false;
}


PointSet BoardWidget::getEmptyNeighbours(const Point point) {
    PointSet neighbours;
    const auto& x = point.x;
    const auto& y = point.y;
    const Point points[]{Point(x - 1, y), Point(x + 1, y),
                         Point(x, y - 1), Point(x, y + 1)};
    for (auto newPoint: points) {
        if (0 <= newPoint.x && newPoint.x < columns && 0 <= newPoint.y &&
                newPoint.y < rows &&
                tiles[newPoint.x][newPoint.y] == wxNullColour)
            neighbours.insert(newPoint);
    }
    return neighbours;
}


Point BoardWidget::nearestToMiddle(const Point point,
                                   const PointSet& empties, bool* move) {
    const auto color = tiles[point.x][point.y];
    const int midX = columns / 2;
    const int midY = rows / 2;
    const double oldRadius = std::hypot(midX - point.x, midY - point.y);
    double shortestRadius = NAN;
    Point radiusPoint;
    for (const auto& newPoint: empties) {
        if (isSquare(newPoint)) {
            double newRadius = std::hypot(midX - newPoint.x,
                                          midY - newPoint.y);
            if (isLegal(newPoint, color))
                newRadius -= 0.1; // Make same colors slightly attract
            if (!radiusPoint.isValid() || shortestRadius > newRadius) {
                shortestRadius = newRadius;
                radiusPoint = newPoint;
            }
        }
    }
    if (!std::isnan(shortestRadius) && oldRadius > shortestRadius) {
        *move = true;
        return radiusPoint;
    }
    *move = false;
    return point;
}


bool BoardWidget::isSquare(const Point& point) {
    const auto x = point.x;
    const auto y = point.y;
    if (x > 0 && tiles[x - 1][y] != wxNullColour)
        return true;
    if (x + 1 < columns && tiles[x + 1][y] != wxNullColour)
        return true;
    if (y > 0 && tiles[x][y - 1] != wxNullColour)
        return true;
    if (y + 1 < rows && tiles[x][y + 1] != wxNullColour)
        return true;
    return false;
}


void BoardWidget::checkGameOver() {
    auto canMove = checkTiles();
    if (userWon)
        announceGameOver(WON);
    else if (!canMove)
        announceGameOver(LOST);
}


bool BoardWidget::checkTiles() {
    std::unordered_map<wxUint32, int> countForColor;
    userWon = true;
    bool canMove = false;
    for (int x = 0; x < columns; ++x)
        for (int y = 0; y < rows; ++y) {
            const auto color = tiles[x][y];
            if (color != wxNullColour) {
                ++countForColor[color.GetRGBA()];
                userWon = false;
                if (isLegal(Point(x, y), color))
                    canMove = true;
            }
        }
    for (auto it = countForColor.cbegin(); it != countForColor.cend(); ++it)
        if (it->second == 1) {
            canMove = false;
            break;
        }
    if (userWon || !canMove) {
        gameOver = true;
        draw();
    }
    return canMove;
}
