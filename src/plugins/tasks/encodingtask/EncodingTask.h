/* 
 * File:   DownloadTask.h
 * Author: HoelscJ
 *
 * Created on 19. Oktober 2011, 11:41
 */

#ifndef DOWNLOADTASK_H
#define	DOWNLOADTASK_H
#include "org/esb/core/Task.h"
#include "exports.h"
#include "org/esb/hive/PresetReaderJson.h"

namespace encodingtask {

  class ENCTASK_EXPORT EncodingTask : public org::esb::core::Task {
  public:
    EncodingTask();
    virtual ~EncodingTask();
    void prepare();
    void execute();
    org::esb::core::OptionsDescription getOptionsDescription();

  private:
    std::string _srcuristr;
    std::string _trguristr;
    org::esb::hive::PresetReaderJson::CodecList _codecs;
    org::esb::hive::PresetReaderJson::FilterList _filters;
    org::esb::hive::PresetReaderJson::Preset _preset ;

  };
//  REGISTER_TASK("DownloadTask", DownloadTask)
}
#endif	/* DOWNLOADTASK_H */

