/* 
 * File:   ProcessListener.cpp
 * Author: HoelscJ
 * 
 * Created on 8. Dezember 2010, 16:10
 */

#include "ProcessListener.h"
namespace org {
  namespace esb {
    namespace lang {

      ProcessEvent::ProcessEvent(int pid, int exit, ProcessEventType ev):_pid(pid), _exitCode(exit), _eventType(ev) {

      }

      ProcessEvent::ProcessEventType ProcessEvent::getEventType() {
        return _eventType;
      }
      int ProcessEvent::getExitCode(){
        return _exitCode;
      }
      int ProcessEvent::getPid(){
        return _pid;
      }

      ProcessListener::ProcessListener() {
      }

      ProcessListener::~ProcessListener() {
      }
      
      void ProcessListener::onEvent(ProcessEvent&) {

      }
    }
  }
}
