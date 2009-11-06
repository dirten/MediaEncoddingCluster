/*----------------------------------------------------------------------
 *  File    : ProcessUnitWatcher.h
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

#include <string>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "ProcessUnitWatcher.h"
#include "ClientHandler.h"

#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/av/FormatStreamFactory.h"
#include "org/esb/av/FormatInputStream.h"

#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/util/Log.h"
#include <deque>
#include <list>
namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        //        std::map<int, boost::shared_ptr<ProcessUnit> > ProcessUnitWatcher::unit_map;
        boost::mutex ProcessUnitWatcher::put_pu_mutex;
        boost::mutex ProcessUnitWatcher::get_pu_mutex;
        boost::mutex ProcessUnitWatcher::terminationMutex;
        boost::condition ProcessUnitWatcher::termination_wait;

        util::Queue<boost::shared_ptr<ProcessUnit> > ProcessUnitWatcher::puQueue;
        org::esb::sql::PreparedStatement * ProcessUnitWatcher::_stmt_fr = NULL;
        org::esb::sql::PreparedStatement * ProcessUnitWatcher::_stmt = NULL;
        bool ProcessUnitWatcher::_isStopSignal = false;
        bool ProcessUnitWatcher::_isRunning = false;
        map<int, ProcessUnitWatcher::StreamData> ProcessUnitWatcher::_stream_map;

        ProcessUnitWatcher::ProcessUnitWatcher() {
          std::string c = org::esb::config::Config::getProperty("db.connection");
          //          _con_tmp = new Connection(c);
          //          _con_tmp2 = new Connection(c);

          //          _stmt = new PreparedStatement(_con_tmp->prepareStatement("insert into process_units (source_stream, target_stream, start_ts, end_ts, frame_count, send, complete) values (:source_stream, :target_stream, :start_ts, :end_ts, :frame_count, now(), null)"));
          //          _stmt_fr = new PreparedStatement(_con_tmp2->prepareStatement("update process_units set complete = now() where id=:id"));

        }

        void ProcessUnitWatcher::onMessage(org::esb::signal::Message & msg) {
          if (msg.getProperty("processunitwatcher") == "start") {
            _isStopSignal = false;
            boost::thread t(boost::bind(&ProcessUnitWatcher::start3, this));
            logdebug("ProcessUnitWatcher started");
          } else if (msg.getProperty("processunitwatcher") == "stop") {
            logdebug("ProcessUnitWatcher stop request");
            stop();
            logdebug("ProcessUnitWatcher stopped");
          }
        }

        void ProcessUnitWatcher::stop() {
          if (!_isStopSignal) {
            _isStopSignal = true;

            puQueue.flush();
            if(_isRunning){
              boost::mutex::scoped_lock terminationLock(terminationMutex);
              termination_wait.wait(terminationLock);
              _isRunning=false;
              delete _con_tmp2;
              delete _stmt_fr;
            }
          }
        }

        void ProcessUnitWatcher::start3() {
          std::string c = org::esb::config::Config::getProperty("db.connection");
          _con_tmp2 = new Connection(c);
          _stmt_fr = new PreparedStatement(_con_tmp2->prepareStatement("update process_units set complete = now() where id=:id"));
          _isRunning=true;
          while (!_isStopSignal) {
            /**
             * getting all jobs that not be completed
             */
            sql::Connection con(std::string(config::Config::getProperty("db.connection")));
            sql::Statement stmt = con.createStatement("select * from jobs, files where jobs.inputfile=files.id and complete is null");
            sql::ResultSet rs = stmt.executeQuery();
            while (rs.next()&&!_isStopSignal) {
              _stream_map.clear();
              string filename = rs.getString("files.path") + "/" + rs.getString("files.filename");
              org::esb::io::File fi(filename);
              org::esb::av::FormatInputStream * fis = new org::esb::av::FormatInputStream(&fi);
              if (fis == NULL) {
                logerror("Error Opening Input Stream from " << filename);
                continue;
              }
              /**
               * building Stream information Map
               */
              {
                sql::Connection con2(std::string(config::Config::getProperty("db.connection")));
                sql::PreparedStatement pstmt = con2.prepareStatement("select * from job_details, streams where job_id=:myid and streams.id=instream");
                pstmt.setInt("myid", rs.getInt("jobs.id"));
                job_id = rs.getInt("jobs.id");
                sql::ResultSet rs2 = pstmt.executeQuery();
                while (rs2.next()&&!_isStopSignal) {
                  int index = rs2.getInt("stream_index");
                  _stream_map[index].instream = rs2.getInt("instream");
                  _stream_map[index].outstream = rs2.getInt("outstream");
                  _stream_map[index].type = rs2.getInt("stream_type");
                  _stream_map[index].decoder = CodecFactory::getStreamDecoder(_stream_map[index].instream);
                  _stream_map[index].encoder = CodecFactory::getStreamEncoder(_stream_map[index].outstream);
                  _stream_map[index].last_start_ts = 0;
                  _stream_map[index].packet_count = 0;
                  _stream_map[index].last_process_unit_id = 0;
                  if (_stream_map[index].type == CODEC_TYPE_VIDEO) {
                    _stream_map[index].min_packet_count = 5;
                    if (_stream_map[index].decoder->getCodecId() == CODEC_ID_MPEG2VIDEO) {
                      _stream_map[index].b_frame_offset = 4;
                    } else {
                      _stream_map[index].b_frame_offset = 2;
                    }
                  } else
                    if (_stream_map[index].type == CODEC_TYPE_AUDIO) {
                    _stream_map[index].min_packet_count = 512;
                  }
                }
              }
              {
                sql::Connection con2(std::string(config::Config::getProperty("db.connection")));
                sql::PreparedStatement pstmt = con2.prepareStatement("SELECT max(id) as last_id,max( start_ts ) as last_start_ts FROM process_units WHERE target_stream = :a GROUP BY target_stream");
                std::map<int, StreamData>::iterator st = _stream_map.begin();
                for (; st != _stream_map.end(); st++) {
                  pstmt.setInt("a", (*st).second.outstream);
                  ResultSet rs_t = pstmt.executeQuery();
                  if (rs_t.next()) {
                    logdebug("Setting last start_ts for stream" << (*st).second.instream << " to" << rs_t.getLong("last_start_ts"))
                        (*st).second.last_start_ts = rs_t.getLong("last_start_ts");
                    (*st).second.last_process_unit_id = rs_t.getLong("last_id");
                  }
                }
              }
              PacketInputStream pis(fis);
              q_filled = false;

              Packet packet;
              /**
               * read while packets in the stream
               */
              while (pis.readPacket(packet) == 0 && !_isStopSignal) {
                /**
                 * if the actuall stream not mapped then discard this and continue with next packet
                 */
                if (
                    _stream_map.find(packet.packet->stream_index) == _stream_map.end() ||
                    _stream_map[packet.packet->stream_index].last_start_ts > packet.packet->dts
                    ) {
                  continue;
                }
                /**
                 * building a shared Pointer from packet because the next read from PacketInputStream kills the Packet data
                 */
                boost::shared_ptr<Packet> pPacket(new Packet(packet));
                if (_stream_map[packet.packet->stream_index].type == CODEC_TYPE_AUDIO) {
                  processAudioPacket(pPacket);
                } else
                  if (_stream_map[packet.packet->stream_index].type == CODEC_TYPE_VIDEO) {
                  processVideoPacket(pPacket);
                } else {
                }
              }
              delete fis;
              /**
               * the rest does not executed because the file is not finisshed
               */
              if(_isStopSignal)continue;
              flushStreamPackets();
              /**
               * @TODO: at this point, here must be a check if all packets are received in case of client crash!
               */
              logdebug("file completed:" << filename);
              sql::Connection con3(std::string(config::Config::getProperty("db.connection")));
              sql::PreparedStatement pstmt2 = con3.prepareStatement("update jobs set complete=now() where id=:jobid");
              pstmt2.setInt("jobid", rs.getInt("jobs.id"));
              pstmt2.execute();
            }

            Thread::sleep2(5000);
          }
          boost::mutex::scoped_lock terminationLock(terminationMutex);
          termination_wait.notify_all();

        }

        void ProcessUnitWatcher::buildProcessUnit(int sIdx) {
          boost::shared_ptr<ProcessUnit> u(new ProcessUnit());
          logdebug("sIdx:" << sIdx);
          u->_source_stream = _stream_map[sIdx].instream;
          u->_target_stream = _stream_map[sIdx].outstream;
          if (u->_source_stream == 0 || u->_target_stream == 0) {
            logdebug("InputStream=" << u->_source_stream << ":OutputStream=" << u->_target_stream << "JobId:" << job_id);
          }

          u->_decoder = _stream_map[sIdx].decoder; //CodecFactory::getStreamDecoder(u->_source_stream);
          u->_encoder = _stream_map[sIdx].encoder; //CodecFactory::getStreamEncoder(u->_target_stream);
          //setting Input Packets
          u->_input_packets = _stream_map[sIdx].packets;
          //Put the ProcessUnit into the Queue
          puQueue.enqueue(u);
          logdebug("ProcessUnit added with packet count:" << u->_input_packets.size());
          //resetting the parameter for the next ProcessUnit
          _stream_map[sIdx].packet_count = 0;
          _stream_map[sIdx].packets.clear();

        }

        void ProcessUnitWatcher::flushStreamPackets() {
          std::map<int, StreamData>::iterator st = _stream_map.begin();
          for (; st != _stream_map.end(); st++) {
            if ((*st).second.packets.size() > 0)
              buildProcessUnit((*st).first);
          }
          packet_queue.clear();
        }

        void ProcessUnitWatcher::processAudioPacket(boost::shared_ptr<Packet> pPacket) {
          //getting resource from shared Pointer
          Packet pRes = *pPacket.get();

          //getting stream index from Packet
          int sIdx = pRes.packet->stream_index;
          //increment the actuall stream counter
          _stream_map[sIdx].packet_count++;
          //put the packet into the entire PacketList
          if (pRes.packet->size > 0)
            _stream_map[sIdx].packets.push_back(pPacket);

          //when we have enough packets for a ProcessUnit
          if (_stream_map[sIdx].packet_count >= _stream_map[sIdx].min_packet_count) {
            logdebug("Building Audio ProcessUnit");
            //build ProcessUnit with Decoder and Encoder
            buildProcessUnit(sIdx);
          }

        }

        void ProcessUnitWatcher::processVideoPacket(boost::shared_ptr<Packet> pPacket) {
          /**
           * put it in the read ahead queue
           */
          packet_queue.push_back(pPacket);

          /**
           * getting stream index from Packet
           */
          int sIdx = pPacket->packet->stream_index;

          if (packet_queue.size() >= _stream_map[sIdx].b_frame_offset)
            q_filled = true;
          
          /**
           * processing packets in the Queue while reading from input stream
           */
          if (q_filled) {
            
            /**
             * getting Packet from queue
             */
            boost::shared_ptr<Packet> p = packet_queue.front();
            
            /**
             * remove packet from queue
             */
            packet_queue.pop_front();
            /**
             * increment the actuall stream counter
             * @TODO: this will be obsolete, this can be read from the size of the packets list
             */
            _stream_map[sIdx].packet_count++;

            /**
             * put the packet into the entire PacketList
             */
            _stream_map[sIdx].packets.push_back(p);
            //when we have enough packets for a ProcessUnit
            if (packet_queue.front()->isKeyFrame() &&
                _stream_map[sIdx].packet_count >= _stream_map[sIdx].min_packet_count) {
              logdebug("Building Video ProcessUnit");
              //build ProcessUnit with Decoder and Encoder
              //setup b frame offset
              for (int a = 0; a < packet_queue.size() && a < _stream_map[sIdx].b_frame_offset - 1; a++) {
                _stream_map[sIdx].packets.push_back(packet_queue[a]);
              }
              buildProcessUnit(sIdx);
            }
          }

        }

        boost::shared_ptr<ProcessUnit> ProcessUnitWatcher::getProcessUnit() {
          boost::mutex::scoped_lock scoped_lock(get_pu_mutex);
          if (_isStopSignal)
            return boost::shared_ptr<ProcessUnit > (new ProcessUnit());
          if(puQueue.size()==0)
            return boost::shared_ptr<ProcessUnit > (new ProcessUnit());
          boost::shared_ptr<ProcessUnit> u = puQueue.dequeue();
          //special case after an interupted encoding session
          int sIdx = u->_input_packets.front()->packet->stream_index;
          if (_stream_map[sIdx].last_process_unit_id == 0) {
            std::string c = org::esb::config::Config::getProperty("db.connection");
            Connection con(c);
            PreparedStatement stmt = con.prepareStatement("insert into process_units (source_stream, target_stream, start_ts, end_ts, frame_count, send, complete) values (:source_stream, :target_stream, :start_ts, :end_ts, :frame_count, now(), null)");
            stmt.setInt("source_stream", u->_source_stream);
            stmt.setInt("target_stream", u->_target_stream);
            stmt.setLong("start_ts", u->_input_packets.front()->packet->dts);
            stmt.setLong("end_ts", u->_input_packets.back()->packet->dts);
            stmt.setInt("frame_count", u->_input_packets.size());
            stmt.execute();
            u->_process_unit = stmt.getLastInsertId();
          } else {
            u->_process_unit = _stream_map[sIdx].last_process_unit_id;
            _stream_map[sIdx].last_process_unit_id = 0;
          }
          return u;
        }

        bool ProcessUnitWatcher::putProcessUnit(ProcessUnit & unit) {
          boost::mutex::scoped_lock scoped_lock(put_pu_mutex);

          std::string name = org::esb::config::Config::getProperty("hive.base_path");
          name += "/tmp/";
          name += org::esb::util::Decimal(unit._process_unit % 10).toString();
          org::esb::io::File dir(name.c_str());
          if (!dir.exists()) {
            dir.mkdir();
          }
          name += "/";
          name += org::esb::util::Decimal(unit._process_unit).toString();
          name += ".unit";
          org::esb::io::File out(name.c_str());
          org::esb::io::FileOutputStream fos(&out);
          org::esb::io::ObjectOutputStream ous(&fos);
          logdebug("Saving ProcessUnit");
          ous.writeObject(unit);
          _stmt_fr->setInt("id", unit._process_unit);
          _stmt_fr->execute();
          return true;
        }

      }
    }
  }
}

