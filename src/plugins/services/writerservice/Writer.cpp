#include "Writer.h"
#include "org/esb/util/Log.h"

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

namespace plugin {
  using org::esb::util::Serializing;
  using org::esb::util::StringUtil;

  Writer::Writer(db::OutputFile outputfile):_outputfile(outputfile){
    LOGDEBUG("Writer()")

    File outfile(outputfile.path);
    _fos=new FormatOutputStream(&outfile, outfile.getExtension().c_str());
    _pos=new PacketOutputStream(_fos.get());

    Serializing::deserialize(encoder_map,outputfile.outfiledata);

    std::map<int, boost::shared_ptr<org::esb::av::Encoder> >::iterator it=encoder_map.begin();
    for (int a=0; it != encoder_map.end(); it++,a++) {
      (*it).second.get()->open();
      _pos->setEncoder(*(*it).second.get(),a);
      _stream_timestamps[a]=0;
    }

    if(!_pos->init()){
      throw new Exception("could not init PackeOutputSTream");
    }
  }

  Writer::~Writer()
  {
    LOGDEBUG("~Writer()")
    LOGDEBUG("cleanup");
    if(_pos)
      _pos->close();
    _pos=NULL;
    if(_fos)
      _fos->close();
    _fos=NULL;

  }

  void Writer::run()
  {
    LOGDEBUG("Writer::run");

    int next_id=0;
    while (true) {
      //LOGDEBUG("running observer")
      //int count=litesql::select<db::ProcessUnit > (_outputfile.getDatabase(), db::ProcessUnit::Jobid == _task_uuid).count();
      //if(count > 0){
        if(next_id==0){
          /*reading out the first id from the database*/
          if(litesql::select<db::ProcessUnit > (_outputfile.getDatabase(), db::ProcessUnit::Jobid == _outputfile.jobid).orderBy(db::ProcessUnit::Id).count()){
            db::ProcessUnit unit=litesql::select<db::ProcessUnit > (_outputfile.getDatabase(), db::ProcessUnit::Jobid == _outputfile.jobid).orderBy(db::ProcessUnit::Id).one();
            next_id=unit.id;
            LOGDEBUG("setting first id:"+StringUtil::toString(next_id))
          }
        }


        int flushed=litesql::select<db::Job > (_outputfile.getDatabase(), db::Job::Uuid == _outputfile.jobid && db::Job::Status == db::Job::Status::Exporting).count();
        /*calculating progress*/
        int completed=litesql::select<db::ProcessUnit > (_outputfile.getDatabase(), db::ProcessUnit::Jobid == _outputfile.jobid && db::ProcessUnit::Recv == 1).count();

        /*!!!!caution: this order is needed to prevent race condition in completition!!!!*/

        /*next process unit ready*/
        int next_ready=litesql::select<db::ProcessUnit > (_outputfile.getDatabase(), db::ProcessUnit::Id==next_id && db::ProcessUnit::Recv > 1 && db::ProcessUnit::Jobid == _outputfile.jobid).count();

        if(next_ready){
          LOGDEBUG("pushing next id"+StringUtil::toString(next_id));
          db::ProcessUnit pu=litesql::select<db::ProcessUnit > (_outputfile.getDatabase(), db::ProcessUnit::Id==next_id).one();

          /*here comes the pushbuffer*/
          std::string base = org::esb::config::Config::get("hive.data_path");
          org::esb::io::File inputfile(base + "/"+pu.jobid+"/"+ pu.recvid);
          FileInputStream inputstream(&inputfile);
          std::string d;
          int readed=inputstream.read(d);
          LOGDEBUG("bytes readed:"+StringUtil::toString(readed))
              boost::shared_ptr<org::esb::hive::job::ProcessUnit> unit;
          try{
            Serializing::deserialize(unit, d);
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
      org::esb::lang::Thread::sleep2(1 * 1000);
    }
    _outputfile.status=db::OutputFile::Status::Completed;
    _outputfile.update();
    LOGDEBUG("finishing output ")
  }
  bool Writer::ptsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b) {
    return a->getPts() < b->getPts();
  }

  bool Writer::dtsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b) {
    return a->getDts() < b->getDts();
  }

}

