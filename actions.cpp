// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "actions.hpp"

#include <wx/aboutdlg.h>
#include <wx/datetime.h>
#include <wx/platinfo.h>


namespace Actions {

void onAbout(MainWindow* parent) {
    auto platform = wxPlatformInfo::Get();
    wxAboutDialogInfo info;
    info.SetName(wxTheApp->GetAppName());
    info.SetVersion(VERSION);
    info.SetDescription(wxString::Format(
        "\nA game similar to TileFall/SameGame.\n\n"
        L"GCC/C++ %s  •  %s\n%s", __VERSION__, wxVERSION_STRING,
        platform.GetOperatingSystemDescription()));
    int year = wxDateTime::GetCurrentYear();
    info.SetCopyright(wxString::Format(
        L"\nCopyright © %s Qtrac Ltd. All Rights Reserved.\n\n"
        "License: GPLv3.",
        year == 2020 ? "2020" : wxString::Format("2020-%d", year)));
    info.SetWebSite("http://www.qtrac.eu/gravitate.html");
    wxAboutBox(info, parent);
}


void onNew(MainWindow *parent) {
    std::cout << "onNew" << std::endl;
}


void onOptions(MainWindow *parent) {
    std::cout << "onOptions" << std::endl;
}


void onHelp(MainWindow *parent) {
    std::cout << "onHelp" << std::endl;
}


}
