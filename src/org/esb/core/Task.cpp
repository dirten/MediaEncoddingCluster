/* 
 * File:   Task.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 10:23
 */

#include "Task.h"
namespace org {
  namespace esb {
    namespace core {

      Task::Task() {
        _status=NONE;
        _progress_length=0;
        _progress=0;
      }

      Task::~Task() {
      }

      void Task::prepare() {
        setStatus(PREPARE);
      }

      void Task::execute() {
        setStatus(EXECUTE);

      }
      
      void Task::cleanup() {
        setStatus(CLEANUP);

        setStatus(DONE);
      }
      
      int Task::getProgress() {
        return _progress;
      }
      
      void Task::setProgress(int p) {
        _progress=(_progress_length*100)/p;
      }
      
      void Task::setProgressLength(unsigned int l) {
        _progress_length=l;
      }

      unsigned int Task::getProgressLength() {
        return _progress_length;
      }
      
      void Task::interrupt() {
        setStatus(INTERRUPT);
      }
      
      void Task::setStatus(STATUS s){
        _status=s;
      }
      Task::STATUS  Task::getStatus(){
        return _status;
      }
      
      void Task::setStatusMessage(std::string s){
        _status_message=s;
      }
      std::string  Task::getStatusMessage(){
        return _status_message;
      }
    }
  }
}
