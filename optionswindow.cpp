// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "optionswindow.hpp"


OptionsWindow::OptionsWindow(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, wxString::Format(
                   L"Options — %s", wxTheApp->GetAppName())) {
}
