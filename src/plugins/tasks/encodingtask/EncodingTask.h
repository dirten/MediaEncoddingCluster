/* 
 * File:   DownloadTask.h
 * Author: HoelscJ
 *
 * Created on 19. Oktober 2011, 11:41
 */

#ifndef DOWNLOADTASK_H
#define	DOWNLOADTASK_H
//#include "org/esb/db/hivedb.hpp"

#include "exports.h"
#include "org/esb/hive/PresetReaderJson.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/util/Queue.h"
#include "ProcessUnitData.h"
#include "StreamData.h"
#include "StreamPacketizer.h"
#include "StreamProcessUnitBuilder.h"
#include "org/esb/core/Task.h"
#include "ProcessUnitList.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>



namespace encodingtask {
  class ENCTASK_EXPORT EncodingTask : public org::esb::core::Task {
  public:
    EncodingTask();
    virtual ~EncodingTask();
    void prepare();
    void execute();
    void cleanup();
    org::esb::core::OptionsDescription getOptionsDescription();
    boost::shared_ptr<org::esb::hive::job::ProcessUnit> getProcessUnit();
    void putProcessUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit>);
    int getPadTypes();
    void pushBuffer(Ptr<Packet> p);
  private:
    classlogger("task.encodingtask")

    void unitListCallback(boost::shared_ptr<org::esb::hive::job::ProcessUnit> unit);
    std::string _srcuristr;
    std::string _partition;
    std::string _task_uuid;

    org::esb::hive::PresetReaderJson::CodecList _codecs;
    org::esb::hive::PresetReaderJson::FilterList _filters;
    org::esb::hive::PresetReaderJson::Preset _preset;
    //Ptr<db::Job> _job;

    void putToPartition(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit, bool isLast = false);
    void enQueue(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit, bool isLast = false);
    std::map<int, Ptr<org::esb::util::FileQueue<boost::shared_ptr<org::esb::hive::job::ProcessUnit> > > > _stream_queues;
    std::list<std::string> _running_pus;
    int _sequence_counter;

    
    
    void exportFile();
    void observeProgress();
    void collector(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit,boost::asio::ip::tcp::endpoint ep);
    static bool ptsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b);
    static bool dtsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b);
    std::list<ProcessUnitData> _pudata;


    //static std::map<int, StreamData> _source_stream_map;

    typedef std::map<int, boost::shared_ptr<org::esb::av::Encoder> > StreamEncoderMap;
    typedef std::map<int, StreamData> InOutStreamMap;
    typedef boost::shared_ptr<org::esb::av::Packet> PacketPtr;
    StreamEncoderMap _stream_encoder;
    InOutStreamMap _in_out_stream_map;
    std::string _target_file;
    std::string _format;

    /*new pushbuffer implementation*/
    std::map<int,StreamPacketizer> _packetizer;
    std::map<int,StreamProcessUnitBuilder> _spu;
    //bool compare(boost::shared_ptr<org::esb::hive::job::ProcessUnit> l, boost::shared_ptr<org::esb::hive::job::ProcessUnit> r);
    boost::mutex _partition_mutex;
    ProcessUnitList _unit_list;
    std::map<int, Ptr<org::esb::av::Encoder> >_encs;

    //Ptr<org::esb::mq::QueueConnection> con;
    //Ptr<safmq::MessageQueue> read_q;
    //std::map<int, Ptr<safmq::MessageQueue> > _queueMap;

    boost::shared_ptr<db::HiveDb> database;
    boost::condition finish_condition;
    boost::mutex finish_mutex;
    boost::mutex finish_notify_mutex;
    bool flushed;



    /*
    template<typename T>
    std::string serializeProcessUnit(const T &object) {
        std::ostringstream archive_stream;
        boost::archive::text_oarchive archive(archive_stream);
        archive << object;
        std::string _outbound_data = archive_stream.str();
        //BytesMessage* message(session->createBytesMessage((const unsigned char*)_outbound_data.c_str(),_outbound_data.length()));
        return _outbound_data;
        //this->getBufferStream()->write(_outbound_data.c_str(),_outbound_data.length());
    }

    template < typename T >
    int deserializeProcessUnit(T & object, std::string data) {
      if (data.length() <= 0) {
        LOGERROR("Fehler in der groesse INBOUND_DATA:" << data.length());
        return -1;
      }
      std::istringstream archive_stream(data);
      boost::archive::text_iarchive archive(archive_stream);
      //boost::archive::text_iarchive archive(archive_stream);
      try {
        archive >> object;
      } catch (std::exception & ex) {
        LOGERROR("Exception reading archive:"<<ex.what());
        return -1;
      }
      return 0;

    }*/
  };
  //  REGISTER_TASK("DownloadTask", DownloadTask)
}
#endif	/* DOWNLOADTASK_H */

