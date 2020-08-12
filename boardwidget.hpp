#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class BoardWidget : public wxWindow {
public:
    explicit BoardWidget(wxWindow* parent);

private:
    bool gameOver;
    bool drawing;
};
