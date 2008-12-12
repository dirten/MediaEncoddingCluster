#include "HiveClient.h"
#include "org/esb/util/Log.h"
#include "org/esb/net/TcpSocket.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/lang/Thread.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
//#include "Version.h"
namespace org {
  namespace esb {
    namespace hive {

      HiveClient::HiveClient(std::string host, int port) {
        _host = host;
        _port = port;
        _toHalt=false;
      }
      void HiveClient::onMessage(org::esb::signal::Message & msg) {
        if(msg.getProperty("hiveclient")=="start"){
          boost::thread t(boost::bind(&HiveClient::start, this));
        }else
        if(msg.getProperty("hiveclient")=="stop"){
          _toHalt=true;
          logdebug("StopSignal Received, waiting for all work done!")
          boost::mutex::scoped_lock terminationLock(terminationMutex);
          ctrlCHit.wait(terminationLock);
        }
      }
      
      void HiveClient::start() {
        _toHalt=false;
        connect();
        process();
      }
      
      void HiveClient::stop() {
//        _toHalt=true;
      }

      void HiveClient::connect() {
        try{
//        logdebug("Connecting to " << _host << " on port " << _port);
        _sock=new org::esb::net::TcpSocket ((char*) _host.c_str(), _port);
        _sock->connect();
        _ois=new org::esb::io::ObjectInputStream(_sock->getInputStream());
        _oos=new org::esb::io::ObjectOutputStream(_sock->getOutputStream());
        loginfo("Server "<<_host<<" connected!!!");
        }catch(...){
//          logerror("cant connect!!!");
        }
      }

      void HiveClient::process() {
        int pCount = 0;
        while (!_toHalt) {
          if(!_sock->isConnected()){
            connect();
          }else{
              while (!_toHalt) {
//                logdebug("ProcessLoop");
                char * text = "get process_unit";
                org::esb::hive::job::ProcessUnit unit;
                try{
                    _sock->getOutputStream()->write(text, strlen(text));
//                logdebug("Command sended");
                    _ois->readObject(unit);
//                logdebug("ProcessUnit received");
                }catch(...){
                    logerror("Connection to Server lost!!!");                
                    _sock->close();
                }
                if (unit._input_packets.size() == 0)break;
                try{
                    unit.process();
                }catch(...){
                    logerror("Error in process");
                }
                char * text_out = "put process_unit";
                try{
                _sock->getOutputStream()->write(text_out, strlen(text_out));
                _oos->writeObject(unit);
                }catch(...){
                    logerror("Connection to Server lost!!!");
                    _sock->close();
                }
                //		break;
              }
          }
          //    break;
          org::esb::lang::Thread::sleep2(5000);
        }
        ctrlCHit.notify_all(); // should be just 1
      }
    }
  }
}
