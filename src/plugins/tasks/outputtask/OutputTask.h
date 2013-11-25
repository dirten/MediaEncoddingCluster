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
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/lang/Ptr.h"

using org::esb::av::FormatOutputStream;
using org::esb::av::PacketOutputStream;
using org::esb::av::Encoder;

namespace plugin {

  class OUTTASK_EXPORT OutputTask : public org::esb::core::Task {
  public:
    OutputTask();
    virtual ~OutputTask();
    void prepare();
    void execute();
    void cleanup();
    void pushBuffer(Ptr<org::esb::av::Packet>);
    org::esb::core::OptionsDescription getOptionsDescription();
    int getPadTypes();
  private:
    classlogger("task.outputtask")

    std::string _trguristr;
    std::string _srcuristr;
    std::string _task_uuid;
    Ptr<FormatOutputStream> _fos;
    Ptr<PacketOutputStream> _pos;
  };
//  REGISTER_TASK("DownloadTask", DownloadTask)
}
#endif	/* DOWNLOADTASK_H */

