#include "org/esb/util/Timer.h"
#include "org/esb/hive/ProtocolCommand.h"

#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"


#include "../job/ProcessUnit.h"
#include "org/esb/util/StringUtil.h"
#include "boost/asio.hpp"
//#include "org/esb/net/TcpSocket.h"

#include "org/esb/signal/Message.h"
#include "org/esb/signal/Messenger.h"

#include <map>
#include <string>
using namespace org::esb::hive::job;
using namespace org::esb::av;

using namespace org::esb::util;
using namespace org::esb::signal;
using namespace org::esb;

#define GET_UNIT  "get process_unit"
#define GET_AUDIO_UNIT  "get audio_process_unit"
#define PUT_AUDIO_UNIT  "put audio_process_unit"
#define PUT_UNIT  "put process_unit"

class DataHandler : public org::esb::hive::ProtocolCommand {
private:
  classlogger("org.esb.hive.protocol.DataHandler")
  InputStream * _is;
  OutputStream * _os;
  //	PacketOutputStream * _pos;
  io::ObjectOutputStream * _oos;
  io::ObjectInputStream * _ois;
  //  ClientHandler* _handler;
  static std::list<std::string> endpoint2stream;
  static boost::mutex removeMutex;

  std::string _own_id;
  //  boost::asio::io_service io_timer;
  //  boost::asio::deadline_timer timer;
  //  boost::shared_ptr<boost::thread> _timer_thread;
  boost::shared_ptr<Timer> _timer;
  //  boost::asio::deadline_timer t2;
  boost::shared_ptr<ProcessUnit> un;
  bool shutdown;

  void remove_endpoint_from_stream(const boost::system::error_code & er) {
    boost::mutex::scoped_lock scoped_lock(removeMutex);
    LOGDEBUG("TimerEvent received");
    if (er == boost::asio::error::operation_aborted) {
      LOGDEBUG("Timer Event was Canceled");
      //    }else if(er ==boost::asio::error::shut_down){
      //      LOGDEBUG("org.esb.hive.protocol.DataHandler","Timer Event regular shutdown");
      //    }else if(er ==boost::asio::error::shut_down){
      //      LOGDEBUG("org.esb.hive.protocol.DataHandler","Timer Event regular shutdown");
    } else {
      LOGWARN("TimeOut received, removing endpoint from list to give an other client a chance!")
      if (er == boost::asio::error::shut_down)
        LOGDEBUG("Timer Event regular shutdown");
      if (endpoint2stream.size() > 0) {
        if (endpoint2stream.front() == _own_id) {
          endpoint2stream.pop_front();
          un.reset();
        }
      }
    }
  }

public:

  DataHandler(InputStream * is, OutputStream * os, boost::asio::ip::tcp::endpoint e) {
    _oos = new io::ObjectOutputStream(os);
    _ois = new io::ObjectInputStream(is);
    _own_id = e.address().to_string();
    _own_id += ":";
    _own_id += StringUtil::toString(e.port());
    shutdown = false;
    LOGDEBUG("endpoint:" << e);
  }
/*
  DataHandler(InputStream * is, OutputStream * os) {
    _is = is;
    _os = os;
    //    t = new boost::asio::deadline_timer(io_timer, boost::posix_time::seconds(20));
    //	    _pos=new PacketOutputStream(_os);
    _oos = new io::ObjectOutputStream(_os);
    _ois = new io::ObjectInputStream(_is);
    //    _handler = new ClientHandler();
    shutdown = false;
    //    timer.async_wait(boost::bind(&DataHandler::remove_endpoint_from_stream, this, boost::asio::error::operation_aborted));
    //    _timer_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_timer)));


  }
*/
  ~DataHandler() {
    shutdown = true;
    _timer.reset();
    remove_endpoint_from_stream(boost::asio::error::shut_down);
    if (_oos)
      delete _oos;
    _oos = NULL;
    if (_ois)
      delete _ois;
    _ois = NULL;
  }

