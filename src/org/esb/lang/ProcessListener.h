/* 
 * File:   ProcessListener.h
 * Author: HoelscJ
 *
 * Created on 8. Dezember 2010, 16:10
 */

#ifndef PROCESSLISTENER_H
#define	PROCESSLISTENER_H
namespace org {
  namespace esb {
    namespace lang {

      class ProcessEvent {
      public:
        enum ProcessEventType {
          PROCESS_STARTED,
          PROCESS_STOPPED,
          PROCESS_KILLED,
          PROCESS_START_FAILED
        };
        ProcessEvent(int pid, int exit, ProcessEventType ev);
        ProcessEventType getEventType();
        int getExitCode();
        int getPid();
      private:
        int _pid;
		int _exitCode;
		ProcessEventType _eventType;
      };

      class ProcessListener {
      public:
        ProcessListener();
        virtual ~ProcessListener();
        virtual void onEvent(ProcessEvent&)=0;
      private:

      };
    }
  }
}

#endif	/* PROCESSLISTENER_H */

