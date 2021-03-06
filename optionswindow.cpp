// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardutil.hpp"
#include "constants.hpp"
#include "optionswindow.hpp"

#include <wx/artprov.h>
#include <wx/config.h>
#include <wx/gbsizer.h>

#include <cmath>
#include <memory>


void onOptions(MainWindow *parent) {
    OptionsWindow optionsWindow(parent);
    if (optionsWindow.ShowModal() == wxID_OK)
        parent->setTemporaryStatusMessage(
            "Updated Options. Click New...", TIMEOUT * 2);
    else
        parent->SetStatusText("");
}


OptionsWindow::OptionsWindow(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, wxString::Format(
                   L"Options — %s", wxTheApp->GetAppName())) {
    SetIcon(wxArtProvider::GetIcon(ICON_ID));
    makeWidgets();
    makeLayout();
    Bind(wxEVT_BUTTON, &OptionsWindow::onOk, this, wxID_OK);
}


void OptionsWindow::makeWidgets() {
    const auto style = wxSP_ARROW_KEYS | wxALIGN_RIGHT;
    std::unique_ptr<wxConfig> config(new wxConfig(wxTheApp->GetAppName()));
    panel = new wxPanel(this);
    columnsLabel = new wxStaticText(panel, wxID_ANY, "Co&lumns");
    int n;
    config->Read(COLUMNS, &n, COLUMNS_DEFAULT);
    columnsSpinCtrl = new wxSpinCtrl(
        panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        style, 5, 30, n);
    columnsSpinCtrl->SetToolTip(wxString::Format(
        "How many columns of tiles to use [default %d]", COLUMNS_DEFAULT));
    rowsLabel = new wxStaticText(panel, wxID_ANY, "&Rows");
    config->Read(ROWS, &n, ROWS_DEFAULT);
    rowsSpinCtrl = new wxSpinCtrl(
        panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        style, 5, 30, n);
    rowsSpinCtrl->SetToolTip(wxString::Format(
        "How many rows of tiles to use [default %d]", ROWS_DEFAULT));
    maxColorsLabel = new wxStaticText(panel, wxID_ANY, "&Max. Colors");
    config->Read(MAX_COLORS, &n, MAX_COLORS_DEFAULT);
    maxColorsSpinCtrl = new wxSpinCtrl(
        panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        style, 2, colorCount(), n);
    maxColorsSpinCtrl->SetToolTip(wxString::Format(
        "How many colors to use [default %d]", MAX_COLORS_DEFAULT));
    delayMsLabel = new wxStaticText(panel, wxID_ANY, "&Delay (ms)");
    config->Read(DELAY_MS, &n, DELAY_MS_DEFAULT);
    delayMsSpinCtrl = new wxSpinCtrl(
        panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        style, 0, 1000, n);
    delayMsSpinCtrl->SetToolTip(wxString::Format(
        "How long to show tile movement in milliseconds (1/1000ths second) "
        "[default %d]", DELAY_MS_DEFAULT));
    okButton = new wxButton(panel, wxID_OK, "&OK");
    okButton->SetDefault();
    okButton->SetToolTip("Confirm option choices: these will take effect "
                         "when you click New for a new game");
    padLabel = new wxStaticText(panel, wxID_ANY, " ");
    cancelButton = new wxButton(panel, wxID_CANCEL, "&Cancel");
    cancelButton->SetToolTip("Leave the option choices unchanged");
}


void OptionsWindow::makeLayout() {
    const auto flag = wxALL;
    const auto flagX = flag | wxEXPAND;
    auto grid = new wxGridBagSizer;
    grid->Add(columnsLabel, wxGBPosition(0, 0), wxDefaultSpan, flag, PAD);
    grid->Add(columnsSpinCtrl, wxGBPosition(0, 1), wxDefaultSpan, flagX,
              PAD);
    grid->Add(rowsLabel, wxGBPosition(1, 0), wxDefaultSpan, flag, PAD);
    grid->Add(rowsSpinCtrl, wxGBPosition(1, 1), wxDefaultSpan, flagX, PAD);
    grid->Add(maxColorsLabel, wxGBPosition(2, 0), wxDefaultSpan, flag, PAD);
    grid->Add(maxColorsSpinCtrl, wxGBPosition(2, 1), wxDefaultSpan, flagX,
              PAD);
    grid->Add(delayMsLabel, wxGBPosition(3, 0), wxDefaultSpan, flag, PAD);
    grid->Add(delayMsSpinCtrl, wxGBPosition(3, 1), wxDefaultSpan, flagX,
              PAD);
    auto buttonSizer = new wxStdDialogButtonSizer;
    buttonSizer->AddButton(okButton);
    buttonSizer->AddButton(cancelButton);
    buttonSizer->Realize();
    grid->Add(buttonSizer, wxGBPosition(4, 0), wxGBSpan(1, 2), flag,
              PAD * 2);
    panel->SetSizerAndFit(grid);
    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(panel, 1, flagX, PAD);
    SetSizerAndFit(mainSizer);
}


void OptionsWindow::onOk(wxCommandEvent&) {
    std::unique_ptr<wxConfig> config(new wxConfig(wxTheApp->GetAppName()));
    config->Write(COLUMNS, columnsSpinCtrl->GetValue());
    config->Write(ROWS, rowsSpinCtrl->GetValue());
    config->Write(MAX_COLORS, maxColorsSpinCtrl->GetValue());
    config->Write(DELAY_MS, delayMsSpinCtrl->GetValue());
    EndModal(wxID_OK);
}
