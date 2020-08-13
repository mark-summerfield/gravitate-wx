// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardwidget.hpp"
#include "constants.hpp"


wxDEFINE_EVENT(SCORE_EVENT, wxCommandEvent);
wxDEFINE_EVENT(GAME_OVER_EVENT, wxCommandEvent);


BoardWidget::BoardWidget(wxWindow* parent)
        : wxWindow(parent, wxID_ANY),
          score(0), gameOver(true), drawing(false) {
    SetDoubleBuffered(true);
}


void BoardWidget::newGame() {
    score = 0;
    announceScore();
    // TODO
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
