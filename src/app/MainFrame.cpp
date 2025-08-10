// For compilers that support precompilation, includes "wx/wxprec.h".
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//wxWidgets
#include <wx/log.h> // For logging messages
#include <wx/graphics.h>
#include <wx/dcclient.h>
#include <wx/log.h>
#include <wx/msgdlg.h>


//agrilla
#include "MainFrame.h"
#include "MainFrame.h"
#include "TheApp.h"
#include "DlgGridOptions.h"
#include "DlgAspectRatio.h"
#include "ToolBar.h"

//std
#include <cmath> // For std::abs
#include <memory>


namespace agrilla
{

const int MIN_CLIENT_DIM = 20; // Minimum client dimension
const double GOLDEN_RATIO = 1.618033988749;

enum
{
    // special IDs
    //wxID_ABOUT and wxID_EXIT are predefined by wxWidgets and have a special meaning
    //since entries using these IDs will be taken out of the normal menus under MacOS X
    //and will be inserted into the system menu (following the appropriate MacOS X
    //interface guideline).
    k_menu_file_quit = wxID_EXIT,
    k_menu_help_about = wxID_ABOUT,

    //tools
    k_evt_grid_options = wxID_HIGHEST + 1,
    k_evt_set_aspect_ratio,
    k_evt_lock_aspect_ratio,
    k_evt_show_grid,
    k_evt_show_golden_lines,
    k_evt_quit,


