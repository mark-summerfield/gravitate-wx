#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class Button : public wxButton {
public:
    static Button* createStandard(wxWindow* parent, wxWindowID id);

    explicit Button(wxWindow* parent, wxWindowID id, const wxString &label,
                    wxKeyCode accelerator=WXK_NONE);

private:
    void onChar(wxKeyEvent& event);

    wxKeyCode accelerator_;
};
