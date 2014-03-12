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
//#include "org/esb/mq/QueueConnection.h"
//#include "org/esb/mq/ObjectMessage.h"
#include "org/esb/db/hivedb.hpp"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/config/config.h"
#include <map>
#include <string>
#include <sstream>
using namespace org::esb::hive::job;
using namespace org::esb::av;

using namespace org::esb::util;
using namespace org::esb::signal;
using namespace org::esb;
using org::esb::io::FileInputStream;
using org::esb::signal::Message;
using org::esb::signal::Messenger;

#define GET_UNIT  "get process_unit"
#define GET_AUDIO_UNIT  "get audio_process_unit"
#define PUT_AUDIO_UNIT  "put audio_process_unit"
#define PUT_UNIT  "put process_unit"

class VideoDataHandler : public org::esb::plugin::ProtocolCommand {
  private:
    classlogger("org.esb.hive.protocol.VideoDataHandler")
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
    //Ptr<org::esb::mq::QueueConnection> con;
    //Ptr<safmq::MessageQueue> read_q;
    //Ptr<safmq::MessageQueue> write_q;
    Ptr<db::ProcessUnit> _current_unit;
    //litesql::DataSource<db::ProcessUnit> _prevEncodedUnits;


    /** @TODO:  1. select next ProcessUnit from database
     *          2. check if it is an Audio Unit
     *          3. when it is an audio packet then check if this client handler is responsible for this audio packet
     *          4. when it is not responsible then goto step 1. otherwise continue with the next step
     *          5. deliver it to the client
     *
     * explain the check of an audio packet to make a decision if this client is responsible
     * 1. if the column clientid is empty then allocate all audio units for this jobid and sourcestream for this clientid by updating the column clientid to this clientif of all rows for this jobid and sourcestream.
     * 2. if the clientid == this handler clientid then responsible=true
     */
    db::ProcessUnit getProcessUnit(){
      db::ProcessUnit result=db::ProcessUnit(_db);
      bool audioProcessunitReceived=false;
      bool responsible=false;

      std::string client_id=StringUtil::toString(_ep);
      _db.query("begin exclusive");
      litesql::DataSource<db::ProcessUnit> nextUnit=litesql::select<db::ProcessUnit > (_db, db::ProcessUnit::Send == 1);
      litesql::Cursor<db::ProcessUnit>unit_cursor=nextUnit.orderBy(db::ProcessUnit::Id, true).cursor();
      if(unit_cursor.rowsLeft()){
        try{
          do{
            result=*unit_cursor;
            LOGDEBUG("fetch next ProcessUnit:"<<result.codectype<<":id:"<<result.id)
            if(false && result.codectype==db::ProcessUnit::Codectype::AUDIO){

              if(((std::string)result.clientid).length()==0){
                _db.query("UPDATE processunit_ set clientid_='"+client_id+"' where jobid_='"+result.jobid+"' and codectype_='AUDIO' AND sorcestream_="+result.sorcestream);
                result.clientid=client_id;
              }
              if(((std::string)result.clientid).length()>0 && result.clientid==client_id){
                responsible=true;
              }else{
                unit_cursor++;
              }
            }else{
              responsible=true;
            }
          }while(!responsible && unit_cursor.rowsLeft());
        }catch(std::exception & ex){
          LOGERROR("exception:"<<ex.what())
        }

        if(result.id>0){
          result.send=litesql::DateTime();
          result.deliverycount=result.deliverycount+1;
          result.clientid=StringUtil::toString(_ep);
          result.update();
        }
      }
      _db.query("end");
      return result;
    }

  public:

    VideoDataHandler(InputStream * is, OutputStream * os, boost::asio::ip::tcp::endpoint e):
    _db(org::esb::hive::DatabaseService::getDatabase())
    {
      _ep=e;
      _os=os;
      _is=is;
      _oos = new io::ObjectOutputStream(os);
      _ois = new io::ObjectInputStream(is);
      _own_id = e.address().to_string();
      _own_id += ":";
      _own_id += StringUtil::toString(e.port());
      shutdown = false;

      LOGDEBUG("endpoint:" << e);
    }


    ~VideoDataHandler() {
      if(_current_unit){
        _current_unit->send=1;
        _current_unit->update();
        /*
        litesql::Records recs = _db.query("select jobid_, sorcestream_ from processunit_ WHERE clientid_='"+StringUtil::toString(_ep)+"' AND codectype_='AUDIO' group by jobid_, sorcestream_ having count(*)>sum(send_>1) order by 1 desc");
        for (litesql::Records::iterator i = recs.begin(); i != recs.end(); i++){
          _db.query("UPDATE processunit_ set clientid_='', send_=1, recv_=1 where jobid_='"+(*i)[0]+"' and codectype_='AUDIO' AND clientid_='"+StringUtil::toString(_ep)+"'");
        }*/
      }
      if(un){
        Message msg;
        msg.setProperty("processunit_rollback",un->uuid);
        Messenger::getInstance().sendRequest(msg);
      }
      shutdown = true;
      if (_oos)
        delete _oos;
      _oos = NULL;
      if (_ois)
        delete _ois;
      _ois = NULL;
    }

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

        Message msg;
        msg.setProperty("processunit_deque",std::string());
        Messenger::getInstance().sendRequest(msg);
        un=msg.getProperty<boost::shared_ptr<org::esb::hive::job::ProcessUnit> >("processunit_deque");
        std::ostringstream ost;
        Serializing::serialize<boost::archive::text_oarchive>(un, ost);

        _os->write((char*) ost.str().c_str(), ost.str().length());
        _os->flush();
        return;

        db::ProcessUnit unit=getProcessUnit();

        /*something is going wrong on the client, resetting the processUnit*/
        if(_current_unit){
          _current_unit->send=1;
          _current_unit->update();
        }
        _current_unit=new db::ProcessUnit(unit);


        std::string d;

        if(unit.id>0){
          /*reading process units from the file system for delivery*/
          std::string base = org::esb::config::Config::get("hive.data_path");
          org::esb::io::File inputfile(base + "/"+unit.jobid+"/"+ unit.sendid);
          if(inputfile.exists() && inputfile.isFile()){
            FileInputStream inputstream(&inputfile);
            inputstream.read(d);
          }else{
            LOGWARN("file not found:"<<inputfile.getFilePath())
            _current_unit->recv=litesql::DateTime();
            _current_unit->update();
            _current_unit.reset();
          }
        }else{
          _current_unit.reset();
        }
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
          std::istringstream iss (data);
          boost::shared_ptr<org::esb::hive::job::ProcessUnit>punit;
          Serializing::deserialize<boost::archive::text_iarchive>(punit, iss);
          Message msg;
          msg.setProperty("processunit_put",punit);
          Messenger::getInstance().sendRequest(msg);
          un.reset();
          return;
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
          return;
          /*
          litesql::Blob blob=litesql::Blob(data.c_str(),data.length());
          _current_unit->responseData=blob;
          */

          _current_unit->recv=litesql::DateTime();
          //_current_unit->update();
          /*
          Message msg;
          msg.setProperty("processunit_encoded",_current_unit->id.value());
          msg.setProperty("jobid",_current_unit->jobid.value());
          msg.setProperty("sequence",_current_unit->sequence.value());
          Messenger::getInstance().sendRequest(msg);
          */
          _current_unit.reset();
        } else {
          LOGERROR("unknown command received:" << command);
        }
      }
    }

    void printHelp() {
    }
};
