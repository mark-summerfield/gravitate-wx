// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardwidget.hpp"
#include "constants.hpp"
#include "optionswindow.hpp"

#include <wx/config.h>
#include <wx/gbsizer.h>

#include <memory>


OptionsWindow::OptionsWindow(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, wxString::Format(
                   L"Options — %s", wxTheApp->GetAppName())) {
    makeWidgets();
    makeLayout();
    Bind(wxEVT_BUTTON, &OptionsWindow::onOk, this, wxID_OK);
}

void OptionsWindow::makeWidgets() {
    auto style = wxSP_ARROW_KEYS | wxALIGN_RIGHT;
    std::unique_ptr<wxConfig> config(new wxConfig(wxTheApp->GetAppName()));
    columnsLabel = new wxStaticText(this, wxID_ANY, "Co&lumns");
    int n;
    config->Read(COLUMNS, &n, COLUMNS_DEFAULT);
    columnsSpinCtrl = new wxSpinCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        style, 5, 30, n);
    rowsLabel = new wxStaticText(this, wxID_ANY, "&Rows");
    config->Read(ROWS, &n, ROWS_DEFAULT);
    rowsSpinCtrl = new wxSpinCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        style, 5, 30, n);
    maxColorsLabel = new wxStaticText(this, wxID_ANY, "&Max. Colors");
    config->Read(MAX_COLORS, &n, MAX_COLORS_DEFAULT);
    maxColorsSpinCtrl = new wxSpinCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        style, 2, BoardWidget::colorMap().size(), n);
    delayMsLabel = new wxStaticText(this, wxID_ANY, "&Delay (ms)");
    config->Read(DELAY_MS, &n, DELAY_MS_DEFAULT);
    delayMsSpinCtrl = new wxSpinCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        style, 0, 1000, n);
    okButton = new wxButton(this, wxID_OK);
    cancelButton = new wxButton(this, wxID_CANCEL);
}


void OptionsWindow::makeLayout() {
    auto flag = wxALL;
    auto flagX = wxALL | wxEXPAND;
    int border = 3;
    auto grid = new wxGridBagSizer;
    grid->Add(columnsLabel, wxGBPosition(0, 0), wxDefaultSpan, flag,
              border);
    grid->Add(columnsSpinCtrl, wxGBPosition(0, 1), wxDefaultSpan, flagX,
              border);
    grid->Add(rowsLabel, wxGBPosition(1, 0), wxDefaultSpan, flag, border);
    grid->Add(rowsSpinCtrl, wxGBPosition(1, 1), wxDefaultSpan, flagX,
              border);
    grid->Add(maxColorsLabel, wxGBPosition(2, 0), wxDefaultSpan, flag,
              border);
    grid->Add(maxColorsSpinCtrl, wxGBPosition(2, 1), wxDefaultSpan, flagX,
              border);
    grid->Add(delayMsLabel, wxGBPosition(3, 0), wxDefaultSpan, flag,
              border);
    grid->Add(delayMsSpinCtrl, wxGBPosition(3, 1), wxDefaultSpan, flagX,
              border);
    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(okButton, flag, border);
    sizer->Add(cancelButton, flag, border);
    grid->Add(sizer, wxGBPosition(4, 0), wxGBSpan(1, 2), wxEXPAND);
    grid->AddGrowableCol(1);
    SetSizer(grid);
    Fit();
}


void OptionsWindow::onOk(wxCommandEvent& event) {
    std::unique_ptr<wxConfig> config(new wxConfig(wxTheApp->GetAppName()));
    /*
    config->Write(COLUMNS, ?);
    config->Write(ROWS, ?);
    config->Write(MAX_COLORS, ?);
    config->Write(DELAY_MS, ?);
    */
    EndModal(wxID_OK);
}
