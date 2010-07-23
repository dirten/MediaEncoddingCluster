#include "Messenger.h"
#include "org/esb/util/Log.h"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
namespace org {
  namespace esb {

    namespace signal {
	  boost::mutex Messenger::request_mutex;
      std::map<std::string, Messenger*> Messenger::messenger;
      std::map<std::string, std::list<MessageListener*> > Messenger::listener;
      //std::string Messenger::DEFAULT_NAME="global";
      //std::map<std::string, std::list<int> > Messenger::listener;

      Messenger::Messenger(std::string name) {
        messenger[name] = this;
      }

      Messenger::~Messenger() {
        //  free();
      }

      void Messenger::freeListener( std::string name) {
        std::list<MessageListener*>::iterator l = (listener[name]).begin();
        for (; l != listener[name].end(); l++) {
          delete (*l);
        }
      }

      void Messenger::free() {
        std::map<std::string, Messenger*>::iterator it = messenger.begin();
        for (; it != messenger.end(); it++) {
          LOGDEBUG("Delete Messenger:" << (*it).first);
          delete (*it).second;
        }
      }

      Messenger & Messenger::getInstance(std::string name) {
		  boost::mutex::scoped_lock scoped_lock(request_mutex);
        Messenger * mess = messenger[name];
		if (mess == NULL){
		  mess = new Messenger(name);
		  messenger[name] = mess;
	    }
        return *mess;
      }

      void Messenger::addMessageListener(MessageListener & l, std::string name) {
        (listener[name]).push_back(&l);
      }

      void Messenger::removeMessageListener(MessageListener & listener, std::string name) {

      }

      void Messenger::sendMessage(Message & msg, std::string name) {
	boost::mutex::scoped_lock scoped_lock(request_mutex);
        std::list<MessageListener*>::iterator l = (listener[name]).begin();
        for (; l != listener[name].end(); l++) {
          boost::thread t(boost::bind(&MessageListener::onMessage, (*l), msg));
          //	(*l)->onMessage(msg);
        }
      }

      void Messenger::sendRequest(Message & msg, std::string name) {
	boost::mutex::scoped_lock scoped_lock(request_mutex);
        std::list<MessageListener*>::iterator l = (listener[name]).begin();
        for (; l != listener[name].end(); l++) {
          (*l)->onMessage(msg);
        }
      }

    }
  }
}
