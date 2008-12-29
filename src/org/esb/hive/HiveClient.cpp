#include "HiveClient.h"
#include "org/esb/util/Log.h"
#include "org/esb/net/TcpSocket.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"

#include "org/esb/lang/Thread.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>


//#include "Version.h"
namespace org {
  namespace esb {
    namespace hive {

		HiveClient::HiveClient(std::string host, int port)
		{
        _host = host;
        _port = port;
        _toHalt=false;
      }
		HiveClient::~HiveClient(){
//			ipc::message_queue::remove("in_pu_queue");
//			ipc::message_queue::remove("out_pu_queue");
		}
      void HiveClient::onMessage(org::esb::signal::Message & msg) {
//			ipc::message_queue::remove("in_pu_queue");
//		  ipc::message_queue::remove("out_pu_queue");
		  if(msg.getProperty("hiveclient")=="start"){
          boost::thread t(boost::bind(&HiveClient::start, this));
        }else
        if(msg.getProperty("hiveclient")=="stop"){
//			ipc::message_queue::remove("in_pu_queue");
//			ipc::message_queue::remove("out_pu_queue");

          _toHalt=true;
          logdebug("StopSignal Received, waiting for all work done!")
          boost::mutex::scoped_lock terminationLock(terminationMutex);
          ctrlCHit.wait(terminationLock);
		  
        }
      }
      
      void HiveClient::start() {
        _toHalt=false;
        connect();
		 boost::thread t1(boost::bind(&HiveClient::packetReader, this));
		 boost::thread t2(boost::bind(&HiveClient::packetWriter, this));
        process2();
      }
      
      void HiveClient::stop() {
//        _toHalt=true;
      }

      void HiveClient::connect() {
        try{
//        logdebug("Connecting to " << _host << " on port " << _port);
        _insock=new org::esb::net::TcpSocket ((char*) _host.c_str(), _port);
        _outsock=new org::esb::net::TcpSocket ((char*) _host.c_str(), _port);
        _insock->connect();
        _outsock->connect();
        _ois=new org::esb::io::ObjectInputStream(_insock->getInputStream());
        _oos=new org::esb::io::ObjectOutputStream(_outsock->getOutputStream());
        loginfo("Server "<<_host<<" connected!!!");
        }catch(...){
//          logerror("cant connect!!!");
        }
      }
		

	  void HiveClient::packetReader(){
		  logdebug("HiveClient::packetReader");
		  while(!_toHalt){	            
                char * text = "get process_unit";
				boost::shared_ptr<job::ProcessUnit> unitptr(new job::ProcessUnit());
//				job::ProcessUnit * unit=NULL;//new job::ProcessUnit();
//                try{
					boost::mutex::scoped_lock queue_lock(thread_read_mutex);
                    _insock->getOutputStream()->write(text, strlen(text));
					logdebug("Command sended");
                    _ois->readObject(*unitptr);
					logdebug("ProcessUnit received");
					inQueue.enqueue(unitptr);
					logdebug("ProcessUnit enqueued");
//				}catch(ipc::interprocess_exception& ex ){
//					logerror(ex.what());
//                    _sock->close();
//                }
		  }
	  }
	  void HiveClient::packetWriter(){
		  logdebug("HiveClient::packetWriter");
		  while(!_toHalt){
            char * text_out = "put process_unit";
//            try{
				boost::mutex::scoped_lock queue_lock(thread_write_mutex);
				boost::shared_ptr<job::ProcessUnit> unitptr(new job::ProcessUnit());
//				org::esb::hive::job::ProcessUnit * unit=NULL;//=new job::ProcessUnit();
				logdebug("outQueue pre dequeue");
				outQueue.dequeue(unitptr);
				logdebug("outQueue post dequeue");
				_outsock->getOutputStream()->write(text_out, strlen(text_out));
				_oos->writeObject(*unitptr);
//            }catch(...){
//                logerror("Connection to Server lost!!!");
//                _sock->close();
//            }
		  }
	  }
		void HiveClient::process2() {
			logdebug("process2()");
			while(!_toHalt){	
//				org::esb::hive::job::ProcessUnit * unit=new org::esb::hive::job::ProcessUnit();
//				boost::shared_ptr<job::ProcessUnit> unitptr(new job::ProcessUnit());
//				org::esb::lang::Thread::sleep2(1000);
				logdebug("inQueue pre dequeue");
				boost::shared_ptr<job::ProcessUnit> unitptr = inQueue.dequeue();
				logdebug("inQueue post dequeue:");
				unitptr->process();
				outQueue.enqueue(unitptr);

/*
                if (unit._input_packets.size() == 0)break;
                try{
                    unit.process();
                }catch(...){
                    logerror("Error in process");
                }
*/				
//				outQueue.send(unit,sizeof(job::ProcessUnit),0);
			}
			boost::mutex::scoped_lock terminationLock(terminationMutex);
	        ctrlCHit.notify_all(); // should be just 1
		}
      void HiveClient::process() {
        int pCount = 0;
        while (!_toHalt) {
          if(!_insock->isConnected()||!_outsock->isConnected()){
            connect();
          }else{
              while (!_toHalt) {

                logdebug("ProcessLoop");

				  char * text = "get process_unit";
                org::esb::hive::job::ProcessUnit * unit=new org::esb::hive::job::ProcessUnit();
                try{
                    _insock->getOutputStream()->write(text, strlen(text));
//                logdebug("Command sended");
                    _ois->readObject(*unit);
//                logdebug("ProcessUnit received");
                }catch(...){
                    logerror("Connection to Server lost!!!");                
                    _insock->close();
                }
/*				
				org::esb::hive::job::ProcessUnit * unit=new job::ProcessUnit();;
				std::size_t recvd_size;
				unsigned int priority;

				inQueue.receive(unit,sizeof(job::ProcessUnit), recvd_size, priority);
*/
                if (unit->_input_packets.size() == 0)break;
                try{
                    unit->process();
                }catch(...){
                    logerror("Error in process");
                }
//				outQueue.send(unit,sizeof(job::ProcessUnit),0);
				
                char * text_out = "put process_unit";
                try{
                _outsock->getOutputStream()->write(text_out, strlen(text_out));
                _oos->writeObject(*unit);
				delete unit;
                }catch(...){
                    logerror("Connection to Server lost!!!");
                    _outsock->close();
                }
				
                //		break;
              }
          }
          //    break;
          org::esb::lang::Thread::sleep2(5000);
        }
		boost::mutex::scoped_lock terminationLock(terminationMutex);
        ctrlCHit.notify_all(); // should be just 1
      }
    }
  }
}
