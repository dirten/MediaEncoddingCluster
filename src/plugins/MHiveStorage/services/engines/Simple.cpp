#include "Simple.h"
#include "org/esb/util/Serializing.h"
#include "org/esb/util/UUID.h"
using org::esb::util::Serializing;
namespace mhivestorage{
  namespace engines {
    typedef boost::archive::text_oarchive oarchive;
    typedef boost::archive::text_iarchive iarchive;

    //typedef boost::archive::binary_oarchive oarchive;
    //typedef boost::archive::binary_iarchive iarchive;

    //typedef portable_binary_oarchive oarchive;
    //typedef portable_binary_iarchive iarchive;
    Simple::Simple(db::HiveDb database, std::string storage_path):database(database), _storage_path(storage_path)
    {

    }

    void Simple::enque(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)
    {
      db::ProcessUnit pu(database);
      pu.sorcestream=unit->_source_stream;
      pu.targetstream=unit->_target_stream;
      pu.send=litesql::DateTime(1);
      pu.recv=litesql::DateTime(1);
      pu.sendid=unit->uuid;
      pu.jobid=unit->getJobId();
      pu.sequence=unit->_sequence;
      if(unit->_decoder->getCodecType() == AVMEDIA_TYPE_VIDEO){
        pu.codectype=db::ProcessUnit::Codectype::VIDEO;
      }

      if(unit->_decoder->getCodecType() == AVMEDIA_TYPE_AUDIO){
        pu.codectype=db::ProcessUnit::Codectype::AUDIO;
      }

      if (unit->_input_packets.size() > 0) {
        boost::shared_ptr<org::esb::av::Packet> first_packet = unit->_input_packets.front();
        boost::shared_ptr<org::esb::av::Packet> last_packet = unit->_input_packets.back();
        pu.startts=(double)first_packet->getDtsTimeStamp().getTime();
        pu.endts=(double)last_packet->getDtsTimeStamp().getTime();
        pu.timebasenum=first_packet->getDtsTimeStamp().getTimeBase().num;
        pu.timebaseden=first_packet->getDtsTimeStamp().getTimeBase().den;
        pu.framecount=(int)unit->_input_packets.size();
      }

      /**/
      /**
       * serialize the process unit into string format
       * @TODO: massive performance impact by serializing ProcessUnit
       */
      std::ofstream ost((_storage_path + "/"+unit->getJobId()+"/"+ unit->uuid).c_str(), std::ofstream::out);
      //Serializing::serialize<boost::archive::text_oarchive>(unit, ost);
      Serializing::serialize<oarchive>(unit, ost);

      LOGDEBUG("written ProcessUnit to "<<_storage_path + "/"+unit->getJobId()+"/"+ unit->uuid)

      pu.update();

    }
    boost::shared_ptr<std::istream> Simple::dequeStream()
    {
      boost::shared_ptr<std::istream> stream;
      db::ProcessUnit result=db::ProcessUnit(database);

      bool responsible=false;
      boost::shared_ptr<org::esb::hive::job::ProcessUnit> unit;

      //std::string client_id=StringUtil::toString(_ep);
      //database->query("begin exclusive");
      litesql::DataSource<db::ProcessUnit> nextUnit=litesql::select<db::ProcessUnit > (database, db::ProcessUnit::Send == 1);
      litesql::Cursor<db::ProcessUnit>unit_cursor=nextUnit.orderBy(db::ProcessUnit::Id, true).cursor();
      if(unit_cursor.rowsLeft()){
        try{
          do{
            result=*unit_cursor;
            LOGDEBUG("fetch next ProcessUnit:"<<result.codectype<<":id:"<<result.id)
            responsible=true;
          }while(!responsible && unit_cursor.rowsLeft());
        }catch(std::exception & ex){
          LOGERROR("exception:"<<ex.what())
        }

        if(result.id>0){
          result.send=litesql::DateTime();
          result.deliverycount=result.deliverycount+1;
          //result.clientid=StringUtil::toString(_ep);
          stream=boost::shared_ptr<std::istream>(new std::ifstream((_storage_path + "/"+result.jobid+"/"+ result.sendid).c_str(), std::ifstream::in));
          //Serializing::deserialize<boost::archive::text_iarchive>(unit, ist);
          //Serializing::deserialize<iarchive>(unit, ist);
          result.update();
        }
      }
      //database->query("end");
      return stream;

    }

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> Simple::deque()
    {
      boost::shared_ptr<std::istream> stream=dequeStream();
      boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit;
      Serializing::deserialize<iarchive>(unit, *stream);

      //database->query("end");
      return unit;
    }

    void Simple::put(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)
    {
      litesql::Cursor<db::ProcessUnit> dbunit=litesql::select<db::ProcessUnit > (database, db::ProcessUnit::Sendid == unit->uuid).cursor();
      if(dbunit.rowsLeft()){
        db::ProcessUnit pu=*dbunit;
        std::string recvid=org::esb::util::PUUID();
        pu.recvid=recvid;
        pu.recv=litesql::DateTime();

        std::ofstream ost((_storage_path + "/"+unit->getJobId()+"/"+ recvid).c_str(), std::ofstream::out);
        Serializing::serialize<oarchive>(unit, ost);

        pu.update();
      }
    }

    void Simple::commit(std::string uuid)
    {

    }

    void Simple::rollback(std::string uuid)
    {
      litesql::Cursor<db::ProcessUnit> dbunit=litesql::select<db::ProcessUnit > (database, db::ProcessUnit::Sendid == uuid).cursor();
      if(dbunit.rowsLeft()){
        db::ProcessUnit pu=*dbunit;
        pu.send=1;
        pu.update();
      }
    }

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> Simple::get()
    {
      boost::shared_ptr<org::esb::hive::job::ProcessUnit> result;

      return result;
    }


  }
}
