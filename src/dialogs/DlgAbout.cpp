//---------------------------------------------------------------------------------------
// This file is part of the AGrilla application.
// Copyright (c) 2025-present, Cecilios Salmeron
//
// Licensed under the MIT license.
//
// See LICENSE file in the root directory of this source tree.
//---------------------------------------------------------------------------------------

//wxWidgets
#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/window.h>
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/artprov.h>
#include <wx/gdicmn.h>

//agrilla
#include "DlgAbout.h"
#include "TheApp.h"     //to get version information


using namespace std;


namespace agrilla
{

//IDs for controls
const int lmID_BTN_ACCEPT = wxNewId();
const int lmID_BTN_ART_CREDITS = wxNewId();
const int lmID_BTN_BUILD_INFO = wxNewId();
const int lmID_BTN_LICENCE = wxNewId();
const int lmID_BTN_PURPOSE = wxNewId();
const int lmID_BTN_SOFTWARE_CREDITS = wxNewId();
const int lmID_BTN_DEVELOPERS = wxNewId();


wxBEGIN_EVENT_TABLE(AboutDialog, wxDialog)
    EVT_BUTTON(lmID_BTN_ACCEPT, AboutDialog::OnAccept )
    EVT_BUTTON(lmID_BTN_PURPOSE, AboutDialog::OnPurpose )
    EVT_BUTTON(lmID_BTN_LICENCE, AboutDialog::OnLicense )
    EVT_BUTTON(lmID_BTN_DEVELOPERS, AboutDialog::OnDevelopers )
    EVT_BUTTON(lmID_BTN_ART_CREDITS, AboutDialog::OnArtCredits )
    EVT_BUTTON(lmID_BTN_SOFTWARE_CREDITS, AboutDialog::OnSoftwareCredits )
    EVT_BUTTON(lmID_BTN_BUILD_INFO, AboutDialog::OnBuildInfo )

wxEND_EVENT_TABLE()


//---------------------------------------------------------------------------------------
AboutDialog::AboutDialog(wxWindow* pParent)
    : wxDialog(pParent, wxID_ANY, "AGrilla", wxDefaultPosition, wxSize(600, 400),
               wxDEFAULT_DIALOG_STYLE, "dialogBox")
{
    // create the dialog controls
    CreateControls();

    //load logo icon
    m_pBmpLogo->SetBitmap( wxArtProvider::GetIcon("logo50x67", wxART_OTHER) );

    //initializations
    m_sHeader =
      "<html>"
      _T("<head><META http-equiv=\"Content-Type\" content=\"text/html; charset=") +
      wxLocale::GetSystemEncodingName() +
      _T("\"></head>")
      _T("<body bgcolor=\"#ffffff\">")
      "<font size=3>";

    //version
    m_sVersionNumber = wxGetApp().get_version_string();

    //title and subtitle
    m_pTxtTitle->SetLabel("AGrilla");
    m_pTxtSubtitle->SetLabel(_("A tool to overlay a grid over any screen region"));

    CentreOnScreen();

    //display purpose information
    wxCommandEvent event;       //no need to initialize it as it is not used
    OnPurpose(event);

}

//---------------------------------------------------------------------------------------
AboutDialog::~AboutDialog()
{
}

//---------------------------------------------------------------------------------------
void AboutDialog::CreateControls()
{
    wxBoxSizer* pMainSizer;
    pMainSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* pHeadersSizer;
    pHeadersSizer = new wxBoxSizer( wxHORIZONTAL );

    m_pBmpLogo = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
    pHeadersSizer->Add( m_pBmpLogo, 0, wxALIGN_TOP|wxALL, 5 );

    wxBoxSizer* pTitlesSizer;
    pTitlesSizer = new wxBoxSizer( wxVERTICAL );

    m_pTxtTitle = new wxStaticText( this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0 );
    m_pTxtTitle->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial" ) );

	pTitlesSizer->Add(m_pTxtTitle, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    m_pTxtSubtitle = new wxStaticText( this, wxID_ANY, _("A free program for music language learning"), wxDefaultPosition, wxDefaultSize, 0 );
    m_pTxtSubtitle->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Arial" ) );

	pTitlesSizer->Add(m_pTxtSubtitle, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    pHeadersSizer->Add( pTitlesSizer, 1, wxEXPAND|wxALL, 5 );

    pMainSizer->Add( pHeadersSizer, 0, wxEXPAND|wxALL, 5 );

    wxBoxSizer* pInfoSizer;
    pInfoSizer = new wxBoxSizer( wxHORIZONTAL );

    m_pHtmlWindow = new wxHtmlWindow( this, wxID_ANY, wxDefaultPosition, wxSize( 200,150 ), wxHW_SCROLLBAR_AUTO|wxBORDER_SUNKEN|wxHSCROLL|wxVSCROLL);
    pInfoSizer->Add( m_pHtmlWindow, 1, wxEXPAND|wxBOTTOM, 5 );

    wxBoxSizer* pButtonsSizer;
    pButtonsSizer = new wxBoxSizer( wxVERTICAL );

    m_pBtnPurpose = new wxButton( this, lmID_BTN_PURPOSE, _("Purpose"), wxDefaultPosition, wxDefaultSize, 0 );
    pButtonsSizer->Add( m_pBtnPurpose, 0, wxEXPAND|wxALL, 5 );

    m_pBtnLicense = new wxButton( this, lmID_BTN_LICENCE, _("Licence"), wxDefaultPosition, wxDefaultSize, 0 );
    pButtonsSizer->Add( m_pBtnLicense, 0, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5 );

    m_pBtnDevelopers = new wxButton( this, lmID_BTN_DEVELOPERS, _("Contributors"), wxDefaultPosition, wxDefaultSize, 0 );
    pButtonsSizer->Add( m_pBtnDevelopers, 0, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5 );

    m_pBtnArtCredits = new wxButton( this, lmID_BTN_ART_CREDITS, _("Art design"), wxDefaultPosition, wxDefaultSize, 0 );
    pButtonsSizer->Add( m_pBtnArtCredits, 0, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5 );

    m_pBtnSoftwareCredits = new wxButton( this, lmID_BTN_SOFTWARE_CREDITS, _("Software used"), wxDefaultPosition, wxDefaultSize, 0 );
    pButtonsSizer->Add( m_pBtnSoftwareCredits, 0, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5 );

    m_pBtnBuildInfo = new wxButton( this, lmID_BTN_BUILD_INFO, _("Build info."), wxDefaultPosition, wxDefaultSize, 0 );
    pButtonsSizer->Add( m_pBtnBuildInfo, 0, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5 );

    pButtonsSizer->Add( 5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    m_pBtnAccept = new wxButton( this, lmID_BTN_ACCEPT, _("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
    pButtonsSizer->Add( m_pBtnAccept, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    pInfoSizer->Add( pButtonsSizer, 0, wxEXPAND|wxLEFT|wxBOTTOM, 5 );

    pMainSizer->Add( pInfoSizer, 1, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5 );

    this->SetSizer( pMainSizer );
    this->Layout();
}

//---------------------------------------------------------------------------------------
void AboutDialog::OnAccept(wxCommandEvent& WXUNUSED(event))
{
   EndModal(wxID_OK);
}

//---------------------------------------------------------------------------------------
void AboutDialog::OnPurpose(wxCommandEvent& WXUNUSED(event))
{
    wxString sContent = m_sHeader +
      "<center>"
      "<h3>" + _("Purpose") + "</h3>"
      "</center><p>" +
    _("AGrilla is a free tool to overlay a grid over any screen region, providing a \
guide to help you in your composition and drawing.") +
      "</p><p>" +
    _("The AGrilla Project is an open project, committed to the principles of \
Open Source and open access to information. It has no comercial \
purpose.") +
      "</p><p>" +
    _("If you find a bug or have a suggestion, please tell about it. Go to the AGrilla \
issues page at <b>https://github.com/cecilios/agrilla/issues</b>. Thank you very much.") +
      "</p></body></html>";

    m_pHtmlWindow->SetPage(sContent);

}

//---------------------------------------------------------------------------------------
void AboutDialog::OnLicense(wxCommandEvent& WXUNUSED(event))
{
    wxString sContent = m_sHeader +
        "<center>"
        "<h3>" + _("MIT License") + "</h3></center><p>"
        "Copyright &copy; 2025-present  Cecilio Salmer&oacute;n." +
        "</p><p>" +
_("Permission is hereby granted, free of charge, to any person obtaining a copy \
of this software and associated documentation files (the \"Software\"), to deal \
in the Software without restriction, including without limitation the rights \
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell \
copies of the Software, and to permit persons to whom the Software is \
furnished to do so, subject to the following conditions:") +
        "</p><p>" +
        _("The above copyright notice and this permission notice shall be included \
in all copies or substantial portions of the Software.") +
        "</p><p>" +
_("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR \
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, \
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE \
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER \
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, \
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE \
SOFTWARE.") +
        "</p></body></html>";

        m_pHtmlWindow->SetPage(sContent);

}

//---------------------------------------------------------------------------------------
void AboutDialog::OnArtCredits(wxCommandEvent& WXUNUSED(event))
{
    wxString sContent = m_sHeader +
        "<center>"
        "<h3>" + _("Art desing credits") + "</h3></center>" +
        _("All AGrilla icons are either:") +
        "<br /><ul><li>" +
        _("Original GNOME Desktop Tango icons taken from wikimedia \
(<b>https://commons.wikimedia.org/wiki/GNOME_Desktop_icons</b>)") +
        "</li><li>" +
        _("Modified GNOME Desktop Tango icons taken from above.") +
        "</li><li>" +
        _("Original icons created by AGrilla contributors") +
        "</li></ul><br />" +
        _("All icons are licensed under GNU General Public License version 2.") +
        "<br /></body></html>";

        m_pHtmlWindow->SetPage(sContent);

}

//---------------------------------------------------------------------------------------
void AboutDialog::OnSoftwareCredits(wxCommandEvent& WXUNUSED(event))
{
    wxString sContent = m_sHeader +
        "<center>"
        "<h3>" + _("Software credits") + "</h3></center><p>" +
        _("AGrilla is built using the <b>wxWidgets</b> application framework \
(<b>https://www.wxwidgets.org</b>). It is <i>Open Source</i>, has multi-platform support, it is \
ease to learn and extend, it has a helpful community, and also has the possibility \
to use it in commercial products without licencing.") +
        "</p></body></html>";

        m_pHtmlWindow->SetPage(sContent);
}

//---------------------------------------------------------------------------------------
void AboutDialog::OnBuildInfo(wxCommandEvent& WXUNUSED(event))
{
    //get info
    wxString sAGrillaNumbers = wxString::Format("[%d.%d.%s]",
                                           wxGetApp().get_version_major(),
                                           wxGetApp().get_version_minor(),
                                           wxGetApp().get_version_patch()
                                          );

    //Prepare build info message
    const wxLanguageInfo* pLanguage = wxLocale::GetLanguageInfo(wxLANGUAGE_DEFAULT);
    wxString sContent = m_sHeader +
        "<center>"
        "<h3>" + _("Build information") + "</h3></center><p>" +
        _("Program version:") + " " + wxGetApp().get_version_long_string() +
        " " + sAGrillaNumbers + "<br>" +
        _("Program build date:") + " " __DATE__ + ", " + __TIME__ + "<br>" +
        wxVERSION_STRING + "<br>" +
        _("Your computer information:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp; - " +
        _("Charset encoding: ") + wxLocale::GetSystemEncodingName() +
        "<br>&nbsp;&nbsp;&nbsp;&nbsp; - " +
        _("Canonical locale name: ") + pLanguage->CanonicalName +
        "</p></body></html>";

    m_pHtmlWindow->SetPage(sContent);

}

//---------------------------------------------------------------------------------------
void AboutDialog::OnDevelopers(wxCommandEvent& WXUNUSED(event))
{
    wxString sContent = m_sHeader +
    "<center>"
    "<p><font size='+2'><b>" + _("Contributors") + "</b></font><br />" +
    "</p></center><p>" +
    "<table border='0' width='100%' cellpadding='0' cellspacing='0'>"
        //
    "<tr><td colspan='3'><b>" +
        _("Project management:") + "</b></td></tr>"
    "  <tr><td width='40'>&nbsp;</td><td colspan='2'>Cecilio Salmer&oacute;n</td></tr>"
    "<tr><td colspan='3'>&nbsp;</td></tr>"
        //
    "</table><p>&nbsp;</p><p>" +
_("If you would like to help developing this program or join the project to help in any \
other way, you are indeed welcome; please, visit AGrilla at GitHub (<b>https://github.com/cecilios/agrilla/</b>) \
and open a new issue. Thank you and welcome.") +
    "</p></body></html>";

    m_pHtmlWindow->SetPage(sContent);
}


}   //namespace agrilla
