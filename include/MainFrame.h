//---------------------------------------------------------------------------------------
// This file is part of the AGrilla application.
// Copyright (c) 2025-present, Cecilio Salmeron
//
// Licensed under the MIT license.
//
// See LICENSE file in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#pragma once

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


namespace agrilla
{

class ToolBar;

// Enum to define which part of the border is being resized
enum class ResizeDirection
{
    NONE,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
};

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxSize& initialSize = wxSize(400, 300));

    //public event handlres so that ToolBar can bind events to this MainFrame
    void on_mouse_left_down(wxMouseEvent& event);
    void on_mouse_motion(wxMouseEvent& event);
    void on_mouse_left_up(wxMouseEvent& event);

private:

    // Event handlers
    void on_paint(wxPaintEvent& event);
    void on_quit(wxCommandEvent &event);
    void on_tool_grid_options(wxCommandEvent& event);
    void on_tool_set_aspect_ratio(wxCommandEvent& event);
    void on_tool_lock_aspect_ratio(wxCommandEvent& event);
    void on_tool_show_grid(wxCommandEvent& event);
    void on_tool_show_golden_lines(wxCommandEvent& event);
    void on_tool_show_frame(wxCommandEvent& event);
    void on_about(wxCommandEvent& WXUNUSED(event));

    //helpers for building
    void create_toolbar();
    void create_shaped_frame();

    //helpers for drawing
    void draw_all_content();
    void draw_grid_lines(wxDC& dc);
    void draw_golden_lines(wxDC& dc);
    void draw_border(wxDC& dc);
    void draw_resize_handlers(wxDC& dc);

    //helpers, to manage options
    void get_grid_options();
    void change_and_lock_aspect_ratio(const double aspectRatio);
    void lock_aspect_ratio(const bool value);

    //helpers, for window resizing
    void resize_window_left_mouse_down(wxMouseEvent& event);
    void resize_window_mouse_motion(wxMouseEvent& event);
    void resize_window_left_mouse_up(wxMouseEvent& event);
    ResizeDirection determine_resize_direction(const wxPoint& mousePos);

    //other helpers
    void compute_aspect_ratio();
    void change_black_colours();

private:
    //GUI layout
    bool        m_fBitmapIsInvalid = true;
    wxBitmap    m_bmpMask;               //the image that will define the opaque regions
    ToolBar*    m_toolbar = nullptr;
    wxColour    m_toolbarColour;
    int m_toolbarHeight = 53;
    wxSize m_toolbarSize;
    wxRect m_clientRect;
    wxRect m_gridRect;

    // for moving the window
    bool m_fMoveMode = false;
    wxPoint m_moveStartPos;  // Mouse position when drag started (relative to frame client area)
    wxPoint m_frameStartPos; // Frame position when drag started (screen coordinates)
    wxRect m_moveHandle;     // The rectangle within the frame that acts as a drag handle

    //handlers for resizing
    int m_handlerSide = 15;
    bool m_fDrawHandlers = true;
    wxRect m_leftHandle;
    wxRect m_rightHandle;
    wxRect m_topHandle;
    wxRect m_bottomHandle;

    // Grid properties
    bool m_fDrawGrid = true;
    int m_gridSize;
    int m_gridLineThickness;
    wxColour m_gridLinesColour;

    // Golden lines
    bool m_fDrawGoldenLines = true;
    wxColour m_goldenLinesColour;

    // Frame around the grid
    bool m_fDrawFrame = false;
    int  m_frameThickness = 40;
    wxColour m_frameColour = wxColour("#2f2f2f");

    // Aspect Ratio lock properties
    double m_aspectRatio = 1.0;          //defined as Width / Height
    bool m_fAspectRatioLocked = false;

    // resize state
    bool m_fMouseCaptured = false;
    bool m_fResizingMode = false;
    ResizeDirection m_resizeDirection = ResizeDirection::NONE; // Which border is currently being dragged
    wxSize m_resizeStartFrameSize;           // Frame size when resize starts

};

} // namespace agrilla
