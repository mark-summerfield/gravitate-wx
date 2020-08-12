// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "aboutwindow.hpp"
#include "helpwindow.hpp"
#include "optionswindow.hpp"
#include "actions.hpp"


namespace Actions {

void onAbout(MainWindow* parent) {
    AboutWindow aboutWindow(parent);
    aboutWindow.ShowModal();
}


void onNew(MainWindow *parent) {
    std::cout << "onNew" << std::endl;
}


void onOptions(MainWindow *parent) {
    OptionsWindow optionsWindow(parent);
    optionsWindow.ShowModal();
}


void onHelp(MainWindow *parent) {
    HelpWindow helpWindow(parent);
    helpWindow.ShowModal();
}


}
