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
