// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "constants.hpp"
#include "help.hpp"

#include <wx/html/htmlwin.h>


const wxString HTML_TEXT(R"RAW(<html>
<body style="background-color: %s;">
<h2><center><u>Gravitate</u></center></h2>
<p>
<b>The purpose of the game is to remove all the tiles.</b>
</p>
<p>
Click a tile that has at least one vertically or
horizontally adjoining tile of the same color to remove it and any
vertically or horizontally adjoining tiles of the same color, and
<i>their</i> vertically or horizontally adjoining tiles, and so on.
<i>(So clicking a tile with no adjoining tiles of the same color does
nothing.)</i> The more tiles that are removed in one go, the higher
the score.
</p>
<p>
Gravitate works like TileFall and the SameGame except that instead of
tiles falling to the bottom and moving off to the left, they
&ldquo;gravitate&rdquo; to the middle.
</p>
</body></html>)RAW");


Help::Help(wxWindow* parent)
        : wxDialog(parent, wxID_ANY,
                   wxString::Format(L"Help — %s", wxTheApp->GetAppName()),
                   wxDefaultPosition, wxSize(400, 500), FRAME_STYLE) {
    SetMinSize(wxSize(200, 200));
    auto htmlLabel = new wxHtmlWindow(this);
    auto background = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
    htmlLabel->SetPage(wxString::Format(HTML_TEXT,
                       background.GetAsString(wxC2S_HTML_SYNTAX)));
    auto okButton = new wxButton(this, wxID_OK);
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(htmlLabel, 1, wxALL | wxEXPAND, 3);
    sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 3);
    SetSizer(sizer);
}
