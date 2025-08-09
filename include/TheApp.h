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

    wxConfigBase* get_config() { return m_pPrefs; }


protected:
    void create_config_file();

    FILE* m_logFilePtr = nullptr;
    wxConfigBase* m_pPrefs = nullptr;

};


}   //namespace agrilla

// Declare the wxGetApp function outside the namespace, but with the full class name
wxDECLARE_APP(agrilla::TheApp);
