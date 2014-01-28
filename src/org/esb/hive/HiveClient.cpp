//#include "org/esb/net/TcpSocket.h"
//#include "org/esb/io/ObjectOutputStream.h"
//#include "org/esb/io/ObjectInputStream.h"
#include "HiveClient.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/lang/Thread.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/shared_ptr.hpp>
#include "org/esb/av/FormatBaseStream.h"
//#include "org/esb/util/Log.h"
#include "org/esb/hive/protocol/PartitionHandler.h"
#include "org/esb/config/config.h"
#include "org/esb/net/SocketException.h"
#include "org/esb/util/StringUtil.h"
//#define USE_SAFMQ
//#include "Version.h"
namespace org {
  namespace esb {
    namespace hive {
  using org::esb::util::StringUtil;
      HiveClient::HiveClient(std::string host, int port) {
        LOGDEBUG("start client")
        _host = host;
        _port = port;
        _sock = NULL;
        _ois = NULL;
        _oos = NULL;
        _toHalt = false;
        _running = false;
        _sock = new org::esb::net::TcpSocket((char*) _host.c_str(), _port);
#ifdef USE_SAFMQ
        //_qis = new QueueInputStream(_host, _port, "punitout");
        //_qos = new QueueOutputStream(_host, _port, "punitin");
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
        LOGDEBUG("message receiver")
        if (msg.getProperty<std::string > ("hiveclient") == "start") {
          LOGDEBUG("start message received")
          _t = boost::thread(boost::bind(&HiveClient::start, this));
        } else
        if (msg.getProperty<std::string > ("hiveclient") == "stop") {
          LOGDEBUG("stop message received")
          stop();
        }
      }

      void HiveClient::start() {
        if(!_running){
          _toHalt = false;
          _running = true;
          connect();
          process();
        }
      }

      void HiveClient::stop() {
        return;
        _toHalt = true;
        if (_running) {
          LOGDEBUG("StopSignal Received, waiting for all work done!");
          //_t.join();
          boost::mutex::scoped_lock terminationLock(terminationMutex);
          ctrlCHit.wait(terminationLock);
          LOGDEBUG("stopping done!")
        }
      }

      void HiveClient::connect() {
        LOGDEBUG("connect")
        try {
          //delete _sock;
          _sock = new org::esb::net::TcpSocket((char*) _host.c_str(), _port);
          _sock->connect();
#ifdef USE_SAFMQ

          //_qis = new QueueInputStream(_host, 20202, "read_q");
          //_qos = new QueueOutputStream(_host, 20202, "write_q");
          _ois = new org::esb::io::ObjectInputStream(_qis.get());
          _oos = new org::esb::io::ObjectOutputStream(_qos.get());
#else
          _ois = new org::esb::io::ObjectInputStream(_sock->getInputStream());
          _oos = new org::esb::io::ObjectOutputStream(_sock->getOutputStream());
#endif
          //std::string cmd = JOIN_PARTITION;
          //_sock->getOutputStream()->write(cmd);
          //_oos->writeObject(org::esb::config::Config::get("partition"));
          LOGINFO("Server " << _host << " connected!!!");
          std::cout << "Processor connected to Server " << _host << ":" << _port << std::endl;
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
              // = new org::esb::hive::job::ProcessUnit();

              try {
                LOGDEBUG("send command :"+std::string(text))
                _sock->getOutputStream()->write(text, strlen(text));
                LOGDEBUG("read object")
                _ois->readObject(unit);
                LOGDEBUG("object received")
              } catch (exception & ex) {
                LOGERROR("failed to get ProcessUnit:" << ex.what());
                //break;
                //if(_sock->isClosed())
                  _sock->close();
              }
              if (!unit || unit->_input_packets.size() == 0) {
                break;
              }
              try {
                processUnit(unit);
              } catch (std::exception & ex) {
                //std::cout << "failed process unit:"<<unit->uuid<<std::endl;
                LOGERROR("processUnit(unit):" << ex.what());
              }
              /**
               * clear the input packets, they are no more nedded
               * they only consumes Network bandwidth and cpu on the server
               */
              unit->_input_packets.clear();

              char * text_out = const_cast<char*> ("put process_unit");
              try {
                _sock->getOutputStream()->write(text_out, strlen(text_out));
                _oos->writeObject(unit);
              } catch (exception & ex) {
                LOGERROR("Connection to Server lost!!!" << ex.what());
                _sock->close();
              }
              unit.reset();
              /*
              delete unit->_decoder;
              unit->_decoder = NULL;
              delete unit->_encoder;
              unit->_encoder = NULL;*/
              //delete unit->_converter;
              //unit->_converter = NULL;
              //delete unit;
              //_toHalt=true;

            }
          }
          if (!_toHalt)
            org::esb::lang::Thread::sleep2(5000);
        }
        /*
        std::string cmd = LEAVE_PARTITION;
        try {
          _sock->getOutputStream()->write(cmd);
          _oos->writeObject(org::esb::config::Config::get("partition"));
        } catch (org::esb::net::SocketException & ex) {
          LOGDEBUG("error while leaving partition:" << ex.what());
        }
        */
        boost::mutex::scoped_lock terminationLock(terminationMutex);
        ctrlCHit.notify_all();
      }

      void HiveClient::processUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit> unit) {
        if (!unit || unit->_input_packets.size() == 0) {
          //delete unit;
          return;
        }
        /*building single stream id, a combination from source_stream and job uuid*/
        std::string stream_id=unit->getJobId();
        stream_id+=StringUtil::toString(unit->_source_stream);

        if (false&&unit->_decoder->getCodecType() == AVMEDIA_TYPE_AUDIO) {


          LOGDEBUG("special handling audio")
          if (_swap_codec_list.find(stream_id) == _swap_codec_list.end()) {
            _swap_codec_list[stream_id] = false;
          }

          if (_swap_codec_list[stream_id]) {
            LOGDEBUG("Swapping codec for audio encoding");
            if (_decoder_list[stream_id])
              unit->_decoder = _decoder_list[stream_id];
            if (_encoder_list[stream_id])
              unit->_encoder = _encoder_list[stream_id];
            if (_converter_list[stream_id])
              unit->_converter = _converter_list[stream_id];
          } else {
            LOGDEBUG("audio no swap!!!");
          }
          _swap_codec_list[stream_id] = true;
        } else {
          LOGDEBUG("no audio codec");
        }
        unit->process();
        if (false&&unit->_decoder->getCodecType() == AVMEDIA_TYPE_AUDIO) {
          if (_swap_codec_list[stream_id]) {
            LOGDEBUG("swap back to hold the data");
            _decoder_list[stream_id] = unit->_decoder;
            _encoder_list[stream_id] = unit->_encoder;
            _converter_list[stream_id] = unit->_converter;
          }
        }
        if (false&&unit->_decoder->getCodecType() == AVMEDIA_TYPE_AUDIO) {

          if (unit->_last_process_unit) {
            LOGDEBUG("Last ProcessUnit for Audio received, clear out");
            _swap_codec_list.clear();
            _decoder_list.clear();
            _encoder_list.clear();
            std::map<std::string, org::esb::av::FrameConverter * >::iterator it = _converter_list.begin();
            for (; it != _converter_list.end(); it++) {
              delete (*it).second;
            }
            _converter_list.clear();
          }
        }
      }
    }
  }
}
