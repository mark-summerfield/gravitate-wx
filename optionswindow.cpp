// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "boardwidget.hpp"
#include "buttonwidget.hpp"
#include "constants.hpp"
#include "optionswindow.hpp"

#include <wx/artprov.h>
#include <wx/config.h>
#include <wx/gbsizer.h>

#include <cmath>
#include <memory>


OptionsWindow::OptionsWindow(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, wxString::Format(
                   L"Options — %s", wxTheApp->GetAppName())) {
    SetIcon(wxArtProvider::GetIcon(ICON_ID));
    makeWidgets();
    makeLayout();
    setSizes();
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
        style, 2, BoardWidget::colorMap().size(), n);
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
    okButton = Button::createStandard(panel, wxID_OK);
    okButton->SetDefault();
    okButton->SetToolTip("Confirm option choices: these will take effect "
                         "when you click New for a new game");
    padLabel = new wxStaticText(panel, wxID_ANY, " ");
    cancelButton = Button::createStandard(panel, wxID_CANCEL);
    cancelButton->SetToolTip("Leave the option choices unchanged");
}


void OptionsWindow::setSizes() {
    auto size = cancelButton->GetBestSize();
    padLabel->SetMaxSize(wxSize(PAD * 2, size.GetHeight()));
    okButton->SetMaxSize(size);
    cancelButton->SetMaxSize(size);
    size = delayMsSpinCtrl->GetBestSize();
    wxSpinCtrl* spinners[]{columnsSpinCtrl, rowsSpinCtrl, maxColorsSpinCtrl,
                           delayMsSpinCtrl};
    size = wxSize(std::round(size.GetWidth() * 1.5), size.GetHeight());
    for (auto& spinner: spinners)
        spinner->SetMinSize(size);
}


void OptionsWindow::makeLayout() {
    const auto flag = wxALL;
    auto grid = new wxGridBagSizer;
    grid->Add(columnsLabel, wxGBPosition(0, 0), wxDefaultSpan, flag, PAD);
    grid->Add(columnsSpinCtrl, wxGBPosition(0, 1), wxDefaultSpan, flag,
              PAD);
    grid->Add(rowsLabel, wxGBPosition(1, 0), wxDefaultSpan, flag, PAD);
    grid->Add(rowsSpinCtrl, wxGBPosition(1, 1), wxDefaultSpan, flag, PAD);
    grid->Add(maxColorsLabel, wxGBPosition(2, 0), wxDefaultSpan, flag, PAD);
    grid->Add(maxColorsSpinCtrl, wxGBPosition(2, 1), wxDefaultSpan, flag,
              PAD);
    grid->Add(delayMsLabel, wxGBPosition(3, 0), wxDefaultSpan, flag, PAD);
    grid->Add(delayMsSpinCtrl, wxGBPosition(3, 1), wxDefaultSpan, flag,
              PAD);
    auto buttonSizer = new wxStdDialogButtonSizer;
    buttonSizer->AddButton(okButton);
    buttonSizer->AddButton(cancelButton);
    buttonSizer->Realize();
    grid->Add(buttonSizer, wxGBPosition(4, 0), wxGBSpan(1, 2), flag,
              PAD * 2);
    panel->SetSizerAndFit(grid);
    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(panel, 1, wxALL | wxEXPAND, PAD);
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
