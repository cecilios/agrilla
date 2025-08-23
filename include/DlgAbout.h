//---------------------------------------------------------------------------------------
// This file is part of the AGrilla application.
// Copyright (c) 2025-present, Cecilios Salmeron
//
// Licensed under the MIT license.
//
// See LICENSE file in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#pragma once

//wxWidgets
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/html/htmlwin.h>
#include <wx/event.h>

//forward declarations
class wxStaticBitmap;
class wxStaticText;
class wxStaticBitmap;
class wxStaticBitmap;

namespace agrilla
{

//---------------------------------------------------------------------------------------
class AboutDialog : public wxDialog
{
public:
    AboutDialog(wxWindow* pParent);
    ~AboutDialog();

    // event handlers
    void OnAccept(wxCommandEvent& WXUNUSED(event));
    void OnPurpose(wxCommandEvent& WXUNUSED(event));
    void OnLicense(wxCommandEvent& WXUNUSED(event));
    void OnDevelopers(wxCommandEvent& WXUNUSED(event));
    void OnArtCredits(wxCommandEvent& WXUNUSED(event));
    void OnSoftwareCredits(wxCommandEvent& WXUNUSED(event));
    void OnBuildInfo(wxCommandEvent& WXUNUSED(event));


private:
	void CreateControls();

    //controls on dialog
	wxStaticBitmap*		m_pBmpLogo;
	wxStaticText*		m_pTxtTitle;
	wxStaticText*		m_pTxtSubtitle;
	wxHtmlWindow*		m_pHtmlWindow;
	wxButton*			m_pBtnPurpose;
	wxButton*			m_pBtnLicense;
	wxButton*			m_pBtnDevelopers;
	wxButton*			m_pBtnArtCredits;
	wxButton*			m_pBtnSoftwareCredits;
	wxButton*			m_pBtnBuildInfo;
	wxButton*			m_pBtnAccept;

    //other variables
    wxString        m_sHeader;          //html code to start a page
    wxString        m_sVersionNumber;   //version number in format "x.x"

    wxDECLARE_EVENT_TABLE();
};


}   //namespace agrilla
