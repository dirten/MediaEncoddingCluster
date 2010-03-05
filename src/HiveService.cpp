/*----------------------------------------------------------------------
 *  File    : HiveService.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose : Starter program for the hive
 *  Created : 22.02.2010, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 * ----------------------------------------------------------------------
 */
#include <stdlib.h>
#include <boost/thread.hpp>
#if !defined(_WIN32)
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#else
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "client/windows/handler/exception_handler.h"
#endif  // !_WIN32

#include <iostream>
#include <string>
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/Properties.h"

#ifdef WIN32

boost::mutex terminationMutex;
boost::condition ctrlCHit;
boost::condition serverStopped;

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type) {
  switch (ctrl_type) {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_SHUTDOWN_EVENT:
    {
      boost::mutex::scoped_lock terminationLock(terminationMutex);
      LOGDEBUG("ctlc event");
      ctrlCHit.notify_all(); // should be just 1

      //      serverStopped.wait(terminationLock);
      return TRUE;
    }
    default:
      return FALSE;
  }
}

void ctrlCHitWait() {
  SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
  boost::mutex::scoped_lock terminationLock(terminationMutex);
  ctrlCHit.wait(terminationLock);
}

#define SVCNAME TEXT("MHiveService")
SERVICE_STATUS gSvcStatus;
SERVICE_STATUS_HANDLE gSvcStatusHandle;
HANDLE ghSvcStopEvent = NULL;

//VOID SvcInstall(void);
VOID WINAPI SvcCtrlHandler(DWORD);
VOID WINAPI SvcMain(DWORD, LPTSTR *);

VOID ReportSvcStatus(DWORD, DWORD, DWORD);
VOID SvcInit(DWORD, LPTSTR *);
VOID SvcReportEvent(LPTSTR);
std::string execargs;
std::string exec;
VOID SvcReportEvent(LPTSTR szFunction) {
  HANDLE hEventSource;
  LPCTSTR lpszStrings[2];
  TCHAR Buffer[80];

  hEventSource = RegisterEventSource(NULL, SVCNAME);

  if (NULL != hEventSource) {
    StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());

    lpszStrings[0] = SVCNAME;
    lpszStrings[1] = Buffer;

    ReportEvent(hEventSource, // event log handle
        EVENTLOG_ERROR_TYPE, // event type
        0, // event category
        NULL, // event identifier
        NULL, // no security identifier
        2, // size of lpszStrings array
        0, // no binary data
        lpszStrings, // array of strings
        NULL); // no binary data

    DeregisterEventSource(hEventSource);
  }
}

VOID WINAPI SvcCtrlHandler(DWORD dwCtrl) {
  // Handle the requested control code.

  switch (dwCtrl) {
    case SERVICE_CONTROL_STOP:
    ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

    // Signal the service to stop.

    SetEvent(ghSvcStopEvent);

    return;

    case SERVICE_CONTROL_INTERROGATE:
    // Fall through to send current status.
    break;

    default:
    break;
  }

  ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
}

VOID ReportSvcStatus(DWORD dwCurrentState,
    DWORD dwWin32ExitCode,
    DWORD dwWaitHint) {
  static DWORD dwCheckPoint = 1;

  // Fill in the SERVICE_STATUS structure.

  gSvcStatus.dwCurrentState = dwCurrentState;
  gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
  gSvcStatus.dwWaitHint = dwWaitHint;

  if (dwCurrentState == SERVICE_START_PENDING)
  gSvcStatus.dwControlsAccepted = 0;
  else gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

  if ((dwCurrentState == SERVICE_RUNNING) ||
      (dwCurrentState == SERVICE_STOPPED))
  gSvcStatus.dwCheckPoint = 0;
  else gSvcStatus.dwCheckPoint = dwCheckPoint++;

  // Report the status of the service to the SCM.
  SetServiceStatus(gSvcStatusHandle, &gSvcStatus);
}

VOID WINAPI SvcMain(DWORD dwArgc, LPTSTR *lpszArgv) {
  gSvcStatusHandle = RegisterServiceCtrlHandler(
      SVCNAME,
      SvcCtrlHandler);

  if (!gSvcStatusHandle) {
    SvcReportEvent(TEXT("RegisterServiceCtrlHandler"));
    return;
  }
  gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
  gSvcStatus.dwServiceSpecificExitCode = 0;

  // Report initial status to the SCM

  ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 3000);
  SvcInit(dwArgc, lpszArgv);

}

