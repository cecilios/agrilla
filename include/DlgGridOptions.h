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
    // Constructor
    DlgGridOptions(wxWindow* parent);

    int get_segments() { return m_numGridSegments; }
    wxColor& get_color() {return m_gridColour; }


private:
    // UI controls
    wxSpinCtrl* m_numSegmentsCtrl;
    wxColourPickerCtrl* m_lineColorPicker;

    // Internal data members
    long        m_numGridSegments;
    wxColour    m_gridColour;

    // Private methods
    void create_dialog();

    // Event handlers
    void on_accept_button(wxCommandEvent& event);
    void on_cancel_button(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

} // namespace agrilla

