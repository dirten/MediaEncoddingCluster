/* 
 * File:   Task.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 10:23
 */

#include "Task.h"
using namespace org::esb::av;

namespace org {
  namespace esb {
    namespace core {

      Task::Task() {
        setStatus(NONE);
        _progress_length=0;
        _progress=0;
        setProgress(0);
        
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

      void Task::interrupt() {
        setStatus(CANCEL);
      }
      
      int Task::getProgress() {
        return (_progress*100)/_progress_length;
      }
      
      void Task::setProgress(unsigned int p) {
        if(p!=_progress){
          _progress=p;//(p*100)/_progress_length;
          if(progressObserver)
            progressObserver(this);
        }
      }
      
      void Task::setProgressLength(unsigned int l) {
        _progress_length=l;
      }

      unsigned int Task::getProgressLength() {
        return _progress_length;
      }
            
      void Task::setStatus(STATUS s){
        _status=s;
        if(statusObserver)
          statusObserver(this);
      }
      Task::STATUS  Task::getStatus(){
        return _status;
      }
      
      void Task::setStatusMessage(std::string s){
        _status_message=s;
         if(statusObserver)
          statusObserver(this);
      }
      std::string  Task::getStatusMessage(){
        return _status_message;
      }

      std::string Task::getSource(){
        return _source;
      }
      
      std::string Task::getSink(){
        return _sink;
      }

      void Task::addSourceTask(Ptr<Task> t)
      {
        _sources.push_back(t);
      }

      void Task::addSinkTask(Ptr<Task> t)
      {
        _sinks.push_back(t);
      }

      void Task::pushBuffer(Ptr<Packet>)
      {
      }

      void Task::pullBuffer(Ptr<Packet>)
      {
      }

      void Task::setSource(std::string s){
        _source=s;
      }
      
      void Task::setSink(std::string s){
        _sink=s;
      }
      
      int Task::getPadTypes(){
        return Task::NOPAD;
      }
      
      void Task::setUUID(std::string uuid){
        _uuid=uuid;
      }
      
      std::string Task::getUUID(){
        return _uuid;
      }
      void Task::addStatusObserver( boost::function<void (Task*)> func){
        statusObserver=func;
      }
      
      void Task::addProgressObserver( boost::function<void (Task*)> func){
        progressObserver=func;      
      }

      void Task::cancel(){
        setStatus(CANCEL);
      }
      
      bool Task::isCanceled(){
        return _status==CANCEL;
      }
    }
  }
}
