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
#include "config.h"

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

    create_preferences_file();

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
void TheApp::create_preferences_file()
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

    wxLogMessage("[TheApp::create_preferences_file] configPath '%s'", configPath.GetFullPath());


    //create the configuration object
    wxFileName configFile(configPath.GetFullPath(), "agrilla.ini");
    wxFileConfig* pConfig = new wxFileConfig("agrilla", "agrilla", configFile.GetFullPath(),
                                             "agrilla", wxCONFIG_USE_LOCAL_FILE );

    wxConfigBase::Set(pConfig);
    pConfig->SetRecordDefaults();
    pConfig->Flush();
    m_pPrefs = wxConfigBase::Get();
}

//---------------------------------------------------------------------------------------
wxString TheApp::get_resources_path()
{
    // Obtaining the resources file with the icons depends on how the progam is build and
    // were is it running from.
    //
    // a) When building without CMake, variable AGRILLA_RES_INSTALL_DIR is not
    //    defined. Then there are two cases:
    //    a.1) The running executable is under the 'agrilla/<branchname>/' tree. The
    //         resources folder can be determined from the executable dir.
    //    a.2) The running executable does not contain the 'agrilla' folder. It is a
    //         local build out of the source tree without using CMake, or an installed
    //         build without using CMake
    //
    // b) For CMake builds there are two cases:
    //    b.1) The new build is not installed, the program is running from the build
    //         directory. Then, AGRILLA_RES_INSTALL_DIR is defined but it is not valid
    //         because it could not exist (not yet created) or, if exists, it contains
    //         the icons for a previous installation.
    //    b.2) The new buils is installed and the program is run from the installation
    //         folder. In this case AGRILLA_RES_INSTALL_DIR is defined and is valid

#ifndef AGRILLA_RES_INSTALL_DIR
    // a.1) For local builds not using CMake, variable AGRILLA_RES_INSTALL_DIR is not
    // defined. To determine the resources directory we assume that any locally built
    // executable will run from a build subdirectory under the 'agrilla/<branchname>/'
    // source tree, like  agrilla/main/my-build/". So the approach is to check the
    // location of the running executable, and to remove all subdirs to leave only
    // the path to 'agrilla/<branchname>/', and to append 'res' to it.

    wxString path = get_resources_path_from_executable();
    if (path != wxEmptyString)
    {
        //a.1) local build without CMake under the 'agrilla' folder
        wxLogMessage("Case a.1) local build without CMake under the 'agrilla' folder. res.path='%s'",
                     path);
        return path;
    }
    else
    {
        // a.2) Macro AGRILLA_RES_INSTALL_DIRThe 'agrilla' is not defined and 'agrilla'
        // folder is not in the executable path. The most likely cause is that
        // it is an installed executable running from the standard system binary
        // directory like /usr/bin/ on Linux, but installed without building with CMake.
        // Alternativelly, it is a local build out of the source tree. In this las case
        // it is impossible to determine the resources path. So let's assume that is
        // an installed executable and that it is properly installed.
        wxLogMessage("Case a.2) Out-of-source builds or installed without CMake. res.path='%s'",
                      wxStandardPaths::Get().GetDataDir());
        return wxStandardPaths::Get().GetDataDir();
    }

#else
    // AGRILLA_RES_INSTALL_DIR exists. It is a build done with CMake but we don't know
    // if it is a locally build executable or an installed executable. Let's assume
    // that any locally build executable runs from a subfolder of the 'agrilla'
    // source tree.

    wxString path = get_resources_path_from_executable();
    if (path != wxEmptyString)
    {
        //b.1) local build with CMake under the 'agrilla' folder
         wxLogMessage("Case b.1) local build with CMake under the 'agrilla' folder. res.path='%s'",
                      path);
        return path;
    }
    else
    {
        //b.2) installed build using CMake. AGRILLA_RES_INSTALL_DIR is valid
        wxFileName fName(AGRILLA_RES_INSTALL_DIR);
        fName.Normalize(wxPATH_NORM_DOTS);
        wxLogMessage("Case b.2) installed build using CMake. res.path='%s'",
                      fName.GetFullPath());
        return fName.GetFullPath();
    }
#endif

}

//---------------------------------------------------------------------------------------
wxString TheApp::get_resources_path_from_executable()
{
    // To determine the resources directory we assume that any locally built
    // executable will run from a build subdirectory under the 'agrilla/<branchname>/'
    // source tree, like  agrilla/main/my-build/". So the approach is to check the
    // location of the running executable, and to remove all subdirs to leave only
    // the path to 'agrilla/<branchname>/', and to append 'res' to it.

    wxFileName fName(wxStandardPaths::Get().GetExecutablePath());

    wxString branchDir;
    bool fFoundAgrillaDir = false;

    // Store the last directory as we loop
    while (!fName.GetDirs().empty())
    {
        wxString lastDir = fName.GetDirs().Last();
        if (lastDir == "agrilla")
        {
            fFoundAgrillaDir = true;
            break;
        }

        // This will only store the immediate child of 'agrilla'
        branchDir = lastDir;
        fName.RemoveLastDir();
    }

    if (fFoundAgrillaDir)
    {
        // Re-append the branch directory if it exists
        if (!branchDir.IsEmpty())
        {
            fName.AppendDir(branchDir);
        }
        fName.AppendDir("res");
        return fName.GetPath(wxPATH_GET_SEPARATOR);
    }
    else
    {
        // The 'agrilla' folder is not in the executable path. It is either an installed
        // executable is placed in a standard system binary directory like /usr/bin/ on
        // Linux, or it is a local build out of the source tree. In any case the
        // resources path cannot be determined from the execution path
        return wxEmptyString;
    }
}


}   //namespace agrilla
