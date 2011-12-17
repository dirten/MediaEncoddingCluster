/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "org/esb/util/Foreach.h"
#include "EncodingTask.h"
#include "org/esb/lang/Ptr.h"
#include "StreamData.h"
#include "Packetizer.h"
#include "ProcessUnitBuilder.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "plugins/services/partitionservice/PartitionManager.h"
#include "org/esb/av/CodecPropertyTransformer.h"

namespace encodingtask {

  EncodingTask::EncodingTask() {
    _sequence_counter = 0;
  }

  EncodingTask::~EncodingTask() {
  }

  void EncodingTask::prepare() {
    _srcuristr = getContext()->getEnvironment<std::string > ("encodingtask.src");
    if(_srcuristr.length()==0){
      _srcuristr=getSource();
    }
    _partition = getContext()->getEnvironment<std::string > ("encodingtask.partition");
    _task_uuid = getContext()->getEnvironment<std::string > ("task.uuid");
    
    //std::string profile = getContext()->getEnvironment<std::string > ("encodingtask.profile");
    std::string profiledata = getContext()->getEnvironment<std::string > ("encodingtask.profiledata");
    if(profiledata.length()==0)
        profiledata = getContext()->getEnvironment<std::string > ("data");
    /*
    if (getContext()->contains("job")) {
      _job = Ptr<db::Job > (new db::Job(getContext()->get<db::Job > ("job")));
    } else {
      setStatus(Task::ERROR);
      setStatusMessage("there is no associated job to this encoding task");
      //return;
    }*/
    try{
      org::esb::hive::PresetReaderJson reader(profiledata);
      _codecs = reader.getCodecList();
      _filters = reader.getFilterList();
      _preset = reader.getPreset();
    } catch (std::exception & ex) {
      setStatus(Task::ERROR);
      setStatusMessage(std::string("Error while parsing JSON Profile:").append(ex.what()));
    }
  }
  int EncodingTask::getPadTypes(){
    return Task::SINK|Task::SOURCE;
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

  void EncodingTask::execute() {
    /*check if we had some errors before*/
    if (getStatus() == Task::ERROR)
      return;
    /*open the input file*/
    org::esb::av::FormatInputStream fis(_srcuristr);

    /*check the input is valid*/
    if (!fis.isValid()) {
      setStatus(Task::ERROR);
      setStatusMessage(std::string("Input file \"").append(_srcuristr).append("\" is not a valid media file!"));
      return;
    }

    /*preprocessing the input streams*/
    int scount = fis.getStreamCount();
    map<int, StreamData> stream_map;
    for (int a = 0; a < scount; a++) {
      /*getting the input stream from the file*/
      org::esb::av::AVInputStream* is = fis.getAVStream(a);

      /*create a stream data element for each stream from the input file*/
      StreamData & sdata = stream_map[is->index];
      sdata.last_start_dts = 0;
      sdata.min_packet_count = 0;
      sdata.outstream = a;
      /*create the decoder objects*/
      sdata.decoder = boost::shared_ptr<org::esb::av::Decoder > (new org::esb::av::Decoder(is));
      sdata.pass2decoder = boost::shared_ptr<org::esb::av::Decoder > (new org::esb::av::Decoder(is));

      /*special need to create a fresh decoder for mpeg2*/
      if (is->codec->codec_id == CODEC_ID_MPEG2VIDEO) {
        LOGDEBUG("MPeg2Video Decoder Found");
        typedef std::map<std::string, std::string> Props;
        Props props = sdata.decoder->getCodecOptions();

        sdata.decoder = boost::shared_ptr<org::esb::av::Decoder > (new org::esb::av::Decoder(is->codec->codec_id));
        sdata.pass2decoder = boost::shared_ptr<org::esb::av::Decoder > (new org::esb::av::Decoder(is->codec->codec_id));

        foreach(Props::value_type prop, props) {
          LOGDEBUG("Key=" << prop.first << " val=" << prop.second);
          sdata.decoder->setCodecOption(prop.first, prop.second);
          sdata.pass2decoder->setCodecOption(prop.first, prop.second);
        }
        sdata.decoder->setFrameRate(is->r_frame_rate);
        sdata.pass2decoder->setFrameRate(is->r_frame_rate);
        sdata.decoder->ctx->time_base = is->codec->time_base;
        sdata.decoder->ctx->ticks_per_frame = is->codec->ticks_per_frame;
        sdata.pass2decoder->ctx->ticks_per_frame = is->codec->ticks_per_frame;

        sdata.decoder->ctx->width = is->codec->width;
        sdata.decoder->ctx->height = is->codec->height;
        sdata.decoder->ctx->extradata_size = is->codec->extradata_size;
        LOGDEBUG("ExtradataSize:" << sdata.decoder->ctx->extradata_size);
        if (sdata.decoder->ctx->extradata_size > 0) {
          sdata.decoder->ctx->extradata = (uint8_t*) av_malloc(sdata.decoder->ctx->extradata_size);
          //memcpy(sdata.decoder->ctx->extradata, is->codec->extradata, sdata.decoder->ctx->extradata_size);
          memset(sdata.decoder->ctx->extradata, 0, sdata.decoder->ctx->extradata_size);
        } else
          sdata.decoder->ctx->extradata = NULL;

        sdata.pass2decoder->ctx->time_base = is->codec->time_base;
        sdata.pass2decoder->ctx->width = is->codec->width;
        sdata.pass2decoder->ctx->height = is->codec->height;
        sdata.pass2decoder->ctx->extradata_size = is->codec->extradata_size;
        LOGDEBUG("ExtradataSize:" << sdata.pass2decoder->ctx->extradata_size);
        if (sdata.pass2decoder->ctx->extradata_size > 0) {
          sdata.pass2decoder->ctx->extradata = (uint8_t*) av_malloc(sdata.pass2decoder->ctx->extradata_size);
          //memcpy(sdata.pass2decoder->ctx->extradata, is->codec->extradata, sdata.pass2decoder->ctx->extradata_size);
          memset(sdata.pass2decoder->ctx->extradata, 0, sdata.pass2decoder->ctx->extradata_size);
        } else
          sdata.pass2decoder->ctx->extradata = NULL;


        sdata.decoder->open();
        sdata.pass2decoder->open();
      }

      //sdata.decoder->reset();
      //sdata.pass2decoder->reset();
      /*create the encoder objects*/
      typedef std::map<std::string, std::string> Parameter;
      if (sdata.decoder->getCodecType() == AVMEDIA_TYPE_VIDEO) {
        if (_codecs["video"].count("codec_id") != 0) {
          sdata.encoder = boost::shared_ptr<org::esb::av::Encoder > (new org::esb::av::Encoder(_codecs["video"]["codec_id"]));
          sdata.pass2encoder = boost::shared_ptr<org::esb::av::Encoder > (new org::esb::av::Encoder(_codecs["video"]["codec_id"]));
          org::esb::av::CodecPropertyTransformer transformer(_codecs["video"]);
          std::map<std::string, std::string> params = transformer.getCodecProperties();

          foreach(Parameter::value_type param, params) {
            LOGDEBUG("Parameter key=" << param.first << " value=" << param.second);
            sdata.encoder->setCodecOption(param.first, param.second);
            sdata.pass2encoder->setCodecOption(param.first, param.second);
          }
        } else {
          LOGERROR("Profile does not define a video codec");
        }
      }
      if (sdata.decoder->getCodecType() == AVMEDIA_TYPE_AUDIO) {
        if (_codecs["audio"].count("codec_id") != 0) {
          sdata.encoder = boost::shared_ptr<org::esb::av::Encoder > (new org::esb::av::Encoder(_codecs["audio"]["codec_id"]));
          sdata.pass2encoder = boost::shared_ptr<org::esb::av::Encoder > (new org::esb::av::Encoder(_codecs["audio"]["codec_id"]));

          foreach(Parameter::value_type param, _codecs["audio"]) {
            LOGDEBUG("Parameter key=" << param.first << " value=" << param.second);
            sdata.encoder->setCodecOption(param.first, param.second);
            sdata.pass2encoder->setCodecOption(param.first, param.second);
          }
        } else {
          LOGERROR("Profile does not define an audio codec");
        }
      }
      if (sdata.encoder) {
        sdata.encoder->open();
        LOGDEBUG(sdata.encoder->toString());
      } else {
        stream_map.erase(is->index);
        LOGERROR("Codec not found")
      }
    }
    
    org::esb::av::PacketInputStream pis(&fis);

    Packetizer packetizer(stream_map);
    ProcessUnitBuilder builder(stream_map);
    Packet * packet;

    while (getStatus() != Task::INTERRUPT && getStatus() != Task::ERROR && (packet = pis.readPacket()) != NULL) {
      /**
       * building a shared Pointer from packet because the next read from PacketInputStream kills the Packet data
       */
      boost::shared_ptr<Packet> pPacket(packet);
      /**
       * if the actuall stream not mapped then discard this and continue with next packet
       */
      if (stream_map.find(packet->packet->stream_index) == stream_map.end())
        continue;
      /**
       * if the actual packets dts is lower than the last packet.dts encoded, then discard this packet
       * this is for the behaviour that the server process restarts an unfinished encoding
       * @TODO: writing detailed tests for this !!!
       */
      if (stream_map[packet->packet->stream_index].last_start_dts > packet->packet->dts)
        continue;
      //              LOGTRACE("Packet DTS:"<<packet->toString());
      //pPacket->setStreamIndex(stream_map[pPacket->getStreamIndex()].outstream);
      //LOGDEBUG("PacketStreamIndex:"<<packet->getStreamIndex());
      if (packetizer.putPacket(pPacket)) {
        LOGDEBUG("PacketizerListPtr ready, build ProcessUnit");
        if (getStatus() == Task::INTERRUPT) {
          setStatus(Task::INTERRUPTED);
          return;
        }
        PacketListPtr packets = packetizer.removePacketList();
        boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit = builder.build(packets);
        putToPartition(unit);

        //putToQueue(unit);
        //wait_for_queue = true;
      }
    }
    LOGDEBUG("Flush streams");
    /*calling flush Method in the Packetizer to get the last pending packets from the streams*/
    packetizer.flushStreams();
    int pc = packetizer.getPacketListCount();
    for (int a = 0; a < pc; a++) {
      PacketListPtr packets = packetizer.removePacketList();
      if (packets.size() > 0) {

        boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit = builder.build(packets);
        putToPartition(unit, true);
      }
    }
    
    while (partitionservice::PartitionManager::getInstance()->getSize(_partition) > 0) {
      setProgress(getProgressLength()-partitionservice::PartitionManager::getInstance()->getSize(_partition));
      org::esb::lang::Thread::sleep2(1 * 1000);
    }
    setStatus(Task::DONE);
    setStatusMessage("Encoding completed successfull");
  }

  void EncodingTask::putToPartition(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit, bool isLast) {
    unit->_sequence = _sequence_counter++;
    unit->setJobId(_task_uuid);

    partitionservice::PartitionManager::Type t = partitionservice::PartitionManager::TYPE_UNKNOWN;
    LOGDEBUG("CodecType:" << unit->_decoder->getCodecType());
    if (unit->_decoder->getCodecType() == AVMEDIA_TYPE_AUDIO) {
      t = partitionservice::PartitionManager::TYPE_AUDIO;
    } else if (unit->_decoder->getCodecType() == AVMEDIA_TYPE_VIDEO) {
      t = partitionservice::PartitionManager::TYPE_VIDEO;
    }
    /*create unique stream index*/
    unit->_source_stream = unit->_input_packets.front()->getStreamIndex();
    unit->_last_process_unit = isLast;
    partitionservice::PartitionManager::getInstance()->putProcessUnit(_partition, unit, t);
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

  REGISTER_TASK("EncodingTask", EncodingTask);

}
