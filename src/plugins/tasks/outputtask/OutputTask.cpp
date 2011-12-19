/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "OutputTask.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/libjson/libjson.h"
#include "Poco/URI.h"
#include "Poco/File.h"
#include "org/esb/config/config.h"

namespace plugin {

  OutputTask::OutputTask():Task() {
  }

  OutputTask::~OutputTask() {
  }

  void OutputTask::prepare() {
    Task::prepare();
        std::string data = getContext()->getEnvironment<std::string > ("data");
    LOGDEBUG("data");
    if (libjson::is_valid(data)) {
      JSONNode node = libjson::parse(data);
      if (node.contains("outfile")) {
        _trguristr = node["outfile"].as_string();
        _srcuristr = getSource();
        _task_uuid=getUUID();
      }
    }
  }

  org::esb::core::OptionsDescription OutputTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("uploadtask");
    result.add_options()
            ("uploadtask.src", boost::program_options::value<std::string > ()->required(), "Upload task file source")
            ("uploadtask.trg", boost::program_options::value<std::string > ()->required(), "Upload task file target")
            ("data", boost::program_options::value<std::string > ()->default_value(""), "");
    return result;
  }
  
  int OutputTask::getPadTypes(){
    return Task::SOURCE;
  }
  
  void OutputTask::execute() {
    Task::execute();
    setProgressLength(1);
    setProgress(0);
    LOGDEBUG("copy " << _srcuristr << " to " << _trguristr);
    setProgressLength(1);
    //Poco::URI uri(_srcuristr);
    //org::esb::io::File fout(base + "/jobs/" + _task_uuid + "/"+_target_file);
    std::string base = org::esb::config::Config::get("hive.tmp_path");

    Poco::File srcfile(base + "/jobs/" + _task_uuid + "/"+_srcuristr);
    if (srcfile.exists()) {
      Poco::File trgfile(_trguristr);
      if(trgfile.isDirectory()){
        srcfile.copyTo(_trguristr+"/"+_srcuristr);
      }else{
        srcfile.copyTo(_trguristr);
      }
      setStatus(Task::DONE);
    } else {
      setStatus(Task::ERROR);
    }
    setProgress(1);
    LOGDEBUG("Download finish!");
  }

  REGISTER_TASK("OutputTask",OutputTask );

}
