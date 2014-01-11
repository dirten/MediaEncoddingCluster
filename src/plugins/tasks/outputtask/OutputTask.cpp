/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 *
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "org/esb/core/TaskException.h"
#include "OutputTask.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/libjson/libjson.h"
#include "Poco/URI.h"
#include "Poco/File.h"
#include "org/esb/config/config.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Foreach.h"
#include "boost/date_time/posix_time/ptime.hpp"
#include "org/esb/io/File.h"
#include "org/esb/lang/Exception.h"

#include "org/esb/util/Serializing.h"
#include "org/esb/io/StringOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/db/hivedb.hpp"

#include <boost/serialization/map.hpp>
#include <boost/serialization/shared_ptr.hpp>

using org::esb::io::File;
using org::esb::lang::Exception;
using org::esb::util::Serializing;


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
        /*
        Poco::File trgfile(_trguristr);
        if(trgfile.exists() && trgfile.isDirectory()){
          _trguristr+="/$input.name$.$profile.ext$";
        }*/
        std::list<std::string> keys=getContext()->keys();
        foreach(std::string key, keys){
          /*
          if(data.type() == typeid (std::string)){
            _trguristr = org::esb::util::StringUtil::replace(_trguristr,std::string("$").append(key).append("$"),getContext()->get<std::string>(key));
          }*/
        }
        _srcuristr = getSource();
        _task_uuid=getUUID();
      }
    }


    /*serializing encoder*/
    std::map<int, Ptr<org::esb::av::Encoder> > encoder_map=getContext()->getProperty<std::map<int, Ptr<org::esb::av::Encoder> > >("encoder");
    std::map<int, boost::shared_ptr<org::esb::av::Encoder> > encoder_map_copy;
    std::map<int, Ptr<org::esb::av::Encoder> >::iterator it = encoder_map.begin();
    for (int a=0; it != encoder_map.end(); it++,a++) {
      encoder_map_copy[a]=(*it).second;
    }

    std::string encdata=Serializing::serialize(encoder_map_copy);
    //litesql::Blob encdata=litesql::Blob(sdata.c_str(),sdata.length());

    boost::shared_ptr<db::HiveDb> database=getContext()->database;

    db::OutputFile oFile(*database);
    oFile.jobid=getContext()->get<std::string > ("uuid");
    oFile.path=_trguristr;
    oFile.outfiledata=encdata;
    oFile.status=db::OutputFile::Status::Waiting;
    oFile.update();

    return;
    /*openning file outputstream*/
    File outfile(_trguristr);
   _fos=new FormatOutputStream(&outfile, outfile.getExtension().c_str());
    _pos=new PacketOutputStream(_fos.get());

    /*adding the encoder for the streams*/
    //std::map<int, Ptr<org::esb::av::Encoder> > encoder_map=getContext()->getProperty<std::map<int, Ptr<org::esb::av::Encoder> > >("encoder");
    //std::map<int, Ptr<org::esb::av::Encoder> >::iterator it = encoder_map.begin();
    for (int a=0; it != encoder_map.end(); it++,a++) {
      _pos->setEncoder(*(*it).second,a);
    }
    if(!_pos->init()){
      throw new Exception("could not init PackeOutputSTream");
    }
  }

  org::esb::core::OptionsDescription OutputTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("uploadtask");
    result.add_options()
        //("uploadtask.src", boost::program_options::value<std::string > ()->required(), "Upload task file source")
        //("uploadtask.trg", boost::program_options::value<std::string > ()->required(), "Upload task file target")
        ("data", boost::program_options::value<std::string > ()->default_value(""), "");
    return result;
  }
  
  int OutputTask::getPadTypes(){
    return Task::SOURCE;
  }

  void OutputTask::pushBuffer(Ptr<org::esb::av::Packet>p){
    return;
    LOGDEBUG("OutputTask pushbuffer");
    _pos->writePacket(*p);
  }
  void OutputTask::cleanup(){
    Task::cleanup();
    LOGDEBUG("cleanup");
    if(_pos)
      _pos->close();
    _pos=NULL;
    if(_fos)
      _fos->close();
    _fos=NULL;
  }

  void OutputTask::execute() {
    Task::execute();
    return;
    setProgressLength(1);
    setProgress(0);
    LOGDEBUG("copy " << _srcuristr << " to " << _trguristr);
    setProgressLength(1);
    //Poco::URI uri(_srcuristr);
    //org::esb::io::File fout(base + "/jobs/" + _task_uuid + "/"+_target_file);
    std::string base = org::esb::config::Config::get("hive.tmp_path");

    Poco::File srcfile(base + "/jobs/" + _task_uuid + "/"+_srcuristr);
    LOGDEBUG("copy " << srcfile.path() << " to " << _trguristr);
    if (srcfile.exists()) {
      if(_trguristr.length()==0){
        setStatus(Task::ERROR);
        throw org::esb::core::TaskException(std::string("target file  could not be empty"));
      }
      Poco::File trgfile(_trguristr);
      LOGDEBUG("copy " << srcfile.path() << " to " << trgfile.path());
      if(trgfile.exists() && trgfile.isDirectory()){
        LOGDEBUG("target is directory");
        srcfile.copyTo(trgfile.path()+"/"+_srcuristr);
      }else{
        LOGDEBUG("target is file");
        srcfile.copyTo(trgfile.path());
      }
      setStatus(Task::DONE);
      setStatusMessage("File successful copied to the target");
      //srcfile.remove(false);
    } else {
      LOGERROR("src file " << srcfile.path() << " does not exist");
      setStatus(Task::ERROR);
      throw org::esb::core::TaskException(std::string("src file ").append(srcfile.path()).append(" does not exist"));
    }
    setProgress(1);
    LOGDEBUG("Output finish!");
  }
  REGISTER_TASK("OutputTask",OutputTask )
  }
