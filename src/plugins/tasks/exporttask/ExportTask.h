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
namespace plugin {

  class EXPTASK_EXPORT ExportTask : public org::esb::core::Task {
  public:
    ExportTask();
    virtual ~ExportTask();
    void prepare();
    void execute();
    org::esb::core::OptionsDescription getOptionsDescription();

  private:
    std::string _job_id;
    std::string _target_file;
  };
//  REGISTER_TASK("DownloadTask", DownloadTask)
}
#endif	/* DOWNLOADTASK_H */

