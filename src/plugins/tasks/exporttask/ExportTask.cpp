/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "ExportTask.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/ScopedTimeCounter.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/config/config.h"
namespace plugin {

  ExportTask::ExportTask() {
  }

  ExportTask::~ExportTask() {
  }

  void ExportTask::prepare() {
    if (getContext()->contains("job")) {
      _job = Ptr<db::Job > (new db::Job(getContext()->get<db::Job > ("job")));
    } else {
      setStatus(Task::ERROR);
      setStatusMessage("there is no associated job to this encoding task");
      return;
    }
    
    if (true || getContext()->contains("exporttask.format")) {
      _format = getContext()->getEnvironment<std::string > ("exporttask.format");
    } else {
      setStatus(Task::ERROR);
    }
    if (true || getContext()->contains("exporttask.trg")) {
      _target_file = getContext()->getEnvironment<std::string > ("exporttask.trg");
    } else {
      setStatus(Task::ERROR);
    }
  }

  org::esb::core::OptionsDescription ExportTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("exporttask");
    result.add_options()
            ("exporttask.overwrite", boost::program_options::value<std::string > ()->default_value("false"), "Export task overwrite existing outfile")
            ("exporttask.format", boost::program_options::value<std::string > ()->required(), "Export task container output format")
            ("exporttask.trg", boost::program_options::value<std::string > ()->required(), "Export task file target");
    return result;
  }

  void ExportTask::execute() {
    if (getStatus() == Task::ERROR) {
      LOGERROR("ExportTask have error");
      return;
    }
    org::esb::util::ScopedTimeCounter stc("export");
    std::string base=org::esb::config::Config::get("hive.tmp_path");
    org::esb::io::File inputdir(base+"/jobs/"+_job->uuid.value()+"/collect");
    org::esb::io::FileList filelist = inputdir.listFiles();
    if(filelist.size()==0){
      setStatus(Task::ERROR);
      setStatusMessage(std::string("no files found to export from ").append(base+"/"+_job->uuid.value()+"/collect"));
      return;
    }
    foreach(Ptr<org::esb::io::File> file, filelist) {
      LOGDEBUG("File : " << file->getPath());
      org::esb::io::FileInputStream fis(file.get());
      org::esb::io::ObjectInputStream ois(&fis);
      //org::esb::hive::job::ProcessUnit pu;
      boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit;
      if (ois.readObject(unit) == 0) {
        if (unit->_output_packets.size() > 0) {
          boost::shared_ptr<org::esb::av::Packet> first_packet = unit->_output_packets.front();
          ProcessUnitData data;
          data.id = file->getPath();
          data.startts = first_packet->getDtsTimeStamp();
          data._sequence = unit->_sequence;
          data.encoder = unit->getEncoder();
          data.stream = unit->_target_stream;
          _pudata.push_back(data);
        } else {
          LOGDEBUG("unit->_output_packets.size()<=0");
        }
      } else {
        LOGDEBUG("OIS !=0")
      }
    }
    _pudata.sort();

    /*creating streams here*/
    foreach(ProcessUnitData & data, _pudata) {
      if (_stream_encoder.count(data.stream) == 0) {
        _stream_encoder[data.stream] = data.encoder;
      }
    }

    org::esb::io::File fout(_target_file.c_str());
    LOGDEBUG("Export file to : " << fout.getPath());
    if (getContext()->getEnvironment<std::string> ("exporttask.overwrite")=="false"&&fout.exists()) {
      LOGDEBUG("File exist:" << _target_file);
      setStatus(Task::ERROR);
      setStatusMessage(std::string("file allready exist:").append(_target_file));
      return;
    }
    /*creating output Directory*/
    org::esb::io::File outDirectory(fout.getFilePath().c_str());
    if (!outDirectory.exists()) {
      outDirectory.mkdirs();
    }
    /*openning the OutputStreams*/
    FormatOutputStream * fos = new FormatOutputStream(&fout, _format.c_str());
    PacketOutputStream * pos = new PacketOutputStream(fos, _job_id + ".stats");
    int a = 0;

    foreach(StreamEncoderMap::value_type & data, _stream_encoder) {
      data.second->open();
      StreamData & sd = _in_out_stream_map[data.first];
      sd.next_timestamp = 0;
      sd.last_timestamp = 0;
      sd.out_stream_index = a;
      sd.stream_type = data.second->getCodecType();
      pos->setEncoder(*data.second, a++);
      LOGDEBUG("StreamEncoder:" << data.first);
    }
    pos->init();

    foreach(ProcessUnitData & data, _pudata) {
      LOGDEBUG("StartTs : " << data.startts.toString());
      LOGDEBUG("Sequence : " << data._sequence);
      LOGDEBUG("File:" << data.id);
      LOGDEBUG("TargetStream:" << data.stream);
      LOGDEBUG("Encoder:" << data.encoder->toString());
      org::esb::io::FileInputStream fis(data.id);
      org::esb::io::ObjectInputStream ois(&fis);
      boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit;
      if (ois.readObject(unit) == 0) {

        unit->_output_packets.sort(ptsComparator);

        foreach(PacketPtr p, unit->_output_packets) {
          int idx = p->getStreamIndex();
          p->setPts(_in_out_stream_map[idx].next_timestamp);
          _in_out_stream_map[idx].last_timestamp = _in_out_stream_map[idx].next_timestamp;
          _in_out_stream_map[idx].next_timestamp += p->getDuration();
        }
        unit->_output_packets.sort(dtsComparator);

        foreach(PacketPtr p, unit->_output_packets) {
          int idx = p->getStreamIndex();
          p->packet->stream_index = _in_out_stream_map[idx].out_stream_index;
          pos->writePacket(*p);
        }

      }
    fis.close();
    org::esb::io::File(data.id).deleteFile();
    //infile.deleteFile();

    }
    pos->close();

    fos->close();
    delete pos;
    delete fos;
    setStatus(Task::DONE);
  }

  bool ExportTask::ptsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b) {
    return a->getPts() < b->getPts();
  }

  bool ExportTask::dtsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b) {
    return a->getDts() < b->getDts();
  }

  REGISTER_TASK("ExportTask", ExportTask);

}
