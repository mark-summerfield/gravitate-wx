#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "mainwindow.hpp"


void onHelp(MainWindow* parent);


class HelpWindow : public wxDialog {
public:
    explicit HelpWindow(wxWindow* parent);
};
