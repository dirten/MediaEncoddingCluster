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

#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"

#include "boost/thread.hpp"
#include "boost/shared_ptr.hpp"
#include "JobController.h"
#include "StreamData.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/StringUtil.h"
#include "PartitionManager.h"

using namespace db;
using namespace org::esb::av;

namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        void ProcessUnitController::onMessage(org::esb::signal::Message & msg) {
          //LOGDEBUG("Message received:" << msg.getProperty("processunitcontroller"));
          if (msg.getProperty("processunitcontroller") == "start") {
            LOGDEBUG("start request");
            _stop_signal = false;
            _isRunning = false;
            boost::thread t(boost::bind(&ProcessUnitController::start, this));
            LOGDEBUG("started");
          } else if (msg.getProperty("processunitcontroller") == "stop") {
            LOGDEBUG("stop request");
            stop();
            LOGDEBUG("stopped");
          } else if (msg.getProperty("processunitcontroller") == "GET_PROCESS_UNIT") {
            //LOGDEBUG("GET_PROCESS_UNIT request");
            boost::shared_ptr<ProcessUnit> unit = getProcessUnit();
            msg.setProperty("processunit", unit);
          } else if (msg.getProperty("processunitcontroller") == "GET_AUDIO_PROCESS_UNIT") {
            //LOGDEBUG("GET_AUDIO_PROCESS_UNIT request");
            boost::shared_ptr<ProcessUnit> unit = getAudioProcessUnit();
            msg.setProperty("processunit", unit);
          } else if (msg.getProperty("processunitcontroller") == "PUT_PROCESS_UNIT") {
            //LOGDEBUG("PUT_PROCESS_UNIT request");

            boost::shared_ptr<ProcessUnit> unit = msg.getPtrProperty("processunit");

            putProcessUnit(unit);
          } else if (msg.getProperty("processunitcontroller") == "STOP_JOB") {
            LOGDEBUG("STOP_JOB request");
            _stop_job_id = msg.getProperty("job_id");
          }
        }

        ProcessUnitController::ProcessUnitController() :
        _dbCon(org::esb::hive::DatabaseService::getDatabase()),
        _dbJobCon(org::esb::hive::DatabaseService::getDatabase())
#ifdef USE_SAFMQ
        , _queue("safmq://admin:@localhost:20200/punitout")
        , _oos(&_queue)
#endif
        {
          LOGTRACEMETHOD("ProcessUnitController::ProcessUnitController()");
          _stop_signal = false;
          _isRunning = false;
          _isWaitingForFinish = false;

        }

        ProcessUnitController::ProcessUnitController(const ProcessUnitController& orig) :
        _dbCon(orig._dbCon),
        _dbJobCon(orig._dbJobCon)
#ifdef USE_SAFMQ
        , _queue(orig._queue)
        , _oos(orig._oos)
