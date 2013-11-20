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
#include "org/esb/mq/QueueConnection.h"
#include "org/esb/mq/ObjectMessage.h"


/*activemq implementation*/
#include "activemq/library/ActiveMQCPP.h"
#include "cms/Connection.h"
#include "activemq/core/ActiveMQConnectionFactory.h"
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>

using namespace activemq::core;
using namespace decaf::lang;
using namespace cms;

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

    Ptr<org::esb::mq::QueueConnection> con;
    Ptr<safmq::MessageQueue> read_q;
    std::map<int, Ptr<safmq::MessageQueue> > _queueMap;

    boost::shared_ptr<db::HiveDb> database;




    /*new Message Queue implementation*/

    Connection* connection;
    Session* session;
    Destination* destination;
    MessageProducer* producer;

    template<typename T>
    BytesMessage * serializeProcessUnit(const T &object) {
        std::ostringstream archive_stream;
        boost::archive::binary_oarchive archive(archive_stream);
        archive << object;
        std::string _outbound_data = archive_stream.str();
        BytesMessage* message(session->createBytesMessage((const unsigned char*)_outbound_data.c_str(),_outbound_data.length()));
        return message;
        //this->getBufferStream()->write(_outbound_data.c_str(),_outbound_data.length());
    }
  };
  //  REGISTER_TASK("DownloadTask", DownloadTask)
}
#endif	/* DOWNLOADTASK_H */

