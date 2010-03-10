/*----------------------------------------------------------------------
 *  File    : ProcessUnitWatcher.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose : here are the Packets will be bundled and received for and from the clients
 *  Created : 6. November 2009, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 * ----------------------------------------------------------------------
 */

#include "org/esb/hive/DatabaseService.h"


#include <string>

//#include <boost/thread.hpp>
//#include <boost/bind.hpp>

#include "ProcessUnitWatcher.h"


#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"

#include "org/esb/hive/CodecFactory.h"
//#include "org/esb/av/FormatStreamFactory.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"

#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
//#include "ClientHandler.h"
#include <algorithm>
#include <deque>
#include <list>
using namespace org::esb::sql;
using namespace org::esb::av;
namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        template<typename T>
        void dummy_deleter(T*d) {
          //do nothing
        }
        //        std::map<int, boost::shared_ptr<ProcessUnit> > ProcessUnitWatcher::unit_map;
        boost::mutex ProcessUnitWatcher::put_pu_mutex;
        boost::mutex ProcessUnitWatcher::get_pu_mutex;
        boost::mutex ProcessUnitWatcher::get_stream_pu_mutex;
        boost::mutex ProcessUnitWatcher::stmt_mutex;
        boost::mutex ProcessUnitWatcher::terminationMutex;
        boost::condition ProcessUnitWatcher::termination_wait;

        boost::mutex ProcessUnitWatcher::queue_empty_wait_mutex;
        boost::condition ProcessUnitWatcher::queue_empty_wait_condition;

        ProcessUnitWatcher::ProcessUnitQueue ProcessUnitWatcher::puQueue;
        org::esb::util::Queue<boost::shared_ptr<ProcessUnit>, 500 > ProcessUnitWatcher::audioQueue;
        //        std::deque<boost::shared_ptr<ProcessUnit> > ProcessUnitWatcher::audioQueue;
        std::map<std::string, int> ProcessUnitWatcher::ip2stream;
        org::esb::sql::PreparedStatement * ProcessUnitWatcher::_stmt_fr = NULL;
        org::esb::sql::PreparedStatement * ProcessUnitWatcher::_stmt = NULL;
        org::esb::sql::PreparedStatement * ProcessUnitWatcher::_stmt_jd = NULL;
        bool ProcessUnitWatcher::_isStopSignal = false;
        bool ProcessUnitWatcher::_isRunning = false;
        AVRational ProcessUnitWatcher::basear;
        map<int, ProcessUnitWatcher::StreamData> ProcessUnitWatcher::_stream_map;

        ProcessUnitWatcher::ProcessUnitWatcher() {
          //          std::string c = org::esb::config::Config::getProperty("db.connection");
          basear.num = 1;
          basear.den = 1000000;

          //          _con_tmp2 = new Connection(c);


          //          _stmt = new PreparedStatement(_con_tmp->prepareStatement("insert into process_units (source_stream, target_stream, start_ts, end_ts, frame_count, send, complete) values (:source_stream, :target_stream, :start_ts, :end_ts, :frame_count, now(), null)"));
          //          _stmt_fr = new PreparedStatement(_con_tmp2->prepareStatement("update process_units set complete = now() where id=:id"));

        }

        void ProcessUnitWatcher::onMessage(org::esb::signal::Message & msg) {
          if (msg.getProperty("processunitwatcher") == "start") {
            _isStopSignal = false;
            boost::thread t(boost::bind(&ProcessUnitWatcher::start3, this));
            LOGDEBUG("started");
          } else if (msg.getProperty("processunitwatcher") == "stop") {
            LOGDEBUG("ProcessUnitWatcher stop request");
            stop();
            LOGDEBUG("stopped");
          }
        }

        void ProcessUnitWatcher::stop() {
          if (!_isStopSignal) {
            _isStopSignal = true;

            puQueue.flush();
            if (_isRunning) {
              boost::mutex::scoped_lock terminationLock(terminationMutex);
              termination_wait.timed_wait(terminationLock, boost::posix_time::seconds(10));
              _isRunning = false;
              delete _stmt_fr;
              delete _con_tmp2;
              delete _stmt;
              delete _stmt_jd;
              delete _con_tmp;
            }
          }
        }

        void ProcessUnitWatcher::start3() {
          DatabaseService::thread_init();
          std::string c = org::esb::config::Config::getProperty("db.connection");
          _con_tmp2 = new Connection(c);
          _stmt_fr = new PreparedStatement(_con_tmp2->prepareStatement("update process_units set complete = now() where id=:id"));
          _con_tmp = new Connection(c);
          _stmt = new PreparedStatement(_con_tmp->prepareStatement("insert into process_units (source_stream, target_stream, start_ts, end_ts, frame_count, send, complete) values (:source_stream, :target_stream, :start_ts, :end_ts, :frame_count, now(), null)"));
          _stmt_jd = new PreparedStatement(_con_tmp->prepareStatement("update job_details set last_pts=:last_pts, last_dts=:last_dts where instream=:instream and outstream=:outstream"));
          _isRunning = true;
          while (!_isStopSignal) {
            /**
             * getting all jobs that are not being completed
             */
            sql::Connection con(std::string(config::Config::getProperty("db.connection")));
            sql::Statement stmt = con.createStatement("select * from jobs, files where jobs.inputfile=files.id and complete is null");
            sql::ResultSet rs = stmt.executeQuery();
            while (rs.next() && !_isStopSignal) {
              std::map<int, Packetizer::StreamData> stream_data;

              _stream_map.clear();
              string filename = rs.getString("files.path") + "/" + rs.getString("files.filename");
              org::esb::io::File fi(filename);
              org::esb::av::FormatInputStream * fis = new org::esb::av::FormatInputStream(&fi);
              if (!fis->isValid()) {
                LOGERROR("Error Opening Input Streams from " << filename);
                delete fis;
                continue;
              }
              /**
               * building Stream information Map
               */
              //              AVRational basear;
              int64_t tsmin = INT64_MAX;
              int64_t tsmax = INT64_MIN;
              {
                sql::Connection con2(std::string(config::Config::getProperty("db.connection")));
                sql::PreparedStatement pstmt = con2.prepareStatement("select * from job_details, streams where job_id=:myid and streams.id=instream");
                pstmt.setInt("myid", rs.getInt("jobs.id"));
                job_id = rs.getInt("jobs.id");
                sql::ResultSet rs2 = pstmt.executeQuery();
                while (rs2.next() && !_isStopSignal) {
                  {
                    sql::Connection con3(std::string(config::Config::getProperty("db.connection")));
                    sql::PreparedStatement pstmt2 = con3.prepareStatement("update jobs set begin=now() where id=:jobid");
                    pstmt2.setInt("jobid", rs.getInt("jobs.id"));
                    pstmt2.execute();
                  }
                  int index = rs2.getInt("stream_index");
                  _stream_map[index].deinterlace=rs.getInt("deinterlace");
                  _stream_map[index].keep_aspect_ratio=rs.getInt("keep_display_aspect");
                  _stream_map[index].instream = rs2.getInt("instream");
                  _stream_map[index].outstream = rs2.getInt("outstream");
                  _stream_map[index].type = rs2.getInt("stream_type");
                  //                  _stream_map[index].decoder=boost::shared_ptr<Decoder>(new Decoder(fis->getAVStream(index)->codec),&dummy_deleter<Decoder>);
                  _stream_map[index].decoder = CodecFactory::getStreamDecoder(_stream_map[index].instream);
                  _stream_map[index].encoder = CodecFactory::getStreamEncoder(_stream_map[index].outstream);
                  AVRational a;
                  a.num = rs2.getInt("time_base_num");
                  a.den = rs2.getInt("time_base_den");
                  _stream_map[index].stream_time_base = a;
                  /**
                   * this case is only for a TestCase, in normal usage this will never happen
                   */
                  if (_stream_map[index].decoder.get() == NULL || _stream_map[index].encoder.get() == NULL)
                    continue;
                  _stream_map[index].decoder->open();
                  _stream_map[index].encoder->open();
                  /*
                  _stream_map[index].last_start_dts = max(0LL,rs2.getLong("first_dts") - 1);
                  _stream_map[index].last_start_pts = max(0LL,rs2.getLong("start_time") - 1);
                   */
                  _stream_map[index].last_start_dts = rs2.getLong("first_dts") - 1;
                  _stream_map[index].last_start_pts = rs2.getLong("start_time") - 1;
                  LOGDEBUG("last_start_dts=" << _stream_map[index].last_start_dts)
                  LOGDEBUG("last_start_pts=" << _stream_map[index].last_start_pts)
                  if(_stream_map[index].decoder->getTimeBase().num<=0||_stream_map[index].decoder->getTimeBase().den<=0){
                    LOGERROR("wrong decoder timebase -> num="<<_stream_map[index].decoder->getTimeBase().num<<" den="<<_stream_map[index].decoder->getTimeBase().den);
                    LOGERROR("skip stream #"<<_stream_map[index].instream);
                    _stream_map.erase(index);
                    continue;
                  }
                  tsmin = min(tsmin, av_rescale_q(_stream_map[index].last_start_pts, _stream_map[index].decoder->getTimeBase(), basear));
                  //                  if(_stream_map[index].last_start_dts>0)
                  tsmax = max(tsmax, av_rescale_q(_stream_map[index].last_start_pts, _stream_map[index].stream_time_base, basear));
                  LOGDEBUG("tsmax=" << tsmax)
                  _stream_map[index].packet_count = 0;
                  _stream_map[index].last_bytes_offset = 0;
                  _stream_map[index].process_unit_count = 0;
                  _stream_map[index].frameRateCompensateBase = 0.0;
                  _stream_map[index].b_frame_offset = 0;
                  //                  _stream_map[index].last_process_unit_id = 0;
                  /**
                   * collecting data for the Packetizer
                   */
                  stream_data[index].codec_type = _stream_map[index].decoder->getCodecType();
                  stream_data[index].codec_id = _stream_map[index].decoder->getCodecId();
                  stream_data[index].decoder = _stream_map[index].decoder;
                  stream_data[index].encoder = _stream_map[index].encoder;

                  if (_stream_map[index].type == CODEC_TYPE_VIDEO) {
                    _stream_map[index].min_packet_count = 5;
                    if (_stream_map[index].decoder->getCodecId() == CODEC_ID_MPEG2VIDEO) {
                      _stream_map[index].b_frame_offset = 3;
                    } else {
                      //                      _stream_map[index].b_frame_offset = 2;
                    }
                  } else
                    if (_stream_map[index].type == CODEC_TYPE_AUDIO) {
                    _stream_map[index].min_packet_count = 512;
                  }
                  LOGDEBUG("StreamInformationMap sid=" << index);

                  LOGDEBUG("StreamInformationMap sid=" << index);

                }
              }
              /**
               * calculating the right start time of the stream when the start stamps from the audio/video streams differ
               */
              {
                map<int, ProcessUnitWatcher::StreamData>::iterator it = _stream_map.begin();
                for (; it != _stream_map.end(); it++) {
                  (*it).second.last_start_pts = av_rescale_q(tsmax, basear, _stream_map[(*it).first].stream_time_base);
                  LOGDEBUG("start TS for stream id#" << (*it).first << " = " << (*it).second.last_start_pts);
                }
              }

              /**
               * special part for restart an unfinished encoding session
               */
              {
                sql::Connection con2(std::string(config::Config::getProperty("db.connection")));
                sql::PreparedStatement pstmt = con2.prepareStatement("SELECT max( start_ts ) as last_start_ts FROM process_units WHERE target_stream = :a  and complete is not null GROUP BY target_stream");
                std::map<int, StreamData>::iterator st = _stream_map.begin();
                for (; st != _stream_map.end(); st++) {
                  pstmt.setInt("a", (*st).second.outstream);
                  ResultSet rs_t = pstmt.executeQuery();
                  if (rs_t.next()) {
                    LOGDEBUG("Setting last start_ts for stream" << (*st).second.instream << " to" << rs_t.getLong("last_start_ts"));
                    (*st).second.last_start_dts = rs_t.getLong("last_start_ts");
                  }
                }
                /**
                 * delete old packetentries after restart an unfinished encoding session
                 */
                st = _stream_map.begin();
                for (; st != _stream_map.end(); st++) {
                sql::Connection con3(std::string(config::Config::getProperty("db.connection")));
                  std::string sql = "DELETE FROM process_units where target_stream=" + org::esb::util::StringUtil::toString((*st).second.outstream) + " AND start_ts > " + org::esb::util::StringUtil::toString((*st).second.last_start_dts);
                  con3.executeNonQuery(sql);
                }
              }
              PacketInputStream pis(fis);
              q_filled = false;
              Packetizer packetizer(stream_data);
              Packet * packet;

              /**
               * read while packets in the stream
               * @TODO: performance bottleneck in read packet and the resulting copy of the Packet
               */
              while ((packet = pis.readPacket()) != NULL && !_isStopSignal) {
                /**
                 * building a shared Pointer from packet because the next read from PacketInputStream kills the Packet data
                 */
                boost::shared_ptr<Packet> pPacket(packet);
                /**
                 * if the actuall stream not mapped then discard this and continue with next packet
                 */
                if (
                    _stream_map.find(packet->packet->stream_index) == _stream_map.end() ||
                    _stream_map[packet->packet->stream_index].last_start_dts > packet->packet->dts
                    || (packet->packet->pts != AV_NOPTS_VALUE && _stream_map[packet->packet->stream_index].last_start_pts > packet->packet->pts)
                    ) {
                  continue;
                }

                if (packetizer.putPacket(pPacket)) {
                  PacketListPtr packets = packetizer.removePacketList();
                  buildProcessUnit(packets, false);
                }
              }
              delete fis;
              /**
               * the rest does not executed because the file is not finisshed
               */
              if (_isStopSignal)continue;
              /**
               * flushing packetizer contents and put this into the Queue
               */
              packetizer.flushStreams();
              int count = packetizer.getPacketListCount();
              for (int a = 0; a < count; a++) {
                PacketListPtr packets = packetizer.removePacketList();
                //                bool last_packet = !a < count - 1;
                buildProcessUnit(packets, true);
              }

              //flushStreamPackets();
              /**
               * @TODO: at this point, here must be a check if all packets are received in case of client crash!
               */
              LOGDEBUG("file completed:" << filename);
              /**
               * cleaning up allocated resources
               */

              boost::mutex::scoped_lock queue_empty_wait_lock(queue_empty_wait_mutex);
              queue_empty_wait_condition.wait(queue_empty_wait_lock);
              /** not to clean it here, clean it after the file export
              map<int, ProcessUnitWatcher::StreamData>::iterator it = _stream_map.begin();
              list<int> codec_nums;
              for (; it != _stream_map.end(); it++) {
                codec_nums.push_back(it->second.instream);
                codec_nums.push_back(it->second.outstream);
//                CodecFactory::clearCodec(it->second.instream);
//                CodecFactory::clearCodec(it->second.outstream);
              }
              list<int>::iterator it_nums=codec_nums.begin();
              for(;it_nums!=codec_nums.end();it_nums++){
//                CodecFactory::clearCodec(*it_nums);
              }
              */
              sql::Connection con3(std::string(config::Config::getProperty("db.connection")));
              sql::PreparedStatement pstmt2 = con3.prepareStatement("update jobs set complete=now(), status='completed' where id=:jobid");
              pstmt2.setInt("jobid", rs.getInt("jobs.id"));
              pstmt2.execute();
              LOGDEBUG("file completed:" << filename);

            }
            if (!_isStopSignal)
              Thread::sleep2(10000);
          }
          DatabaseService::thread_end();

          boost::mutex::scoped_lock terminationLock(terminationMutex);
          termination_wait.notify_all();

        }

        void ProcessUnitWatcher::buildProcessUnit(PacketListPtr list, bool lastPackets) {
          if (list.size() == 0)return;
          boost::shared_ptr<ProcessUnit> u(new ProcessUnit());
          int sIdx = list.front()->getStreamIndex();

          u->_source_stream = _stream_map[sIdx].instream;
          u->_target_stream = _stream_map[sIdx].outstream;
          if (u->_source_stream == 0 || u->_target_stream == 0) {
            LOGERROR("InputStream=" << u->_source_stream << ":OutputStream=" << u->_target_stream << "JobId:" << job_id);
          }
          u->_decoder = _stream_map[sIdx].decoder;
          u->_encoder = _stream_map[sIdx].encoder;
          u->_deinterlace=_stream_map[sIdx].deinterlace;
          u->_keep_aspect_ratio=_stream_map[sIdx].keep_aspect_ratio;

          u->_input_packets = std::list<boost::shared_ptr<Packet> >(list.begin(), list.end());
          /**
           * Calculating frameRateCompensateBase for the next ProcessUnit
           * this is needed in case of pull up or pull down frame rate conversion
           * e.g. from 1/25 => 1/30 or 1/25 => 1/15
           */

          u->_gop_size = u->_input_packets.size() - _stream_map[sIdx].b_frame_offset;
          /*
          double tmp=u->_gop_size;
          u->_expected_frame_count=floor(tmp+_stream_map[sIdx].frameRateCompensateBase*av_q2d(u->_encoder->getTimeBase())/av_q2d(u->_decoder->getTimeBase()));
          u->_frameRateCompensateBase = _stream_map[sIdx].frameRateCompensateBase;
           */
          if (_stream_map[sIdx].decoder->getCodecType() == CODEC_TYPE_VIDEO) {
            u->_frameRateCompensateBase = _stream_map[sIdx].frameRateCompensateBase;


            /**
             * calculating the framerate difference
             * delta=(input_timestamp/decoder_framerate_num*decoder_framerate_den*encoder_framerate_num/encoder_framerate_den)-output_timestamp/encoder_framerate_den
             *
             * */


            AVRational input_framerate=u->_decoder->getFrameRate();
            AVRational output_framerate=u->_encoder->getFrameRate();

            double in=(((double)u->_gop_size)/input_framerate.num*input_framerate.den*output_framerate.num/output_framerate.den);
            in+=_stream_map[sIdx].frameRateCompensateBase;
            //+0.001 is against some rounding issues
            double out=floor(in+0.0001);
            double delta=in-out;
            u->_expected_frame_count = static_cast<int> (out);
            _stream_map[sIdx].frameRateCompensateBase = delta;


            //            int64_t tmp_dur=((int64_t)AV_TIME_BASE * u->_decoder->getTimeBase().num * u->_decoder->ctx->ticks_per_frame) / u->_decoder->ctx->time_base.den;
/*
            AVRational ar;
            ar.num = u->_decoder->getFrameRate().den;
            ar.den = u->_decoder->getFrameRate().num;
            //            int64_t dur = av_rescale_q(tmp_dur, ar, u->_decoder->getTimeBase());
            //            Packet * tmp_pac=u->_input_packets.front().get();
            int dur = av_rescale_q(1, ar, u->_decoder->getTimeBase());
            double target = (u->_gop_size * dur) * av_q2d(u->_decoder->getTimeBase()) / av_q2d(u->_encoder->getTimeBase()) + _stream_map[sIdx].frameRateCompensateBase;
            double base = floor(target);
            u->_expected_frame_count = static_cast<int> (base);
            double delta = target - base;
            _stream_map[sIdx].frameRateCompensateBase = delta;*/
            /*
            _stream_map[sIdx].packet_count += u->_gop_size * u->_input_packets.front()->getDuration();
            double base = floor(_stream_map[sIdx].packet_count * av_q2d(u->_decoder->getTimeBase()) / av_q2d(u->_encoder->getTimeBase()));
            double delta = _stream_map[sIdx].packet_count * av_q2d(u->_decoder->getTimeBase()) / av_q2d(u->_encoder->getTimeBase()) - base;
            _stream_map[sIdx].frameRateCompensateBase = delta;
             * */
            //          if(delta>=1.0)
            //            _stream_map[sIdx].packet_count-=u->_input_packets.front()->getDuration();
//            LOGDEBUG("gop_size=" << u->_gop_size << ":Duration=" << dur << ":target=" << target << ":Base=" << base << ":Delta=" << delta << " dectb=" << u->_decoder->getTimeBase().num << "/" << u->_decoder->getTimeBase().den << " enctb=" << u->_encoder->getTimeBase().num << "/" << u->_encoder->getTimeBase().den);
            //          _stream_map[sIdx].packet_count=av_rescale_q(base,u->_encoder->getTimeBase(),u->_decoder->getTimeBase());
            //          LOGDEBUG("org.esb.hive.job.ProcessUnitWatcher","PacketCount="<<_stream_map[sIdx].packet_count<<"gop_size="<<u->_gop_size<<":Duration="<<u->_input_packets.front()->getDuration()<<":Base="<<base<<":Delta="<<delta);
          }

          u->_last_process_unit = lastPackets;
          /**
           * need some special calculations for Audio Packets to avoid Video/Audio drift
           */
          u->_encoder->_bytes_discard = 0; //_stream_map[sIdx].last_bytes_offset;
          if (false && _stream_map[sIdx].decoder->getCodecType() == CODEC_TYPE_AUDIO) {
            /**
             * calculating decoded sample size
             */
            int64_t in_frame_size = av_rescale_q(list.front()->getDuration(), list.front()->getTimeBase(), u->_encoder->getTimeBase())*4;
            int64_t out_frame_size = u->_encoder->getFrameBytes();
            std::cout << "Last Bytes Offset:" << _stream_map[sIdx].last_bytes_offset << std::endl;
            std::cout << "in_frame_size:" << in_frame_size << std::endl;
            std::cout << "out_frame_size:" << out_frame_size << std::endl;
            /**
             * calculating number of bytes to discard
             */
            int64_t out_packet_count = ((in_frame_size * list.size()) - _stream_map[sIdx].last_bytes_offset) / out_frame_size;
            std::cout << "_packet_count:" << list.size() << std::endl;
            std::cout << "out_packet_count:" << out_packet_count << std::endl;
            _stream_map[sIdx].last_bytes_offset = in_frame_size - (((in_frame_size * list.size()) - _stream_map[sIdx].last_bytes_offset)-(out_frame_size * out_packet_count));

          }
          /**
           * some special handling for audio Packets, they must be currently all encoded on the same Client
           * to avoid Video/Audio sync drift
           */
          {
            //boost::mutex::scoped_lock scoped_lock(get_pu_mutex);
            if (u->_decoder->getCodecType() == CODEC_TYPE_AUDIO) {
              //              audioQueue.push_back(u);
              audioQueue.enqueue(u);
            } else {
              puQueue.enqueue(u);
            }
          }

          LOGDEBUG("ProcessUnit added with packet count:" << u->_input_packets.size());
        }

        boost::shared_ptr<ProcessUnit> ProcessUnitWatcher::getStreamProcessUnit() {
          if (audioQueue.size() == 0 && puQueue.size() == 0)
            queue_empty_wait_condition.notify_all();
          boost::mutex::scoped_lock scoped_lock(get_stream_pu_mutex); //get_stream_pu_mutex
          LOGDEBUG("audio queue size:" << audioQueue.size());
          if (audioQueue.size() == 0 || _isStopSignal)
            return boost::shared_ptr<ProcessUnit > (new ProcessUnit());
          boost::shared_ptr<ProcessUnit> u = audioQueue.dequeue();

          {
            boost::mutex::scoped_lock scoped_lock(stmt_mutex); //get_stream_pu_mutex
            _stmt_jd->setInt("instream", u->_source_stream);
            _stmt_jd->setInt("outstream", u->_target_stream);
            _stmt_jd->setLong("last_pts", av_rescale_q(u->_input_packets.back()->packet->pts, u->_input_packets.back()->getTimeBase(), basear));
            _stmt_jd->setLong("last_dts", av_rescale_q(u->_input_packets.back()->packet->dts, u->_input_packets.back()->getTimeBase(), basear));

            _stmt->setInt("source_stream", u->_source_stream);
            _stmt->setInt("target_stream", u->_target_stream);
            if (u->_input_packets.size() > 0) {
              _stmt->setLong("start_ts", u->_input_packets.front()->packet->dts);
              _stmt->setLong("end_ts", u->_input_packets.back()->packet->dts);
            } else {
              _stmt->setLong("start_ts", 0);
              _stmt->setLong("end_ts", 0);
            }
            _stmt->setInt("frame_count", u->_input_packets.size());
            _stmt->execute();
            u->_process_unit = _stmt->getLastInsertId();
            _stmt_jd->execute();
          }
          return u;
        }

        boost::shared_ptr<ProcessUnit> ProcessUnitWatcher::getProcessUnit() {
          if (audioQueue.size() == 0 && puQueue.size() == 0)
            queue_empty_wait_condition.notify_all();
          boost::mutex::scoped_lock scoped_lock(get_pu_mutex);
          LOGDEBUG("video queue size:" << puQueue.size());
          if (puQueue.size() == 0 || _isStopSignal)
            return boost::shared_ptr<ProcessUnit > (new ProcessUnit());
          boost::shared_ptr<ProcessUnit> u = puQueue.dequeue();
          {
            boost::mutex::scoped_lock scoped_lock(stmt_mutex);
            _stmt_jd->setInt("instream", u->_source_stream);
            _stmt_jd->setInt("outstream", u->_target_stream);
            _stmt_jd->setLong("last_pts", av_rescale_q(u->_input_packets.back()->packet->pts, u->_input_packets.back()->getTimeBase(), basear));
            _stmt_jd->setLong("last_dts", av_rescale_q(u->_input_packets.back()->packet->dts, u->_input_packets.back()->getTimeBase(), basear));

            _stmt->setInt("source_stream", u->_source_stream);
            _stmt->setInt("target_stream", u->_target_stream);
            if (u->_input_packets.size() > 0) {
              _stmt->setLong("start_ts", u->_input_packets.front()->packet->dts);
              _stmt->setLong("end_ts", u->_input_packets.back()->packet->dts);
            } else {
              _stmt->setLong("start_ts", 0);
              _stmt->setLong("end_ts", 0);
            }
            _stmt->setInt("frame_count", u->_input_packets.size());
            _stmt->execute();
            u->_process_unit = _stmt->getLastInsertId();
            _stmt_jd->execute();
          }
          return u;
        }

        bool ProcessUnitWatcher::putProcessUnit(int pu_id) {
          boost::mutex::scoped_lock scoped_lock(put_pu_mutex);
          _stmt_fr->setInt("id", pu_id);
          _stmt_fr->execute();
          return true;
        }

        bool ProcessUnitWatcher::putProcessUnit(boost::shared_ptr<ProcessUnit> & unit) {
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
          LOGDEBUG("Saving ProcessUnit");
          ous.writeObject(*unit.get());
          ous.close();
          /*
          delete unit._decoder;
          unit._decoder = NULL;
          delete unit._encoder;
          unit._encoder = NULL;
           */
          _stmt_fr->setInt("id", unit->_process_unit);
          _stmt_fr->execute();

          if (unit->_last_process_unit) {
            sql::Connection con3(std::string(config::Config::getProperty("db.connection")));
            sql::PreparedStatement pstmt2 = con3.prepareStatement("update jobs set status='completed' where id = (select job_id from job_details where instream=:instream and outstream=:outstream)");
            pstmt2.setInt("instream", unit->_source_stream);
            pstmt2.setInt("outstream", unit->_target_stream);
            pstmt2.execute();
          }

          return true;
        }

      }
    }
  }
}

