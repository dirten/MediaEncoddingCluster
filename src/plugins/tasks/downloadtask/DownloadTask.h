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

  class DLTASK_EXPORT DownloadTask : public org::esb::core::Task {
  public:
    DownloadTask();
    virtual ~DownloadTask();
    void prepare();
    void execute();
    org::esb::core::OptionsDescription getOptionsDescription();

  private:
    std::string _srcuristr;
    std::string _trguristr;
  };
  REGISTER_TASK("DownloadTask", DownloadTask)
}
#endif	/* DOWNLOADTASK_H */

