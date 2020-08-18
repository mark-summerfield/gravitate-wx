// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

#include "svgartprovider.hpp"

#include "images/new32.xpm" // TODO replace with SVG


wxBitmap SvgArtProvider::CreateBitmap(const wxArtID& id,
                                      const wxArtClient& client,
                                      const wxSize& size) {
    return wxBitmap(new32_xpm);
}
