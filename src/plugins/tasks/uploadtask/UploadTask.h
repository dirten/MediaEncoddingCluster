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

  class UPTASK_EXPORT UploadTask : public org::esb::core::Task {
  public:
    UploadTask();
    virtual ~UploadTask();
    void prepare();
    void execute();
    org::esb::core::OptionsDescription getOptionsDescription();
    int getPadTypes();
  private:
  };
//  REGISTER_TASK("DownloadTask", DownloadTask)
}
#endif	/* DOWNLOADTASK_H */

