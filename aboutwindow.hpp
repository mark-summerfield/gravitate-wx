#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class AboutWindow : public wxDialog {
public:
    explicit AboutWindow(wxWindow* parent);
};
