#include "RedundantEngine.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/Serializing.h"
#include "org/esb/core/PluginContext.h"
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
    RedundantEngine::RedundantEngine():Simple()
    {

    }
    RedundantEngine::RedundantEngine(db::HiveDb database, std::string storage_path, std::vector<std::string> hosts, int self_port) : Simple()
    {}

    void RedundantEngine::startup()
    {
      mongo::Status state= mongo::client::initialize();
      if(!state.isOK()){
        std::cout << "could not initialize mongo driver"<<std::endl;
      }else{
        std::cout << "initialize mongo driver"<<std::endl;
      }

      if(getContext()->getProperty<std::string>("mongodb.engine")=="simple"){
        initSimple();
      }else if(getContext()->getProperty<std::string>("mongodb.engine")=="redundant"){
        initRedundant();
      }

      gridfs=new mongo::GridFS(*dbcrs, "mhive");
    }

    void RedundantEngine::initRedundant()
    {
      std::vector<string> hosts = getContext()->get<std::vector<string> >("mongodb.hosts");
      std::vector<mongo::HostAndPort>rs_hosts;
      foreach(std::string host, hosts){
        std::cout << "setup mongo host:"<<host<<std::endl;
        rs_hosts.push_back(mongo::HostAndPort(host));
      }
      std::string replicaset=getContext()->get<string>("mongodb.replicaset");
      mongo::DBClientReplicaSet * db=new mongo::DBClientReplicaSet(replicaset, rs_hosts);
      db->connect();
      dbcrs=db;

    }

    void RedundantEngine::initSimple()
    {


      mongo::DBClientConnection * db=new mongo::DBClientConnection(true);
      std::vector<string> hosts = getContext()->get<std::vector<string> >("mongodb.hosts");

      foreach(std::string host, hosts){
        std::cout << "setup mongo host:"<<host<<std::endl;
        std::string errmsg;
        if(db->connect(mongo::HostAndPort(host), errmsg)){
          break;
        }
      }
      dbcrs=db;
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
      /*this for loop is for a failover process (push to fail)*/
      for(int a=3;a>0;a--){
        try{
          LOGDEBUG("try writing processunit "<<unit->uuid);
          gridfs->storeFile(ost.str().c_str(),ost.str().length(),unit->uuid);
          dbcrs->insert("mhive.processunits", p);
          /*break retry loop*/
          break;
        }catch(std::exception & ex){
          LOGERROR("could not write to mongo, try reconnect");
          //dbcrs->connect();
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
    org::esb::core::OptionsDescription RedundantEngine::getOptionsDescription()
    {
      org::esb::core::OptionsDescription result("mongodb");
      result.add_options()
      ("mongodb.engine", boost::program_options::value<string >()->default_value("simple"), "which storage engine to use(simple,redundant)")
      ("mongodb.hosts", boost::program_options::value< std::vector<string> >()->multitoken(), "ip:port tuples for the other redundant storage")
      ("mongodb.replicaset", boost::program_options::value< string> ()->default_value("mhive"), "replicaset name for the mongodb redundant storage")
      //("mongodb.port", boost::program_options::value< int >()->default_value(20202), "port number for own redundant storage")
      //("mhivestorage.redundancy_level", boost::program_options::value<int >()->default_value(2), "which redundancy level should the storage engine \"redundant\" use")
      ;
      return result;
    }

    REGISTER_STORAGE("mongodb", RedundantEngine)

  }

}
