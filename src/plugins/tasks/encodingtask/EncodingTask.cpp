/*
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 *
 * Created on 19. Oktober 2011, 11:41
 */
#include "plugins/services/partitionservice/PartitionManager.h"
#include "org/esb/db/hivedb.hpp"
#include "org/esb/util/Foreach.h"
#include "org/esb/lang/Ptr.h"
#include "Packetizer.h"
#include "ProcessUnitBuilder.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/CodecPropertyTransformer.h"
#include "org/esb/util/ScopedTimeCounter.h"
#include "org/esb/config/config.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/core/TaskException.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "EncodingTask.h"

namespace encodingtask {

  EncodingTask::EncodingTask() : Task() {
    _sequence_counter = 0;
    ConnectionFactory* connectionFactory(ConnectionFactory::createCMSConnectionFactory("failover:tcp://127.0.0.1:61616"));
    connection = connectionFactory->createConnection();
    connection->start();

    session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
    //Session*session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
    destination = session->createQueue("MHIVE.OUTPUT");
    producer = session->createProducer(destination);
    producer->setDeliveryMode(DeliveryMode::PERSISTENT);

    //lastSequence=0;
  }

  EncodingTask::~EncodingTask() {
  }

  void EncodingTask::prepare() {

    LOGDEBUG("PluginContext uuid="+getContext()->get<std::string > ("uuid"))
    database=getContext()->database;
    _srcuristr = getContext()->getEnvironment<std::string > ("encodingtask.src");
    if (_srcuristr.length() == 0) {
      _srcuristr = getSource();
    }
    _partition = getContext()->getEnvironment<std::string > ("encodingtask.partition");
    _task_uuid = getContext()->get<std::string > ("uuid");//org::esb::util::PUUID();//getUUID(); //getContext()->getEnvironment<std::string > ("task.uuid");
    _target_file = getSink();

    std::string profiledata = getContext()->getEnvironment<std::string > ("data");
    //try {
    org::esb::hive::PresetReaderJson reader(profiledata);
    _codecs = reader.getCodecList();
    _filters = reader.getFilterList();
    _preset = reader.getPreset();
    _format = _preset["id"];
    con=new org::esb::mq::QueueConnection("localhost", 20202);

    //Ptr<Task> itask=_sources.front();
    if (getContext()->contains("decoder")) {
      std::map<int, Ptr<org::esb::av::Decoder> >tmp = getContext()->get<std::map<int, Ptr<org::esb::av::Decoder> > >("decoder");
      std::map<int, Ptr<org::esb::av::Decoder> >::iterator it = tmp.begin();

      for (int a=0; it != tmp.end(); it++) {
        if ((*it).second->getCodecType() == AVMEDIA_TYPE_VIDEO) {
          /*special prepare of the encoder for using the input width/heigth*/
          if (_codecs["video"].count("width") == 0 ||
          atoi(_codecs["video"]["width"].c_str()) == 0 ||
          _codecs["video"].count("height") == 0 ||
          atoi(_codecs["video"]["height"].c_str()) == 0) {
            _codecs["video"]["width"] = org::esb::util::StringUtil::toString(tmp[(*it).first]->getWidth());
            _codecs["video"]["height"] = org::esb::util::StringUtil::toString(tmp[(*it).first]->getHeight());
            LOGDEBUG("setting video size from input to : " << _codecs["video"]["width"] << "*" << _codecs["video"]["height"]);
          }

          if (_codecs["video"].count("time_base") == 0 || _codecs["video"]["time_base"].length() == 0) {
            std::ostringstream oss;
            oss << tmp[(*it).first]->getFrameRate().den << "/" << tmp[(*it).first]->getFrameRate().num;
            _codecs["video"]["time_base"] = oss.str();
            LOGDEBUG("setting framerate from input to : " << oss.str());
          }
          _encs[(*it).first]=new Encoder(_codecs["video"]);
          _encs[(*it).first]->setStreamIndex(a++);
          if(!_encs[(*it).first]->open()){
            _encs.erase((*it).first);
            throw org::esb::core::TaskException("could not open Video Encoder");
          }
        }
        if ((*it).second->getCodecType() == AVMEDIA_TYPE_AUDIO) {
          _encs[(*it).first]=new Encoder(_codecs["audio"]);
          _encs[(*it).first]->setStreamIndex(a++);
          if(!_encs[(*it).first]->open()){
            _encs.erase((*it).first);
            throw org::esb::core::TaskException("could not open Audio Encoder");
          }
        }
        StreamData & sd = _in_out_stream_map[(*it).first];
        sd.next_timestamp = 0;
        sd.last_timestamp = 0;
        //sd.out_stream_index = a;
        //sd.stream_type = data.second->getCodecType();

        _packetizer[(*it).first] = StreamPacketizer(0, (*it).second);
        _spu[(*it).first]=StreamProcessUnitBuilder();
        std::string qReadName=_task_uuid+"#read#"+StringUtil::toString( (*it).first );
        std::string qWriteName=_task_uuid+"#write#"+StringUtil::toString( (*it).first );
        /*creating queue for each stream*/
        /*
        if(!con->queueExist(qReadName)){
            con->createQueue(qReadName);
        }
        if(!con->queueExist(qWriteName)){
            con->createQueue(qWriteName);
        }
        _queueMap[(*it).first]=con->getMessageQueue(qReadName);
        */
        _queueMap[(*it).first]=con->getMessageQueue("read_q");
        db::Queue queue(*database);
        //queue.job.link(job);
        queue.outputname=qReadName;
        queue.inputname=qWriteName;
        queue.uuid=_task_uuid;
        if ((*it).second->getCodecType() == AVMEDIA_TYPE_AUDIO) {
          queue.qtype=db::Queue::Qtype::ONE4ALL;
        }
        if ((*it).second->getCodecType() == AVMEDIA_TYPE_VIDEO) {
          queue.qtype=db::Queue::Qtype::ONE2ONE;
        }
        queue.update();

      }
    } else {
      throw org::esb::core::TaskException("could not find decoder in Tasks PluginContext");
    }
    /*
    if(!con->queueExist("read_q-"+_task_uuid)){
        con->createQueue("read_q-"+_task_uuid);
    }
    if(!con->queueExist("write_q-"+_task_uuid)){
        con->createQueue("write_q-"+_task_uuid);
    }
    read_q=con->getMessageQueue("read_q-"+_task_uuid);
    */
    /*
    if(!con->queueExist("read_q")){
        con->createQueue("read_q");
    }
    if(!con->queueExist("write_q")){
        con->createQueue("write_q");
    }
    read_q=con->getMessageQueue("read_q");
    */
    getContext()->setProperty<std::map<int, Ptr<org::esb::av::Encoder> > >("encoder",_encs);

    getContext()->setProperty<std::string > ("profile.name", _preset["name"]);
    getContext()->setProperty<std::string > ("video.codec", _codecs["video"]["codec_id"]);
    if (_codecs["video"].count("multipass") && _codecs["video"]["multipass"] == "1") {
      getContext()->setProperty<std::string > ("video.passes", "2");
    } else {
      getContext()->setProperty<std::string > ("video.passes", "1");
    }
    getContext()->setProperty<std::string > ("video.bitrate", _codecs["video"]["b"]);
    getContext()->setProperty<std::string > ("video.height", _codecs["video"]["height"]);
    getContext()->setProperty<std::string > ("video.width", _codecs["video"]["width"]);

    getContext()->setProperty<std::string > ("audio.codec", _codecs["audio"]["codec_id"]);
    getContext()->setProperty<std::string > ("audio.bitrate", _codecs["audio"]["ab"]);
    getContext()->setProperty<std::string > ("audio.channels", _codecs["audio"]["ac"]);
    getContext()->setProperty<std::string > ("audio.samples", _codecs["audio"]["ar"]);

    /*} catch (std::exception & ex) {
      setStatus(Task::ERROR);
      setStatusMessage(std::string("Error while parsing JSON Profile:").append(ex.what()));
      throw org::esb::core::TaskException(getStatusMessage());
    }*/
    partitionservice::PartitionManager::getInstance()->addCollector(boost::bind(&EncodingTask::collector, this, _1,_2));
    _unit_list.addCallback(boost::bind(&EncodingTask::unitListCallback, this, _1));

    setStatus(PREPARED);
  }

