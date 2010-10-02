/* 
 * File:   CtrlCHitWaiter.cpp
 * Author: HoelscJ
 * 
 * Created on 6. August 2010, 12:56
 */

#include "CtrlCHitWaiter.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition.hpp"
#if !defined(_WIN32)
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#else
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
//#include "client/windows/handler/exception_handler.h"
#endif  // !_WIN32
namespace org {
  namespace esb {
    namespace lang {

      CtrlCHitWaiter::CtrlCHitWaiter() {
      }

      CtrlCHitWaiter::~CtrlCHitWaiter() {
      }

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
              
              ctrlCHit.notify_all(); // should be just 1

              //      serverStopped.wait(terminationLock);
              return TRUE;
            }
            default:
              return FALSE;
          }
        }

#endif
      void CtrlCHitWaiter::wait() {
#ifdef WIN32


   
          SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
          boost::mutex::scoped_lock terminationLock(terminationMutex);
          ctrlCHit.wait(terminationLock);
        
#else

        
          sigset_t wait_mask2;
          sigemptyset(&wait_mask2);
          sigaddset(&wait_mask2, SIGINT);
          sigaddset(&wait_mask2, SIGQUIT);
          sigaddset(&wait_mask2, SIGTERM);
          sigaddset(&wait_mask2, SIGCHLD);
          pthread_sigmask(SIG_BLOCK, &wait_mask2, 0);
          int sig = 0;
          //sigdelset(&wait_mask, SIGCHLD);

          int err;
          do {
            err = sigwait(&wait_mask2, &sig);
          } while (err != 0);

        
#endif

      }
    }
  }
}
