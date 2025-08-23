//---------------------------------------------------------------------------------------
// This file is part of the AGrilla application.
// Copyright (c) 2025-present, Cecilio Salmeron
//
// Licensed under the MIT license.
//
// See LICENSE file in the root directory of this source tree.
//---------------------------------------------------------------------------------------

#pragma once

//wxWidgets
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <wx/config.h>


namespace agrilla
{


// Define a new application type, each program should derive a class from wxApp
class TheApp : public wxApp
{
public:
    virtual bool OnInit() override;
    virtual int OnExit() override;

    wxConfigBase* get_preferences() { return m_pPrefs; }
    wxString get_resources_path();

    //program info
    static wxString get_version_string();
    static wxString get_version_long_string();
    static int get_version_major();
    static int get_version_minor();
    static wxString get_version_patch();
    static wxString get_build_date();

protected:
    void create_preferences_file();
    void create_log_file();
    wxString get_resources_path_from_executable();
    wxString ensure_log_folder_exists(const wxString& logFileName);
    wxString ensure_config_folder_exists(const wxString& configFileName);

    FILE* m_logFilePtr = nullptr;
    wxLogWindow* m_pLogWindow = nullptr;

    wxConfigBase* m_pPrefs = nullptr;

};


}   //namespace agrilla

// Declare the wxGetApp function outside the namespace, but with the full class name
wxDECLARE_APP(agrilla::TheApp);
