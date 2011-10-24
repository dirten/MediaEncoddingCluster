/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "EncodingTask.h"
#include "org/esb/lang/Ptr.h"
namespace plugin {

  EncodingTask::EncodingTask() {
  }

  EncodingTask::~EncodingTask() {
  }

  void EncodingTask::prepare() {

  }

  org::esb::core::OptionsDescription EncodingTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("encodingtask");
    result.add_options()
            ("encodingtask.src", boost::program_options::value<std::string > ()->default_value(""), "Encoding task source")
            ("encodingtask.profile", boost::program_options::value<std::string > ()->default_value(""), "Encoding task profile");
    return result;
  }

  void EncodingTask::execute() {

  }

  REGISTER_TASK("EncodingTask",EncodingTask );

}
