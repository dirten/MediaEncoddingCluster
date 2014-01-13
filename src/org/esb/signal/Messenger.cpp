#include "Messenger.h"
#include "org/esb/util/Log.h"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
namespace org {
  namespace esb {
    namespace signal {
      boost::mutex Messenger::messenger_mutex;
      boost::mutex Messenger::request_mutex;
      std::map<std::string, Messenger*> Messenger::messenger;

      Messenger::Messenger(std::string name) {
        messenger[name] = this;
      }

      Messenger::~Messenger() {
        //  free();
      }

      void Messenger::freeListener(std::string name) {
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
          (*it).second=NULL;
        }
      }

      Messenger & Messenger::getInstance(std::string name) {
        boost::mutex::scoped_lock scoped_lock(messenger_mutex);
        Messenger * mess = messenger[name];
        if (mess == NULL) {
          mess = new Messenger(name);
          messenger[name] = mess;
        }
        return *mess;
      }

      void Messenger::addMessageListener(MessageListener & l, std::string name) {
        boost::mutex::scoped_lock scoped_lock(request_mutex);
        (listener[name]).push_back(&l);
      }

      void Messenger::removeMessageListener(MessageListener & l, std::string name) {
        boost::mutex::scoped_lock scoped_lock(request_mutex);
        (listener[name]).remove(&l);
      }

      void Messenger::sendMessage(Message & msg, std::string name) {
        boost::mutex::scoped_lock scoped_lock(request_mutex);
        std::list<MessageListener*>::iterator l = (listener[name]).begin();
        for (; l != listener[name].end(); l++) {
          boost::thread t(boost::bind(&MessageListener::onMessage, (*l), msg));
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
