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

  class HTTPPULLSOURCETASK_EXPORT HTTPPullSource : public org::esb::core::Task {
  public:
    HTTPPullSource();
    virtual ~HTTPPullSource();
    void prepare();
    void execute();
    //org::esb::core::OptionsDescription getOptionsDescription();
    int getPadTypes();
  private:
    std::string _srcuristr;
    std::string _trguristr;
  };
}
#endif	/* DOWNLOADTASK_H */

