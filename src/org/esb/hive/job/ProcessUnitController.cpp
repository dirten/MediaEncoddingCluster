/* 
 * File:   ProcessUnitController.cpp
 * Author: HoelscJ
 * 
 * Created on 16. Juli 2010, 17:09
 */

#include "org/esb/db/hivedb.hpp"
#include "ProcessUnitController.h"
#include "ProcessUnitBuilder.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/hive/job/Packetizer.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "boost/thread.hpp"
#include "boost/shared_ptr.hpp"
#include "JobController.h"
#include "StreamData.h"
#include "org/esb/hive/CodecFactory.h"

using namespace db;
using namespace org::esb::av;

namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        void ProcessUnitController::onMessage(org::esb::signal::Message & msg) {
          if (msg.getProperty("processunitcontroller") == "start") {
            LOGDEBUG("start request");
            boost::thread t(boost::bind(&ProcessUnitController::start, this));
            LOGDEBUG("started");
          } else if (msg.getProperty("processunitcontroller") == "stop") {
            LOGDEBUG("stop request");
            _stop_signal = true;
            LOGDEBUG("stopped");
          } else if (msg.getProperty("processunitcontroller") == "GET_PROCESS_UNIT") {
            LOGDEBUG("GET_PROCESS_UNIT request");
          } else if (msg.getProperty("processunitcontroller") == "GET_AUDIO_PROCESS_UNIT") {
            LOGDEBUG("GET_AUDIO_PROCESS_UNIT request");
          } else if (msg.getProperty("processunitcontroller") == "PUT_PROCESS_UNIT") {
            LOGDEBUG("PUT_PROCESS_UNIT request");
          } else if (msg.getProperty("processunitcontroller") == "PUT_AUDIO_PROCESS_UNIT") {
            LOGDEBUG("PUT_AUDIO_PROCESS_UNIT request");
          }
        }

        ProcessUnitController::ProcessUnitController() {
          LOGTRACEMETHOD("ProcessUnitController::ProcessUnitController()");
          _stop_signal = false;
        }

        void ProcessUnitController::start() {
          LOGTRACEMETHOD("void ProcessUnitController::start() ")
          //          JobController job_ctrl;
          db::HiveDb _dbCon("mysql", org::esb::config::Config::getProperty("db.url"));
          while (!_stop_signal) {
            try {
              db::Job job = litesql::select<db::Job > (_dbCon, db::Job::Begintime == 0).one();
              //db::Job job = job_ctrl.getJob();
              LOGDEBUG("new job found")
              processJob(job);

            } catch (litesql::NotFound ex) {
              LOGDEBUG("no new job found");
              org::esb::lang::Thread::sleep2(1000);
            }
          }
        }

        ProcessUnitController::ProcessUnitController(const ProcessUnitController& orig) {

        }

        ProcessUnitController::~ProcessUnitController() {
          LOGTRACEMETHOD("ProcessUnitController::~ProcessUnitController()")
        }

        void ProcessUnitController::processJob(db::Job job) {
          LOGTRACEMETHOD("void ProcessUnitController::processJob(db::Job job)")
          MediaFile infile = job.inputfile().get().one();

          string filename = infile.path + "/" + infile.filename;
          org::esb::io::File fi(filename);
          org::esb::av::FormatInputStream fis(&fi);
          if (fis.isValid()) {
            map<int, StreamData> stream_map;
            std::map<int, Packetizer::StreamData> stream_data;
            vector<JobDetail>details = job.jobdetails().get().all();
            vector<JobDetail>::iterator s_it = details.begin();
            for (; s_it != details.end(); s_it++) {
              JobDetail detail = *s_it;
              int idx = detail.inputstream().get().one().streamindex;
              stream_map[idx].instream = detail.inputstream().get().one().id;
              stream_map[idx].outstream = detail.outputstream().get().one().id;
              stream_map[idx].decoder = CodecFactory::getStreamDecoder(stream_map[idx].instream);
              stream_map[idx].encoder = CodecFactory::getStreamEncoder(stream_map[idx].outstream);

              stream_map[idx].last_start_pts = detail.inputstream().get().one().firstpts;
              stream_map[idx].last_start_dts = detail.inputstream().get().one().firstdts;

              if (stream_map[idx].decoder->getTimeBase().num <= 0 || stream_map[idx].decoder->getTimeBase().den <= 0) {
                LOGERROR("wrong decoder timebase -> num=" << stream_map[idx].decoder->getTimeBase().num << " den=" << stream_map[idx].decoder->getTimeBase().den);
                LOGERROR("skip stream #" << stream_map[idx].instream);
                stream_map.erase(idx);
                continue;
              }
              stream_data[idx].decoder = stream_map[idx].decoder;
              stream_data[idx].encoder = stream_map[idx].encoder;
            }
            PacketInputStream pis(&fis);

            Packetizer packetizer(stream_data);
            ProcessUnitBuilder builder(stream_map);
            Packet * packet;
            /**
             * read while packets in the stream
             * @TODO: performance bottleneck in read packet and the resulting copy of the Packet
             */
            while ((packet = pis.readPacket()) != NULL /*&& !_isStopSignal*/) {
              /**
               * building a shared Pointer from packet because the next read from PacketInputStream kills the Packet data
               */
              boost::shared_ptr<Packet> pPacket(packet);
              /**
               * if the actuall stream not mapped then discard this and continue with next packet
               */
              if (stream_map.find(packet->packet->stream_index) == stream_map.end() ||
                      stream_map[packet->packet->stream_index].last_start_dts > packet->packet->dts
                      || (packet->packet->pts != AV_NOPTS_VALUE && stream_map[packet->packet->stream_index].last_start_pts > packet->packet->pts)
                      ) {
                continue;
              }

              if (packetizer.putPacket(pPacket)) {
                LOGDEBUG("PacketizerListPtr ready, build ProcessUnit");
                PacketListPtr packets = packetizer.removePacketList();
                boost::shared_ptr<ProcessUnit>unit = builder.build(packets);
                /*
                if (unit->_decoder->getCodecType() == CODEC_TYPE_AUDIO) {
                  audioQueue.enqueue(unit);
                } else {
                  puQueue.enqueue(unit);
                }*/
              }
            }
          } else {
            LOGERROR("Error Opening Input Streams from " << filename);
          }
          job.begintime = 1;
          job.update();
        }
      }
    }
  }
}