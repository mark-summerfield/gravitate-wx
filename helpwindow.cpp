// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "constants.hpp"
#include "helpwindow.hpp"

#include <wx/artprov.h>
#include <wx/html/htmlwin.h>


const wxString HTML_TEXT(LR"RAW(<html>
<body style="background-color: %s;">
<h2><center><font color=navy>Gravitate</font></center></h2>
<p><font color=navy><center>
<b>The purpose of the game is to remove all the tiles.</b></center></font>
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
“gravitate” to the middle.
</p>
<hr>
<table>
<tr><td><u>Key</u></td><td><u>Action</u></td></tr>
<tr><td><b>a</b></td><td>Show About box</td></tr>
<tr><td><b>h</b> or <b>F1</b></td><td>Show Help (this window)</td></tr>
<tr><td><b>n</b></td><td>New game</td></tr>
<tr><td><b>o</b></td><td>View or edit options</td></tr>
<tr><td><b>q</b></td><td>Quit</td></tr>
<tr><td><b>←</b></td><td>Move focus left</td></tr>
<tr><td><b>→</b></td><td>Move focus right</td></tr>
<tr><td><b>↑</b></td><td>Move focus up</td></tr>
<tr><td><b>↓</b></td><td>Move focus down</td></tr>
<tr><td><b>Space</b></td><td>Click focused tile</td></tr>
</table>
</body></html>)RAW");


HelpWindow::HelpWindow(wxWindow* parent)
        : wxDialog(parent, wxID_ANY,
                   wxString::Format(L"Help — %s", wxTheApp->GetAppName()),
                   wxDefaultPosition, wxSize(400, 500), FRAME_STYLE) {
    SetIcon(wxArtProvider::GetIcon(ICON_ID));
    SetMinSize(wxSize(200, 200));
    auto htmlLabel = new wxHtmlWindow(this);
    const auto background = wxSystemSettings::GetColour(
        wxSYS_COLOUR_BTNFACE);
    htmlLabel->SetPage(wxString::Format(HTML_TEXT,
                       background.GetAsString(wxC2S_HTML_SYNTAX)));
    auto okButton = new wxButton(this, wxID_OK, "&OK");
    okButton->SetDefault();
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(htmlLabel, 1, wxALL | wxEXPAND, 3);
    sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 3);
    SetSizer(sizer);
}
