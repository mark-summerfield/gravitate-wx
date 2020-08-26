#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


const wxString VERSION("6.1.1");

const wxString COLUMNS("Board/Columns");
const wxString ROWS("Board/Rows");
const wxString MAX_COLORS("Board/MaxColors");
const wxString DELAY_MS("Board/DelayMs");
const wxString HIGH_SCORE("HighScore");
const wxString WINDOW_HEIGHT("Window/Height");
const wxString WINDOW_WIDTH("Window/Width");
const wxString WINDOW_X("Window/X");
const wxString WINDOW_Y("Window/Y");

const int COLUMNS_DEFAULT = 9;
const int ROWS_DEFAULT = 9;
const int MAX_COLORS_DEFAULT = 4;
const int DELAY_MS_DEFAULT = 200;
const int HIGH_SCORE_DEFAULT = 0;

const int TIMEOUT = 5000; // 5 sec
const int PAD = 5;
const int COORDS_LEN = 4;

const auto FRAME_STYLE = wxDEFAULT_FRAME_STYLE | wxDEFAULT_DIALOG_STYLE |
                         wxRESIZE_BORDER;

const wxString ICON_ID("ICON");
const wxString OPTIONS_ID("OPTIONS");

const wxString LOST("LOST");
const wxString WON("WON");
