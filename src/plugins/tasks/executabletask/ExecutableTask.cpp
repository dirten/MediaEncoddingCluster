/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "ExecutableTask.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/lang/Process.h"
#include "org/esb/libjson/libjson.h"
namespace plugin {

  ExecutableTask::ExecutableTask() : Task() {
  }

  ExecutableTask::~ExecutableTask() {
  }

  void ExecutableTask::prepare() {
    Task::prepare();
    std::string data = getContext()->getEnvironment<std::string > ("data");
    std::string source = getSource();
    std::string sink = getSink();

    LOGDEBUG("data");
    if (libjson::is_valid(data)) {
      JSONNode node = libjson::parse(data);
      if (node.contains("executable")) {
        _executable = node["executable"].as_string();
        _executable = org::esb::util::StringUtil::replace(_executable, "$source$", source);
        _executable = org::esb::util::StringUtil::replace(_executable, "$sink$", sink);
      }
    }
  }

  org::esb::core::OptionsDescription ExecutableTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("uploadtask");
    result.add_options()
            ("executabletask.exec", boost::program_options::value<std::string > ()->default_value("test"), "Upload task file source")
            ("data", boost::program_options::value<std::string > ()->default_value(""), "");
    return result;
  }

  int ExecutableTask::getPadTypes() {
    return Task::SOURCE | Task::SINK;
  }

  void ExecutableTask::execute() {
    Task::execute();

    setProgressLength(1);
    setProgress(1);
    //setStatus(Task::DONE);
  }

  REGISTER_TASK("ExecutableTask", ExecutableTask);

}
