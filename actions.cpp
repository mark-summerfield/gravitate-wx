// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "aboutwindow.hpp"
#include "constants.hpp"
#include "helpwindow.hpp"
#include "optionswindow.hpp"
#include "actions.hpp"


namespace Actions {

void onAbout(MainWindow* parent) {
    AboutWindow aboutWindow(parent);
    aboutWindow.ShowModal();
}


void onOptions(MainWindow *parent) {
    OptionsWindow optionsWindow(parent);
    if (optionsWindow.ShowModal() == wxID_OK)
        parent->setTemporaryStatusMessage(
            "Updated Options: Click New to use them", TIMEOUT * 2);
}


void onHelp(MainWindow *parent) {
    HelpWindow helpWindow(parent);
    helpWindow.ShowModal();
}


}
