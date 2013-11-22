#include "org/esb/util/Timer.h"
//#include "org/esb/hive/ProtocolCommand.h"

#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/io/FileInputStream.h"
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
#include "org/esb/db/hivedb.hpp"
#include "org/esb/config/config.h"
#include <map>
#include <string>
using namespace org::esb::hive::job;
using namespace org::esb::av;

using namespace org::esb::util;
using namespace org::esb::signal;
using namespace org::esb;
using org::esb::io::FileInputStream;

#define GET_UNIT  "get process_unit"
#define GET_AUDIO_UNIT  "get audio_process_unit"
#define PUT_AUDIO_UNIT  "put audio_process_unit"
#define PUT_UNIT  "put process_unit"

class VideoDataHandler : public org::esb::plugin::ProtocolCommand {
  private:
    classlogger("org.esb.hive.protocol.DataHandler")
    db::HiveDb _db;
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
    Ptr<db::ProcessUnit> _current_unit;

    std::string getMessageQueueName(){
      db::Queue queue = litesql::select<db::Queue > (_db, db::Queue::Qtype==db::Queue::Qtype.ONE2ONE).one();
      return queue.outputname;
    }

    Ptr<safmq::MessageQueue> getMessageQueue(std::string name){
      if(con->queueExist(name)){
        return con->getMessageQueue(name);
      }
      return Ptr<safmq::MessageQueue>();
    }

    void removeMessageQueue(std::string name){
      if(con->queueExist(name)){
        con->deleteQueue(name);
      }
    }

    void removeMessageQueueName(std::string name){
      db::Queue queue = litesql::select<db::Queue > (_db, db::Queue::Outputname==name).one();
      queue.del();
    }

    Ptr<safmq::MessageQueue> getMessageQueue(){
        std::string name=getMessageQueueName();
        Ptr<safmq::MessageQueue> mq=getMessageQueue(name);
        if(mq){
          return mq;
        }else{
          removeMessageQueueName(name);
        }
    }

    db::ProcessUnit getProcessUnit(){
      if(litesql::select<db::ProcessUnit > (_db, db::ProcessUnit::Send == 1).count()){
        _db.query("begin exclusive");
        db::ProcessUnit unit=litesql::select<db::ProcessUnit > (_db, db::ProcessUnit::Send == 1).orderBy(db::ProcessUnit::Id, true).one();
        unit.send=litesql::DateTime();
        unit.update();
        _db.query("end");
        return unit;
      }
      return  db::ProcessUnit(_db);
    }

  public:

    VideoDataHandler(InputStream * is, OutputStream * os, boost::asio::ip::tcp::endpoint e): _db(db::HiveDb("sqlite3", org::esb::config::Config::get("db.url"))) {
      _ep=e;
      _os=os;
      _is=is;
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
      if(_current_unit){
        _current_unit->send=1;
        _current_unit->update();
      }
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
        /*
        //partitionservice::PartitionManager * man = partitionservice::PartitionManager::getInstance();
        org::esb::mq::ObjectMessage msg;
        //Ptr<safmq::MessageQueue> mq=getMessageQueue();
        //mq
        //mq->Retrieve(true,1,msg);
        read_q->Retrieve(true,-1,msg);
        msg.getObject(un);
        //un = man->getProcessUnit(_ep);
        _oos->writeObject(un);
        */
        db::ProcessUnit unit=getProcessUnit();
        _current_unit=new db::ProcessUnit(unit);



        /*reading process units from the file system for delivery*/
        std::string base = org::esb::config::Config::get("hive.data_path");
        org::esb::io::File inputfile(base + "/"+unit.jobid+"/"+ unit.sendid);
        FileInputStream inputstream(&inputfile);
        std::string d;
        inputstream.read(d);
        /*
        litesql::Blob blob2=unit.data.value();
        d.reserve(blob2.length());
        LOGDEBUG("start copying bytes:"+StringUtil::toString(blob2.length()))
        for(int a=0;a<blob2.length();a++){
          char ch=blob2.data(a);
          d.push_back(ch);
        }
        LOGDEBUG("finish copy")
        */
        _os->write((char*) d.c_str(), d.length());
        _os->flush();

      } else {
        if (strcmp(command, PUT_UNIT) == 0) {

          string data;
          _is->read(data);


          std::string recvid=org::esb::util::PUUID();
          _current_unit->recvid=recvid;
          /*writing process units to the file system for delivery*/
          std::string base = org::esb::config::Config::get("hive.data_path");
          org::esb::io::File outputfile(base + "/"+_current_unit->jobid+"/"+ recvid);
          if(!File(outputfile.getParent()).exists()){
            File(outputfile.getParent()).mkdirs();
          }
          FileOutputStream outstream(&outputfile);
          outstream.write(data);
          outstream.close();

          /*
          litesql::Blob blob=litesql::Blob(data.c_str(),data.length());
          _current_unit->responseData=blob;
          */

          _current_unit->recv=litesql::DateTime();
          _current_unit->update();
        } else {
          LOGERROR("unknown command received:" << command);
        }
      }
    }

    void printHelp() {
    }
};
