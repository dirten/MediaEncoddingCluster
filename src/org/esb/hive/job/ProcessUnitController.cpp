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
#include "org/esb/util/Decimal.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/hive/job/Packetizer.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"

#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"

#include "boost/thread.hpp"
#include "boost/shared_ptr.hpp"
#include "JobController.h"
#include "StreamData.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/hive/DatabaseService.h"

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
            boost::shared_ptr<ProcessUnit> unit = getProcessUnit();
            msg.setProperty("processunit", unit);
          } else if (msg.getProperty("processunitcontroller") == "GET_AUDIO_PROCESS_UNIT") {
            LOGDEBUG("GET_AUDIO_PROCESS_UNIT request");
            boost::shared_ptr<ProcessUnit> unit = getAudioProcessUnit();
            msg.setProperty("processunit", unit);
          } else if (msg.getProperty("processunitcontroller") == "PUT_PROCESS_UNIT") {
            LOGDEBUG("PUT_PROCESS_UNIT request");
			return;
            boost::shared_ptr<ProcessUnit> unit = msg.getPtrProperty("processunit");
            putProcessUnit(unit);
          }
        }

        ProcessUnitController::ProcessUnitController() : 
          _dbCon("mysql", org::esb::config::Config::getProperty("db.url")),
          _dbJobCon("mysql", org::esb::config::Config::getProperty("db.url")){
          LOGTRACEMETHOD("ProcessUnitController::ProcessUnitController()");
          _stop_signal = false;
          _isRunning = false;

        }

        ProcessUnitController::ProcessUnitController(const ProcessUnitController& orig) : 
          _dbCon(orig._dbCon),
          _dbJobCon(orig._dbJobCon){

        }

        ProcessUnitController::~ProcessUnitController() {
          LOGTRACEMETHOD("ProcessUnitController::~ProcessUnitController()")
        }

        void ProcessUnitController::start() {
          LOGTRACEMETHOD("void ProcessUnitController::start() ")
                  //          JobController job_ctrl;
                  //db::HiveDb _dbCon("mysql", org::esb::config::Config::getProperty("db.url"));
              org::esb::hive::DatabaseService::thread_init();
          while (!_stop_signal) {
            try {
              db::Job job = litesql::select<db::Job > (_dbJobCon, db::Job::Begintime == -1).one();
              //db::Job job = job_ctrl.getJob();
              LOGDEBUG("new job found")
              processJob(job);

            } catch (litesql::NotFound ex) {
              LOGDEBUG("no new job found");
              org::esb::lang::Thread::sleep2(1000);
            }
          }
	  org::esb::hive::DatabaseService::thread_end();
        }

        void ProcessUnitController::processJob(db::Job job) {
          LOGTRACEMETHOD("void ProcessUnitController::processJob(db::Job job)")
          job.begintime = 0;
          {
            boost::mutex::scoped_lock scoped_lock(db_con_mutex);
            job.update();
          }

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
			  stream_data[idx].min_packet_count=0;
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

                if (unit->_decoder->getCodecType() == CODEC_TYPE_AUDIO) {
                  LOGDEBUG("audioQueue.enqueue(unit);")
                  audioQueue.enqueue(unit);
                } else {
                  LOGDEBUG("puQueue.enqueue(unit);")
                  puQueue.enqueue(unit);
                }
              }
            }
          } else {
            LOGERROR("Error Opening Input Streams from " << filename);
          }

          LOGDEBUG("File complete : " << job << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
          job.endtime = 0;
          {
            boost::mutex::scoped_lock scoped_lock(db_con_mutex);
            job.update();
          }
        }

        boost::shared_ptr<ProcessUnit> ProcessUnitController::getProcessUnit() {
          if (audioQueue.size() == 0 && puQueue.size() == 0)
            queue_empty_wait_condition.notify_all();
          boost::mutex::scoped_lock scoped_lock(get_pu_mutex);
          LOGDEBUG("video queue size:" << puQueue.size());
          if (puQueue.size() == 0 || _stop_signal)
            return boost::shared_ptr<ProcessUnit > (new ProcessUnit());
          boost::shared_ptr<ProcessUnit> u = puQueue.dequeue();

          db::ProcessUnit dbunit(_dbCon);
          dbunit.sorcestream = u->_source_stream;
          dbunit.targetstream = u->_target_stream;
          dbunit.startts = (double) u->_input_packets.front()->getDts();
          dbunit.endts = (double) u->_input_packets.back()->getDts();
          dbunit.framecount = (int) u->_input_packets.size();
          {
            boost::mutex::scoped_lock scoped_lock(db_con_mutex);
            DatabaseService::thread_init();
	    dbunit.update();
            dbunit.recv = -1;
            dbunit.update();
            DatabaseService::thread_end();
          }
          u->_process_unit = dbunit.id;
          return u;
        }

        boost::shared_ptr<ProcessUnit> ProcessUnitController::getAudioProcessUnit() {
          if (audioQueue.size() == 0 && puQueue.size() == 0)
            queue_empty_wait_condition.notify_all();
          boost::mutex::scoped_lock scoped_lock(get_pu_mutex);
          LOGDEBUG("audio queue size:" << audioQueue.size());
          if (audioQueue.size() == 0 || _stop_signal)
            return boost::shared_ptr<ProcessUnit > (new ProcessUnit());
          boost::shared_ptr<ProcessUnit> u = audioQueue.dequeue();

          db::ProcessUnit dbunit(_dbCon);
          dbunit.sorcestream = u->_source_stream;
          dbunit.targetstream = u->_target_stream;
          dbunit.startts = (double) u->_input_packets.front()->getDts();
          dbunit.endts = (double) u->_input_packets.back()->getDts();
          dbunit.framecount = (int) u->_input_packets.size();
          {
            boost::mutex::scoped_lock scoped_lock(db_con_mutex);
			DatabaseService::thread_init();
            dbunit.update();
            dbunit.recv = -1;
            dbunit.update();
		    DatabaseService::thread_end();
          }
          u->_process_unit = dbunit.id;
          return u;
        }

        bool ProcessUnitController::putProcessUnit(boost::shared_ptr<ProcessUnit> & unit) {
          boost::mutex::scoped_lock scoped_lock(put_pu_mutex);

          std::string name = org::esb::config::Config::getProperty("hive.base_path");
          name += "/tmp/";
          name += org::esb::util::Decimal(unit->_process_unit % 10).toString();

          org::esb::io::File dir(name.c_str());
          if (!dir.exists()) {
            dir.mkdir();
          }
          name += "/";
          name += org::esb::util::Decimal(unit->_process_unit).toString();
          name += ".unit";
          org::esb::io::File out(name.c_str());
          org::esb::io::FileOutputStream fos(&out);
          org::esb::io::ObjectOutputStream ous(&fos);
          LOGDEBUG("Saving ProcessUnit:"<<unit->_process_unit);
          ous.writeObject(*unit.get());
          ous.close();
          try {
            boost::mutex::scoped_lock scoped_lock(db_con_mutex);
	    DatabaseService::thread_init();
            db::ProcessUnit dbunit = litesql::select<db::ProcessUnit > (_dbCon, db::ProcessUnit::Id == unit->_process_unit).one();
            dbunit.recv = 0;
            dbunit.update();
	    DatabaseService::thread_end();
          } catch (litesql::NotFound ex) {
            LOGERROR("db::ProcessUnit not found for :" << unit->_process_unit);
          }
          /*
          delete unit._decoder;
          unit._decoder = NULL;
          delete unit._encoder;
          unit._encoder = NULL;
           */
          /*
          _stmt_fr->setInt("id", unit->_process_unit);
          _stmt_fr->execute();

          if (unit->_last_process_unit) {
            sql::Connection con3(std::string(config::Config::getProperty("db.connection")));
            sql::PreparedStatement pstmt2 = con3.prepareStatement("update jobs set status='completed' where id = (select job_id from job_details where instream=:instream and outstream=:outstream)");
            pstmt2.setInt("instream", unit->_source_stream);
            pstmt2.setInt("outstream", unit->_target_stream);
            pstmt2.execute();
          }
           */
          return true;
        }
      }
    }
  }
}