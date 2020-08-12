// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "about.hpp"
#include "constants.hpp"
#include "images/gravitate32.xpm"

#include <wx/datetime.h>
#include <wx/html/htmlwin.h>
#include <wx/platinfo.h>


const wxString HTML_TEXT(R"RAW(<html>
<body>
<h2><center><u>Gravitate %s</u></center></h2>
<p>
<center>A game similar to TileFall/SameGame.</center>
</p>
<p>
<center>
<a href="http://www.qtrac.eu/gravitate.html">www.qtrac.eu/gravitate.html</a>
</center>
</p>
<hr>
<p>
<center>
GCC/C++ %s&nbsp;&nbsp;&bull;&nbsp;&nbsp;%s<br>%s
</center>
</p>
<hr>
<p>
<center>
Copyright &copy; %s Mark Summerfield.<br>All Rights Reserved.
</center>
</p>
<p><small>
This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version. It is provided for education and enjoyment and is
distributed in the hope that it will be useful, but without any warranty;
without even the implied warranty of merchantability or fitness for a
particular purpose. see the GNU General Public License for more details.
</small></p>
</body></html>)RAW");


About::About(wxWindow* parent)
        : wxDialog(parent, wxID_ANY,
                   wxString::Format(L"About — %s", wxTheApp->GetAppName()),
                   wxDefaultPosition, wxSize(450, 550), FRAME_STYLE) {
    SetMinSize(wxSize(200, 200));
    auto iconBitmap = new wxStaticBitmap(this, wxID_ANY,
                                         wxBitmap(gravitate32_xpm));
    auto htmlLabel = new wxHtmlWindow(this);
    int year = wxDateTime::GetCurrentYear();
    auto platform = wxPlatformInfo::Get();
    htmlLabel->SetPage(wxString::Format(
        HTML_TEXT, VERSION, __VERSION__, wxVERSION_STRING,
        platform.GetOperatingSystemDescription(),
        year == 2020 ? "2020" : wxString::Format("2020-%d", year)));
    auto okButton = new wxButton(this, wxID_OK);
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(iconBitmap, 0, wxALL | wxALIGN_CENTER, 3);
    sizer->Add(htmlLabel, 1, wxALL | wxEXPAND, 3);
    sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 3);
    SetSizer(sizer);
}
