#pragma once
// Copyright © 2020 Mark Summerfield. All rights reserved.
// License: GPLv3

/*
    Applications should call
        wxArtProvider::Push(new ArtProvider);
    in Application::OnInit().
*/

#include <wx/artprov.h>


class ArtProvider : public wxArtProvider
{
protected:
#if wxVERSION_NUMBER >= 3100
  virtual wxBitmap CreateBitmap(const wxArtID& id,
                                const wxArtClient& client,
                                const wxSize& size) wxOVERRIDE;
#else
  virtual wxBitmap CreateBitmap(const wxArtID& id,
                                const wxArtClient& client,
                                const wxSize& size);
#endif
};
