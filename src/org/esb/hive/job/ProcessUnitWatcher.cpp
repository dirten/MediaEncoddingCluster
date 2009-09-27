
#include <string>

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "ProcessUnitWatcher.h"
#include "ClientHandler.h"

#include "org/esb/lang/Thread.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/av/FormatStreamFactory.h"
#include "org/esb/av/FormatInputStream.h"


#include <deque>
#include <list>
namespace org {
  namespace esb {
    namespace hive {
      namespace job {
        //        std::map<int, boost::shared_ptr<ProcessUnit> > ProcessUnitWatcher::unit_map;

        ProcessUnitWatcher::ProcessUnitWatcher() {
          _isStopSignal = false;
        }

        void ProcessUnitWatcher::onMessage(org::esb::signal::Message & msg) {
          if (msg.getProperty("processunitwatcher") == "start") {
            boost::thread t(boost::bind(&ProcessUnitWatcher::start3, this));
            logdebug("ProcessUnitWatcher started");
          } else
            if (msg.getProperty("processunitwatcher") == "stop") {
            _isStopSignal = true;
          }
        }

        void ProcessUnitWatcher::stop() {

        }

        void ProcessUnitWatcher::start3() {
          while (!_isStopSignal) {
            //getting all jobs that not be completed
            sql::Connection con(std::string(config::Config::getProperty("db.connection")));
            sql::Statement stmt = con.createStatement("select * from jobs, files where jobs.inputfile=files.id and complete is null");
            sql::ResultSet rs = stmt.executeQuery();
            if (rs.next()) {
              string filename = rs.getString("files.path") + "/" + rs.getString("files.filename");
              org::esb::av::FormatInputStream * fis = org::esb::av::FormatStreamFactory::getInputStream(filename);
              if (fis == NULL) {
                logerror("Error Opening Input Stream from " << filename);
                continue;
              }
              stream_packet_counter.clear();
              stream_packets.clear();

              //getting Stream Map
              sql::Connection con2(std::string(config::Config::getProperty("db.connection")));
              sql::PreparedStatement pstmt = con2.prepareStatement("select * from job_details, streams where job_id=:myid and streams.id=instream");
              pstmt.setInt("myid", rs.getInt("jobs.id"));
              sql::ResultSet rs2 = pstmt.executeQuery();
              while (rs2.next()) {
                idx[rs2.getInt("stream_index")] = rs2.getInt("instream");
                inout[rs2.getInt("instream")] = rs2.getInt("outstream");
                stream_type[rs2.getInt("stream_index")] = rs2.getInt("stream_type");
              }

              long offset = 0; //(den/(type==CODEC_TYPE_VIDEO?framerate:samplerate))*gop;
              logdebug("building seek offset -" << offset);
              //              fis->seek(offset);
              PacketInputStream pis(fis);
              b_frame_offset = 4;
              q_filled = false;
              min_frame_group_count = 5;

              Packet packet;
              //read while packets in the stream
              while (pis.readPacket(packet) == 0) {
                //if the actuall stream not mapped then discard this and continue with next packet
                if (/*packet.packet->stream_index != 0 ||*/ idx.find(packet.packet->stream_index) == idx.end()) {
                 // loginfo("Stream Index not mapped:stream_index#" << packet.packet->stream_index);
                  continue;
                }
                //building a shared Pointer from packet because the next read from PacketInputStream kills the Packet data
                boost::shared_ptr<Packet> pPacket(new Packet(packet));
                if (stream_type[packet.packet->stream_index] == CODEC_TYPE_AUDIO) {
                  processAudioPacket(pPacket);
                } else if (stream_type[packet.packet->stream_index] == CODEC_TYPE_VIDEO) {
                  processVideoPacket(pPacket);
                } else {
                }
              }
              processAudioPacket(boost::shared_ptr<Packet > (new Packet()));
              processVideoPacket(boost::shared_ptr<Packet > (new Packet()));
              logdebug("file completed:" << filename);
              sql::Connection con3(std::string(config::Config::getProperty("db.connection")));
              sql::PreparedStatement pstmt2 = con3.prepareStatement("update jobs set complete=now() where id=:jobid");
              pstmt2.setInt("jobid", rs.getInt("jobs.id"));
              pstmt2.execute();
            }
            Thread::sleep2(1000);
          }
        }

