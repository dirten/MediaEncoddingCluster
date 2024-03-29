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
#include "org/esb/av/FormatBaseStream.h"


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
        _sock = NULL; //new org::esb::net::TcpSocket((char*) _host.c_str(), _port);
        org::esb::av::FormatBaseStream::initialize();
        //        avcodec_register_all();
        //        av_register_all();
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
        if (msg.getProperty<std::string > ("hiveclientaudio") == "start") {
          _t = boost::thread(boost::bind(&HiveClientAudio::start, this));
          _running = true;
        } else
          if (msg.getProperty<std::string > ("hiveclientaudio") == "stop") {
          stop();
        }
      }

      void HiveClientAudio::start() {
        _toHalt = false;
        connect();
        process();
      }

      void HiveClientAudio::stop() {
        _toHalt = true;
        if (_running) {
          LOGDEBUG("StopSignal Received, waiting for all work done!");
          _t.join();
          LOGDEBUG("stopping done!")
        }
      }

      void HiveClientAudio::connect() {
        try {
          delete _sock;
          _sock = new org::esb::net::TcpSocket((char*) _host.c_str(), _port);
          _sock->connect();
          delete _ois;
          delete _oos;
          _ois = new org::esb::io::ObjectInputStream(_sock->getInputStream());
          _oos = new org::esb::io::ObjectOutputStream(_sock->getOutputStream());
          std::cout << "Audio Processor connected to Server " << _host << ":" << _port << std::endl;
          LOGINFO("Server " << _host << " connected!!!");
        } catch (exception & ex) {
          LOGERROR("cant connect to \"" << _host << ":" << _port << "\"!!!" << ex.what());
          std::cout << "cant connect to \"" << _host << ":" << _port << "\"!!!" << ex.what() << std::endl;
          std::cout << "retry it in 5 seconds." << std::endl;
        }
      }

      void HiveClientAudio::process() {
        //int pCount = 0;
        while (!_toHalt) {
          if (!_sock->isConnected()) {
            connect();
          } else {
            while (!_toHalt) {
              char * text = const_cast<char*> ("get audio_process_unit");
              org::esb::hive::job::ProcessUnit * unit = new org::esb::hive::job::ProcessUnit();
              try {
                _sock->getOutputStream()->write(text, strlen(text));
                //LOGDEBUG("sended command");
                _ois->readObject(*unit);
                //LOGDEBUG("pu received");
              } catch (exception & ex) {
                LOGERROR("Connection to Server lost, while waiting for audio process unit!!!" << ex.what());
                _sock->close();
              }
              if (unit->_input_packets.size() == 0) {
                //                delete _dec;
                //                delete _enc;
                delete unit;
                break;
              }
              if (_swap_codec_list.find(unit->_source_stream) == _swap_codec_list.end()) {
                _swap_codec_list[unit->_source_stream] = false;
              }

              if (_swap_codec_list[unit->_source_stream]) {
                unit->_decoder = _decoder_list[unit->_source_stream];
                unit->_encoder = _encoder_list[unit->_target_stream];
                unit->_converter = _converter_list[unit->_target_stream];
              }
              _swap_codec_list[unit->_source_stream] = true;

              unit->process();

              if (_swap_codec_list[unit->_source_stream]) {
                _decoder_list[unit->_source_stream] = unit->_decoder;
                _encoder_list[unit->_target_stream] = unit->_encoder;
                _converter_list[unit->_target_stream] = unit->_converter;
              }

              /**
               * clear the input packets, there are no more nedded
               * they only consumes Network bandwidth and cpu on the server
               */
              //unit->_input_packets.clear();

              char * text_out = const_cast<char*> ("put audio_process_unit");
              try {
                _sock->getOutputStream()->write(text_out, strlen(text_out));
                _oos->writeObject(*unit);
              } catch (exception & ex) {
                LOGERROR("Connection to Server lost, while sending audio process unit!!!" << ex.what());
                _sock->close();
              }
              if (unit->_last_process_unit) {
                LOGDEBUG("Last ProcessUnit for Audio received, clear out");
                _swap_codec_list.clear();
                _decoder_list.clear();
                _encoder_list.clear();
                std::map<int, org::esb::av::FrameConverter * >::iterator it = _converter_list.begin();
                for (; it != _converter_list.end(); it++) {
                  delete (*it).second;
                }
                _converter_list.clear();
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
