#include "HiveClient.h"
#include "org/esb/util/Log.h"
#include "org/esb/net/TcpSocket.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
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
			ipc::message_queue queue1(ipc::open_or_create,"in_pu_queue",1,10000000);
			ipc::message_queue queue2(ipc::open_or_create,"out_pu_queue",1,10000000);
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
//		 boost::thread t1(boost::bind(&HiveClient::packetReader, this));
//		 boost::thread t2(boost::bind(&HiveClient::packetWriter, this));
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
		

	  void HiveClient::packetReader(){
		  ipc::message_queue inQueue1(ipc::open_only,"in_pu_queue");
		  while(!_toHalt){	            
                char * text = "get process_unit";
				boost::shared_ptr<job::ProcessUnit> unitptr(new job::ProcessUnit());
                try{
				/*
					if(inQueue.get_max_msg()==inQueue.get_num_msg()){
						logdebug("Queue full wait 5 sec");
						org::esb::lang::Thread::sleep2(1000);
						continue;
					}
					*/
                    _sock->getOutputStream()->write(text, strlen(text));
					logdebug("Command sended");
					std::string t;
					_sock->getInputStream()->read(t);
//                    _ois->readObject(*unitptr);
					logdebug("ProcessUnit received");
					inQueue1.send(t.c_str(),t.size(),0);
//					inQueue.send(&unitptr,sizeof(boost::shared_ptr<job::ProcessUnit>),0);
					logdebug("ProcessUnit enqueued");
				}catch(ipc::interprocess_exception& ex ){
					logerror(ex.what());
//                    _sock->close();
                }
		  }
	  }
	  void HiveClient::packetWriter(){
		 ipc::message_queue outQueue1(ipc::open_only,"out_pu_queue");
		  while(!_toHalt){
            char * text_out = "put process_unit";
            try{
				org::esb::hive::job::ProcessUnit * unit=new job::ProcessUnit();
				std::size_t recvd_size;
				unsigned int priority;
				logdebug("outQueue pre dequeue");
				char * t=new char[10000000];
				outQueue1.receive(t,10000000, recvd_size, priority);
				logdebug("outQueue post dequeue");

//				_sock->getOutputStream()->write(text_out, strlen(text_out));
//				_sock->getOutputStream()->write(t, recvd_size);
				delete t;
//				_oos->writeObject(unit);
            }catch(...){
                logerror("Connection to Server lost!!!");
                _sock->close();
            }
		  }
	  }
		void HiveClient::process2() {
			logdebug("process2()");
			ipc::message_queue inQueue2(ipc::open_only,"in_pu_queue");
			ipc::message_queue outQueue2(ipc::open_only,"out_pu_queue");
			while(!_toHalt){	
				org::esb::hive::job::ProcessUnit * unit=new job::ProcessUnit();
				std::size_t recvd_size;
				unsigned int priority;
//				std::string t;
				char * t=new char[10000000];
				logdebug("inQueue pre dequeue");
				inQueue2.receive(t,10000000, recvd_size, priority);
				logdebug("inQueue post dequeue:"<<recvd_size);
				std::string data(t,recvd_size);
				delete t;
				istringstream archive_stream (data);
			    boost::archive::binary_iarchive archive (archive_stream);
				job::ProcessUnit un;
			    archive >> un;
				un.process();
				std::ostringstream archive_out_stream;
                boost::archive::binary_oarchive oarchive(archive_out_stream);
				oarchive << un;
				std::string _outbound_data = archive_out_stream.str();
				outQueue2.send(_outbound_data.c_str(),_outbound_data.size(),0);

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
		}
      void HiveClient::process() {
        int pCount = 0;
        while (!_toHalt) {
          if(!_sock->isConnected()){
            connect();
          }else{
              while (!_toHalt) {

                logdebug("ProcessLoop");

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
/*				
				org::esb::hive::job::ProcessUnit * unit=new job::ProcessUnit();;
				std::size_t recvd_size;
				unsigned int priority;

				inQueue.receive(unit,sizeof(job::ProcessUnit), recvd_size, priority);
*/
                if (unit._input_packets.size() == 0)break;
                try{
                    unit.process();
                }catch(...){
                    logerror("Error in process");
                }
//				outQueue.send(unit,sizeof(job::ProcessUnit),0);
				
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
		boost::mutex::scoped_lock terminationLock(terminationMutex);
        ctrlCHit.notify_all(); // should be just 1
      }
    }
  }
}
