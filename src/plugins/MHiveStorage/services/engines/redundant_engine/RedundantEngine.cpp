#include "RedundantEngine.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/Serializing.h"
#include "mongo/bson/bsonobjbuilder.h"
using org::esb::util::StringTokenizer;
using Poco::Net::HTTPClientSession;
using org::esb::util::Serializing;
namespace mhivestorage{
  namespace engines {
    //typedef boost::archive::text_oarchive oarchive;
    //typedef boost::archive::text_iarchive iarchive;
    typedef boost::archive::binary_oarchive oarchive;
    typedef boost::archive::binary_iarchive iarchive;

    RedundantEngine::RedundantEngine(db::HiveDb database, std::string storage_path, std::vector<std::string> hosts, int self_port) : Simple(database, storage_path)
    {
      mongo::Status state= mongo::client::initialize();
      if(!state.isOK()){
        std::cout << "could not initialize mongo driver"<<std::endl;
      }
      std::vector<mongo::HostAndPort>rs_hosts;
      foreach(std::string host, hosts){
        rs_hosts.push_back(mongo::HostAndPort(host));
      }
      dbcrs=new mongo::DBClientReplicaSet("first", rs_hosts);
      dbcrs->connect();

      gridfs=new mongo::GridFS(*dbcrs, "mhive");

      //c=new mongo::DBClientConnection(true, dbcrs);
       //c->connect();
      //LOGDEBUG("Mongo Connected:"<<c->isStillConnected());

    }

    void RedundantEngine::put(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)
    {
      Simple::put(unit);
    }

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> RedundantEngine::get()
    {
      return Simple::get();
    }

    void RedundantEngine::enque(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)
    {
      boost::mutex::scoped_lock enqueue_lock(enqueue_mutex);

      mongo::BSONObjBuilder b;

      b.append("sourcestream",unit->_source_stream);
      b.append("targetstream",unit->_target_stream);
      b.append("send",0);
      b.append("sendid",unit->uuid);
      b.append("jobid",unit->getJobId());
      b.append("sequence",unit->_sequence);


      mongo::BSONObj p=b.obj();

      std::ostringstream ost;
      //Serializing::serialize<boost::archive::text_oarchive>(unit, ost);
      Serializing::serialize<oarchive>(unit, ost);
      for(int a=3;a>0;a--){
        try{
          LOGDEBUG("tra writing processunit "<<unit->uuid)
          gridfs->storeFile(ost.str().c_str(),ost.str().length(),unit->uuid);
          dbcrs->insert("mhive.processunits", p);
          /*break retry loop*/
          break;
        }catch(std::exception & ex){
          LOGERROR("could not write to mongo, try reconnect");
          dbcrs->connect();
          org::esb::lang::Thread::sleep2(1000);
          //throw ex;
        }
      }
    }

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> RedundantEngine::deque()
    {
      //boost::mutex::scoped_lock enqueue_lock(_mutex);
      //dbcrs->query("");
      return Simple::deque();
    }

    void RedundantEngine::commit(std::string uuid)
    {
      Simple::commit(uuid);
    }

    void RedundantEngine::rollback(std::string uuid)
    {
      Simple::rollback(uuid);
    }

    }
  }
