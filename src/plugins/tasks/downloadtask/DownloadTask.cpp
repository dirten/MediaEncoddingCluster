/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "DownloadTask.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/core/TaskException.h"
#include "org/esb/lang/Ptr.h"

#include "org/esb/libjson/libjson.h"
#include "org/esb/io/File.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
namespace plugin {

  DownloadTask::DownloadTask() : Task() {
  }

  DownloadTask::~DownloadTask() {
  }

  void DownloadTask::prepare() {
    Task::prepare();
    _srcuristr = getContext()->getEnvironment<std::string > ("downloadtask.src");
    _trguristr = getContext()->getEnvironment<std::string > ("downloadtask.trg");

    if (_trguristr.length() == 0) {
      _trguristr = getSink();
    }

    std::string data = getContext()->getEnvironment<std::string > ("data");
    if (libjson::is_valid(data)) {
      JSONNode node = libjson::parse(data);
      if (node.contains("infile")) {
        _srcuristr = node["infile"].as_string();
      }
    }
    if (_srcuristr.length() == 0) {
      throw org::esb::core::TaskException("No Source File given!");
    }
    org::esb::io::File f(_srcuristr);
    getContext()->set<std::string > ("input.name", org::esb::util::StringUtil::replace(f.getFileName(), f.getExtension(), ""));
    getContext()->set<std::string > ("input.ext", f.getExtension());

    /*
    if(_trguristr.length()==0){
      throw org::esb::core::TaskException("No Target File given!");
    }*/
  }

  org::esb::core::OptionsDescription DownloadTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("downloadtask");
    result.add_options()
            ("downloadtask.src", boost::program_options::value<std::string > ()->default_value(""), "Download task source")
            ("downloadtask.trg", boost::program_options::value<std::string > ()->default_value(""), "Download task target")
            ("data", boost::program_options::value<std::string > ()->default_value(""), "");
    return result;
  }

  int DownloadTask::getPadTypes() {
    return Task::SINK;
  }

  void DownloadTask::execute() {
    Task::execute();
    setProgressLength(1);
    std::string base = org::esb::config::Config::get("hive.tmp_path");
    org::esb::io::File sfile(_srcuristr);
    LOGDEBUG("try copy srcfile " << _srcuristr << " to " << base << "/jobs/" << getUUID() << "/" << _trguristr);
    if (sfile.exists() && sfile.isFile()) {
      org::esb::io::File tfile(base + "/jobs/" + getUUID() + "/" + _trguristr);
      org::esb::io::File tdir(base + "/jobs/" + getUUID());
      tdir.mkdirs();
      sfile.copyTo(tfile);
      setStatus(Task::DONE);
      setStatusMessage("File successful copied into the workspace");
    } else {
      setStatus(Task::ERROR);
      throw org::esb::core::TaskException("Source File not found");
    }
    setProgress(1);
    LOGDEBUG("Download finish!");
  }

  typedef DownloadTask InputTask;
  REGISTER_TASK("InputTask", InputTask)
}

