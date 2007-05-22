
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
using namespace org::esb::lang;
////////////////////////////////////////////////////////////////////////////////
Thread::Thread()
{
  task = this;
  started = false;
  joined = false;
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread( Runnable* task )
{
  this->task = task;
  started = false;
  joined = false;
}

////////////////////////////////////////////////////////////////////////////////
Thread::~Thread()
{}

////////////////////////////////////////////////////////////////////////////////
void Thread::start() throw ( Exception )
{
  if (this->started)
  {

    throw Exception( __FILE__, __LINE__,
                     "Thread already started");

  }
#ifndef WIN32
//    cout <<"Start"<<endl;

  pthread_attr_init (&attributes);
  pthread_attr_setdetachstate (&attributes, PTHREAD_CREATE_JOINABLE);
  int err = pthread_create (
              &this->threadHandle,
              &attributes,
              runCallback,
	      this);
  if (err != 0)
  {
    throw Exception( __FILE__, __LINE__,
                     "Coud not start thread");
  }

#else

  unsigned int threadId = 0;
  this->threadHandle =
    (HANDLE)::_beginthreadex(NULL, 0, runCallback, this, 0, &threadId);
  if (this->threadHandle == NULL)
  {
    throw Exception( __FILE__, __LINE__,
                     "Coud not start thread");
  }

#endif

  // Mark the thread as started.
  started = true;
}

////////////////////////////////////////////////////////////////////////////////
void Thread::join() throw( Exception )
{
  if (!this->started)
  {
    throw Exception( __FILE__, __LINE__,
                     "Thread::join() called without having called Thread::start()");
  }
  if (!this->joined)
  {

#ifndef WIN32
    pthread_join(this->threadHandle, NULL);
#else
    WaitForSingleObject (this->threadHandle, INFINITE);
#endif

  }
  this->joined = true;
}

////////////////////////////////////////////////////////////////////////////////
void Thread::sleep( int millisecs )
{
#ifndef WIN32
  struct timespec rec, rem;
  rec.tv_sec = millisecs / 1000;
  rec.tv_nsec = (millisecs % 1000) * 1000000;
  while( nanosleep( &rec, &rem ) == -1 )
  {
    if( errno != EINTR )
    {
      break;
    }
  }

#else
  ::Sleep (millisecs);
#endif
}

////////////////////////////////////////////////////////////////////////////////
unsigned long Thread::getId(void)
{
#ifndef WIN32
  return (long)(pthread_self());
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
Thread::runCallback( void* param )
{
  // Get the instance.
  Thread* thread = (Thread*)param;
  // Invoke run on the task.
  try
  {
    thread->task->run();
  }
  catch( ... )
  {
    Exception ex(__FILE__, __LINE__, "unhandled exception bubbled up to Thread::run");
    //    ex.printStackTrace();
  }
  delete thread->task;

#ifndef WIN32
  pthread_attr_destroy( &thread->attributes );
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


