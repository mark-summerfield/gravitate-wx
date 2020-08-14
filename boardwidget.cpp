// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardwidget.hpp"
#include "constants.hpp"


wxDEFINE_EVENT(SCORE_EVENT, wxCommandEvent);
wxDEFINE_EVENT(GAME_OVER_EVENT, wxCommandEvent);


BoardWidget::BoardWidget(wxWindow* parent)
        : wxWindow(parent, wxID_ANY),
          score(0), gameOver(true), drawing(false),
          columns(COLUMNS_DEFAULT), rows(ROWS_DEFAULT),
          maxColors(MAX_COLORS_DEFAULT), delayMs(DELAY_MS_DEFAULT) {
    SetDoubleBuffered(true);
}


void BoardWidget::newGame() {
    score = 0;
    announceScore();
    // TODO
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


// outcome is LOST or WON: call from checkGameOver()
void BoardWidget::announceGameOver(const wxString& outcome) {
    wxCommandEvent event(GAME_OVER_EVENT, GetId());
    event.SetEventObject(this);
    event.SetInt(score);
    event.SetString(outcome);
    ProcessWindowEvent(event);
}
