/*----------------------------------------------------------------------
 *  File    : Thread.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 15. Dezember 2009, 15:41 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001   Jan Hölscher
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

#include "org/esb/lang/Thread.h"
#include <errno.h>

#ifndef WIN32
#include <errno.h> 
extern int errno;
#else
#include <process.h> // _endthreadex
#endif

//#include <activemq/exceptions/ActiveMQException.h>
//#include <activemq/exceptions/RuntimeException.h>

using namespace std;
//using namespace org::esb::lang;
namespace org {
  namespace esb {
    namespace lang {
      ////////////////////////////////////////////////////////////////////////////////

      Thread::Thread() {
        task = this;
        started = false;
        joined = false;
        _autoDelete = false;
      }

      ////////////////////////////////////////////////////////////////////////////////

      Thread::Thread(Runnable* task) {
        this->task = task;
        started = false;
        joined = false;
        _autoDelete = false;
      }

      ////////////////////////////////////////////////////////////////////////////////

      Thread::~Thread() {
        //    delete this->task;
      }

      ////////////////////////////////////////////////////////////////////////////////

      void Thread::start() {
        if (this->started) {

          throw Exception(__FILE__, __LINE__,
              "Thread already started");

        }
#ifndef WIN32
        //    cout <<"Start"<<endl;

        pthread_attr_init(&attributes);
        pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);
        int err = pthread_create(
            &this->threadHandle,
            &attributes,
            runCallback,
            this);

        if (err != 0) {
          throw Exception(__FILE__, __LINE__,
              "Coud not start thread");
        }

#else

        unsigned int threadId = 0;
        this->threadHandle =
            (HANDLE)::_beginthreadex(NULL, 0, runCallback, this, 0, &threadId);
        if (this->threadHandle == NULL) {
          throw Exception(__FILE__, __LINE__,
              "Coud not start thread");
        }

#endif

        // Mark the thread as started.
        started = true;
      }

      ////////////////////////////////////////////////////////////////////////////////

      void Thread::join() {
        if (!this->started) {
          throw Exception(__FILE__, __LINE__,
              "Thread::join() called without having called Thread::start()");
        }
        if (!this->joined) {

#ifndef WIN32
          pthread_join(this->threadHandle, NULL);
#else
          WaitForSingleObject(this->threadHandle, INFINITE);
#endif

        }
        this->joined = true;
      }

      ////////////////////////////////////////////////////////////////////////////////

      void Thread::sleep2(int millisecs) {
#ifndef WIN32
        struct timespec rec, rem;
        rec.tv_sec = millisecs / 1000;
        rec.tv_nsec = (millisecs % 1000) * 1000000;
        while (nanosleep(&rec, &rem) == -1) {
          if (errno != EINTR) {
            break;
          }
        }

#else
        ::Sleep(millisecs);
#endif
      }

      ////////////////////////////////////////////////////////////////////////////////

      unsigned long Thread::getId(void) {
#ifndef WIN32
        return (long) (pthread_self());
#else
        return GetCurrentThreadId();
#endif
      }

      ////////////////////////////////////////////////////////////////////////////////
#ifndef WIN32
      void*
#else

unsigned int WINAPI
#endif
      Thread::runCallback(void* param) {
        // Get the instance.
        Thread* thread = (Thread*) param;
        // Invoke run on the task.
        //  try
        //  {
        thread->task->run();
        //  }
        //  catch(Exception &ex){
        //  	throw ex;
        //  }
        if (thread->_autoDelete) {
          delete thread->task;
          thread->task = 0;
        }

#ifndef WIN32
        pthread_attr_destroy(&thread->attributes);
        return NULL;
#else

        // Needed when using threads and CRT in Windows. Otherwise memleak can appear.
        _endthreadex(0);

        // _endthreadex (unlike _endthread) does not automatically close the thread handle
        // so we need to do this manually.
        CloseHandle(thread->threadHandle);

        return 0;
#endif
      }

      void Thread::setAutoDelete(bool autoDelete) {
        _autoDelete = autoDelete;
      }

    }
  }
}

