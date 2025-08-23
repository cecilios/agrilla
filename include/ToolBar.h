//---------------------------------------------------------------------------------------
// This file is part of the AGrilla application.
// Copyright (c) 2025-present, Cecilios Salmeron
//
// Licensed under the MIT license.
//
// See LICENSE file in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#pragma once

#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/bmpbuttn.h>
#include <wx/vector.h>
#include <wx/event.h>
#include <map>

namespace agrilla
{

class MainFrame;

//=======================================================================================
//In Linux with GTK I've found no simple approach to change the background color of a
//wxToolBar. Probably this is due to the way the underlying native toolkit handles
//drawing. The wxToolBar probably uses the system's default theme settings for its
//appearance and can not be overriden directly.
//Another problem is that as dgrig is a borderless frame, it could be difficult for the
//user to see the background of the toolbar on the screen. It depends very much on
//the system's theme settings,
//So it has been decided to switch to a generic, custom-drawn toobar to have full
//control over the toolbar and also to maintain the same appearance across platforms.
//---------------------------------------------------------------------------------------
class ToolBar : public wxPanel
{
public:
    ToolBar(MainFrame* parent, wxWindowID id, int width, const wxSize& iconsSize,
            const wxColour bgColour);

    //building
    void add_tool(wxWindowID id, const wxBitmapBundle& bitmap, const wxString& tooltip);
    void add_check_tool(wxWindowID id, const wxBitmapBundle& normalBitmap,
                        const wxBitmapBundle& checkedBitmap,
                        const wxString& normalTooltip, const wxString& checkedTooltip);
    void change_colour(const wxColour bgColour);

    //accessors for information
    void set_tool_checked(wxWindowID id, bool checked);
    bool is_tool_checked(wxWindowID id) const;
    wxSize get_size() const;
    wxRect get_move_handle() const;

private:
    void on_button_click(wxCommandEvent& event);

    wxSize m_iconSize;
    wxColour m_bgColour;
    wxVector<wxBitmapButton*> m_buttons;
    std::map<wxWindowID, wxBitmapButton*> m_buttonMap; // Map ID to button pointer for quick lookup
    std::map<wxWindowID, wxBitmapBundle> m_toolNormalBitmaps;
    std::map<wxWindowID, wxBitmapBundle> m_toolCheckedBitmaps;
    std::map<wxWindowID, wxString> m_toolNormalTooltips;
    std::map<wxWindowID, wxString> m_toolCheckedTooltips;
    std::map<wxWindowID, bool> m_toolStates; // Stores the checked state for check tools
    int m_nextButtonX;      //x position for next button to add. Does not include padding
    int m_buttonPadding;     //padding around buttons and between them
};


} // namespace agrilla
