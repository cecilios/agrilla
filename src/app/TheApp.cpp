//wxWidgets
#include <wx/graphics.h>
#include <wx/log.h> // For logging messages
#include <wx/stdpaths.h> // For finding standard directories
#include <wx/file.h>     // For creating and writing to a file
#include <wx/filename.h> // For path manipulation
#include <wx/fileconf.h>

//agrilla
#include "TheApp.h"
#include "MainFrame.h"

//other
#include <fstream>



//implement function wxGetApp() that returns a reference to TheApp instance
IMPLEMENT_APP(agrilla::TheApp)


namespace agrilla
{


bool TheApp::OnInit()
{
    // 'Main program' equivalent: the program execution "starts" here

////    // Redirect wxLog messages to stderr for visibility in console
////    wxLog::SetLogLevel(wxLOG_Info);
////    wxLog::SetActiveTarget(new wxLogStderr);
//
//    // Try to open the log file.
//    // We use a wxLogNull guard here to prevent a message box from appearing
//    // if the file cannot be opened.
//    {
//        wxLogNull noLog;
//        m_logFilePtr = fopen("agrilla.log", "w");
//        //~/.config/agrilla/options.ini
//    }
//
//    // Check if the file was opened successfully.
//    if (m_logFilePtr != nullptr)
//    {
//        // If successful, create a wxLogStderr object using the file pointer.
//        // wxLog::SetActiveTarget() takes ownership of the pointer to the log target.
//        // This will redirect all wxLog messages to our file.
//        wxLog::SetActiveTarget(new wxLogStderr(m_logFilePtr));
//        wxLogMessage("[TheApp::OnInit] Application started successfully.");
//    }
//    else
//    {
//        // If the file could not be opened, we can log an error.
//        // This will go to the default log target (usually a message box).
//        wxLogError("[TheApp::OnInit] Could not open log file 'agrilla.log'. Logging will be disabled.");
//    }

    create_config_file();

    // Create the MainFrame
    MainFrame* mainFrame = new MainFrame();

    #if defined(__WXDEBUG__)
        #ifdef __WXGTK__
            //in Debug build, use a window to show wxLog messages. This is
            //the only way I found to see wxLog messages with Code::Blocks
            wxLogWindow* pMyLog = new wxLogWindow(mainFrame,
                "Debug window: wxLogMessages", true /*show*/,
                false /*do not pass message to previous logger*/);
            pMyLog->GetFrame()->SetSize(1000, 250);
            wxLog::SetActiveTarget(pMyLog);
            pMyLog->Flush();
        #else
            wxLog::SetActiveTarget( new wxLogStderr );
        #endif
    #endif
    mainFrame->Show(true);

    return true;    //to indicate that the application should continue running
}

//---------------------------------------------------------------------------------------
int TheApp::OnExit()
{
    if (m_logFilePtr != nullptr)
    {
        fclose(m_logFilePtr);
    }

    m_pPrefs->Flush();
    delete m_pPrefs;

    return wxApp::OnExit();
}

//---------------------------------------------------------------------------------------
void TheApp::create_config_file()
{
//    //get the user's standard configuration directory. On Linux "~/.config"
//    wxStandardPaths::SetFileLayout(FileLayout_XDG);
//    wxString configDir = wxStandardPaths::Get().GetUserConfigDir();
//    wxFileName configPath(configDir, "agrilla");

    //'FileLayout_XDG' is not defined in wxWidgets 3.2, so let's do it manually
    wxString homeDir = wxGetHomeDir();
    wxFileName configDir(homeDir, wxEmptyString, wxEmptyString, wxPATH_NATIVE);
    configDir.AppendDir(".config");
    wxFileName configPath(configDir);
    configPath.AppendDir("agrilla");

    wxLogMessage("[TheApp::create_config_file] configPath '%s'", configPath.GetFullPath());


    //create the configuration object
    wxFileName configFile(configPath.GetFullPath(), "agrilla.ini");
    wxFileConfig* pConfig = new wxFileConfig("agrilla", "agrilla", configFile.GetFullPath(),
                                             "agrilla", wxCONFIG_USE_LOCAL_FILE );

    wxConfigBase::Set(pConfig);
    pConfig->SetRecordDefaults();
    pConfig->Flush();
    m_pPrefs = wxConfigBase::Get();
}


}   //namespace agrilla
