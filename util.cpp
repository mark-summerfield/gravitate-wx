// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "util.hpp"


wxButton* createStandardButton(wxWindow* parent, wxWindowID id) {
    wxString label;
    if (id == wxID_OK)
        label = L"<span color='green'>✔</span> <u>O</u>K";
    else if (id == wxID_CANCEL)
        label = L"<span color='red'>✘</span> <u>C</u>ancel";
    else
        label = "<span color='#808080'><b>INVALID ID</b></span>";
    auto button = new wxButton(parent, id);
    button->SetLabelMarkup(label);
    return button;
}
