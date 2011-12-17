/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "OutputTask.h"
#include "org/esb/lang/Ptr.h"
namespace plugin {

  OutputTask::OutputTask() {
  }

  OutputTask::~OutputTask() {
  }

  void OutputTask::prepare() {

  }

  org::esb::core::OptionsDescription OutputTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("uploadtask");
    result.add_options()
            ("uploadtask.src", boost::program_options::value<std::string > ()->required(), "Upload task file source")
            ("uploadtask.trg", boost::program_options::value<std::string > ()->required(), "Upload task file target");
    return result;
  }
  
  int OutputTask::getPadTypes(){
    return Task::SOURCE;
  }
  
  void OutputTask::execute() {
        setStatus(Task::DONE);
  }

  REGISTER_TASK("OutputTask",OutputTask );

}
