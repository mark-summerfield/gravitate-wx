// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "artprovider.hpp"
#include "mainwindow.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class Application : public wxApp {
public:
    virtual bool OnInit();
};


wxIMPLEMENT_APP(Application);


bool Application::OnInit() {
    SetAppName("Gravitate");
    SetVendorName("qtrac.eu");
    wxArtProvider::Push(new ArtProvider);
    MainWindow *window = new MainWindow();
    window->Show(true);
    return true;
}
