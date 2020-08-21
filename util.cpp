// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "util.hpp"

#include <iostream>
#include <locale>
#include <sstream>


std::string humanize(const int i) {
    std::ostringstream oss;
    oss.imbue(std::locale(""));
    oss << i;
    return oss.str();
}
