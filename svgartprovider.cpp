// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "constants.hpp"
#include "svgartprovider.hpp"

#include "images/about.xpm"
#include "images/gravitate.xpm"
#include "images/help.xpm"
#include "images/new.xpm"
#include "images/options.xpm"
#include "images/shutdown.xpm"

#include <wx/dcsvg.h>


wxBitmap SvgArtProvider::CreateBitmap(const wxArtID& id, const wxArtClient&,
                                      const wxSize&) {
    if (id == ICON_ID)
        return wxBitmap(gravitate_xpm);
    if (id == wxART_NEW)
        return wxBitmap(new_xpm);
    if (id == OPTIONS_ID)
        return wxBitmap(options_xpm);
    if (id == wxART_INFORMATION)
        return wxBitmap(about_xpm);
    if (id == wxART_HELP)
        return wxBitmap(help_xpm);
    if (id == wxART_QUIT)
        return wxBitmap(shutdown_xpm);
    return wxNullBitmap;
}
