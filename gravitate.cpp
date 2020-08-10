// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "mainwindow.hpp"
#include "gravitate.hpp"


wxIMPLEMENT_APP(App);


bool App::OnInit() {
    SetAppName("Gravitate");
    MainWindow *window = new MainWindow();
    window->Show(true);
    return true;
}
