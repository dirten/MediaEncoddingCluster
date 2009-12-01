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
  static std::list<boost::asio::ip::tcp::endpoint> endpoint2stream;
  boost::asio::ip::tcp::endpoint ep;
public:

  DataHandler(InputStream * is, OutputStream * os) {
    _is = is;
    _os = os;
    //	    _pos=new PacketOutputStream(_os);
    _oos = new io::ObjectOutputStream(_os);
    _ois = new io::ObjectInputStream(_is);
    //    _handler = new ClientHandler();
    ep = socket->getRemoteEndpoint();
    logdebug("endpoint:"<<ep);

  }

  ~DataHandler() {
    if (endpoint2stream.size()>0) {
        if (endpoint2stream.front() == ep) {
            endpoint2stream.pop_front();
        }
    }
  }

  DataHandler(TcpSocket * s) {
    socket = s;
    _is = socket->getInputStream();
    _os = socket->getOutputStream();
    _oos = new io::ObjectOutputStream(_os);
    _ois = new io::ObjectInputStream(_is);
    ep = socket->getRemoteEndpoint();
    logdebug("endpoint:"<<ep);
  }

  int isResponsible(cmdId & cmid) {
    return CMD_NA;
  }

  int isResponsible(char * command) {
    if (
        strcmp(command, GET_UNIT) == 0 ||
        strcmp(command, PUT_UNIT) == 0 ||
        strcmp(command, GET_AUDIO_UNIT) == 0||
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
        if (endpoint2stream.front() == ep) {
          un = ProcessUnitWatcher::getStreamProcessUnit();
        } else {
          un = boost::shared_ptr<ProcessUnit > (new ProcessUnit());
        }
      } else {
        un = ProcessUnitWatcher::getStreamProcessUnit();
        endpoint2stream.push_back(ep);
      }
      _oos->writeObject(*un.get());
    } else if (strcmp(command, PUT_AUDIO_UNIT) == 0) {
      ProcessUnit un;
      _ois->readObject(un);
      if (!ProcessUnitWatcher::putProcessUnit(un)) {
        logerror("error while putProcessUnit!");
      }      
    }else{                                                 
      logerror("unknown command received:" << command);
    }
  }

  void printHelp() {
  }
};
std::list<boost::asio::ip::tcp::endpoint> DataHandler::endpoint2stream;
