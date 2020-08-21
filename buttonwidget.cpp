// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "buttonwidget.hpp"


Button::Button(wxWindow* parent, wxWindowID id, const wxString &label,
               wxKeyCode accelerator)
        : wxButton(parent, id), accelerator_(accelerator) {
    SetLabelMarkup(label);
    parent->Bind(wxEVT_CHAR_HOOK, &Button::onChar, this);
}


void Button::onChar(wxKeyEvent& event) {
    if (accelerator_ != WXK_NONE && event.AltDown() &&
            accelerator_ == event.GetKeyCode()) {
        wxCommandEvent click(wxEVT_BUTTON, GetId());
        Command(click);
    }
    else
        event.Skip();
}


Button* Button::createStandard(wxWindow* parent, wxWindowID id) {
    wxString label;
    wxKeyCode accelerator;
    if (id == wxID_OK) {
#if wxVERSION_NUMBER >= 3100
        label = L"<span color='#00BB00'>✔</span> <u>O</u>K";
#else
        label = L"<u>O</u>K";
#endif
        accelerator = static_cast<wxKeyCode>('O');
    }
    else if (id == wxID_CANCEL) {
#if wxVERSION_NUMBER >= 3100
        label = L"<span color='red'>✘</span> <u>C</u>ancel";
#else
        label = L"<u>C</u>ancel";
#endif
        accelerator = static_cast<wxKeyCode>('C');
    }
    else {
        label = "<span color='#808080'><b>INVALID ID</b></span>";
        accelerator = WXK_NONE;
    }
    return new Button(parent, id, label, accelerator);
}
