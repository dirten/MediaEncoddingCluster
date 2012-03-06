#include "org/esb/util/Timer.h"
#include "org/esb/hive/ProtocolCommand.h"

#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"


#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/util/StringUtil.h"
#include "boost/asio.hpp"
//#include "org/esb/net/TcpSocket.h"

#include "org/esb/signal/Message.h"
#include "org/esb/signal/Messenger.h"
#include "plugins/services/partitionservice/PartitionManager.h"
#include "org/esb/mq/QueueConnection.h"
#include "org/esb/mq/ObjectMessage.h"
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

class VideoDataHandler : public org::esb::plugin::ProtocolCommand {
  private:
    classlogger("org.esb.hive.protocol.DataHandler")
    InputStream * _is;
    OutputStream * _os;
    //	PacketOutputStream * _pos;
    io::ObjectOutputStream * _oos;
    io::ObjectInputStream * _ois;
    //  ClientHandler* _handler;

    std::string _own_id;
    //  boost::asio::io_service io_timer;
    //  boost::asio::deadline_timer timer;
    //  boost::shared_ptr<boost::thread> _timer_thread;
    //  boost::asio::deadline_timer t2;
    boost::shared_ptr<ProcessUnit> un;
    boost::asio::ip::tcp::endpoint _ep;
    bool shutdown;
    static std::map<std::string, boost::asio::ip::tcp::endpoint> _label_ep_map;
    Ptr<org::esb::mq::QueueConnection> con;
    Ptr<safmq::MessageQueue> read_q;
    Ptr<safmq::MessageQueue> write_q;


  public:

    VideoDataHandler(InputStream * is, OutputStream * os, boost::asio::ip::tcp::endpoint e) {
      _ep=e;
      _oos = new io::ObjectOutputStream(os);
      _ois = new io::ObjectInputStream(is);
      _own_id = e.address().to_string();
      _own_id += ":";
      _own_id += StringUtil::toString(e.port());
      shutdown = false;
      con=new org::esb::mq::QueueConnection("localhost", 20202);
      read_q=con->getMessageQueue("read_q");
      write_q=con->getMessageQueue("write_q");

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
    ~VideoDataHandler() {
      partitionservice::PartitionManager::getInstance()->getInstance()->leavePartition("", _ep);
      shutdown = true;
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
      strcmp(command, PUT_UNIT) == 0
      ) {
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
        //partitionservice::PartitionManager * man = partitionservice::PartitionManager::getInstance();
        org::esb::mq::ObjectMessage msg;
        read_q->Retrieve(true,-1,msg);
        msg.getObject(un);
        //un = man->getProcessUnit(_ep);
        _oos->writeObject(un);
      } else {
        if (strcmp(command, PUT_UNIT) == 0) {
          //un = boost::shared_ptr<ProcessUnit > (new ProcessUnit());
          _ois->readObject(un);
          org::esb::mq::ObjectMessage msg;
          msg.setObject(un);
          write_q->Enqueue(msg);
          //partitionservice::PartitionManager * man = partitionservice::PartitionManager::getInstance();
          //man->collectProcessUnit(un, _ep);
        } else {
          LOGERROR("unknown command received:" << command);
        }
      }
    }

    void printHelp() {
    }
};