  void EncodingTask::cleanup() {
    partitionservice::PartitionManager::getInstance()->resetFps();
    std::string base = org::esb::config::Config::get("hive.tmp_path");
    org::esb::io::File infile(base + "/jobs/" + getUUID() + "/" + _srcuristr);
    if (infile.exists()) {
      //infile.deleteFile();
    }
  }

  int EncodingTask::getPadTypes() {
    return Task::SINK | Task::SOURCE;
  }

  org::esb::core::OptionsDescription EncodingTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("encodingtask");
    result.add_options()
    ("encodingtask.src", boost::program_options::value<std::string > ()->default_value(""), "Encoding task source file")
    ("encodingtask.partition", boost::program_options::value<std::string > ()->default_value("global"), "Encoding task partition")
    //("encodingtask.profile", boost::program_options::value<std::string > ()->default_value(""), "Encoding task profile");
    ("encodingtask.profiledata", boost::program_options::value<std::string > ()->default_value(""), "Encoding task profile data")
    ("data", boost::program_options::value<std::string > ()->default_value(""), "Encoding task profile data");
    return result;
  }

  void EncodingTask::observeProgress() {
    while (getStatus() == Task::EXECUTE) {
      setProgress(getProgressLength() - partitionservice::PartitionManager::getInstance()->getSize(_partition));
      org::esb::lang::Thread::sleep2(1 * 1000);
    }
  }

  void EncodingTask::unitListCallback(boost::shared_ptr<org::esb::hive::job::ProcessUnit> unit){
    unit->_output_packets.sort(EncodingTask::ptsComparator);
    foreach(PacketPtr p, unit->_output_packets) {
      int idx = p->getStreamIndex();
      p->setPts(_in_out_stream_map[idx].next_timestamp);
      _in_out_stream_map[idx].last_timestamp = _in_out_stream_map[idx].next_timestamp;
      _in_out_stream_map[idx].next_timestamp += p->getDuration();
    }
    unit->_output_packets.sort(EncodingTask::dtsComparator);
    foreach(boost::shared_ptr<Packet>  p,unit->_output_packets){
      Task::pushBuffer(Ptr<Packet>(p));
    }
  }

  void EncodingTask::collector(boost::shared_ptr<org::esb::hive::job::ProcessUnit> unit, boost::asio::ip::tcp::endpoint ep)
  {
    //boost::mutex::scoped_lock partition_get_lock(_partition_mutex);

    std::cerr<<unit->getJobId()<<"=="<<_task_uuid<<":"<<boost::this_thread::get_id()<<" collector invoked "<<unit->getDecoder()->getCodecName()<<", pu count"<<unit->_output_packets.size()<<" sequence:"<<unit->_sequence<<std::endl;
    if(unit->getJobId()==_task_uuid)
      _unit_list.pushUnit(unit);
    /*
    while(_unit_list.size()&&(*_unit_list.begin())->_sequence==lastSequence){
      std::cerr<<"lastSequence found:"<<lastSequence<<std::endl;
      foreach(boost::shared_ptr<Packet>  p,(*_unit_list.begin())->_output_packets){
        Task::pushBuffer(Ptr<Packet>(p));
      }
      _unit_list.erase(_unit_list.begin());
      lastSequence++;
    }*/
  }

  void EncodingTask::pushBuffer(Ptr<Packet>p) {
    if (!getStatus() == PREPARED) {
      setStatusMessage(std::string("Task is not in PREPARED State"));
      throw org::esb::core::TaskException(getStatusMessage());
    }
    //LOGDEBUG("Encoding push")
    if(p){
      if(_packetizer.count(p->getStreamIndex())){
        StreamPacketizer & pti = _packetizer[p->getStreamIndex()];
        StreamProcessUnitBuilder & spub=_spu[p->getStreamIndex()];
        if(!pti.getDecoder()){
          return;
        }
        if (pti.putPacket(p)) {
          /*when the packet count per ProcessUnit reached*/
          PacketListPtr list = pti.removePacketList();
          Ptr<org::esb::av::Encoder>enc=_encs[p->getStreamIndex()];
          boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit = spub.build(list, pti.getDecoder(), enc);
          putToPartition(unit);
          LOGDEBUG("Process PU");
        }
      }else{
        LOGERROR("no StreamPacketizer found for stream:"<<p->getStreamIndex());
      }
    }else{
      /*flush buffer for all streams*/
      std::cerr<<"flushing packets"<<std::endl;
      std::map<int,StreamPacketizer>::iterator it=_packetizer.begin();
      for(;it!=_packetizer.end();it++){
        (*it).second.flushStreams();
        PacketListPtr list = (*it).second.removePacketList();
        if(list.size()){
          std::cerr<<"putflushto partition"<<std::endl;
          Ptr<org::esb::av::Encoder>enc=_encs[(*it).first];
          StreamProcessUnitBuilder & spub=_spu[(*it).first];
          boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit = spub.build(list, (*it).second.getDecoder(), enc);
          putToPartition(unit, true);
        }
      }
    }
  }

  void EncodingTask::execute() {
    Task::execute();
    return;
  }

  void EncodingTask::putToPartition(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit, bool isLast) {
    org::esb::mq::ObjectMessage msg;
    unit->_sequence = _sequence_counter++;
    //std::cerr <<"Sequence="<<_sequence_counter<<std::endl;
    unit->setJobId(_task_uuid);
    //unit->setJobId("121212");

    partitionservice::PartitionManager::Type t = partitionservice::PartitionManager::TYPE_UNKNOWN;
    LOGDEBUG("CodecType:" << unit->_decoder->getCodecType());
    if (unit->_decoder->getCodecType() == AVMEDIA_TYPE_AUDIO) {
      msg.setLabel(_task_uuid+org::esb::util::StringUtil::toString(unit->_input_packets.front()->getStreamIndex()));
      t = partitionservice::PartitionManager::TYPE_AUDIO;
    } else if (unit->_decoder->getCodecType() == AVMEDIA_TYPE_VIDEO) {
      t = partitionservice::PartitionManager::TYPE_VIDEO;
    }
    /*create unique stream index*/
    unit->_source_stream = unit->_input_packets.front()->getStreamIndex();
    unit->_last_process_unit = isLast;
    BytesMessage * bmsg=serializeProcessUnit(unit);
    producer->send(bmsg);
    delete bmsg;
    //msg.setObject(unit);
    //_queueMap[unit->_source_stream]->Enqueue(msg);
    db::ProcessUnit pu(*database);
    pu.sorcestream=unit->_source_stream;
    pu.targetstream=unit->_target_stream;
    if (unit->_input_packets.size() > 0) {
      boost::shared_ptr<org::esb::av::Packet> first_packet = unit->_input_packets.front();
      boost::shared_ptr<org::esb::av::Packet> last_packet = unit->_input_packets.back();
      pu.startts=(double)first_packet->getDts();
      pu.endts=(double)last_packet->getDts();
      pu.framecount=(int)unit->_input_packets.size();
    }
    std::ostringstream oss;
    oss << msg.getMessageID();
    pu.sendid=std::string(oss.str());

    pu.update();
    //read_q->Enqueue(msg);
    //partitionservice::PartitionManager::getInstance()->putProcessUnit(_partition, unit, t);
    setProgressLength(getProgressLength() + 1);
  }

  void EncodingTask::enQueue(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit, bool isLast) {
    unit->setJobId(_task_uuid);

    partitionservice::PartitionManager::Type t = partitionservice::PartitionManager::TYPE_UNKNOWN;

    if (unit->_decoder->getCodecType() == AVMEDIA_TYPE_AUDIO) {
      t = partitionservice::PartitionManager::TYPE_AUDIO;
    } else if (unit->_decoder->getCodecType() == AVMEDIA_TYPE_VIDEO) {
      t = partitionservice::PartitionManager::TYPE_VIDEO;
    }
    /*create unique stream index*/
    unit->_source_stream = unit->_input_packets.front()->getStreamIndex();
    unit->_last_process_unit = isLast;
  }

  boost::shared_ptr<org::esb::hive::job::ProcessUnit> EncodingTask::getProcessUnit() {
    boost::shared_ptr<org::esb::hive::job::ProcessUnit> u;
    //u = puQueue.dequeue();
    return u;
  }

  void EncodingTask::putProcessUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit>) {

  }

  void EncodingTask::exportFile() {
    if (getStatus() == Task::ERROR) {
      LOGERROR("ExportTask have error");
      return;
    }
    org::esb::util::ScopedTimeCounter stc("export");
    std::string base = org::esb::config::Config::get("hive.tmp_path");
    org::esb::io::File inputdir(base + "/jobs/" + _task_uuid + "/collect");
    org::esb::io::FileList filelist = inputdir.listFiles();
    if (filelist.size() == 0) {
      setStatus(Task::ERROR);
      setStatusMessage(std::string("no files found to export from ").append(base + "/" + _task_uuid + "/collect"));
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

    org::esb::io::File fout(base + "/jobs/" + _task_uuid + "/" + _target_file);
    LOGDEBUG("Export file to : " << fout.getPath());
    if (getContext()->getEnvironment<std::string > ("exporttask.overwrite") == "false" && fout.exists()) {
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
    PacketOutputStream * pos = new PacketOutputStream(fos, base + "/jobs/" + _task_uuid + "/" + getSink() + ".stats");
    if (fos->_fmt->extensions) {
      org::esb::util::StringTokenizer tok(fos->_fmt->extensions, ",");
      std::string ext = tok.nextToken();
      if (ext.length()) {
        getContext()->set<std::string > ("profile.ext", ext);
      }
    }

    if (getContext()->get<std::string > ("profile.ext").length() == 0) {
      getContext()->set<std::string > ("profile.ext", "unknown");
    }
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

        unit->_output_packets.sort(EncodingTask::ptsComparator);

        foreach(PacketPtr p, unit->_output_packets) {
          int idx = p->getStreamIndex();
          p->setPts(_in_out_stream_map[idx].next_timestamp);
          _in_out_stream_map[idx].last_timestamp = _in_out_stream_map[idx].next_timestamp;
          _in_out_stream_map[idx].next_timestamp += p->getDuration();
        }
        unit->_output_packets.sort(EncodingTask::dtsComparator);

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
    //setStatus(Task::DONE);

  }

  bool EncodingTask::ptsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b) {
    return a->getPts() < b->getPts();
  }

  bool EncodingTask::dtsComparator(boost::shared_ptr<Packet> a, boost::shared_ptr<Packet> b) {
    return a->getDts() < b->getDts();
  }

  REGISTER_TASK("EncodingTask", EncodingTask)

}
