#include "HiveClientAudio.h"
//#include "org/esb/net/TcpSocket.h"
//#include "org/esb/io/ObjectOutputStream.h"
//#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/lang/Thread.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/shared_ptr.hpp>
#include "org/esb/util/Log.h"


//#include "Version.h"
namespace org {
  namespace esb {
    namespace hive {

      HiveClientAudio::HiveClientAudio(std::string host, int port) {
        _host = host;
        _port = port;
        _sock = NULL;
        _ois = NULL;
        _oos = NULL;
        _toHalt = false;
        _running = false;
        _conv = NULL;
        _swap_codecs = false;
        _sock = new org::esb::net::TcpSocket((char*) _host.c_str(), _port);
        avcodec_register_all();
        av_register_all();
        //        avcodec_init();

      }

      HiveClientAudio::~HiveClientAudio() {
        delete _ois;
        delete _oos;
        if (_sock)
          _sock->close();
        delete _sock;
      }

      void HiveClientAudio::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty("hiveclientaudio") == "start") {
          boost::thread t(boost::bind(&HiveClientAudio::start, this));
          _running = true;
        } else
          if (msg.getProperty("hiveclientaudio") == "stop") {
          _toHalt = true;
          if (_running) {
            logdebug("StopSignal Received, waiting for all work done!")
            boost::mutex::scoped_lock terminationLock(terminationMutex);
            ctrlCHit.wait(terminationLock);
          }
        }
      }

      void HiveClientAudio::start() {
        _toHalt = false;
        connect();
        process();
      }

      void HiveClientAudio::stop() {
        //        _toHalt=true;
      }

      void HiveClientAudio::connect() {
        try {
          _sock->connect();
          _ois = new org::esb::io::ObjectInputStream(_sock->getInputStream());
          _oos = new org::esb::io::ObjectOutputStream(_sock->getOutputStream());
          loginfo("Server " << _host << " connected!!!");
        } catch (exception & ex) {
          logerror("cant connect to \"" << _host << ":" << _port << "\"!!!" << ex.what());
        }
      }

      void HiveClientAudio::process() {
        int pCount = 0;
        while (!_toHalt) {
          if (!_sock->isConnected()) {
            connect();
          } else {
            while (!_toHalt) {
              char * text = "get audio_process_unit";
              org::esb::hive::job::ProcessUnit * unit = new org::esb::hive::job::ProcessUnit();
              try {
                _sock->getOutputStream()->write(text, strlen(text));
                logdebug("sended command");
                _ois->readObject(*unit);
                logdebug("pu received");
              } catch (exception & ex) {
                logerror("Connection to Server lost, while waiting for audio process unit!!!" << ex.what());
                _sock->close();
              }
              if (unit->_input_packets.size() == 0) {
                //                delete _dec;
                //                delete _enc;
                delete unit;
                break;
              }
              if (_swap_codecs) {
                unit->_decoder = _dec;
              } else {
                _dec = unit->_decoder;
              }
              if (_swap_codecs) {
                unit->_encoder = _enc;
              } else {
                _enc = unit->_encoder;
              }

              if (_conv != NULL) {
                unit->_converter = _conv;
              }
              _swap_codecs = true;
              unit->process();
//              Thread::sleep2(20000);
              if (_conv == NULL) {
                _conv = unit->_converter;
              }
              /**
               * clear the input packets, there are no more nedded
               * they only consumes Network bandwidth and cpu on the server
               */
              unit->_input_packets.clear();

              char * text_out = "put audio_process_unit";
              try {
                _sock->getOutputStream()->write(text_out, strlen(text_out));
                _oos->writeObject(*unit);
              } catch (exception & ex) {
                logerror("Connection to Server lost, while sending audio process unit!!!" << ex.what());
                _sock->close();
              }
              if (unit->_last_process_unit) {
                _swap_codecs = false;
                if (_conv)
                  delete _conv;
                _conv = NULL;
              }

              delete unit;
            }
          }
          org::esb::lang::Thread::sleep2(5000);
        }
        boost::mutex::scoped_lock terminationLock(terminationMutex);
        ctrlCHit.notify_all();
      }
    }
  }
}
