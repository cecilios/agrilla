
//wxWidgets
#include "wx/wxprec.h"      //For compilers that support precompilation, includes "wx/wx.h".
#ifndef WX_PRECOMP
    #include "wx/wx.h"
    //#include <wx/string.h>
    //#include <wx/stattext.h>
    //#include <wx/gdicmn.h>
    //#include <wx/font.h>
    //#include <wx/colour.h>
    //#include <wx/settings.h>
    //#include <wx/textctrl.h>
    //#include <wx/button.h>
    //#include <wx/sizer.h>
    //#include <wx/panel.h>
    //#include <wx/bitmap.h>
    //#include <wx/image.h>
    //#include <wx/icon.h>
    //#include <wx/dialog.h>
    //#include <wx/config.h> // The explicit fix for wxConfigBase
#endif


//agrilla
#include "DlgGridOptions.h"
#include "TheApp.h"


namespace agrilla
{

//extern wxConfigBase* g_pPrefs;      //the config object

// IDs for the buttons
const int k_id_accept = ::wxNewId();
const int k_id_cancel = ::wxNewId();


BEGIN_EVENT_TABLE(DlgGridOptions, wxDialog)
    EVT_BUTTON(k_id_accept, DlgGridOptions::on_accept_button )
    EVT_BUTTON(k_id_cancel, DlgGridOptions::on_cancel_button )
END_EVENT_TABLE()

//---------------------------------------------------------------------------------------
DlgGridOptions::DlgGridOptions(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, _T("Grid Options"), wxDefaultPosition, wxDefaultSize,
               wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX)
{
    create_dialog();

    // Get current values for the grid options from the configuration
    wxConfigBase* pPrefs =  wxGetApp().get_config();
    m_numGridSegments = pPrefs->Read("/Grid/Segments", 3L);
    wxString sColour("#FFFFFF");
    pPrefs->Read("/Grid/Color", &sColour, "#FFFFFF");
    m_gridColour.Set(sColour);

    // Set initial values on the controls
    m_numSegmentsCtrl->SetValue(m_numGridSegments);
    m_lineColorPicker->SetColour(m_gridColour);
}

//---------------------------------------------------------------------------------------
void DlgGridOptions::create_dialog()
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    this->SetExtraStyle(wxWS_EX_BLOCK_EVENTS);

    // The main sizer for the dialog
    wxBoxSizer* pMainSizer = new wxBoxSizer(wxVERTICAL);

    // Sizer for the grid options controls
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, wxSize(10, 10));

    // Number of Segments
    wxStaticText* numSegmentsLabel = new wxStaticText(this, wxID_ANY, "Number of Segments:");
    m_numSegmentsCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString,
                                       wxDefaultPosition, wxDefaultSize,
                                       wxSP_ARROW_KEYS, 2, 100, 3);
    m_numSegmentsCtrl->SetToolTip("Sets the number of segments for the grid (e.g., 3 segments means 2 lines).");
    gridSizer->Add(numSegmentsLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    gridSizer->Add(m_numSegmentsCtrl, 0, wxEXPAND);

    // Line Color Picker
    wxStaticText* lineColorLabel = new wxStaticText(this, wxID_ANY, "Line Color:");
    m_lineColorPicker = new wxColourPickerCtrl(this, wxID_ANY, *wxBLACK);
    m_lineColorPicker->SetToolTip("Choose the color for the grid lines.");
    gridSizer->Add(lineColorLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    gridSizer->Add(m_lineColorPicker, 0, wxEXPAND);

    pMainSizer->Add(gridSizer, 0, wxALL | wxEXPAND, 15);

    // Buttons
    wxBoxSizer* pButtonsSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* pBtAccept = new wxButton(this, k_id_accept, wxT("Accept"), wxDefaultPosition, wxDefaultSize, 0);
    pBtAccept->SetDefault();
    pButtonsSizer->Add(pBtAccept, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxButton* pBtCancel = new wxButton(this, k_id_cancel, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
    pButtonsSizer->Add(pBtCancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    pMainSizer->Add(pButtonsSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    this->SetSizer(pMainSizer);
    this->Layout();
    pMainSizer->Fit(this);
    this->Centre(wxBOTH);
}

//---------------------------------------------------------------------------------------
void DlgGridOptions::on_accept_button(wxCommandEvent& WXUNUSED(event))
{
    // Save the new grid options
    m_numGridSegments = m_numSegmentsCtrl->GetValue();
    m_gridColour = m_lineColorPicker->GetColour();

    // Save the new grid options
    wxConfigBase* pPrefs =  wxGetApp().get_config();
    pPrefs->Write("/Grid/Segments", m_numSegmentsCtrl->GetValue());
    pPrefs->Write("/Grid/Color", m_lineColorPicker->GetColour().GetAsString(wxC2S_HTML_SYNTAX));

    EndDialog(wxID_OK);
}

//---------------------------------------------------------------------------------------
void DlgGridOptions::on_cancel_button(wxCommandEvent& WXUNUSED(event))
{
    EndDialog(wxID_CANCEL);
}

} // namespace agrilla
