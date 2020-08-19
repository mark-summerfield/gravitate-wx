// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "aboutwindow.hpp"
#include "buttonwidget.hpp"
#include "constants.hpp"

#include <wx/artprov.h>
#include <wx/datetime.h>
#include <wx/platinfo.h>


const wxString HTML_TEXT(LR"RAW(<html>
<body style="background-color: %s;">
<h2><center><font color=navy>Gravitate %s</font></center></h2>
<p>
<center><font color=navy><b>A game similar to
TileFall/SameGame.</b></font></center>
</p>
<p>
<center>
<a href="http://www.qtrac.eu/gravitate.html">www.qtrac.eu/gravitate.html</a>
</center>
</p>
<hr>
<p>
<center>
GCC/C++ %s&nbsp;&nbsp;•&nbsp;&nbsp;%s<br>%s
</center>
</p>
<hr>
<p>
<center>
Copyright © %s Mark Summerfield.<br>All Rights Reserved.
</center>
</p>
<p><font size=-1 color=#0F0F0F>
This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version. It is provided for education and enjoyment and is
distributed in the hope that it will be useful, but without any warranty;
without even the implied warranty of merchantability or fitness for a
particular purpose. see the GNU General Public License for more details.
</font></p>
</body></html>)RAW");


AboutWindow::AboutWindow(wxWindow* parent)
        : wxDialog(parent, wxID_ANY,
                   wxString::Format(L"About — %s", wxTheApp->GetAppName()),
                   wxDefaultPosition, wxSize(450, 550), FRAME_STYLE) {
    SetIcon(wxArtProvider::GetIcon(ICON_ID));
    SetMinSize(wxSize(200, 200));
    auto iconBitmap = new wxStaticBitmap(
        this, wxID_ANY, wxArtProvider::GetBitmap(ICON_ID, wxART_OTHER,
                                                 FromDIP(wxSize(48, 48))));
    auto htmlLabel = new wxHtmlWindow(this);
    const int year = wxDateTime::GetCurrentYear();
    auto platform = wxPlatformInfo::Get();
    auto background = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
    htmlLabel->SetPage(wxString::Format(
        HTML_TEXT, background.GetAsString(wxC2S_HTML_SYNTAX), VERSION,
        __VERSION__, wxVERSION_STRING,
        platform.GetOperatingSystemDescription(),
        year == 2020 ? "2020" : wxString::Format("2020-%d", year)));
    auto okButton = Button::createStandard(this, wxID_OK);
    okButton->SetDefault();
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(iconBitmap, 0, wxALL | wxALIGN_CENTER, 3);
    sizer->Add(htmlLabel, 1, wxALL | wxEXPAND, 3);
    sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 3);
    SetSizer(sizer);
    Bind(wxEVT_HTML_LINK_CLICKED, [&](wxHtmlLinkEvent event) {
         wxLaunchDefaultBrowser(event.GetLinkInfo().GetHref()); });
}
