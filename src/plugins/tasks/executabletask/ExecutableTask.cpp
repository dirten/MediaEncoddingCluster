/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "ExecutableTask.h"
#include "org/esb/lang/Ptr.h"
namespace plugin {

  ExecutableTask::ExecutableTask() {
  }

  ExecutableTask::~ExecutableTask() {
  }

  void ExecutableTask::prepare() {

  }

  org::esb::core::OptionsDescription ExecutableTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("uploadtask");
    result.add_options()
            ("executabletask.exec", boost::program_options::value<std::string > ()->default_value("test"), "Upload task file source");
    return result;
  }

  void ExecutableTask::execute() {
        setStatus(Task::DONE);
  }

  REGISTER_TASK("ExecutableTask",ExecutableTask );

}
