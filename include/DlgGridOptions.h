//---------------------------------------------------------------------------------------
// This file is part of the AGrilla application.
// Copyright (c) 2025-present, Cecilio Salmeron
//
// Licensed under the MIT license.
//
// See LICENSE file in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#pragma once

#include <wx/dialog.h>
#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/clrpicker.h>
#include <wx/spinctrl.h>
#include <wx/config.h>


namespace agrilla
{


class DlgGridOptions : public wxDialog
{
public:
    DlgGridOptions(wxWindow* parent, int gridSegments, int lineThickness,
                   const wxColour gridLinesColour, const wxColour goldenLinesColour,
                   const wxColour toolbarColour, const wxColour frameColour);

    int get_segments() { return m_numGridSegments; }
    int get_line_thickness() { return m_lineThickness; }
    wxColor& get_grid_line_color() { return m_gridLineColour; }
    wxColor& get_golden_line_color() { return m_goldenLineColour; }
    wxColor& get_toolbar_color() { return m_toolbarColour; }
    wxColor& get_frame_color() { return m_frameColour; }

private:
    // UI controls
    wxSpinCtrl* m_numSegmentsCtrl;
    wxSpinCtrl* m_lineThicknessCtrl;
    wxColourPickerCtrl* m_gridLineColorPicker;
    wxColourPickerCtrl* m_goldenLineColorPicker;
    wxColourPickerCtrl* m_toolbarColorPicker;
    wxColourPickerCtrl* m_frameColorPicker;

    // Internal data members
    long        m_numGridSegments;
    wxColour    m_gridLineColour;
    wxColor     m_goldenLineColour;
    int         m_lineThickness;
    wxColour    m_toolbarColour;
    wxColour    m_frameColour;

    // Private methods
    void create_dialog();

    // Event handlers
    void on_accept_button(wxCommandEvent& event);
    void on_cancel_button(wxCommandEvent& event);
};


} // namespace agrilla
