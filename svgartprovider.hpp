#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

/*
    Applications should call
        wxArtProvider::Push(new SvgArtProvider);
    in Application::OnInit().
*/

#include <wx/artprov.h>


class SvgArtProvider : public wxArtProvider
{
protected:
  virtual wxBitmap CreateBitmap(const wxArtID& id,
                                const wxArtClient& client,
                                const wxSize& size) wxOVERRIDE;
};
