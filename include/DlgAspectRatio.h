//---------------------------------------------------------------------------------------
// This file is part of the AGrilla application.
// Copyright (c) 2025-present, Cecilio Salmeron
//
// Licensed under the MIT license.
//
// See LICENSE file in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#pragma once

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace agrilla
{


class DlgAspectRatio : public wxDialog
{
public:
    //---------------------------------------------------------------------------------------
    // DlgAspectRatio class constructor.
    DlgAspectRatio(wxWindow* parent, double currentAspectRatio);

    //---------------------------------------------------------------------------------------
    // Returns the final aspect ratio selected by the user.
    double get_aspect_ratio() const;

private:
    // Member variables for UI controls
    wxRadioButton* m_radioAspectRatio = nullptr;
    wxRadioButton* m_radioPaperSize = nullptr;
    wxTextCtrl* m_textCtrlAspectRatio = nullptr;
    wxTextCtrl* m_textCtrlWidth = nullptr;
    wxTextCtrl* m_textCtrlHeight = nullptr;
    wxStaticText* m_staticTextAspectRatioError = nullptr;
    wxStaticText* m_staticTextWidthError = nullptr;
    wxStaticText* m_staticTextHeightError = nullptr;
    wxButton* m_buttonAccept = nullptr;

    // Private member variables
    double m_aspectRatio = 1.0;
    double m_initialAspectRatio = 1.28;

    // Private event handler methods
    void on_init_dialog(wxInitDialogEvent& event);
    void on_radio_aspect_ratio_selected(wxCommandEvent& event);
    void on_radio_paper_size_selected(wxCommandEvent& event);
    void on_text_ctrl_updated(wxCommandEvent& event);
    void on_accept_button(wxCommandEvent& event);
    void on_cancel_button(wxCommandEvent& event);

    // Private helper methods
    void validate_input();
    bool parse_double(const wxString& text, double& value);
    void set_error_message(wxStaticText* staticText, const wxString& message);
};

} //namespace agrilla

