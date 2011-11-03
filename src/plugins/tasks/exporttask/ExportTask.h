/* 
 * File:   DownloadTask.h
 * Author: HoelscJ
 *
 * Created on 19. Oktober 2011, 11:41
 */

#ifndef DOWNLOADTASK_H
#define	DOWNLOADTASK_H
#include "org/esb/db/hivedb.hpp"

#include "org/esb/core/Task.h"
#include "ProcessUnitData.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/av/Packet.h"
#include "exports.h"
namespace plugin {

  class EXPTASK_EXPORT ExportTask : public org::esb::core::Task {
  public:
    ExportTask();
    virtual ~ExportTask();
    void prepare();
    void execute();
    org::esb::core::OptionsDescription getOptionsDescription();

  private:

    struct StreamData {
      int64_t in_start_time;
      int64_t out_start_time;
      int64_t last_timestamp;
      int64_t next_timestamp;
      int packet_duration;
      AVRational packet_timebase;

      int in_stream_index;
      int out_stream_index;
      AVRational in_timebase;
      AVRational out_timebase;
      int stream_type;
    };

    static std::map<int, StreamData> _source_stream_map;

    std::string _job_id;
    std::string _target_file;
    std::string _format;
    std::list<ProcessUnitData> _pudata;
    static bool ptsComparator(boost::shared_ptr<org::esb::av::Packet> a, boost::shared_ptr<org::esb::av::Packet> b);
    static bool dtsComparator(boost::shared_ptr<org::esb::av::Packet> a, boost::shared_ptr<org::esb::av::Packet> b);
    typedef std::map<int, boost::shared_ptr<org::esb::av::Encoder> > StreamEncoderMap;
    typedef std::map<int, StreamData> InOutStreamMap;
    typedef boost::shared_ptr<org::esb::av::Packet> PacketPtr;
    StreamEncoderMap _stream_encoder;
    InOutStreamMap _in_out_stream_map;
    Ptr<db::Job> _job;
  };
  //  REGISTER_TASK("DownloadTask", DownloadTask)
}
#endif	/* DOWNLOADTASK_H */

