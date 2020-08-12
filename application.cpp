// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "application.hpp"
#include "mainwindow.hpp"


wxIMPLEMENT_APP(Application);


bool Application::OnInit() {
    SetAppName("Gravitate");
    SetVendorName("qtrac.eu");
    MainWindow *window = new MainWindow();
    window->Show(true);
    return true;
}