#endif
        {

        }

        ProcessUnitController::~ProcessUnitController() {
          LOGTRACEMETHOD("ProcessUnitController::~ProcessUnitController()")
          stop();
          Thread::sleep2(1000);
        }

        void ProcessUnitController::stop() {
          _stop_signal = true;
          audioQueue.flush();
          puQueue.flush();
          queue_empty_wait_condition.notify_all();
          _isRunning = false;
        }

        void ProcessUnitController::start() {
          LOGTRACEMETHOD("void ProcessUnitController::start() ")
                  //          JobController job_ctrl;
                  //db::HiveDb _dbCon("mysql", org::esb::config::Config::getProperty("db.url"));
                  //              org::esb::hive::DatabaseService::thread_init();
          while (!_stop_signal) {
            //_dbJobCon.begin();
            litesql::DataSource<db::Job> source = litesql::select<db::Job > (_dbJobCon, db::Job::Endtime <= 1 && (db::Job::Status == "queued" || db::Job::Status == "running"));
            if (source.count() > 0) {
              db::Job job = source.one();
              //db::Job job = job_ctrl.getJob();
              LOGDEBUG("new job found : " << job.id);
              current_job = Ptr<db::Job > (new db::Job(job));

              try {
                processJob(*current_job.get());
                //_dbJobCon.commit();

              } catch (litesql::NotFound ex) {
                LOGDEBUG("error while processing job : " << ex.what());
                job.status = "error";
                job.update();
                org::esb::lang::Thread::sleep2(1000);
              }
            }
            org::esb::lang::Thread::sleep2(1000);
          }
          //	  org::esb::hive::DatabaseService::thread_end();
        }

        bool ProcessUnitController::stopJob() {
          bool result = false;
          if (current_job && atoi(_stop_job_id.c_str()) == current_job->id.value()) {
            LOGDEBUG("stop job received");
            puQueue.flush();
            audioQueue.flush();
            result = true;
            current_job->status = "stopped";
            current_job->update();
            queue_empty_wait_condition.notify_all();
            _stop_job_id = "";
          }
          return result;
        }

        void ProcessUnitController::processJob(db::Job job) {
          bool wait_for_queue = false;
          _isWaitingForFinish = false;
          LOGTRACEMETHOD("void ProcessUnitController::processJob(db::Job job)")

          MediaFile infile = job.inputfile().get().one();
          string filename = infile.path + "/" + infile.filename;
          //org::esb::io::File fi(filename);
          //if (fi.exists()) {
          LOGDEBUG("try");
          org::esb::av::FormatInputStream fis(filename);
          if (fis.isValid()) {
            LOGDEBUG("valid file");
            job.begintime = 0;
            job.status = "running";
            {
              boost::mutex::scoped_lock scoped_lock(db_con_mutex);
              job.update();
            }
            {
              db::JobLog log(job.getDatabase());
              std::string message = "Encoding started for ";
              message += filename;
              log.message = message;
              log.update();

              job.joblog().link(log);
            }
            if (job.outputfile().get().one().filter().get().count() > 0)
              vector<db::Filter> filters = job.outputfile().get().one().filter().get().all();

            map<int, StreamData> stream_map;
            std::map<int, Packetizer::StreamData> stream_data;
            vector<JobDetail>details = job.jobdetails().get().all();
            vector<JobDetail>::iterator s_it = details.begin();
            for (; s_it != details.end(); s_it++) {
              JobDetail detail = *s_it;
              int idx = detail.inputstream().get().one().streamindex;
              stream_map[idx].instream = detail.inputstream().get().one().id;
              stream_map[idx].outstream = detail.outputstream().get().one().id;
              /*special need to create a fresh decoder for mpeg2*/
              if (fis.getAVStream(idx)->codec->codec_id != CODEC_ID_MPEG2VIDEO) {
                stream_map[idx].decoder = boost::shared_ptr<Decoder > (new Decoder(fis.getAVStream(idx)));
                stream_map[idx].pass2decoder = boost::shared_ptr<Decoder > (new Decoder(fis.getAVStream(idx)));
              } else {
                stream_map[idx].decoder = CodecFactory::getStreamDecoder(stream_map[idx].instream);
                stream_map[idx].pass2decoder = CodecFactory::getStreamDecoder(stream_map[idx].instream);
              }
              //stream_map[idx].decoder = CodecFactory::getStreamDecoder(stream_map[idx].instream);
              stream_map[idx].encoder = CodecFactory::getStreamEncoder(stream_map[idx].outstream);
              stream_map[idx].pass2encoder = CodecFactory::getStreamEncoder(stream_map[idx].outstream);

              stream_map[idx].deinterlace = detail.deinterlace.value();
              //              stream_map[idx].last_start_pts = detail.inputstream().get().one().firstpts;
              stream_map[idx].last_start_dts = detail.lastdts;
              LOGDEBUG("Last start DTS:" << stream_map[idx].last_start_dts);
              if (stream_map[idx].decoder->getTimeBase().num <= 0 || stream_map[idx].decoder->getTimeBase().den <= 0) {
                LOGERROR("wrong decoder timebase -> num=" << stream_map[idx].decoder->getTimeBase().num << " den=" << stream_map[idx].decoder->getTimeBase().den);
                LOGERROR("skip stream #" << stream_map[idx].instream);
                stream_map.erase(idx);
                continue;
              }
              if (!stream_map[idx].encoder->open()) {
                db::JobLog log(job.getDatabase());
                std::string message = "Could not open Encoder for Stream#";
                message += org::esb::util::StringUtil::toString(idx);
                message += " for file ";
                message += filename;

                log.message = message;
                log.update();

                job.joblog().link(log);
                stream_map.erase(idx);
                continue;
              }
              stream_data[idx].decoder = stream_map[idx].decoder;
              stream_data[idx].encoder = stream_map[idx].encoder;
              if (stream_map[idx].encoder->getCodecType() == AVMEDIA_TYPE_VIDEO)
                stream_data[idx].min_packet_count = stream_map[idx].encoder->getGopSize();
              else
                stream_data[idx].min_packet_count = 0;
            }
            PacketInputStream pis(&fis);

            Packetizer packetizer(stream_data);
            ProcessUnitBuilder builder(stream_map);
            Packet * packet;
            /**
             * read while packets in the stream
             * @TODO: performance bottleneck in read packet and the resulting copy of the Packet
             */
            while (!_stop_signal && (packet = pis.readPacket()) != NULL) {
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
                if (stopJob()) {
                  return;
                }
                PacketListPtr packets = packetizer.removePacketList();
                boost::shared_ptr<ProcessUnit>unit = builder.build(packets);
                putToQueue(unit);
                wait_for_queue = true;
              }
            }
            /*calling flush Method in the Packetizer to get the last pending packets from the streams*/
            packetizer.flushStreams();
            int pc = packetizer.getPacketListCount();
            for (int a = 0; a < pc; a++) {
              PacketListPtr packets = packetizer.removePacketList();
              if (packets.size() > 0) {
                if (stopJob()) {
                  return;
                }

                boost::shared_ptr<ProcessUnit>unit = builder.build(packets);
                putToQueue(unit);
                wait_for_queue = true;
              }
            }

            if (wait_for_queue) {
              _isWaitingForFinish = true;
              boost::mutex::scoped_lock queue_empty_wait_lock(queue_empty_wait_mutex);
              queue_empty_wait_condition.wait(queue_empty_wait_lock);
            }

            if (_stop_signal) {
              LOGDEBUG("stop signal, returning here");
              return;
            }
            if (stopJob()) {
              return;
            }
            LOGDEBUG("File complete : " << job << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            job.endtime = 0;
            job.status = "completed";
            {
              db::JobLog log(job.getDatabase());
              std::string message = "Encoding completed for the file ";
              message += filename;
              log.message = message;
              log.update();

              job.joblog().link(log);
            }
            {
              boost::mutex::scoped_lock scoped_lock(db_con_mutex);
              job.update();
            }

          } else {
            LOGERROR("Error Opening Input Streams from " << filename);
            job.endtime = 0;
            job.status = "failed";
            db::JobLog log(job.getDatabase());
            std::string message = "failed to open the file ";
            message += filename;
            log.message = message;
            log.update();

            job.joblog().link(log);

            {
              boost::mutex::scoped_lock scoped_lock(db_con_mutex);
              job.update();
            }

          }
          /*} else {

            LOGERROR("Error Opening Input File : " << filename);
            job.endtime = 0;
            job.status = "failed";
            db::JobLog log(job.getDatabase());
            std::string message = "failed to find the input file ";
            message += filename;
            log.message = message;
            log.update();

            job.joblog().link(log);

            {
              boost::mutex::scoped_lock scoped_lock(db_con_mutex);
              job.update();
            }

          }*/
        }

        void ProcessUnitController::putToQueue(boost::shared_ptr<ProcessUnit>unit) {
#ifdef USE_SAFMQ
          db::ProcessUnit dbunit(_dbCon);
          dbunit.sorcestream = unit->_source_stream;
          dbunit.targetstream = unit->_target_stream;
          dbunit.timebasenum = unit->_input_packets.front()->getTimeBase().num;
          dbunit.timebaseden = unit->_input_packets.front()->getTimeBase().den;
          dbunit.startts = (double) unit->_input_packets.front()->getDts();
          dbunit.endts = (double) unit->_input_packets.back()->getDts();
          dbunit.framecount = (int) unit->_input_packets.size();
          {
            boost::mutex::scoped_lock scoped_lock(db_con_mutex);
            dbunit.update();
            dbunit.recv = -1;
            dbunit.update();
          }
          unit->_process_unit = dbunit.id;

          _oos.writeObject(*unit.get());
#else
          //PartitionManager::getInstance()->putProcessUnit("global", unit);
          //return;
          if (unit->_decoder->getCodecType() == AVMEDIA_TYPE_AUDIO) {
            LOGDEBUG("audioQueue.enqueue(unit);")
            audioQueue.enqueue(unit);
          } else {
            LOGDEBUG("puQueue.enqueue(unit);")
            puQueue.enqueue(unit);
          }
#endif

        }

        boost::shared_ptr<ProcessUnit> ProcessUnitController::getProcessUnit() {
          boost::mutex::scoped_lock scoped_lock(get_pu_mutex);
          boost::shared_ptr<ProcessUnit> u;
          LOGDEBUG("video queue size:" << puQueue.size());
          if (stopJob()) {
            return boost::shared_ptr<ProcessUnit > (new ProcessUnit());
          }
          if (puQueue.size() == 0 || _stop_signal)
            return boost::shared_ptr<ProcessUnit > (new ProcessUnit());

#ifdef USE_SAFMQ

#else
          u = puQueue.dequeue();
#endif
          db::ProcessUnit dbunit(_dbCon);
          dbunit.sorcestream = u->_source_stream;
          dbunit.targetstream = u->_target_stream;
          dbunit.timebasenum = u->_input_packets.front()->getTimeBase().num;
          dbunit.timebaseden = u->_input_packets.front()->getTimeBase().den;
          dbunit.startts = (double) u->_input_packets.front()->getDts();
          dbunit.endts = (double) u->_input_packets.back()->getDts();
          dbunit.framecount = (int) u->_input_packets.size();
          {
            boost::mutex::scoped_lock scoped_lock(db_con_mutex);
            //            DatabaseService::thread_init();
            dbunit.update();
            dbunit.recv = -1;
            dbunit.update();
            //            DatabaseService::thread_end();
          }
          u->_process_unit = dbunit.id;
          /*this is only for debugging*/
          if (true) {
            std::string name = org::esb::config::Config::getProperty("hive.base_path");
            name += "/tmp/";
            name += org::esb::util::StringUtil::toString(u->_process_unit % 10);

            org::esb::io::File dir(name.c_str());

            if (!dir.exists()) {
              dir.mkdir();
            }
            name += "/";
            name += org::esb::util::StringUtil::toString(u->_process_unit);
            name += ".unit.out";
            org::esb::io::File out(name.c_str());
            org::esb::io::FileOutputStream fos(&out);
            org::esb::io::ObjectOutputStream ous(&fos);
            LOGDEBUG("Saving ProcessUnit:" << u->_process_unit);
            ous.writeObject(*u.get());
            ous.close();


          }

          return u;
        }

        boost::shared_ptr<ProcessUnit> ProcessUnitController::getAudioProcessUnit() {
          boost::mutex::scoped_lock scoped_lock(get_pu_mutex);
          boost::shared_ptr<ProcessUnit> u;
          LOGDEBUG("audio queue size:" << audioQueue.size());
          if (stopJob()) {
            return boost::shared_ptr<ProcessUnit > (new ProcessUnit());
          }

          if (audioQueue.size() == 0 || _stop_signal)
            return boost::shared_ptr<ProcessUnit > (new ProcessUnit());
#ifdef USE_SAFMQ

#else
          u = audioQueue.dequeue();
          if ((audioQueue.size() == 0 && _isWaitingForFinish) || _stop_signal)
            u->_last_process_unit = true;
#endif
          db::ProcessUnit dbunit(_dbCon);
          dbunit.sorcestream = u->_source_stream;
          dbunit.targetstream = u->_target_stream;
          dbunit.timebasenum = u->_input_packets.front()->getTimeBase().num;
          dbunit.timebaseden = u->_input_packets.front()->getTimeBase().den;
          dbunit.startts = (double) u->_input_packets.front()->getDts();
          dbunit.endts = (double) u->_input_packets.back()->getDts();
          dbunit.framecount = (int) u->_input_packets.size();
          {
            boost::mutex::scoped_lock scoped_lock(db_con_mutex);
            //			DatabaseService::thread_init();
            dbunit.update();
            dbunit.recv = -1;
            dbunit.update();
            //		    DatabaseService::thread_end();
          }
          u->_process_unit = dbunit.id;
          return u;
        }

        bool ProcessUnitController::putProcessUnit(boost::shared_ptr<ProcessUnit> & unit) {
          boost::mutex::scoped_lock scoped_lock(put_pu_mutex);
          int stream_type = AVMEDIA_TYPE_UNKNOWN;
          std::string name = org::esb::config::Config::getProperty("hive.base_path");
          name += "/tmp/";
          name += org::esb::util::StringUtil::toString(unit->_process_unit % 10);

          org::esb::io::File dir(name.c_str());

          if (!dir.exists()) {
            dir.mkdir();
          }
          name += "/";
          name += org::esb::util::StringUtil::toString(unit->_process_unit);
          name += ".unit";
          org::esb::io::File out(name.c_str());
          org::esb::io::FileOutputStream fos(&out);
          org::esb::io::ObjectOutputStream ous(&fos);
          //LOGDEBUG("Saving ProcessUnit:" << unit->_process_unit);
          ous.writeObject(*unit.get());
          ous.close();
          try {
            boost::mutex::scoped_lock scoped_lock(db_con_mutex);
            //	    DatabaseService::thread_init();
            db::ProcessUnit dbunit = litesql::select<db::ProcessUnit > (_dbCon, db::ProcessUnit::Id == unit->_process_unit).one();
            dbunit.recv = 0;
            dbunit.update();
            //            LOGDEBUG(""<<dbunit);
            vector<db::JobDetail> details = current_job->jobdetails().get().all();
            for (uint8_t a = 0; a < details.size(); a++) {
              if (details[a].inputstream().get().one().id == (int) dbunit.sorcestream) {
                details[a].lastdts = (double) dbunit.endts;
                details[a].update();
                stream_type = details[a].outputstream().get().one().streamtype.value();
                LOGDEBUG("" << details[a]);
                LOGDEBUG("StreamType=" << stream_type);
              }
            }

            /** calculate current progress from the Job*/
            if (unit->getOutputPacketList().size() > 0) {
              if (audioQueue.size() == 0 && puQueue.size() == 0) {
                current_job->progress = 100;
                current_job->fps=0;
              } else {
                if (stream_type == AVMEDIA_TYPE_VIDEO) {
                  LOGDEBUG("Calculating Progress!");
                  AVRational ar_target;
                  ar_target.num = 1;
                  ar_target.den = 1000000;
                  AVRational ar_source;
                  ar_source.num = dbunit.timebasenum;
                  ar_source.den = dbunit.timebaseden;
                  int64_t lastdb = dbunit.endts.value();
                  //int64_t last = unit->getOutputPacketList().back()->getDts();

                  int64_t ts = av_rescale_q(lastdb, ar_source, ar_target);
                  int64_t starttime = current_job->starttime.value();
                  int64_t duration = current_job->duration.value();
                  int progress = (ts - starttime)*100 / duration;
                  current_job->progress = progress;
                  /*calculating frames per second*/
                  std::string csql="select sum(framecount_)/(max(recv_)-min(send_)) from ProcessUnit_ where send_>(SELECT strftime('%s', 'now'))-600 and targetstream_ = ";
                  csql+=org::esb::util::StringUtil::toString(dbunit.targetstream);
                  csql+=" group by targetstream_";
                  LOGDEBUG(csql);
                  litesql::Records recs = current_job->getDatabase().query(csql);
                  if (recs.begin() != recs.end()){
                    LOGDEBUG("setting fps to"<<(*recs.begin())[0]);
                    current_job->fps=(*recs.begin())[0];
                  }
                }
              }
              current_job->update();
            }

            //	    DatabaseService::thread_end();
          } catch (litesql::NotFound ex) {
            LOGERROR("db::ProcessUnit not found for :" << unit->_process_unit);
          }
          if (audioQueue.size() == 0 && puQueue.size() == 0)
            queue_empty_wait_condition.notify_all();

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