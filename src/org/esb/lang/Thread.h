#ifndef ORG_ESB_LANG_THREAD_H
#define ORG_ESB_LANG_THREAD_H

#if defined(WIN32)
    #include <windows.h>
#else
    #include <pthread.h>
#endif

#include "org/esb/lang/Exception.h"
#include "org/esb/lang/Runnable.h"
#include <stdexcept>
#include <assert.h>
using namespace std;
//using namespace org::esb::lang;

namespace org{
namespace esb{
namespace lang{   
    /** 
     * Basic thread class - mimics the Java Thread.  Derived classes may 
     * implement the run method, or this class can be used as is with 
     * a provided Runnable delegate.
     */
    class Thread : public Runnable
    {
    private:
   
        /**
         * The task to be run by this thread, defaults to 
         * this thread object.
         */
        Runnable* task;
      
        #ifndef WIN32
            pthread_attr_t attributes;
            pthread_t threadHandle;
        #else
            HANDLE threadHandle;
        #endif
   
        /**
         * Started state of this thread.
         */
        bool started;
       
        /**
         * Indicates whether the thread has already been
         * joined.
         */
        bool joined;
       
    public:
      
        /**
         * default Constructor
         */
        Thread();
        
        /**
         * Constructor
         * @param task the Runnable that this thread manages
         */
        Thread( Runnable* task );
         
        virtual ~Thread();
   
        /** 
         * Creates a system thread and starts it in a joinable mode.  
         * Upon creation, the
         * run() method of either this object or the provided Runnable
         * object will be invoked in the context of this thread.
         * @exception runtime_error is thrown if the system could
         * not start the thread.
         */
        virtual void start() throw ( Exception );
   
        /**
         * Wait til the thread exits. This is when the run()
         * method has returned or has thrown an exception.
         */
        virtual void join() throw ( Exception );
       
        /**
         * Default implementation of the run method - does nothing.
         */
        virtual void run(){};

        virtual void setAutoDelete(bool autoDelete=false);
               
    public:
   
        /**
         * Halts execution of the calling thread for a specified no of millisec.
         *   
         * Note that this method is a static method that applies to the
         * calling thread and not to the thread object.
         * @param millisecs time in milliseconds to sleep
         */
        static void sleep( int millisecs );
       
        /**
         * Obtains the Thread Id of the current thread
         * @return Thread Id
         */
        static unsigned long getId(void); 
   
    private:
   
        // Internal thread handling
        #ifndef WIN32
            static void* runCallback (void* param);
        #else
            static unsigned int WINAPI runCallback (void* param);
        #endif
        
        bool _autoDelete;
    };

}}}

#endif 
