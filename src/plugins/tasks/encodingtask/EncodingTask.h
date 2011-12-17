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
#include "exports.h"
#include "org/esb/hive/PresetReaderJson.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/util/Queue.h"
namespace encodingtask {

  class ENCTASK_EXPORT EncodingTask : public org::esb::core::Task {
  public:
    EncodingTask();
    virtual ~EncodingTask();
    void prepare();
    void execute();
    org::esb::core::OptionsDescription getOptionsDescription();
    boost::shared_ptr<org::esb::hive::job::ProcessUnit> getProcessUnit();
    void putProcessUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit>);
    int getPadTypes();
  private:
    std::string _srcuristr;
    std::string _partition;
    std::string _task_uuid;
    
    org::esb::hive::PresetReaderJson::CodecList _codecs;
    org::esb::hive::PresetReaderJson::FilterList _filters;
    org::esb::hive::PresetReaderJson::Preset _preset ;
    //Ptr<db::Job> _job;
    
    void putToPartition(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit, bool isLast=false);
    void enQueue(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit, bool isLast=false);
    std::map<int, Ptr<org::esb::util::FileQueue<boost::shared_ptr<org::esb::hive::job::ProcessUnit> > > > _stream_queues;
    std::list<std::string> _running_pus;
    int _sequence_counter;
  };
//  REGISTER_TASK("DownloadTask", DownloadTask)
}
#endif	/* DOWNLOADTASK_H */