VOID SvcInit(DWORD dwArgc, LPTSTR *lpszArgv) {
  // TO_DO: Declare and set any required variables.
  //   Be sure to periodically call ReportSvcStatus() with
  //   SERVICE_START_PENDING. If initialization fails, call
  //   ReportSvcStatus with SERVICE_STOPPED.

  // Create an event. The control handler function, SvcCtrlHandler,
  // signals this event when it receives the stop control code.

  ghSvcStopEvent = CreateEvent(
      NULL, // default security attributes
      TRUE, // manual reset event
      FALSE, // not signaled
      NULL); // no name

  if (ghSvcStopEvent == NULL) {
    ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
    return;
  }

  // Report running status when initialization is complete.
  /*
   *
   * Initializing Application Services
   *
   */
  /**
   *
   * STARTING THE EXECUTALBE HERE
   *
   * */
  bool bWorked;
  STARTUPINFO suInfo;
  PROCESS_INFORMATION procInfo;
  std::string m_Process = exec.c_str();
   char *vip = const_cast<char*>(exec.append(execargs).c_str());

  memset (&suInfo, 0, sizeof(suInfo));
  memset (&procInfo, 0, sizeof(procInfo));
  suInfo.cb = sizeof(suInfo);

  bWorked = ::CreateProcess(m_Process.c_str(),
             vip,      // can also be NULL

             NULL,
             NULL,
             FALSE,
             NORMAL_PRIORITY_CLASS,
             NULL,
             NULL,
             &suInfo,
             &procInfo);

/*
procInfo has these members
    HANDLE hProcess;   // process handle
    HANDLE hThread;    // primary thread handle
    DWORD dwProcessId; // process PID
    DWORD dwThreadId;  // thread ID
*/


  ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);

  // TO_DO: Perform work until service stops.
  while (1) {
    // Check whether to stop the service.

    WaitForSingleObject(ghSvcStopEvent, INFINITE);
    /*
     *
     * Stopping Application Services from configuration
     *
     */
	
	  HANDLE hProcess;
	  hProcess = OpenProcess( PROCESS_TERMINATE, FALSE, procInfo.dwProcessId );
	  TerminateProcess( hProcess, (DWORD) -1 );


    /**
     *
     * STOPPING THE EXECUTALBE HERE
     *
     * */

    ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
    return;
  }
}

int start(std::string executable, std::string arguments) {
	execargs=arguments;
	exec=executable;
  SERVICE_TABLE_ENTRY DispatchTable[] = {
    { SVCNAME, (LPSERVICE_MAIN_FUNCTION) SvcMain},
    { NULL, NULL}
  };
  if (!StartServiceCtrlDispatcher(DispatchTable)) {
    SvcReportEvent(TEXT("StartServiceCtrlDispatcher"));
  }
  return 0;
}
#else
int last_pid;
void signalHandler(int signum){
  std::cout <<"shutdown mhiveservice: last_pid"<<last_pid<<std::endl;
  kill(last_pid,SIGTERM);
  int status = 0;
  waitpid(last_pid, &status, 0);
  exit(0);
}
void signalWait() {
  sigset_t wait_mask2;
  sigemptyset(&wait_mask2);
  sigaddset(&wait_mask2, SIGINT);
  sigaddset(&wait_mask2, SIGQUIT);
  sigaddset(&wait_mask2, SIGTERM);
  sigaddset(&wait_mask2, SIGCHLD);
  pthread_sigmask(SIG_BLOCK, &wait_mask2, 0);
  int sig = 0;

  int err;
  do {
    err = sigwait(&wait_mask2, &sig);
  } while (err != 0);
}

int start(std::string executable, std::string arguments) {

  /**
   * fork this process
   * */
  int parentpid=fork();
  if(parentpid>0)exit(0);

  signal(SIGTERM,&signalHandler);
  signal(SIGQUIT,&signalHandler);
  signal(SIGINT,&signalHandler);
  char * args[4];
  int a = 0;
  args[a++] = "sh";
  args[a++] = "-c";
  args[a++] = const_cast<char*> (executable.append(arguments).c_str());
  args[a++] = NULL;
  bool loop = true;
  while (loop) {
    int pid;
    pid = fork();
    if (pid < 0) {
      std::cerr << "could not fork process" << std::endl;
      exit(1);
    }
    if (pid == 0) {
      int s = execv("/bin/sh", args);
    } else {
      /**
       * wait for child process exit
       * */
      last_pid=pid;
      int status = 0;
      waitpid(pid, &status, 0);
    }
  }
}
#endif
int main(int argc, char**argv) {

  /**
   * needing the base path from the executable
   * */
  int mode=0;
  org::esb::io::File f(argv[0]);
  std::cout << f.getParent() << std::endl;
  std::string path=f.getParent();
  std::string executable = path;
  std::string arguments;
  org::esb::util::Properties props;
  org::esb::io::File file(path.append("/../hive"));
  if(file.isDirectory()){
    mode=1;
  }
  executable.append("/mhive ");
  if (mode == 1) {
    executable.append("-r");
  } else if (mode == 0) {
    executable.append("-i");
  }else{
    exit(1);
  }
  start(executable,arguments);
}
