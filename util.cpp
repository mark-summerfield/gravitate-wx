// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "util.hpp"


wxButton* createStandardButton(wxWindow* parent, wxWindowID id) {
    wxString label;
    if (id == wxID_OK)
        label = L"✔ &OK";
    else if (id == wxID_CANCEL)
        label = L"✘ &Cancel";
    else
        label = "INVALID ID";
    return new wxButton(parent, id, label);
}
