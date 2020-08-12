// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "about.hpp"
#include "help.hpp"
#include "options.hpp"
#include "actions.hpp"


namespace Actions {

void onAbout(MainWindow* parent) {
    About about(parent);
    about.ShowModal();
}


void onNew(MainWindow *parent) {
    std::cout << "onNew" << std::endl;
}


void onOptions(MainWindow *parent) {
    Options options(parent);
    options.ShowModal();
}


void onHelp(MainWindow *parent) {
    Help help(parent);
    help.ShowModal();
}


}
