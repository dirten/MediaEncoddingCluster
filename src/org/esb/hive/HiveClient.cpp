//#include "org/esb/net/TcpSocket.h"
//#include "org/esb/io/ObjectOutputStream.h"
//#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/net/TcpSocket.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/lang/Thread.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/shared_ptr.hpp>
#include "org/esb/av/FormatBaseStream.h"
//#include "org/esb/util/Log.h"
#include "HiveClient.h"
#include "org/esb/hive/protocol/PartitionHandler.h"
#include "org/esb/config/config.h"

//#include "Version.h"
namespace org {
  namespace esb {
    namespace hive {

      HiveClient::HiveClient(std::string host, int port) {
        _host = host;
        _port = port;
        _sock = NULL;
        _ois = NULL;
        _oos = NULL;
        _toHalt = false;
        _running = false;
        _sock = new org::esb::net::TcpSocket((char*) _host.c_str(), _port);
#ifdef USE_SAFMQ
        _qis = new QueueInputStream(_host, _port, "punitout");
        _qos = new QueueOutputStream(_host, _port, "punitin");
#endif
        org::esb::av::FormatBaseStream::initialize();
        //        avcodec_register_all();
        //        av_register_all();
        //        avcodec_init();

      }

      HiveClient::~HiveClient() {
        if (_sock)
          _sock->close();
        //delete _sock;
      }

      void HiveClient::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty<std::string>("hiveclient") == "start") {
          _t=boost::thread(boost::bind(&HiveClient::start, this));
          _running = true;
        } else
          if (msg.getProperty<std::string>("hiveclient") == "stop") {
          _toHalt = true;
          if (_running) {
            LOGDEBUG("StopSignal Received, waiting for all work done!");
            //_toHalt = true;
            //boost::mutex::scoped_lock terminationLock(terminationMutex);
            //ctrlCHit.wait(terminationLock);
            _t.join();
            LOGDEBUG("stopping done!")
          }
        }
      }

      void HiveClient::start() {
        _toHalt = false;
        connect();
        process();
      }

      void HiveClient::stop() {
        //        _toHalt=true;
      }

      void HiveClient::connect() {
        try {
          //delete _sock;
          _sock = new org::esb::net::TcpSocket((char*) _host.c_str(), _port);
          _sock->connect();
#ifdef USE_SAFMQ

          _qis = new QueueInputStream(_host, _port, "punitout");
          _qos = new QueueOutputStream(_host, _port, "punitin");
          _ois = new org::esb::io::ObjectInputStream(_qis.get());
          _oos = new org::esb::io::ObjectOutputStream(_qos.get());
#else
          _ois = new org::esb::io::ObjectInputStream(_sock->getInputStream());
          _oos = new org::esb::io::ObjectOutputStream(_sock->getOutputStream());
#endif
          std::string cmd = JOIN_PARTITION;
          _sock->getOutputStream()->write(cmd);
          _oos->writeObject(org::esb::config::Config::get("partition"));
          LOGINFO("Server " << _host << " connected!!!");
          std::cout << "Video Processor connected to Server " << _host << ":" << _port << std::endl;
        } catch (exception & ex) {
          LOGERROR("cant connect to \"" << _host << ":" << _port << "\"!!!" << ex.what());
          std::cout << "cant connect to \"" << _host << ":" << _port << "\"!!!" << ex.what() << std::endl;
          std::cout << "retry it in 5 seconds." << std::endl;
          //          logerror("cant connect to \"" << _host << ":" << _port << "\"!!!" << ex.what());
        }
      }

      void HiveClient::process() {
        //int pCount = 0;
        while (!_toHalt) {
          if (!_sock->isConnected()) {
            connect();
          } else {
            while (!_toHalt) {
              char * text = const_cast<char*> ("get process_unit");
              org::esb::hive::job::ProcessUnit * unit = new org::esb::hive::job::ProcessUnit();
              try {
                _sock->getOutputStream()->write(text, strlen(text));
                _ois->readObject(*unit);
              } catch (exception & ex) {
                LOGERROR("Connection to Server lost!!!" << ex.what());
                _sock->close();
              }
              if (unit->_input_packets.size() == 0) {
                delete unit;
                break;
              }
              unit->process();

              /**
               * clear the input packets, they are no more nedded
               * they only consumes Network bandwidth and cpu on the server
               */
              //unit->_input_packets.clear();

              char * text_out = const_cast<char*> ("put process_unit");
              try {
                _sock->getOutputStream()->write(text_out, strlen(text_out));
                _oos->writeObject(*unit);
              } catch (exception & ex) {
                LOGERROR("Connection to Server lost!!!" << ex.what());
                _sock->close();
              }
              /*
              delete unit->_decoder;
              unit->_decoder = NULL;
              delete unit->_encoder;
              unit->_encoder = NULL;*/
              delete unit->_converter;
              unit->_converter = NULL;
              delete unit;
              //_toHalt=true;

            }
          }
          org::esb::lang::Thread::sleep2(5000);
        }

        std::string cmd = LEAVE_PARTITION;
        _sock->getOutputStream()->write(cmd);
        _oos->writeObject(org::esb::config::Config::get("partition"));

        boost::mutex::scoped_lock terminationLock(terminationMutex);
        ctrlCHit.notify_all();
      }
    }
  }
}
