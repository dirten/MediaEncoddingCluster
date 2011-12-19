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

  class EXECTASK_EXPORT ExecutableTask : public org::esb::core::Task {
  public:
    ExecutableTask();
    virtual ~ExecutableTask();
    void prepare();
    void execute();
    org::esb::core::OptionsDescription getOptionsDescription();
    int getPadTypes();
  private:
    std::string _executable;
  };
//  REGISTER_TASK("DownloadTask", DownloadTask)
}
#endif	/* DOWNLOADTASK_H */

