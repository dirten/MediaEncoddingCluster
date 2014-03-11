#include "WriterService.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/PluginContext.h"
#include "Writer.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/hive/Environment.h"
#include "Poco/Process.h"
#include "Poco/Pipe.h"
#include "org/esb/lang/Process.h"

#include "org/esb/signal/Message.h"
#include "org/esb/signal/Messenger.h"
namespace plugin {
  using org::esb::util::StringUtil;
  using org::esb::hive::Environment;
  using org::esb::signal::Message;
  using org::esb::signal::Messenger;

  WriterService::WriterService()
  {

  }


  void WriterService::startService(){
    /*restart export from aborted writings*/


    LOGDEBUG("looking for aborted exports")
    litesql::DataSource<db::OutputFile> source = litesql::select<db::OutputFile > (getContext()->database, db::OutputFile::Status==db::OutputFile::Status::Processing);
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
    go(WriterService::observeProcessUnits, this);
  }

  void WriterService::stopService(){
    _status=STOPPING;
    //Thread::sleep2(1500);
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


  void WriterService::observeProcessUnits(){
    int next_id=0;
    while (_status == RUNNING) {
      if(next_id==0){
        litesql::DataSource<db::ProcessUnit> pu_ds=litesql::select<db::ProcessUnit > (getContext()->database, db::ProcessUnit::Recv == 1).orderBy(db::ProcessUnit::Id);
        if(pu_ds.count()){
          next_id=pu_ds.one().id;
        }
      }else{
        litesql::DataSource<db::ProcessUnit> next_pu_ds=litesql::select<db::ProcessUnit > (getContext()->database, db::ProcessUnit::Id == next_id && db::ProcessUnit::Recv > 1);
        if(next_pu_ds.count()){
          db::ProcessUnit pu=next_pu_ds.one();
          Message msg;
          msg.setProperty("processunit_encoded",pu.id.value());
          msg.setProperty("jobid",pu.jobid.value());
          msg.setProperty("sequence",pu.sequence.value());
          Messenger::getInstance().sendMessage(msg);
          next_id++;
          continue;
        }
      }
      Thread::sleep2(1000);
    }
  }

  void WriterService::run(){
    LOGDEBUG("WriterService");
    while (_status == RUNNING) {
      litesql::DataSource<db::OutputFile> source = litesql::select<db::OutputFile > (getContext()->database, db::OutputFile::Status==db::OutputFile::Status::Waiting);
      if(source.count()>0){
        db::OutputFile outputfile=source.one();
        Ptr<Writer> writer=new Writer(outputfile);
        go(Writer::run, writer);

        /*

        std::string cmd=Environment::get(Environment::EXE_PATH)+"/"+Environment::get(Environment::EXE_NAME);
        //std::cout <<cmd<<std::endl;
        std::vector<std::string> args;

        //args=Environment::getArguments();
        args.push_back("--explicit");
        args.push_back("writer");
        //args.push_back("--waitonstdin");
        args.push_back("--writer.jobid");
        args.push_back(StringUtil::toString(outputfile.id));
        std::list<string>arglist(args.begin(), args.end());

        Poco::Pipe outPipe;
        Poco::Pipe inPipe;
        Poco::ProcessHandle handle=Poco::Process::launch(cmd, args, &inPipe, &outPipe, 0);
        db::Process proc(*getContext()->database);
        proc.pid=(int)handle.id();
        proc.executable=cmd;
        proc.arguments=(std::string)StringUtil::join(args.begin(), args.end(), std::string(" "));
        proc.update();
        //Poco::ProcessHandle handle=Poco::Process::launch(cmd, args);
        go(Poco::ProcessHandle::wait, handle);

        org::esb::lang::Process p(cmd, arglist, "mhive-writer");
        p.run(false);
        */
        //handle.wait();
        outputfile.status=db::OutputFile::Status::Processing;
        outputfile.update();

      }else{
        Thread::sleep2(1000);
      }
    }
  }

}
