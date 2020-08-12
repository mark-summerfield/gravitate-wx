// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardwidget.hpp"


BoardWidget::BoardWidget(wxWindow* parent)
        : wxWindow(parent, wxID_ANY), gameOver(true), drawing(false) {
    SetDoubleBuffered(true);
}