        void ProcessUnitWatcher::processAudioPacket(boost::shared_ptr<Packet> pPacket) {
          //getting resource from shared Pointer
          Packet pRes = *pPacket.get();

          //getting stream index from Packet
          int sidx = pRes.packet->stream_index;
          //increment the actuall stream counter
          stream_packet_counter[sidx]++;
          //put the packet into the entire PacketList
          if (pRes.packet->size>0)
            stream_packets[sidx].push_back(pPacket);

          //when we have enough packets for a ProcessUnit
          if (pRes.packet->size == 0 || (stream_packet_counter[sidx] >= min_frame_group_count * (stream_type[sidx] == CODEC_TYPE_AUDIO ? 1000 : 1))) {
            logdebug("Building Audio ProcessUnit");
            //build ProcessUnit with Decoder and Encoder
            boost::shared_ptr<ProcessUnit> u(new ProcessUnit());
            u->_source_stream = idx[sidx];
            u->_target_stream = inout[idx[sidx]];
            u->_decoder = CodecFactory::getStreamDecoder(u->_source_stream);
            u->_encoder = CodecFactory::getStreamEncoder(u->_target_stream);
            //setting Input Packets
            u->_input_packets = stream_packets[sidx];
            //Put the ProcessUnit into the Queue
            ClientHandler::addProcessUnit(u);
            logdebug("ProcessUnit added with packet count:" << u->_input_packets.size());
            //resetting the parameter for the next ProcessUnit
            stream_packet_counter[sidx] = 0;
            stream_packets[sidx].clear();
            //                    unit_map.erase(tmp_p.packet->stream_index);
          }


        }

        void ProcessUnitWatcher::processVideoPacket(boost::shared_ptr<Packet> pPacket) {
          //and put it in the read ahead queue
          packet_queue.push_back(pPacket);

          if (packet_queue.size() >= b_frame_offset)
            q_filled = true;
          //processing packets in the Queue while reading from input stream
          if (q_filled) {
            //getting Packet from queue
            boost::shared_ptr<Packet> p = packet_queue.front();
            //getting resource from shared Pointer
            Packet pRes = *p.get();
            //remove packet from queue
            packet_queue.pop_front();
            //getting stream index from Packet
            int sidx = pRes.packet->stream_index;
            //increment the actuall stream counter
            stream_packet_counter[sidx]++;
            //put the packet into the entire PacketList
//            if (pRes.packet->data != NULL)
              stream_packets[sidx].push_back(p);
            //when we have enough packets for a ProcessUnit
            if (pPacket->packet->data == NULL || (packet_queue.front()->isKeyFrame() && stream_packet_counter[sidx] >= min_frame_group_count * (stream_type[sidx] == CODEC_TYPE_AUDIO ? 1000 : 1))) {
              logdebug("Building Video ProcessUnit");
              //build ProcessUnit with Decoder and Encoder
              boost::shared_ptr<ProcessUnit> u(new ProcessUnit());
              u->_source_stream = idx[sidx];
              u->_target_stream = inout[idx[sidx]];
              u->_decoder = CodecFactory::getStreamDecoder(u->_source_stream);
              u->_encoder = CodecFactory::getStreamEncoder(u->_target_stream);
              //setup b frame offset
              for (int a = 0; a<packet_queue.size()&&a < b_frame_offset - 1; a++) {
                stream_packets[sidx].push_back(packet_queue[a]);
              }
              //setting Input Packets
              u->_input_packets = stream_packets[sidx];
              //Put the ProcessUnit into the Queue
              ClientHandler::addProcessUnit(u);
              logdebug("ProcessUnit added with packet count:" << u->_input_packets.size());
              //resetting the parameter for the next ProcessUnit
              stream_packet_counter[sidx] = 0;
              stream_packets[sidx].clear();
              //                    unit_map.erase(tmp_p.packet->stream_index);
            }
          }

        }

