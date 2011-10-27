/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "ExportTask.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/io/File.h"
#include "org/esb/util/Foreach.h"
namespace plugin {

  ExportTask::ExportTask() {
  }

  ExportTask::~ExportTask() {
  }

  void ExportTask::prepare() {
    if(getContext()->contains("exporttask.jobid")){
        _job_id=getContext()->getEnvironment<std::string>("exporttask.jobid");
    }else{
      setStatus(Task::ERROR);
    }
    if(getContext()->contains("exporttask.trg")){
        _target_file=getContext()->getEnvironment<std::string>("exporttask.trg");
    }else{
      setStatus(Task::ERROR);      
    }
  }

  org::esb::core::OptionsDescription ExportTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("exporttask");
    result.add_options()
            ("exporttask.jobid", boost::program_options::value<std::string > ()->required(), "Export task job id")
            ("exporttask.trg", boost::program_options::value<std::string > ()->required(), "Export task file target");
    return result;
  }

  
  void ExportTask::execute() {
    if(getStatus()==Task::ERROR)return;
    
    org::esb::io::File inputdir(_job_id);
    org::esb::io::FileList filelist = inputdir.listFiles();
    foreach(Ptr<org::esb::io::File> file, filelist){
      LOGDEBUG("File : "<<file->getPath());
    }
  }

  REGISTER_TASK("ExportTask", ExportTask);

}