    //other
    k_id_toolbar,

};


//=======================================================================================
// MainFrame implementation
//=======================================================================================
MainFrame::MainFrame(const wxSize& initialSize)
    : wxFrame(nullptr, wxID_ANY, "AGrilla", wxDefaultPosition, initialSize
    , wxFRAME_SHAPED | wxCLIP_CHILDREN | wxBORDER_NONE | wxSTAY_ON_TOP)
{
    get_grid_options();
    create_shaped_frame();
    create_toolbar();

    //bind the events
    Bind(wxEVT_PAINT, &MainFrame::on_paint, this);
    Bind(wxEVT_SIZE, &MainFrame::on_size, this);
    Bind(wxEVT_LEFT_DOWN, &MainFrame::on_mouse_left_down, this);
    Bind(wxEVT_MOTION, &MainFrame::on_mouse_motion, this);
    Bind(wxEVT_LEFT_UP, &MainFrame::on_mouse_left_up, this);
    Bind(wxEVT_BUTTON, &MainFrame::on_quit, this, k_evt_quit);


    Refresh();      //good practice to force an initial paint after setup
}

//---------------------------------------------------------------------------------------
void MainFrame::create_toolbar()
{
    // If m_toolbar was previously created, ensure it's deleted before re-creating.
    if (m_toolbar)
    {
        m_toolbar->Destroy();
        m_toolbar = nullptr;
    }

    // Define indices for the bitmap vector.
    enum BitmapIndices
    {
        k_bmp_grid_options = 0,
        k_bmp_set_aspect_ratio,
        k_bmp_unlocked_aspect_ratio,
        k_bmp_locked_aspect_ratio,
        k_bmp_show_grid,
        k_bmp_show_golden_lines,
        k_bmp_quit,
        //
        k_bmp_max
    };

    // Create a bitmap bundle from the SVG file, with a 32x32 reference size
    wxString sResPath = wxGetApp().get_resources_path();
    wxLogMessage("[MainFrame::create_toolbar] icons path '%s'", sResPath);
    wxSize iconsSize(32,32);
    wxVector<wxBitmapBundle> bitmaps(k_bmp_max);
    bitmaps[k_bmp_grid_options] = wxBitmapBundle::FromSVGFile(sResPath + "options.svg", iconsSize);
    bitmaps[k_bmp_set_aspect_ratio] = wxBitmapBundle::FromSVGFile(sResPath + "ZoomPrintSize2.svg", iconsSize);
    bitmaps[k_bmp_quit] = wxBitmapBundle::FromSVGFile(sResPath + "shutdown.svg", iconsSize);
    bitmaps[k_bmp_unlocked_aspect_ratio]= wxBitmapBundle::FromSVGFile(sResPath + "Next.svg", iconsSize);
    bitmaps[k_bmp_locked_aspect_ratio]= wxBitmapBundle::FromSVGFile(sResPath + "lock-screen.svg", iconsSize);
    bitmaps[k_bmp_show_grid]= wxBitmapBundle::FromSVGFile(sResPath + "grid.svg", iconsSize);
    bitmaps[k_bmp_show_golden_lines]= wxBitmapBundle::FromSVGFile(sResPath + "ZoomFitScreen.svg", iconsSize);

    // Create the custom toolbar panel
    m_toolbar = new ToolBar(this, k_id_toolbar, GetClientSize().GetWidth(), iconsSize);

    // Add tools using the custom toolbar's methods
    m_toolbar->add_tool(k_evt_grid_options, bitmaps[k_bmp_grid_options], "Grid options");
    m_toolbar->add_tool(k_evt_set_aspect_ratio, bitmaps[k_bmp_set_aspect_ratio], "Change aspect ratio");
    m_toolbar->add_check_tool(k_evt_lock_aspect_ratio,
                            bitmaps[k_bmp_unlocked_aspect_ratio],   // Normal state icon
                            bitmaps[k_bmp_locked_aspect_ratio],     // Checked state icon
                            "Lock aspect ratio",                    // Normal state tooltip
                            "Unlock the aspect ratio");             // Checked state tooltip
    m_toolbar->add_check_tool(k_evt_show_grid,
                            bitmaps[k_bmp_show_grid],   // Normal state icon
                            bitmaps[k_bmp_show_grid],   // Checked state icon (same as normal for this case)
                            "Hide grid",                // Normal state tooltip
                            "Show grid");               // Checked state tooltip
    m_toolbar->add_check_tool(k_evt_show_golden_lines,
                            bitmaps[k_bmp_show_golden_lines],   // Normal state icon
                            bitmaps[k_bmp_show_golden_lines],   // Checked state icon
                            "Hide golden lines",                // Normal state tooltip
                            "Show golden lines");               // Checked state tooltip
    m_toolbar->add_tool(k_evt_quit, bitmaps[k_bmp_quit], "Quit");

    //tools
    Bind(wxEVT_BUTTON, &MainFrame::on_quit, this, k_evt_quit);
    Bind(wxEVT_BUTTON, &MainFrame::on_tool_grid_options, this, k_evt_grid_options);
    Bind(wxEVT_BUTTON, &MainFrame::on_tool_set_aspect_ratio, this, k_evt_set_aspect_ratio);
    Bind(wxEVT_BUTTON, &MainFrame::on_tool_lock_aspect_ratio, this, k_evt_lock_aspect_ratio);
    Bind(wxEVT_BUTTON, &MainFrame::on_tool_show_grid, this, k_evt_show_grid);
    Bind(wxEVT_BUTTON, &MainFrame::on_tool_show_golden_lines, this, k_evt_show_golden_lines);

    m_toolbarHeight = m_toolbar->get_size().GetHeight();

    //initial state
    m_toolbar->set_tool_checked(k_evt_show_grid, true);
    m_toolbar->set_tool_checked(k_evt_show_golden_lines, true);

}

//---------------------------------------------------------------------------------------
void MainFrame::create_shaped_frame()
{
    //prepare the bitmap to draw on the screen.
    draw_all_content();

    //create the wxRegion that will define the frame shape. Set black as the
    //colour for pixels that will define the mask for the transparent region
    wxRegion rgn(m_bmpMask, *wxBLACK);

    //apply the region to the frame to set its shape.
    if (!SetShape(rgn))
    {
        wxLogError("[create_shaped_frame] Failed to set shape. The window will not be shaped.");
    }
}

//---------------------------------------------------------------------------------------
void MainFrame::on_size(wxSizeEvent& WXUNUSED(event))
{
//    // Ensure the toolbar exists before trying to resize it
//    if (m_toolbar)
//    {
//        wxSize clientSize = GetClientSize();
//        // Set the toolbar's size to occupy the full width and its fixed height
//        m_toolbar->SetSize(0, 0, clientSize.GetWidth(), m_toolbarHeight);
//
//        // IMPORTANT: If you have other panels or controls below the toolbar,
//        // you must also explicitly resize and reposition them here.
//        // For example, if you have a main content panel named 'm_contentPanel':
//        // if (m_contentPanel)
//        // {
//        //     m_contentPanel->SetSize(0, m_toolbarHeight,
//        //                             clientSize.GetWidth(), clientSize.GetHeight() - m_toolbarHeight);
//        // }
//    }
//
//    // Call the base class event handler to allow default processing
//    event.Skip();
}

//---------------------------------------------------------------------------------------
void MainFrame::on_paint(wxPaintEvent& WXUNUSED(event))
{
    wxSize size = GetClientSize();
    if (m_fBitmapIsInvalid || m_bmpMask.GetWidth() != size.GetWidth()
        || m_bmpMask.GetHeight() != size.GetHeight() )
    {
        create_shaped_frame();
    }

    if (m_bmpMask.IsOk())
    {
        wxPaintDC dc(this);
        dc.DrawBitmap(m_bmpMask, 0, 0, false); // `false` to ignore transparency
    }
}

//---------------------------------------------------------------------------------------
void MainFrame::on_quit(wxCommandEvent& WXUNUSED(event))
{
    wxConfigBase* pPrefs =  wxGetApp().get_preferences();
    pPrefs->Write("/Size/Ratio", m_aspectRatio);
    pPrefs->Write("/Size/Locked", m_fAspectRatioLocked);
    pPrefs->Write("/Grid/Segments", m_gridSize);

    wxString sColour = m_gridColour.GetAsString();
    pPrefs->Write("/Grid/Color", sColour);

    Close(true);
}

//---------------------------------------------------------------------------------------
void MainFrame::draw_all_content()
{
    // Create a new bitmap of the specified window size and draw all content on it.
    //AWARE: Black regions will be transparent so do not use black colour in
    //       the image. BLACK COLOUR WILL BE TRANSPARENT!

    wxSize size = GetClientSize();

    m_bmpMask = wxBitmap(size);
    if (!m_bmpMask.IsOk())
    {
        wxLogError("[MainFrame::draw_all_content] Failed to create mask bitmap.");
        return;
    }

    // Create a memory device context and select the bitmap into it.
    wxMemoryDC dc;
    dc.SelectObject(m_bmpMask);

    // Set the background color to black (all window will be transparent).
    dc.SetBackground(*wxBLACK);
    dc.Clear();

    // Draw the mid-grey rectangle at the top, for the toolbar.
    dc.SetBrush(wxBrush(*wxYELLOW));    //wxColour(128, 128, 128)));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(0, 0, size.GetWidth(), m_toolbarHeight);

    // Define the bottom rectangle where the grid will be drawn.
    m_clientRect = wxRect(0, m_toolbarHeight, size.GetWidth(), size.GetHeight() - m_toolbarHeight);

    //The grid is small than the client rectangle as somo space must be placed aroun to
    //fit the resizin handlers
    int halfHandle = (m_handlerSide - m_gridLineThickness) / 2;
    m_gridRect = wxRect(0 + halfHandle, m_toolbarHeight + halfHandle,
                        m_clientRect.GetWidth() - m_handlerSide,
                        m_clientRect.GetHeight() - m_handlerSide);
    m_gridRect = m_clientRect;

//    wxLogMessage("[MainFrame::draw_all_content] client=(%d,%d,%d,%d), grid=(%d,%d,%d,%d)",
//        m_clientRect.x, m_clientRect.y, m_clientRect.width, m_clientRect.height,
//        m_gridRect.x, m_gridRect.y, m_gridRect.width, m_gridRect.height );

    //Draw all content
    draw_grid_lines(dc);
    draw_golden_lines(dc);
    draw_border(dc);
    draw_resize_handlers(dc);

    // Deselect the bitmap from the device context.
    dc.SelectObject(wxNullBitmap);
    m_fBitmapIsInvalid = false;

//    wxLogMessage("[MainFrame::draw_all_content] Bitmap recreated.");


//    // Save the bitmap as mask2.png.
//    if (!m_bmpMask.SaveFile("mask.png", wxBITMAP_TYPE_PNG))
//    {
//        wxLogError("[MainFrame::draw_all_content] Failed to save 'mask.png'.");
//    }
}

//---------------------------------------------------------------------------------------
void MainFrame::get_grid_options()
{
    wxConfigBase* pPrefs =  wxGetApp().get_preferences();
    m_gridSize = pPrefs->Read("/Grid/Segments", 3L);

    wxString sColor("#FFFFFF");
    pPrefs->Read("/Grid/Color", &sColor, "#FFFFFF");

    //Don't allow black as it will transformed into transparent
    if (sColor == "#000000")
        sColor = "#000005";

    m_gridColour.Set(sColor);
}

//---------------------------------------------------------------------------------------
void MainFrame::draw_border(wxDC& dc)
{
    // Draw a white border around the grid bitmap.
    // We use a transparent brush so it doesn't fill the rectangle.

    wxPen whitePen(*wxWHITE, m_gridLineThickness);
    dc.SetPen(whitePen);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(m_gridRect);
}

//---------------------------------------------------------------------------------------
void MainFrame::draw_grid_lines(wxDC& dc)
{
    if (m_fDrawGrid && m_gridSize > 1)
    {
        int width = m_gridRect.GetWidth();
        int height = m_gridRect.GetHeight();
        int left = m_gridRect.GetLeft();
        int top = m_gridRect.GetTop();

        wxPen gridPen(m_gridColour, m_gridLineThickness, wxPENSTYLE_SOLID);
        dc.SetPen(gridPen);

        // Draw vertical lines
        for (int i = 1; i < m_gridSize; ++i)
        {
            int x = ((width * i) / m_gridSize) + left;
            dc.DrawLine(x, top, x, height + top);
       }
        // Draw horizontal lines
        for (int i = 1; i < m_gridSize; ++i)
        {
            int y = ((height * i) / m_gridSize) + top;
            dc.DrawLine(left, y, width + left, y);
        }
    }
}

//---------------------------------------------------------------------------------------
void MainFrame::draw_golden_lines(wxDC& dc)
{
    if (m_fDrawGoldenLines)
    {
        int width = m_gridRect.GetWidth();
        int height = m_gridRect.GetHeight();
        int left = m_gridRect.GetLeft();
        int top = m_gridRect.GetTop();

        m_goldenLinesColour = *wxRED;
        wxPen goldenPen(m_goldenLinesColour, m_goldenLinesThickness, wxPENSTYLE_SOLID);
        dc.SetPen(goldenPen);

        //calculate golden ratio segments for width and height
        int golden_width_b = static_cast<int>(std::round(width / GOLDEN_RATIO));
        int golden_height_b = static_cast<int>(std::round(height / GOLDEN_RATIO));

        //vertical golden lines
        dc.DrawLine(golden_width_b + left, top, golden_width_b + left, height + top);
        dc.DrawLine(left + width - golden_width_b, top, left + width - golden_width_b, height + top);

        //horizontal golden lines
        dc.DrawLine(left, golden_height_b + top, width + left, golden_height_b + top);
        dc.DrawLine(left, top + height - golden_height_b, width + left, top + height - golden_height_b);
    }
}

//---------------------------------------------------------------------------------------
void MainFrame::draw_resize_handlers(wxDC& dc)
{
    if (m_fDrawHandlers)
    {
        int halfHandle = (m_handlerSide - m_gridLineThickness) / 2;
        wxPen borderPen(wxColour(0,0,5), 2);    //nearly black, black cannot be used
        dc.SetBrush(*wxWHITE);
        dc.SetPen(borderPen);

        //right handle
        m_rightHandle = wxRect(m_gridRect.GetRight() - halfHandle,
                               m_gridRect.GetHeight() / 2 + m_toolbarHeight,
                               m_handlerSide, m_handlerSide);
        dc.DrawRectangle(m_rightHandle);


        //left handle
        m_leftHandle  = wxRect(m_gridRect.GetLeft() - halfHandle,
                               m_gridRect.GetHeight() / 2 + m_toolbarHeight,
                               m_handlerSide, m_handlerSide);
        dc.DrawRectangle(m_leftHandle);

        //top handle
        m_topHandle = wxRect(m_gridRect.GetWidth() / 2,
                               m_gridRect.GetTop() - halfHandle,
                               m_handlerSide, m_handlerSide);
        dc.DrawRectangle(m_topHandle);

        //bottom handle
        m_bottomHandle = wxRect(m_gridRect.GetWidth() / 2,
                               m_gridRect.GetBottom() - halfHandle,
                               m_handlerSide, m_handlerSide);
        dc.DrawRectangle(m_bottomHandle);


//        //adjust rectangles to compare with mouse position
//        m_clientRect.SetTop( m_clientRect.GetTop() - m_toolbarHeight);
//        m_rightHandle.SetTop( m_rightHandle.GetTop() - m_toolbarHeight);
//        m_bottomHandle.SetTop( m_bottomHandle.GetTop() - m_toolbarHeight);
//        m_leftHandle.SetTop( m_leftHandle.GetTop() - m_toolbarHeight);
//        m_topHandle.SetTop( m_topHandle.GetTop() - m_toolbarHeight);
    }
}

//---------------------------------------------------------------------------------------
void MainFrame::change_and_lock_aspect_ratio(const double newAspectRatio)
{
    // Updates the aspect ratio and resizes the window to fit without enlarging.
    // Aspect ratio is locked after invoking this method.

    // Set the new aspect ratio and lock the resizing
    m_aspectRatio = newAspectRatio;
    m_fAspectRatioLocked = true;
    m_toolbar->set_tool_checked(k_evt_lock_aspect_ratio, true);

    // Get the current client size
    wxSize clientSize = GetClientSize();

    // Calculate new dimensions based on the current size and the new aspect ratio
    int newWidthFromHeight = static_cast<int>(clientSize.GetHeight() * m_aspectRatio);
    int newHeightFromWidth = static_cast<int>(clientSize.GetWidth() / m_aspectRatio);

    // Choose the new size that results in a smaller or equal window size
    if (newWidthFromHeight <= clientSize.GetWidth())
    {
        SetClientSize(newWidthFromHeight, clientSize.GetHeight() + m_toolbarHeight);
    }
    else
    {
        SetClientSize(clientSize.GetWidth(), newHeightFromWidth + m_toolbarHeight);
    }
}

//---------------------------------------------------------------------------------------
void MainFrame::lock_aspect_ratio(const bool value)
{
    if (!m_fAspectRatioLocked && value)
    {
        wxSize clientSize = GetClientSize();
        m_aspectRatio = (double)clientSize.GetWidth() / (double)clientSize.GetHeight();
    }
    m_fAspectRatioLocked = value;
}

//---------------------------------------------------------------------------------------
ResizeDirection MainFrame::determine_resize_direction(const wxPoint& mousePos)
{
    if (m_rightHandle.Contains(mousePos))
    {
        return ResizeDirection::RIGHT;
    }
    else if (m_leftHandle.Contains(mousePos))
    {
        return ResizeDirection::LEFT;
    }
    else if (m_topHandle.Contains(mousePos))
    {
        return ResizeDirection::TOP;
    }
    else if (m_bottomHandle.Contains(mousePos))
    {
        return ResizeDirection::BOTTOM;
    }

    return ResizeDirection::NONE;
}

//---------------------------------------------------------------------------------------
void MainFrame::on_mouse_left_down(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition(); // Mouse position relative to frame client area
    m_moveStartPos = ClientToScreen(pos);
    m_frameStartPos = GetPosition();

    m_moveHandle = m_toolbar->get_move_handle();
    if (m_moveHandle.Contains(pos)) // Check if click is within the move handle area
    {
        m_fMoveMode = true;
        m_moveStartPos = ClientToScreen(pos);
        m_frameStartPos = GetPosition();
//        wxLogMessage("Move handle clicked. Starting moving the window");
        SetCursor(wxCursor(wxCURSOR_CROSS));
    }
    else
    {
        m_resizeDirection = determine_resize_direction(event.GetPosition());
        if (m_resizeDirection != ResizeDirection::NONE)
        {
            resize_window_left_mouse_down(event);
        }
    }
    event.Skip();
}

//---------------------------------------------------------------------------------------
void MainFrame::on_mouse_motion(wxMouseEvent& event)
{
    //mose pos is relative to m_clientRect origin. so negative vules are out of it,
    //no top, left, and positive values grater than m_clientRect size are also outside.
    wxPoint mousePos = event.GetPosition();
//    wxLogMessage("[MainFrame::on_mouse_motion] Mouse=(%d,%d), right handle=(%d,%d, %d, %d)"
//                 ", Client=(%d, %d, %d, %d), grid==(%d, %d, %d, %d)",
//                 mousePos.x, mousePos.y,
//                 m_rightHandle.x, m_rightHandle.y, m_rightHandle.width, m_rightHandle.height,
//                 m_clientRect.x, m_clientRect.y, m_clientRect.width, m_clientRect.height,
//                 m_gridRect.x, m_gridRect.y, m_gridRect.width, m_gridRect.height);

    //handle capture/release mouse and mouse icon
    if (!m_fResizingMode && !m_fMoveMode)
    {
        bool fMouseInside = m_gridRect.Contains(mousePos);
        if (fMouseInside && !m_fMouseCaptured)
        {
            CaptureMouse();
//            wxLogMessage("[MainFrame::on_mouse_motion] Mouse captured");
            m_fMouseCaptured = true;
        }
        else if (fMouseInside && m_fMouseCaptured)
        {
            if (m_rightHandle.Contains(mousePos) || m_leftHandle.Contains(mousePos))
            {
                SetCursor(wxCursor(wxCURSOR_SIZEWE));
            }
            else if (m_topHandle.Contains(mousePos) || m_bottomHandle.Contains(mousePos))
            {
                SetCursor(wxCursor(wxCURSOR_SIZENS));
            }
            else
            {
                SetCursor(wxCursor(wxCURSOR_ARROW));
            }
        }
        else if (!fMouseInside && HasCapture())
        {
            ReleaseMouse();
            m_fMouseCaptured = false;
//            wxLogMessage("[MainFrame::on_mouse_motion] Mouse Released");
            SetCursor(wxCursor(wxCURSOR_ARROW));
        }
        event.Skip();
        return;
    }


    //handle resizing logic
    if (m_fResizingMode && event.LeftIsDown())
    {
        resize_window_mouse_motion(event);
        event.Skip();
        return;
    }

    //handle window moving logic
    if (m_fMoveMode && event.LeftIsDown())
    {
        wxPoint pos = event.GetPosition(); // Mouse position relative to frame client area

        // Calculate the new frame position
        wxPoint currentScreenPos = ClientToScreen(pos); // Convert current mouse pos to screen coords
        int deltaX = currentScreenPos.x - m_moveStartPos.x;
        int deltaY = currentScreenPos.y - m_moveStartPos.y;

        SetPosition(m_frameStartPos + wxPoint(deltaX, deltaY));
    }

    event.Skip();
}

//---------------------------------------------------------------------------------------
void MainFrame::on_mouse_left_up(wxMouseEvent& event)
{
    if (m_fResizingMode)
    {
        resize_window_left_mouse_up(event);
    }
    SetCursor(wxCursor(wxCURSOR_ARROW));
    m_fMoveMode = false;
    event.Skip();
}

//---------------------------------------------------------------------------------------
void MainFrame::resize_window_left_mouse_down(wxMouseEvent& WXUNUSED(event))
{
    m_fResizingMode = true;
    m_resizeStartFrameSize = GetSize();                    // Store current frame size
    if (!HasCapture())
    {
        CaptureMouse(); // Capture mouse for smooth dragging
        m_fMouseCaptured = true;
    }
}

//---------------------------------------------------------------------------------------
void MainFrame::resize_window_mouse_motion(wxMouseEvent& event)
{
    wxPoint currentScreenPos = ClientToScreen(event.GetPosition()); // Convert current mouse pos to screen coords
    int deltaX = currentScreenPos.x - m_moveStartPos.x;
    int deltaY = currentScreenPos.y - m_moveStartPos.y;

    int newFrameWidth = m_resizeStartFrameSize.GetWidth();
    int newFrameHeight = m_resizeStartFrameSize.GetHeight();
    int newFrameX = m_frameStartPos.x;
    int newFrameY = m_frameStartPos.y;

    switch (m_resizeDirection)
    {
        case ResizeDirection::RIGHT:
            newFrameWidth += deltaX;
            break;

        case ResizeDirection::BOTTOM:
            newFrameHeight += deltaY;
            break;

        case ResizeDirection::LEFT:
            newFrameWidth -= deltaX;
            newFrameX += deltaX;
            break;

        case ResizeDirection::TOP:
            newFrameHeight -= deltaY;
            newFrameY += deltaY;
            break;
        case ResizeDirection::NONE:
            // Should not happen if m_fResizingMode is true
            return;
    }
    newFrameWidth = std::max(newFrameWidth, MIN_CLIENT_DIM);
    newFrameHeight = std::max(newFrameHeight, MIN_CLIENT_DIM);


    //adjust if aspect ratio is locked
    if (m_fAspectRatioLocked)
    {
        wxSize borders = GetSize() - GetClientSize();
        int newGridWidth = newFrameWidth - borders.x;
        int newGridHeigt = newFrameHeight - borders.y - m_toolbarHeight;

        if (m_resizeDirection == ResizeDirection::RIGHT
            || m_resizeDirection == ResizeDirection::LEFT)
        {
            newGridHeigt = static_cast<int>(newGridWidth / m_aspectRatio);
            newFrameHeight = newGridHeigt + borders.y + m_toolbarHeight;
        }
        else
        {
            newGridWidth = static_cast<int>(newGridHeigt * m_aspectRatio);
            newFrameWidth = newGridWidth + borders.x;
        }
        wxLogMessage("[MainFrame::resize_window_mouse_motion] new grid(%d, %d), aspect ratio %.4f",
                     newGridWidth, newGridHeigt, (double)newGridWidth/(double)newGridHeigt);
    }

    SetSize(newFrameX, newFrameY, newFrameWidth, newFrameHeight);
}

//---------------------------------------------------------------------------------------
void MainFrame::resize_window_left_mouse_up(wxMouseEvent& WXUNUSED(event))
{
    if (m_fResizingMode && !m_fAspectRatioLocked)
        compute_aspect_ratio();

    m_fResizingMode = false;
    m_resizeDirection = ResizeDirection::NONE;

    if (HasCapture())
    {
        ReleaseMouse();
        m_fMouseCaptured = false;
    }
//    wxLogMessage("[MainFrame::resize_window_left_mouse_up] Ended border drag.");
}

//---------------------------------------------------------------------------------------
void MainFrame::compute_aspect_ratio()
{
    wxSize size = GetClientSize();
    double width = static_cast<double>(size.GetWidth());
    int height = static_cast<double>(size.GetHeight() - m_toolbarHeight);
    m_aspectRatio = 1.0;
    if (width > 0 && height > 0)
    {
        m_aspectRatio = width / height;
    }
    wxLogWarning("[MainFrame::compute_aspect_ratio] Aspect ratio %.4f", m_aspectRatio);
}

//---------------------------------------------------------------------------------------
void MainFrame::on_tool_grid_options(wxCommandEvent& WXUNUSED(event))
{
    DlgGridOptions dlg(this);
    if (dlg.ShowModal() == wxID_OK)
    {
        m_gridSize = dlg.get_segments();
        m_gridColour = dlg.get_color();
        m_fBitmapIsInvalid = true;

        //Don't allow black as it will transformed into transparent
        if (m_gridColour == *wxBLACK)
            m_gridColour = wxColour("#000005");

        Refresh();  //trigger repaint
    }
}

//---------------------------------------------------------------------------------------
void MainFrame::on_tool_set_aspect_ratio(wxCommandEvent& WXUNUSED(event))
{
    compute_aspect_ratio();
    DlgAspectRatio dlg(this, m_aspectRatio);
    if (dlg.ShowModal() == wxID_OK)
    {
        double aspectRatio = dlg.get_aspect_ratio();
        wxLogMessage("[MainFrame::on_set_aspect_ratio] New aspect ratio: %.4f", aspectRatio);
        wxConfigBase* pPrefs = wxGetApp().get_preferences();
        pPrefs->Write("/Size/Ratio", aspectRatio);
        change_and_lock_aspect_ratio(aspectRatio);
        m_fBitmapIsInvalid = true;
    }
}

//---------------------------------------------------------------------------------------
void MainFrame::on_tool_lock_aspect_ratio(wxCommandEvent& event)
{
    m_fAspectRatioLocked = event.IsChecked();
}

//---------------------------------------------------------------------------------------
void MainFrame::on_tool_show_grid(wxCommandEvent& event)
{
    m_fDrawGrid = event.IsChecked();
    m_fBitmapIsInvalid = true;
    Refresh();
}

//---------------------------------------------------------------------------------------
void MainFrame::on_tool_show_golden_lines(wxCommandEvent& event)
{
    m_fDrawGoldenLines = event.IsChecked();
    m_fBitmapIsInvalid = true;
    Refresh();
}



}   //namespace agrilla

