//---------------------------------------------------------------------------------------
// This file is part of the AGrilla application.
// Copyright (c) 2025-present, Cecilios Salmeron
//
// Licensed under the MIT license.
//
// See LICENSE file in the root directory of this source tree.
//---------------------------------------------------------------------------------------

//wxWidgets
#include "wx/wxprec.h"      //For compilers that support precompilation, includes "wx/wx.h".
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//agrilla
#include "DlgGridOptions.h"
#include "TheApp.h" //for get_preferences()

namespace agrilla
{

// IDs for the buttons
const int k_id_accept = ::wxNewId();
const int k_id_cancel = ::wxNewId();

//---------------------------------------------------------------------------------------
DlgGridOptions::DlgGridOptions(wxWindow* parent, int numGridSegments, int lineThickness,
                   const wxColour gridLinesColour, const wxColour goldenLinesColour,
                   const wxColour toolbarColour)
    : wxDialog(parent, wxID_ANY, _T("Grid Options"), wxDefaultPosition, wxDefaultSize,
               wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX)
{
    // Bind event handlers
    Bind(wxEVT_BUTTON, &DlgGridOptions::on_accept_button, this, k_id_accept);
    Bind(wxEVT_BUTTON, &DlgGridOptions::on_cancel_button, this, k_id_cancel);

    // Create the dialog UI
    create_dialog();

    // Set initial values
    m_numSegmentsCtrl->SetValue(numGridSegments);
    m_lineThicknessCtrl->SetValue(lineThickness);
    m_gridLineColorPicker->SetColour(gridLinesColour);
    m_goldenLineColorPicker->SetColour(goldenLinesColour);
    m_toolbarColorPicker->SetColour(toolbarColour);
}

//---------------------------------------------------------------------------------------
void DlgGridOptions::create_dialog()
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    this->SetExtraStyle(wxWS_EX_BLOCK_EVENTS);

    // The main sizer for the dialog
    wxBoxSizer* pMainSizer = new wxBoxSizer(wxVERTICAL);

    // Sizer for the grid options controls
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, wxSize(10, 10)); // 2 columns, 10x10 gaps
    gridSizer->AddGrowableCol(1); // Allow the second column (controls) to expand

    // Number of Segments
    wxStaticText* numSegmentsLabel = new wxStaticText(this, wxID_ANY, "Number of Segments:");
    m_numSegmentsCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString,
                                       wxDefaultPosition, wxDefaultSize,
                                       wxSP_ARROW_KEYS, 2, 100, 3);
    m_numSegmentsCtrl->SetToolTip("Sets the number of segments for the grid (e.g., 3 segments means 2 lines).");
    gridSizer->Add(numSegmentsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    gridSizer->Add(m_numSegmentsCtrl, 0, wxEXPAND | wxALL, 5);

    // Lines Thickness
    wxStaticText* lineThicknessLabel = new wxStaticText(this, wxID_ANY, "Line Thickness:");
    m_lineThicknessCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString,
                                         wxDefaultPosition, wxDefaultSize,
                                         wxSP_ARROW_KEYS, 1, 10, 1); // Thickness from 1 to 10 pixels
    m_lineThicknessCtrl->SetToolTip("Sets the thickness of the grid and golden lines in pixels.");
    gridSizer->Add(lineThicknessLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    gridSizer->Add(m_lineThicknessCtrl, 0, wxEXPAND | wxALL, 5);

    // Grid Line Color Picker
    wxStaticText* gridLineColorLabel = new wxStaticText(this, wxID_ANY, "Grid Line Color:");
    m_gridLineColorPicker = new wxColourPickerCtrl(this, wxID_ANY, *wxBLACK);
    m_gridLineColorPicker->SetToolTip("Choose the color for the grid lines.");
    gridSizer->Add(gridLineColorLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    gridSizer->Add(m_gridLineColorPicker, 0, wxEXPAND | wxALL, 5);

    // Golden Lines Color Picker
    wxStaticText* goldenLineColorLabel = new wxStaticText(this, wxID_ANY, "Golden Line Color:");
    m_goldenLineColorPicker = new wxColourPickerCtrl(this, wxID_ANY, wxColour(255, 215, 0)); // Default Gold color
    m_goldenLineColorPicker->SetToolTip("Choose the color for the golden ratio lines.");
    gridSizer->Add(goldenLineColorLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    gridSizer->Add(m_goldenLineColorPicker, 0, wxEXPAND | wxALL, 5);

    // Toolbar Color Picker
    wxStaticText* toolbarColorLabel = new wxStaticText(this, wxID_ANY, "Toolbar Color:");
    m_toolbarColorPicker = new wxColourPickerCtrl(this, wxID_ANY, *wxYELLOW);   //wxSystemSettings::GetColour(wxSYS_COLOUR_TOOLBAR)); // Default toolbar color
    m_toolbarColorPicker->SetToolTip("Choose the background color for the toolbar.");
    gridSizer->Add(toolbarColorLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    gridSizer->Add(m_toolbarColorPicker, 0, wxEXPAND | wxALL, 5);

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
    // Save the new grid options to member variables
    m_numGridSegments = m_numSegmentsCtrl->GetValue();
    m_lineThickness = m_lineThicknessCtrl->GetValue();
    m_gridLineColour = m_gridLineColorPicker->GetColour();
    m_goldenLineColour = m_goldenLineColorPicker->GetColour();
    m_toolbarColour = m_toolbarColorPicker->GetColour();

    EndDialog(wxID_OK);
}

//---------------------------------------------------------------------------------------
void DlgGridOptions::on_cancel_button(wxCommandEvent& WXUNUSED(event))
{
    EndDialog(wxID_CANCEL);
}

} // namespace agrilla
