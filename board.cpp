// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "board.hpp"


Board::Board(wxWindow* parent) : wxWindow(parent, wxID_ANY),
                                 gameOver(true), drawing(false) {
    SetDoubleBuffered(true);
}