    /*
    DataHandler(TcpSocket * s) {
      socket = s;
      _is = socket->getInputStream();
      _os = socket->getOutputStream();
      _oos = new io::ObjectOutputStream(_os);
      _ois = new io::ObjectInputStream(_is);
      boost::asio::ip::tcp::endpoint ep = socket->getRemoteEndpoint();
      _own_id = ep.address().to_string();
      _own_id += ":";
      _own_id += StringUtil::toString(ep.port());
      shutdown = false;
      //    timer.async_wait(boost::bind(&DataHandler::remove_endpoint_from_stream, this, boost::asio::error::operation_aborted));
      //    boost::thread thread(boost::bind(&boost::asio::io_service::run, &io_timer));
      //    _timer_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_timer)));
      //    io_timer.run();
      LOGDEBUG("endpoint:" << ep);
    }
   */
  int isResponsible(cmdId & cmid) {
    return CMD_NA;
  }

  int isResponsible(char * command) {
    if (
            strcmp(command, GET_UNIT) == 0 ||
            strcmp(command, PUT_UNIT) == 0 ||
            strcmp(command, GET_AUDIO_UNIT) == 0 ||
            strcmp(command, PUT_AUDIO_UNIT) == 0) {
      return CMD_PROCESS;
    } else
      if (strcmp(command, "help") == 0) {
      return CMD_HELP;
    }
    return CMD_NA;
  }

  void process(char * command) {
    if (_oos == NULL || _ois == NULL)return;
    if (strcmp(command, GET_UNIT) == 0) {
      Message msg;
      msg.setProperty("processunitcontroller", "GET_PROCESS_UNIT");
      Messenger::getInstance().sendRequest(msg);
      un = msg.getPtrProperty("processunit");
      if (un.get() != NULL)
        _oos->writeObject(*un.get());
    } else
      if (strcmp(command, PUT_UNIT) == 0) {
      un = boost::shared_ptr<ProcessUnit > (new ProcessUnit());
      _ois->readObject(*un.get());
      Message msg;
      msg.setProperty("processunitcontroller", "PUT_PROCESS_UNIT");
      msg.setProperty("processunit", un);
      Messenger::getInstance().sendRequest(msg);
    } else if (strcmp(command, GET_AUDIO_UNIT) == 0) {

      if (endpoint2stream.size() > 0) {
        if (endpoint2stream.front() == _own_id) {
          Message msg;
          msg.setProperty("processunitcontroller", "GET_AUDIO_PROCESS_UNIT");
          Messenger::getInstance().sendRequest(msg);
          un = msg.getPtrProperty("processunit");
        } else {
          un = boost::shared_ptr<ProcessUnit > (new ProcessUnit());
        }
      } else {
        LOGDEBUG("new client " << _own_id)
        Message msg;
        msg.setProperty("processunitcontroller", "GET_AUDIO_PROCESS_UNIT");
        Messenger::getInstance().sendRequest(msg);
        un = msg.getPtrProperty("processunit");
        endpoint2stream.push_back(_own_id);
      }
      if (un && un->_input_packets.size() > 0) {
        _timer.reset(new Timer(300, boost::bind(&DataHandler::remove_endpoint_from_stream, this, boost::asio::placeholders::error)));
      } else {
      }

      _oos->writeObject(*un.get());
    } else if (strcmp(command, PUT_AUDIO_UNIT) == 0) {
      un = boost::shared_ptr<ProcessUnit > (new ProcessUnit());
      _ois->readObject(*un.get());
      if (un.get() == NULL) {
        LOGDEBUG("ProcessUnit timed out, discard it");
        return;
      }
      if (_timer.get() != NULL)
        _timer.reset();
      Message msg;
      msg.setProperty("processunitcontroller", "PUT_PROCESS_UNIT");
      msg.setProperty("processunit", un);
      Messenger::getInstance().sendRequest(msg);
    } else {
      LOGERROR("unknown command received:" << command);
    }
  }

  void printHelp() {
  }
};
std::list<std::string> DataHandler::endpoint2stream;
boost::mutex DataHandler::removeMutex;
