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

namespace encodingtask {

  EncodingTask::EncodingTask() {
  }

  EncodingTask::~EncodingTask() {
  }

  void EncodingTask::prepare() {
    _srcuristr = getContext()->getEnvironment<std::string > ("encodingtask.src");
    _partition = getContext()->getEnvironment<std::string > ("encodingtask.partition");
    std::string profile = getContext()->getEnvironment<std::string > ("encodingtask.profile");
    if (getContext()->contains("job")) {
      _job = Ptr<db::Job > (new db::Job(getContext()->get<db::Job > ("job")));
    } else {
      setStatus(Task::ERROR);
      setStatusMessage("there is no associated job to this encoding task");
    }

    try {
      org::esb::hive::PresetReaderJson reader(profile);
      _codecs = reader.getCodecList();
      _filters = reader.getFilterList();
      _preset = reader.getPreset();
    } catch (std::exception & ex) {
      setStatus(Task::ERROR);
      setStatusMessage("Error while parsing JSON Profile");
    }
  }

  org::esb::core::OptionsDescription EncodingTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("encodingtask");
    result.add_options()
            ("encodingtask.src", boost::program_options::value<std::string > ()->default_value(""), "Encoding task source")
            ("encodingtask.partition", boost::program_options::value<std::string > ()->default_value("global"), "Encoding task partition")
            ("encodingtask.profile", boost::program_options::value<std::string > ()->default_value(""), "Encoding task profile");
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
      setStatusMessage(std::string("Input file ").append(_srcuristr).append(" is not a valid media file!"));
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

      /*create the decoder objects*/
      sdata.decoder = boost::shared_ptr<org::esb::av::Decoder > (new org::esb::av::Decoder(is));
      sdata.pass2decoder = boost::shared_ptr<org::esb::av::Decoder > (new org::esb::av::Decoder(is));

      /*create the encoder objects*/
      typedef std::map<std::string, std::string> Parameter;
      if (sdata.decoder->getCodecType() == AVMEDIA_TYPE_VIDEO) {
        if (_codecs["video"].count("codec_id") != 0) {
          sdata.encoder = boost::shared_ptr<org::esb::av::Encoder > (new org::esb::av::Encoder(_codecs["video"]["codec_id"]));
          sdata.pass2encoder = boost::shared_ptr<org::esb::av::Encoder > (new org::esb::av::Encoder(_codecs["video"]["codec_id"]));

          foreach(Parameter::value_type param, _codecs["video"]) {
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
  }

  void EncodingTask::putToPartition(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit, bool isLast) {
    unit->setJobId(_job->uuid.value());

    partitionservice::PartitionManager::Type t = partitionservice::PartitionManager::TYPE_UNKNOWN;

    if (unit->_decoder->getCodecType() == AVMEDIA_TYPE_AUDIO) {
      t = partitionservice::PartitionManager::TYPE_AUDIO;
    } else if (unit->_decoder->getCodecType() == AVMEDIA_TYPE_VIDEO) {
      t = partitionservice::PartitionManager::TYPE_VIDEO;
    }
    /*create unique stream index*/
    unit->_source_stream = unit->_input_packets.front()->getStreamIndex();
    unit->_last_process_unit = isLast;
    partitionservice::PartitionManager::getInstance()->putProcessUnit(_partition, unit, t);
  }

  void EncodingTask::enQueue(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit, bool isLast) {
    unit->setJobId(_job->uuid.value());

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

  }

  void EncodingTask::putProcessUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit>) {

  }

  REGISTER_TASK("EncodingTask", EncodingTask);

}
