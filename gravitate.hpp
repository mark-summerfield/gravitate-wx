#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


const wxString VERSION("6.0.0");


class App : public wxApp {
public:
    virtual bool OnInit();
};
