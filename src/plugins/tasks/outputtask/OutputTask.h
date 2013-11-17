/* 
 * File:   DownloadTask.h
 * Author: HoelscJ
 *
 * Created on 19. Oktober 2011, 11:41
 */

#ifndef OUTPUTTASK_H
#define	OUTPUTTASK_H
#include "org/esb/core/Task.h"
#include "exports.h"
namespace plugin {

  class OUTTASK_EXPORT OutputTask : public org::esb::core::Task {
  public:
    OutputTask();
    virtual ~OutputTask();
    void prepare();
    void execute();
    org::esb::core::OptionsDescription getOptionsDescription();
    int getPadTypes();
  private:
    classlogger("task.outputtask")

    std::string _trguristr;
    std::string _srcuristr;
    std::string _task_uuid;
  };
//  REGISTER_TASK("DownloadTask", DownloadTask)
}
#endif	/* DOWNLOADTASK_H */

