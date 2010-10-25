#include "FileExporter.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"

#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/config/config.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/hive/CodecFactory.h"
#include <boost/filesystem/exception.hpp>
#include <map>
#include <algorithm>
#include <stdint.h>
using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::util;
using namespace org::esb::lang;
using namespace org::esb::hive;
using namespace org::esb::config;

std::map<int, FileExporter::StreamData> FileExporter::_source_stream_map;

void FileExporter::exportFile(db::MediaFile outfile) {
  /*creatign the filename*/
  std::string filename;
  if (outfile.path.value().size() == 0) {
    filename = org::esb::config::Config::getProperty("hive.base_path");
  } else {
    filename = outfile.path;
  }
  filename += "/";
  filename += outfile.filename;
  db::Job job=outfile.jobsout().get().one();
  org::esb::io::File fout(filename.c_str());
  if(fout.exists()){
    LOGDEBUG("File exist:"<<filename);

    db::JobLog log(job.getDatabase());
    std::string message="could not export the file ";
    message+=filename;
    message+=" -- file allready exist on the traget!";
    log.message=message;
    log.update();
    job.joblog().link(log);
    job.status="warning";
    job.update();
    return;
  }

  /*creating output Directory*/
  org::esb::io::File outDirectory(fout.getFilePath().c_str());
  if (!outDirectory.exists()) {
//    try {
      outDirectory.mkdir();
//    } catch (boost::filesystem::filesystem_error & e) {
//      LOGERROR(e.what());
//      return;
//    }
  }
  /*openning the OutputStreams*/
  FormatOutputStream * fos = new FormatOutputStream(&fout, outfile.containertype.value().c_str());
  PacketOutputStream * pos = new PacketOutputStream(fos);
  
  vector<db::Stream> streams=outfile.streams().get().all();
  vector<db::Stream>::iterator stream_it=streams.begin();
  litesql::Or expr((db::ProcessUnit::Targetstream>0),litesql::Expr());
  std::string sql_expr="ProcessUnit_.targetstream_ IN(";
  bool success=true;
  for(int a=0;stream_it!=streams.end();stream_it++){
    db::Stream stream=(*stream_it);
    db::JobDetail d=stream.jobsout().get().one();
    LOGDEBUG(d);
    db::Stream inStream=d.inputstream().get().one();

    boost::shared_ptr<Encoder> codec = CodecFactory::getStreamEncoder(stream.id);
    if(codec->open()){
      pos->setEncoder(*codec, a);
      sql_expr+=StringUtil::toString(stream.id.value());
      sql_expr+=", ";
      
      _source_stream_map[inStream.streamindex].last_timestamp = 0;
      _source_stream_map[inStream.streamindex].next_timestamp = 0;
      _source_stream_map[inStream.streamindex].out_stream_index=stream.streamindex;
      a++;
    }else{
      success=false;
    }
  }
  sql_expr=sql_expr.erase(sql_expr.length()-2,2);
  sql_expr+=")";
  LOGDEBUG("expression="<<sql_expr);
  if(!success){
    db::JobLog log(job.getDatabase());
    std::string message="could not create all codecs to write the output file ";
    message+=filename;
    log.message=message;
    log.update();
    job.joblog().link(log);
    job.status="warning";
    job.update();
//    return;
  }
  pos->init();
  litesql::Cursor<db::ProcessUnit> units=litesql::select<db::ProcessUnit>(outfile.getDatabase(),litesql::RawExpr(sql_expr)).cursor();
  





  std::string path = org::esb::config::Config::getProperty("hive.base_path");
    path += "/tmp/";
    for(;units.rowsLeft();units++) {
      db::ProcessUnit unit=(*units);

      int pu_id = unit.id;
      //      logdebug("open PU with id : " << pu_id)
      std::string name = path;
      name += org::esb::util::StringUtil::toString(pu_id % 10);
      name += "/";
      name += org::esb::util::StringUtil::toString(pu_id);
      name += ".unit";
      org::esb::io::File infile(name.c_str());
      if (!infile.exists()) {
        LOGERROR(infile.getFileName() << ": not found, this may lead in a resulting audio/video desync");
        if(false){
        db::JobLog log(job.getDatabase());
        std::string message=infile.getFileName();
        message+=": not found, this may lead in a resulting audio/video desync";
        log.message=message;
        log.update();
        job.joblog().link(log);
        }
        continue;
      }
      org::esb::io::FileInputStream fis(&infile);
      org::esb::io::ObjectInputStream ois(&fis);
      org::esb::hive::job::ProcessUnit pu;
      if (ois.readObject(pu) != 0) {
        if(false){
        db::JobLog log(job.getDatabase());
        std::string message=infile.getFileName();
        message+=": error reading archive!";
        log.message=message;
        log.update();
        job.joblog().link(log);
        }
        LOGERROR("reading archive # " << pu_id);
        continue;
      }


      LOGDEBUG("resorting Packets");
      pu._output_packets.sort(ptsComparator);
      std::list<boost::shared_ptr<Packet> >::iterator ptslist = pu._output_packets.begin();
      for (; ptslist != pu._output_packets.end(); ptslist++) {
        Packet * p = (*ptslist).get();
        int idx = p->getStreamIndex();
        LOGTRACE("resorting Packets pts to " << p->toString());
        p->setPts(_source_stream_map[idx].next_timestamp);
        LOGTRACE("resorting Packets pts to " << _source_stream_map[idx].next_timestamp);
        //        p->setTimeBase(_source_stream_map[idx].packet_timebase);
        //        p->setDuration(_source_stream_map[idx].packet_duration);
        _source_stream_map[idx].last_timestamp = _source_stream_map[idx].next_timestamp;
        //        _source_stream_map[idx].next_timestamp += _source_stream_map[idx].packet_duration;
        _source_stream_map[idx].next_timestamp += p->getDuration();
      }
      /**
       * reorder right back to dts
       */
      pu._output_packets.sort(dtsComparator);

      std::list<boost::shared_ptr<Packet> >::iterator plist = pu._output_packets.begin();
      for (; plist != pu._output_packets.end(); plist++) {
        Packet * p = (*plist).get();
        int idx = p->getStreamIndex();
        p->packet->stream_index = _source_stream_map[idx].out_stream_index;
        pos->writePacket(*p);
      }

/**
       * clean up temporary files, they are no longer needed
       */
      fis.close();
//      infile.deleteFile();
    }
    if(true){
    db::JobLog log(job.getDatabase());
    std::string message=filename;
    message+=" exported.";
    log.message=message;
    log.update();
    job.joblog().link(log);

    job.status="exported";
    job.update();
    }
  pos->close();

  fos->close();
  FormatInputStream fis(&fout);

  outfile.filesize=(double)fis.getFileSize();
  outfile.duration=(double)fis.getFormatContext()->duration;
  outfile.update();

  delete pos;
  delete fos;


}


bool FileExporter::ptsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b) {
  return a->getPts() < b->getPts();
}

bool FileExporter::dtsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b) {
  return a->getDts() < b->getDts();
}

FileExporter::FileExporter(void) {
}

FileExporter::~FileExporter(void) {
}
