#include "WriterService.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/PluginContext.h"
#include "Writer.h"
namespace plugin {

  WriterService::WriterService()
  {

  }

  void WriterService::onMessage(org::esb::signal::Message &){

  }

  void WriterService::startService(){
    /*restart export from aborted writings*/


    LOGDEBUG("looking for aborted exports")
    litesql::DataSource<db::OutputFile> source = litesql::select<db::OutputFile > (*getContext()->database, db::OutputFile::Status==db::OutputFile::Status::Processing);
    litesql::Cursor<db::OutputFile> cur=source.cursor();
    for (;cur.rowsLeft(); cur++){
      db::OutputFile out=(*cur);
      LOGDEBUG("aborted export found, restart it:"<<out)
      out.status=db::OutputFile::Status::Waiting;
      out.update();
      LOGDEBUG("changed to:"<<out)
    }

    _status=RUNNING;
    go(WriterService::run, this);
  }

  void WriterService::stopService(){
    _status=STOPPING;
  }

  org::esb::core::OptionsDescription WriterService::getOptionsDescription(){
    org::esb::core::OptionsDescription result("writerservice");
    result.add_options()
            ("writerservice.intervall", boost::program_options::value<int>()->default_value(5), "interval in seconds to look for new Jobs");

    return result;
  }

  org::esb::core::ServicePlugin::ServiceType WriterService::getServiceType(){
    return SERVICE_TYPE_SERVER;
  }

  void WriterService::init(){

  }

  void WriterService::run(){
    LOGDEBUG("WriterService");
    while (_status == RUNNING) {
      litesql::DataSource<db::OutputFile> source = litesql::select<db::OutputFile > (*getContext()->database, db::OutputFile::Status==db::OutputFile::Status::Waiting);
      if(source.count()>0){
        db::OutputFile outputfile=source.one();
        Ptr<Writer> writer=new Writer(outputfile);
        go(Writer::run, writer);
        outputfile.status=db::OutputFile::Status::Processing;
        outputfile.update();
      }else{
        Thread::sleep2(1000);
      }
    }
  }

}
