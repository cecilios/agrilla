#include "ToolBar.h"
#include "MainFrame.h"
#include <wx/tooltip.h> // For tooltips
#include <wx/dcclient.h> // Required for wxPaintDC
#include <wx/brush.h>    // Required for wxBrush


namespace agrilla
{

//---------------------------------------------------------------------------------------
ToolBar::ToolBar(MainFrame* parent, wxWindowID id, int width, const wxSize& iconsSize)
    : wxPanel(parent, id, wxPoint(0,0), wxSize(width, 53), wxTAB_TRAVERSAL,
              wxPanelNameStr)
    , m_iconSize(iconsSize)
    , m_nextButtonX(0)
    , m_buttonPadding(5)
{
    //Tric to deal with the mouse. As MainFrame is a shaped frame, the mouse needs
    //to be captured and managed by MainFrame. Thus, mouse events are first
    //processed by MainFrame and, then, if not valid there, will be skipped to
    //arrive to the toolbar controls. Without this hack, mouse events are processed
    //by ToolBal and never arrive to MainFrame.
    Bind(wxEVT_MOTION, &MainFrame::on_mouse_motion, parent);
    Bind(wxEVT_LEFT_DOWN, &MainFrame::on_mouse_left_down, parent);
    Bind(wxEVT_LEFT_UP, &MainFrame::on_mouse_left_up, parent);
}

//---------------------------------------------------------------------------------------
void ToolBar::add_tool(wxWindowID id, const wxBitmapBundle& bitmap,
                       const wxString& tooltip)
{
    // Calculate button size including internal padding to make it square around the icon
    wxSize buttonSize(m_iconSize.GetWidth() + m_buttonPadding * 2, m_iconSize.GetHeight() + m_buttonPadding * 2);

    wxBitmapButton* button = new wxBitmapButton(this, id, bitmap,
                                                wxPoint(m_nextButtonX + m_buttonPadding, m_buttonPadding),
                                                buttonSize, wxBORDER_NONE);
    button->SetToolTip(tooltip);
    button->SetLabel(wxEmptyString);
    button->SetBackgroundColour(*wxYELLOW);
    button->Bind(wxEVT_BUTTON, &ToolBar::on_button_click, this);

    m_buttons.push_back(button);
    m_buttonMap[id] = button;
    m_toolNormalBitmaps[id] = bitmap;
    m_toolStates[id] = false; // Not a check tool, so state is always false

    // Update position for the next button
    m_nextButtonX += (buttonSize.GetWidth() + m_buttonPadding);
}

//---------------------------------------------------------------------------------------
void ToolBar::add_check_tool(wxWindowID id, const wxBitmapBundle& normalBitmap,
                             const wxBitmapBundle& checkedBitmap,
                             const wxString& normalTooltip, const wxString& checkedTooltip)
{
    wxSize buttonSize(m_iconSize.GetWidth() + m_buttonPadding * 2, m_iconSize.GetHeight() + m_buttonPadding * 2);

    wxBitmapButton* button = new wxBitmapButton(this, id, normalBitmap,
                                                wxPoint(m_nextButtonX + m_buttonPadding, m_buttonPadding),
                                                buttonSize, wxBORDER_NONE);
    button->SetToolTip(normalTooltip);
    button->SetLabel(wxEmptyString);
    button->SetBackgroundColour(*wxYELLOW);
    button->Bind(wxEVT_BUTTON, &ToolBar::on_button_click, this);

    m_buttons.push_back(button);
    m_buttonMap[id] = button;
    m_toolNormalBitmaps[id] = normalBitmap;
    m_toolCheckedBitmaps[id] = checkedBitmap;
    m_toolNormalTooltips[id] = normalTooltip;
    m_toolCheckedTooltips[id] = checkedTooltip;
    m_toolStates[id] = false; // Initial state is unchecked

    m_nextButtonX += (buttonSize.GetWidth() + m_buttonPadding);
}

//---------------------------------------------------------------------------------------
void ToolBar::set_tool_checked(wxWindowID id, bool checked)
{
    auto it = m_buttonMap.find(id);
    if (it != m_buttonMap.end())
    {
        wxBitmapButton* button = it->second;
        m_toolStates[id] = checked; // Update the internal state
        if (checked && m_toolCheckedBitmaps.count(id))
        {
            button->SetBitmapLabel(m_toolCheckedBitmaps[id]);
            button->SetToolTip(m_toolCheckedTooltips[id]);
        }
        else if (m_toolNormalBitmaps.count(id))
        {
            button->SetBitmapLabel(m_toolNormalBitmaps[id]);
            button->SetToolTip(m_toolNormalTooltips[id]);
        }
        button->Refresh(); // Redraw the button to show the new bitmap
    }
}

//---------------------------------------------------------------------------------------
wxRect ToolBar::get_move_handle() const
{
    //the rectangle after last button
    int height = m_iconSize.GetHeight() + m_buttonPadding * 4;
    int width = this->GetParent()->GetSize().GetWidth() - m_nextButtonX;
    return wxRect(m_nextButtonX, 0, width, height);
}

//---------------------------------------------------------------------------------------
bool ToolBar::is_tool_checked(wxWindowID id) const
{
    auto it = m_toolStates.find(id);
    if (it != m_toolStates.end())
    {
        return it->second;
    }
    return false; // Default for non-check tools or unknown ID
}

//---------------------------------------------------------------------------------------
wxSize ToolBar::get_size() const
{
    // Calculate the height based on icon size and padding:
    // Icon height + 2 * button internal padding + 2 * toolbar panel padding
    int height = m_iconSize.GetHeight() + m_buttonPadding * 4;

    // Calculate the width based on the accumulated x position of buttons
    // m_nextButtonX already includes the padding after the last button
    int width = m_nextButtonX + m_buttonPadding; // Add padding for the right edge of the toolbar

    // Ensure a minimum height if no buttons are added yet
    if (m_buttons.empty())
    {
        height = m_iconSize.GetHeight() + m_buttonPadding * 4; // A default height
        width = 0;
    }

    return wxSize(width, height);
}

//---------------------------------------------------------------------------------------
void ToolBar::on_button_click(wxCommandEvent& event)
{
    wxWindowID id = event.GetId();
    // If it's a check tool, toggle its state internally and update its bitmap
    if (m_toolCheckedBitmaps.count(id))
    {
        bool newCheckedState = !m_toolStates[id];
        set_tool_checked(id, newCheckedState);
        // For check tools, set the 'checked' state in the event object for the parent
        event.SetInt(newCheckedState ? 1 : 0);
    }
    // Propagate the event up to the parent (MainFrame)
    // The MainFrame will receive a wxEVT_BUTTON event for the tool's ID.
    // It can then query the toolbar's is_tool_checked(id) if it needs the state.
    GetParent()->GetEventHandler()->ProcessEvent(event);
}

} // namespace agrilla
