#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/html/htmlwin.h>


class About : public wxDialog {
public:
    explicit About(wxWindow* parent);
};
