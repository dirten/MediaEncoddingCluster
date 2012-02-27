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

namespace encodingtask {
  struct classcomp {
    bool operator() (boost::shared_ptr<org::esb::hive::job::ProcessUnit> l, boost::shared_ptr<org::esb::hive::job::ProcessUnit> r) const
    {return l->_sequence<r->_sequence;}
  };
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
    Ptr<Encoder> _video_encoder;
    Ptr<Encoder> _audio_encoder;
    //bool compare(boost::shared_ptr<org::esb::hive::job::ProcessUnit> l, boost::shared_ptr<org::esb::hive::job::ProcessUnit> r);
    boost::mutex _partition_mutex;
    std::set<boost::shared_ptr<org::esb::hive::job::ProcessUnit>, classcomp> _unit_list;
    int lastSequence;
  };
  //  REGISTER_TASK("DownloadTask", DownloadTask)
}
#endif	/* DOWNLOADTASK_H */

