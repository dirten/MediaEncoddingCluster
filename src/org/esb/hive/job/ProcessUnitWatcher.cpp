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
#include "../FormatStreamFactory.h"
#include "org/esb/av/FormatInputStream.h"


#include <deque>
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
            boost::thread t(boost::bind(&ProcessUnitWatcher::start2, this));
          } else
            if (msg.getProperty("processunitwatcher") == "stop") {
            _isStopSignal = true;
          }
        }

        void ProcessUnitWatcher::stop() {

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
              org::esb::av::FormatInputStream * fis = org::esb::hive::FormatStreamFactory::getInputStream(filename);
			  logdebug("B-FRAMES:"<<fis->getFormatContext()->streams[0]->codec->max_b_frames);
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
			  int b_frame_offset=3;
			  deque<boost::shared_ptr<Packet> > offset_queue;
              while (true) {

//                for (int a = 0; true; a++) {
                  Packet tmp_p;
                  if (pis.readPacket(tmp_p) < 0) {
                    logdebug("Null Packet from Stream");
                    break;
                  }
                  boost::shared_ptr<ProcessUnit> u = unit_map[tmp_p.packet->stream_index];
                  boost::shared_ptr<Packet> p(new Packet(tmp_p));
//				  offset_queue.push_back(p);
                  if (idx.find(tmp_p.packet->stream_index) == idx.end())continue;
                  stream_packet_counter[tmp_p.packet->stream_index]++;

                  if (unit_map.find(tmp_p.packet->stream_index)!=unit_map.end()&&tmp_p.isKeyFrame()&&stream_packet_counter[tmp_p.packet->stream_index] >= min_frame_group_count * (unit_map[tmp_p.packet->stream_index]->_decoder->getCodecType() == CODEC_TYPE_AUDIO ? 1000 : 1)) {
                    boost::shared_ptr<ProcessUnit> u = unit_map[tmp_p.packet->stream_index];
                    ClientHandler::addProcessUnit(u);
                    logdebug("ProcessUnit added with packet count:"<<u->_input_packets.size());
                    stream_packet_counter[tmp_p.packet->stream_index]=0;
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

                  u->_input_packets.push_back(p);

                
              }
			}else{
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

