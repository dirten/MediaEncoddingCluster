#include "Writer.h"
#include "org/esb/util/Log.h"

#include "org/esb/core/PluginContext.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/Serializing.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/lang/Thread.h"
#include <boost/serialization/map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include "org/esb/util/Foreach.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
namespace plugin {
  using org::esb::util::Serializing;
  using org::esb::util::StringUtil;

  Writer::Writer(){
  }

  Writer::Writer(db::OutputFile outputfile){
    setOutputFile(outputfile);
  }

  void Writer::setOutputFile(db::OutputFile outputfile){
    LOGDEBUG("Writer()");
    _outputfile=new db::OutputFile(outputfile);
    File outfile(outputfile.path);
    if(!File(outfile.getParent()).exists()){
      outfile.mkdirs();
    }
    _fos=new FormatOutputStream(&outfile, outfile.getExtension().c_str());
    _pos=new PacketOutputStream(_fos.get());

    Serializing::deserialize(encoder_map,outputfile.outfiledata);

    std::map<int, boost::shared_ptr<org::esb::av::Encoder> >::iterator it=encoder_map.begin();
    for (int a=0; it != encoder_map.end(); it++,a++) {
      _encoderList.push_back((*it).second);
      (*it).second->open();
      _pos->setEncoder(*(*it).second.get(),a);
      _stream_timestamps[a]=0;
    }

    if(!_pos->init()){
      throw new Exception("could not init PackeOutputSTream");
    }
  }

  void Writer::setOutputFileById(string id){
    // _jobid=id;
  }

  void Writer::startService(){
    std::string outfileid=getContext()->getEnvironment<std::string > ("writer.jobid");
    LOGDEBUG("looking for output with id:"<<outfileid);
    litesql::DataSource<db::OutputFile> source = litesql::select<db::OutputFile > (*getContext()->database, db::OutputFile::Id==outfileid);
    if(source.count()>0){
      db::OutputFile outputfile=source.one();
      setOutputFile(outputfile);
      outputfile.status=db::OutputFile::Status::Processing;
      outputfile.update();
      run();
      //go(Writer::run, this);
      //org::esb::lang::CtrlCHitWaiter::wait();
      //getc(stdin);
    }else{
      LOGWARN("OutputFile with id "<<outfileid<< " not found");
    }
  }

  void Writer::stopService(){

  }

  org::esb::core::OptionsDescription Writer::getOptionsDescription(){
    org::esb::core::OptionsDescription result("writer");
    result.add_options()
        ("writer.jobid", boost::program_options::value<string>()->default_value(""), "job id to write");
    return result;
  }

  org::esb::core::ServicePlugin::ServiceType Writer::getServiceType(){
    return SERVICE_TYPE_EXPLICIT;
  }

  Writer::~Writer()
  {
    LOGDEBUG("~Writer()")
        LOGDEBUG("cleanup");

    if(_pos)
      _pos->close();
    _pos.reset();
    if(_fos)
      _fos->close();
    _fos.reset();

    std::map<int, boost::shared_ptr<org::esb::av::Encoder> >::iterator it=encoder_map.begin();
    for (int a=0; it != encoder_map.end(); it++,a++) {
      (*it).second->close();
      (*it).second.reset();
    }
  }

  void Writer::run()
  {
    LOGDEBUG("Writer::run");

    int next_id=0;
    //litesql::DataSource<db::ProcessUnit > ds=litesql::select<db::ProcessUnit > (_outputfile.getDatabase(), db::ProcessUnit::Jobid == _outputfile.jobid).orderBy(db::ProcessUnit::Id);
    while (true) {

      LOGDEBUG("running observer for sequence id:"<<next_id <<" for job "<<_outputfile->jobid);
      //int count=litesql::select<db::ProcessUnit > (_outputfile.getDatabase(), db::ProcessUnit::Jobid == _task_uuid).count();
      //if(count > 0){


      int flushed=litesql::select<db::Job > (_outputfile->getDatabase(), db::Job::Uuid == _outputfile->jobid && db::Job::Status == db::Job::Status::Exporting).count();
      /*calculating progress*/
      int completed=litesql::select<db::ProcessUnit > (_outputfile->getDatabase(), db::ProcessUnit::Jobid == _outputfile->jobid && db::ProcessUnit::Recv == 1).count();

      /*!!!!caution: this order is needed to prevent race condition in completition!!!!*/

      /*next process unit ready*/
      //Records recs = db.query("SELECT count(*) from "+db::ProcessUnit::table__+" where "+db::ProcessUnit::Id.name()+" = ");
      int next_ready=litesql::select<db::ProcessUnit > (_outputfile->getDatabase(), db::ProcessUnit::Sequence==next_id && db::ProcessUnit::Recv > 1 && db::ProcessUnit::Jobid == _outputfile->jobid).count();

      if(next_ready){
        LOGDEBUG("pushing next id"+StringUtil::toString(next_id));
        db::ProcessUnit pu=litesql::select<db::ProcessUnit > (_outputfile->getDatabase(), db::ProcessUnit::Sequence==next_id && db::ProcessUnit::Jobid == _outputfile->jobid).one();

        /*here comes the pushbuffer*/
        std::string base = org::esb::config::Config::get("hive.data_path");
        org::esb::io::File inputfile(base + "/"+pu.jobid+"/"+ pu.recvid);
        base+="/"+pu.jobid+"/"+ pu.recvid;
        std::ifstream stream(base.c_str(), std::ifstream::in);
        //FileInputStream inputstream(&inputfile);
        //std::string d;
        //int readed=inputstream.read(d);
        //LOGDEBUG("bytes readed:"+StringUtil::toString(readed))
        boost::shared_ptr<org::esb::hive::job::ProcessUnit> unit;
        try{
          Serializing::deserialize(unit, stream);
          unit->_output_packets.sort(Writer::ptsComparator);

          foreach(PacketPtr p, unit->_output_packets) {
            int idx = p->getStreamIndex();
            p->setPts(_stream_timestamps[idx]);
            //_in_out_stream_map[idx].last_timestamp = _in_out_stream_map[idx].next_timestamp;
            _stream_timestamps[idx] += p->getDuration();
          }
          unit->_output_packets.sort(Writer::dtsComparator);

          foreach(PacketPtr p, unit->_output_packets) {
            _pos->writePacket(*p);
            //Task::pushBuffer(p);
          }
        }catch(std::exception & ex){
          LOGERROR(ex.what())
        }
        next_id++;
        continue;
      }

      /*division by zero!!!??*/
      //setProgress((completed*100)/count);
      if(flushed && completed==0){
        break;
      }
      //}
      org::esb::lang::Thread::sleep2(5 * 1000);
    }
    /*mark completed*/
    _outputfile->status=db::OutputFile::Status::Completed;
    _outputfile->update();

    /*cleanup files*/
    if(_outputfile->jobid.value().length()>0){ //not to delete accidentally "data directory"
      std::string base = org::esb::config::Config::get("hive.data_path");
      if(base.length()>0){                    //not to delete accidentally "root directory"
        org::esb::io::File job_dir(base + "/"+_outputfile->jobid);
        if(job_dir.isDirectory()){

          job_dir.deleteFile();

        }
      }
    }

    LOGDEBUG("finishing output ")
    //fclose(stdin);
  }
  bool Writer::ptsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b) {
    return a->getPts() < b->getPts();
  }

  bool Writer::dtsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b) {
    return a->getDts() < b->getDts();
  }

  REGISTER_SERVICE("writer", Writer)

  }

