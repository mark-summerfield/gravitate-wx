#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "mainwindow.hpp"


void onAbout(MainWindow* parent);


class AboutWindow : public wxDialog {
public:
    explicit AboutWindow(wxWindow* parent);

private:
#if wxVERSION_NUMBER < 3100
    wxSize FromDIP(const wxSize& size) { return size; }
#endif
};