        void ProcessUnitWatcher::start2() {
          while (!_isStopSignal) {

            //            sql::Statement stmt = con.createStatement("select * from jobs, files, job_details, streams ins, streams outs where jobs.inputfile=files.id and jobs.id=job_details.job_id and instream=ins.id and outstream=outs.id and complete is null");
            sql::Connection con(std::string(config::Config::getProperty("db.connection")));
            sql::Statement stmt = con.createStatement("select * from jobs, files where jobs.inputfile=files.id and complete is null");
            sql::ResultSet rs = stmt.executeQuery();
            if (rs.next()) {
              string filename = rs.getString("files.path");
              filename += "/";
              filename += rs.getString("files.filename");
              org::esb::av::FormatInputStream * fis = org::esb::av::FormatStreamFactory::getInputStream(filename);
              logdebug("B-FRAMES:" << fis->getFormatContext()->streams[0]->codec->max_b_frames);
              if (fis == NULL) {
                logerror("Error Opening Input Stream from " << filename);
                continue;
              }
              logerror("hier");
              sql::Connection con2(std::string(config::Config::getProperty("db.connection")));
              sql::PreparedStatement pstmt = con2.prepareStatement("select * from job_details, streams where job_id=:myid and streams.id=instream");
              pstmt.setInt("myid", rs.getInt("jobs.id"));
              sql::ResultSet rs2 = pstmt.executeQuery();
              while (rs2.next()) {
                idx[rs2.getInt("stream_index")] = rs2.getInt("instream");
                inout[rs2.getInt("instream")] = rs2.getInt("outstream");
              }
              //              continue;
              std::map<int, int> stream_packet_counter;
              int min_frame_group_count = 5;

              PacketInputStream pis(fis);
              int b_frame_offset = 3;
              bool q_filled = false;
              deque<boost::shared_ptr<Packet> > packet_queue;
              while (true) {

                //                for (int a = 0; true; a++) {
                Packet pack;
                if (pis.readPacket(pack) < 0) {
                  logdebug("Null Packet from Stream");
                  break;
                }
                boost::shared_ptr<Packet> p2(new Packet(pack));
                packet_queue.push_back(p2);
                if (packet_queue.size() >= b_frame_offset) {
                  q_filled = true;
                }
                if (q_filled) {
                  boost::shared_ptr<Packet> p = packet_queue.front();
                  Packet tmp_p = *p.get();
                  packet_queue.pop_front();
                  if (idx.find(tmp_p.packet->stream_index) == idx.end())continue;
                  stream_packet_counter[tmp_p.packet->stream_index]++;

                  if (unit_map.find(tmp_p.packet->stream_index) != unit_map.end() && tmp_p.isKeyFrame() && stream_packet_counter[tmp_p.packet->stream_index] >= min_frame_group_count * (unit_map[tmp_p.packet->stream_index]->_decoder->getCodecType() == CODEC_TYPE_AUDIO ? 1000 : 1)) {
                    boost::shared_ptr<ProcessUnit> u = unit_map[tmp_p.packet->stream_index];
                    if (unit_map[tmp_p.packet->stream_index]->_decoder->getCodecType() == CODEC_TYPE_VIDEO) {
                      for (int a = 1; a < packet_queue.size() && a <= b_frame_offset; a++) {
                        unit_map[tmp_p.packet->stream_index]->_input_packets.push_back(packet_queue[a]);
                      }
                    }
                    ClientHandler::addProcessUnit(u);
                    logdebug("ProcessUnit added with packet count:" << u->_input_packets.size());
                    stream_packet_counter[tmp_p.packet->stream_index] = 0;
                    unit_map.erase(tmp_p.packet->stream_index);
                  }

                  if (unit_map.find(tmp_p.packet->stream_index) == unit_map.end()) {
                    boost::shared_ptr<ProcessUnit> u(new ProcessUnit());
                    u->_source_stream = idx[tmp_p.packet->stream_index];
                    u->_target_stream = inout[idx[tmp_p.packet->stream_index]];
                    u->_decoder = CodecFactory::getStreamDecoder(u->_source_stream);
                    u->_decoder->open();
                    u->_encoder = CodecFactory::getStreamEncoder(u->_target_stream);
                    unit_map[tmp_p.packet->stream_index] = u;
                  }
                  boost::shared_ptr<ProcessUnit> u = unit_map[tmp_p.packet->stream_index];
                  u->_input_packets.push_back(p);
                }
              }
            } else {
              Thread::sleep2(10000);
            }
            sql::Connection con2(std::string(config::Config::getProperty("db.connection")));
            sql::PreparedStatement pstmt = con2.prepareStatement("update jobs set complete=now() where id=:jobid");
            pstmt.setInt("jobid", rs.getInt("jobs.id"));
            pstmt.execute();
          }
        }

        void ProcessUnitWatcher::start() {
          sql::Connection con(std::string(config::Config::getProperty("db.connection")));
          while (!_isStopSignal) {
            //	    logdebug("ProcessUnitWatcher cycle");
            sql::Statement stmt = con.createStatement("select id, source_stream, target_stream from process_units where send is null order by priority limit 100");
            sql::ResultSet rs = stmt.executeQuery();
            while (rs.next()) {
              boost::shared_ptr<ProcessUnit> unit(new ProcessUnit());
              unit->id = rs.getInt("id");
              unit->_decoder = hive::CodecFactory::getStreamDecoder(rs.getInt("source_stream"));
              unit->_encoder = hive::CodecFactory::getStreamEncoder(rs.getInt("target_stream"));
              unit->_source_stream = rs.getInt("source_stream");
              unit->_target_stream = rs.getInt("target_stream");

              if (ClientHandler::addProcessUnit(unit)) {
                logdebug("ProcessUnit added with id " << rs.getInt("id"));
              }
            }
            org::esb::lang::Thread::sleep2(1000);
          }
        }



      }
    }
  }
}

