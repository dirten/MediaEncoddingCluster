/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "ExportTask.h"
#include "org/esb/lang/Ptr.h"
namespace plugin {

  ExportTask::ExportTask() {
  }

  ExportTask::~ExportTask() {
  }

  void ExportTask::prepare() {

  }

  org::esb::core::OptionsDescription ExportTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("exporttask");
    result.add_options()
            ("exporttask.trg", boost::program_options::value<std::string > ()->required(), "Export task file target");            
    return result;
  }

  void ExportTask::execute() {

  }

  REGISTER_TASK("ExportTask",ExportTask );

}
