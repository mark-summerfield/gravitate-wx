#!/bin/bash
tokei -slines -f -c80 -tC++,'C++ Header'
cppcheck -q --enable=all . 2>&1 \
    | grep -v application.*OnInit..is.never.used \
    | grep -v artprovider.*CreateBitmap..is.never.used \
    | grep -v boardutil.*operator==..is.never.used \
    | grep -v boardwidget.*DoGetBestClientSize..is.never.used \
    | grep -v Cppcheck.cannot.find.all.the.include.files
git st
