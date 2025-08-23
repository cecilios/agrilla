//---------------------------------------------------------------------------------------
// This file is part of the AGrilla application.
// Copyright (c) 2025-present, Cecilio Salmeron
//
// Licensed under the MIT license.
//
// See LICENSE file in the root directory of this source tree.
//---------------------------------------------------------------------------------------

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
#include "version.h"

//other
#include <fstream>



//implement function wxGetApp() that returns a reference to TheApp instance
IMPLEMENT_APP(agrilla::TheApp)


namespace agrilla
{


//---------------------------------------------------------------------------------------
bool TheApp::OnInit()
{
    // 'Main program' equivalent: the program execution "starts" here

    create_log_file();
    create_preferences_file();

    MainFrame* mainFrame = new MainFrame();
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

    delete m_pLogWindow;

    m_pPrefs->Flush();
//    delete m_pPrefs;      //causes double delete !!  Why?

    return wxApp::OnExit();
}

//---------------------------------------------------------------------------------------
void TheApp::create_preferences_file()
{
    wxString configPath = ensure_config_folder_exists("agrilla.ini");
    wxLogMessage("[TheApp::create_preferences_file] configPath '%s'", configPath);
    wxFileConfig* pConfig = new wxFileConfig("agrilla", "agrilla", configPath,
                                             "agrilla", wxCONFIG_USE_LOCAL_FILE );

    wxConfigBase::Set(pConfig);
    pConfig->SetRecordDefaults();
    pConfig->Flush();
    m_pPrefs = wxConfigBase::Get();
}

//---------------------------------------------------------------------------------------
void TheApp::create_log_file()
{
//    //In Release builds suppress all log messages with high or equal severity than warning
//    #if defined(NDEBUG)
//         wxLog::SetLogLevel(wxLOG_Warning);
//    #endif


#if defined(DEBUG) && defined(__WXGTK__)

    //in local Debug build in Linux with with Code::Blocks, use a window to show wxLog
    //messages. This is the only way I've found to see wxLog messages in real time
    //with Code::Blocks

    m_pLogWindow = new wxLogWindow(nullptr,
                                   "Debug window: wxLogMessages", true /*show*/,
                                   false /*do not pass message to previous logger*/);
    m_pLogWindow->GetFrame()->SetSize(1000, 250);
    wxLog::SetActiveTarget(m_pLogWindow);
    m_pLogWindow->Flush();


#else
    //In all other scenarios create a log file

    // Try to open/create the log file.
    wxString logPath = ensure_log_folder_exists("agrilla.log");
    m_logFilePtr = fopen(logPath, "w");

    // Check if the file was opened successfully.
    if (m_logFilePtr != nullptr)
    {
        // If successful, create a wxLogStderr object using the file pointer.
        // wxLog::SetActiveTarget() takes ownership of the pointer to the log target.
        // This will redirect all wxLog messages to our file.
        wxLog::SetActiveTarget(new wxLogStderr(m_logFilePtr));
        wxLogMessage("[TheApp::create_log_file] Application started successfully.");
    }
    else
    {
        // If the file could not be opened, we can log an error.
        // This will go to the default log target (usually a message box).
        wxLogError("[TheApp::create_log_file] Could not open log file 'agrilla.log'. Logging will go to the default log target.");
    }
#endif

    #if defined(DEBUG)
        wxLogMessage("[TheApp::create_log_file] macro DEBUG is defined");
    #endif
    #if defined(wxDEBUG_LEVEL)
        wxLogMessage("[TheApp::create_log_file] macro wxDEBUG_LEVEL is defined. Value %d", wxDEBUG_LEVEL);
    #endif
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
        wxLogMessage("[TheApp::get_resources_path] Case a.1) local build without CMake under the 'agrilla' folder.\n"
                     "                   Resources path='%s'", path);
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
        path = wxStandardPaths::Get().GetDataDir();
        wxLogMessage("[TheApp::get_resources_path] Case a.2) Out-of-source builds or installed without CMake.\n"
                     "                   Resources path='%s'", path);
        return path;
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
         wxLogMessage("[TheApp::get_resources_path] Case b.1) local build with CMake under the 'agrilla' folder.\n"
                      "                   Resources path='%s'", path);
        return path;
    }
    else
    {
        //b.2) installed build using CMake. AGRILLA_RES_INSTALL_DIR is valid
        wxFileName fName(AGRILLA_RES_INSTALL_DIR);
        fName.Normalize(wxPATH_NORM_DOTS);
        path = fName.GetFullPath();
        wxLogMessage("[TheApp::get_resources_path] Case b.2) installed build using CMake.\n"
                     "                   Resources path='%s'", path);
        return path;
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

//---------------------------------------------------------------------------------------
wxString TheApp::ensure_log_folder_exists(const wxString& logFileName)
{
    //Ensures the existence of the directory path for application logs
    //based on the operating system's standard user local data directories,
    //typically:
    // - Linux: ~/.local/share
    // - macOS: ~/Library/Application Support
    // - Windows: C:\Users\<User>\AppData\Local
    //It creates the application directory if it not yet exists.
    //Returns the full path for the log file or wxEmptyString if any failure

    // Get the standard user local data directory.
    wxStandardPaths::Get().SetFileLayout(wxStandardPaths::FileLayout_XDG);
    wxString userLocalDataDir = wxStandardPaths::Get().GetUserLocalDataDir();

    // Construct the full directory path for the application logs
    wxFileName logDirPath(userLocalDataDir, logFileName);

    // Check if the directory exists. If not, create it.
    // wxPATH_MKDIR_FULL ensures that all intermediate directories are created.
    // wxS_DIR_DEFAULT uses default permissions.
    if (!logDirPath.DirExists())
    {
        if (!wxFileName::Mkdir(logDirPath.GetPath(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL))
        {
            wxLogError("Failed to create log directory: %s", logDirPath.GetPath());
            return wxEmptyString; // Return empty string on failure
        }
    }

    //Return the full path to the log file
    wxFileName logFilePath(logDirPath.GetPath(), logFileName);

     #if defined(DEBUG)
        wxLogMessage("[TheApp::ensure_log_folder_exists] userLocalDataDir: '%s", userLocalDataDir);
        wxLogMessage("[TheApp::ensure_log_folder_exists] logDirPath: '%s", logDirPath.GetPath());
        wxLogMessage("[TheApp::create_log_file] log path '%s'", logFilePath.GetFullPath());
    #endif

    return logFilePath.GetFullPath();
}

//---------------------------------------------------------------------------------------
wxString TheApp::ensure_config_folder_exists(const wxString& configFileName)
{
    //Ensures the existence of the directory path for application config file
    //based on the operating system's standard user local data directories.
    //It creates the application directory if it not yet exists.
    //Returns the full path for the config file or wxEmptyString if any failure

    //get the user's standard configuration directory. On Linux "~/.config"
    wxStandardPaths::Get().SetFileLayout(wxStandardPaths::FileLayout_XDG);
    wxString configDir = wxStandardPaths::Get().GetUserConfigDir();
    wxFileName configPath(configDir);
    configPath.AppendDir("agrilla");
    wxLogMessage("[TheApp::ensure_config_folder_exists] configPath: '%s'", configPath.GetPath());

    // Check if the directory exists. If not, create it.
    // wxPATH_MKDIR_FULL ensures that all intermediate directories are created.
    // wxS_DIR_DEFAULT uses default permissions.
    if (!configPath.DirExists())
    {
        if (!wxFileName::Mkdir(configPath.GetPath(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL))
        {
            wxLogError("Failed to create config directory: %s", configPath.GetPath());
            return wxEmptyString; // Return empty string on failure
        }
    }

    //Return the full path to the log file
    wxFileName fullPath(configPath.GetPath(), configFileName);
    return fullPath.GetFullPath();
}

//---------------------------------------------------------------------------------------
int TheApp::get_version_major() { return AGRILLA_VERSION_MAJOR; }

//---------------------------------------------------------------------------------------
int TheApp::get_version_minor() { return AGRILLA_VERSION_MINOR; }

//---------------------------------------------------------------------------------------
wxString TheApp::get_version_patch() { return AGRILLA_VERSION_PATCH; }

//---------------------------------------------------------------------------------------
wxString TheApp::get_version_string()
{
    return AGRILLA_VERSION;
}

//---------------------------------------------------------------------------------------
wxString TheApp::get_version_long_string()
{
    return AGRILLA_VERSION_LONG;
}

//---------------------------------------------------------------------------------------
wxString TheApp::get_build_date()
{
    //__DATE__ string: contains eleven characters and looks like "Feb 12 1996".
    // If the day of the month is less than 10, it is padded with a space on the
    // left, e.g., "Oct  8 2013"
    //__TIME__ : the time at which the preprocessor is being run. The string
    // contains eight characters and looks like "23:59:01"

    wxString date(__DATE__);
    return date.substr(4,2) + "-" + date.substr(0,3) + "-" + date.substr(7,4)
           + " " + wxString(__TIME__);
}


}   //namespace agrilla
