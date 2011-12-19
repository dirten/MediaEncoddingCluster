/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "UploadTask.h"
#include "org/esb/lang/Ptr.h"
namespace plugin {

  UploadTask::UploadTask() {
  }

  UploadTask::~UploadTask() {
  }

  void UploadTask::prepare() {

  }

  org::esb::core::OptionsDescription UploadTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("uploadtask");
    result.add_options()
            ("uploadtask.src", boost::program_options::value<std::string > ()->required(), "Upload task file source")
            ("uploadtask.trg", boost::program_options::value<std::string > ()->required(), "Upload task file target");
    return result;
  }
  
  int UploadTask::getPadTypes(){
    return Task::SOURCE;
  }
  
  void UploadTask::execute() {
    Task::execute();
    setProgressLength(1);
    setProgress(1);
  }

  REGISTER_TASK("UploadTask",UploadTask );

}
