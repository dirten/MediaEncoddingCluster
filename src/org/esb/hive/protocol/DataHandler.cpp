#include "../ProtocolCommand.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/CodecOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/av/Packet.h"
#include "../job/Job.h"
#include "../job/JobHandler.h"
#include "../job/ProcessUnitWatcher.h"
#include "../job/ProcessUnit.h"
#include "org/esb/util/StringUtil.h"

#include "org/esb/net/TcpSocket.h"

#include <map>
#include <string>
using namespace org::esb::hive::job;
using namespace org::esb::av;
using namespace org::esb::net;
using namespace org::esb;


#define GET_UNIT  "get process_unit"
#define GET_AUDIO_UNIT  "get audio_process_unit"
#define PUT_AUDIO_UNIT  "put audio_process_unit"
#define PUT_UNIT  "put process_unit"

class DataHandler : public ProtocolCommand {
private:
  InputStream * _is;
  OutputStream * _os;
  //	PacketOutputStream * _pos;
  io::ObjectOutputStream * _oos;
  io::ObjectInputStream * _ois;
  //  ClientHandler* _handler;
  static std::list<std::string> endpoint2stream;

  std::string _own_id;
  boost::asio::io_service io_timer;
  boost::asio::deadline_timer t;
  //  boost::asio::deadline_timer t2;
  void dummy_timeout(const boost::system::error_code & er) {

  }

  void remove_endpoint_from_stream(const boost::system::error_code & er) {
    logdebug("TimerEvent received");
    if (er == boost::asio::error::operation_aborted) {
      logdebug("Timer Event was Canceled");
      return;
    }
    logdebug("TimeOut received, removing endpoint from list to give an other client a chance!")
    if (endpoint2stream.size() > 0) {
      if (endpoint2stream.front() == _own_id) {
        endpoint2stream.pop_front();
      }
    }
  }
public:

  DataHandler(InputStream * is, OutputStream * os) : t(io_timer) {
    _is = is;
    _os = os;
    //    t = new boost::asio::deadline_timer(io_timer, boost::posix_time::seconds(20));
    //	    _pos=new PacketOutputStream(_os);
    _oos = new io::ObjectOutputStream(_os);
    _ois = new io::ObjectInputStream(_is);
    //    _handler = new ClientHandler();
    boost::asio::ip::tcp::endpoint ep = socket->getRemoteEndpoint();
    _own_id = ep.address().to_string();
    _own_id += ":";
    _own_id += StringUtil::toString(ep.port());
    logdebug("endpoint:" << ep);
    t.async_wait(boost::bind(&DataHandler::remove_endpoint_from_stream, this,  boost::asio::error::operation_aborted));
    //    io_timer.run();
    boost::thread thread(boost::bind(&boost::asio::io_service::run, &io_timer));


  }

  ~DataHandler() {
    io_timer.stop();
    if (endpoint2stream.size() > 0) {
      if (endpoint2stream.front() == _own_id) {
        logdebug("remove me from endpoint list for audio encoding");
        endpoint2stream.pop_front();
      }
    }
  }

  DataHandler(TcpSocket * s) : t(io_timer) {
    socket = s;
    _is = socket->getInputStream();
    _os = socket->getOutputStream();
    _oos = new io::ObjectOutputStream(_os);
    _ois = new io::ObjectInputStream(_is);
    boost::asio::ip::tcp::endpoint ep = socket->getRemoteEndpoint();
    _own_id = ep.address().to_string();
    _own_id += ":";
    _own_id += StringUtil::toString(ep.port());
    t.async_wait(boost::bind(&DataHandler::remove_endpoint_from_stream, this,  boost::asio::error::operation_aborted));
    boost::thread thread(boost::bind(&boost::asio::io_service::run, &io_timer));

    //    io_timer.run();
    logdebug("endpoint:" << ep);
  }

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
    if (strcmp(command, GET_UNIT) == 0) {
      boost::shared_ptr<ProcessUnit> un = ProcessUnitWatcher::getProcessUnit();
      _oos->writeObject(*un.get());
    } else
      if (strcmp(command, PUT_UNIT) == 0) {
      ProcessUnit un;
      _ois->readObject(un);
      if (!ProcessUnitWatcher::putProcessUnit(un)) {
        logerror("error while putProcessUnit!");
      }
    } else if (strcmp(command, GET_AUDIO_UNIT) == 0) {
      boost::shared_ptr<ProcessUnit> un;
      if (endpoint2stream.size() > 0) {
        if (endpoint2stream.front() == _own_id) {
          un = ProcessUnitWatcher::getStreamProcessUnit();
        } else {
          un = boost::shared_ptr<ProcessUnit > (new ProcessUnit());
        }
      } else {
        un = ProcessUnitWatcher::getStreamProcessUnit();
        endpoint2stream.push_back(_own_id);
      }
      if (un->_input_packets.size() > 0) {
        logdebug("setting timer");
        t.expires_from_now(boost::posix_time::seconds(10));
        t.async_wait(boost::bind(&DataHandler::remove_endpoint_from_stream, this, boost::asio::placeholders::error));
      } else {
        logdebug("dummy audio packet");
      }

      _oos->writeObject(*un.get());
    } else if (strcmp(command, PUT_AUDIO_UNIT) == 0) {
      ProcessUnit un;
      _ois->readObject(un);
      t.cancel();
      if (!ProcessUnitWatcher::putProcessUnit(un)) {
        logerror("error while putProcessUnit!");
      }
    } else {
      logerror("unknown command received:" << command);
    }
  }

  void printHelp() {
  }
};
std::list<std::string> DataHandler::endpoint2stream;
