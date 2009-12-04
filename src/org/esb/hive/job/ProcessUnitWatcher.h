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
#ifndef ORG_ESB_HIVE_JOB_PROCESSUNITWATCHER_H
#define ORG_ESB_HIVE_JOB_PROCESSUNITWATCHER_H
#include "org/esb/lang/Runnable.h"
#include "org/esb/util/Log.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include "org/esb/hive/job/ProcessUnit.h"

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/util/Queue.h"
#include "Packetizer.h"

#include <map>
#include <deque>
#include <boost/shared_ptr.hpp>
#include <boost/thread/condition.hpp>
//#include <boost/asio.hpp>
namespace org {
    namespace esb {
        namespace hive {
            namespace job {

                class ProcessUnitWatcher : public org::esb::signal::MessageListener {
                    logger("hive.ProcessUnitWatcher")
                public:
                    ProcessUnitWatcher();
                    //			void run();
                    void start();
                    void start2();
                    void start3();
                    void stop();
                    void onMessage(org::esb::signal::Message & msg);
                    static boost::shared_ptr<ProcessUnit> getProcessUnit();
                    static boost::shared_ptr<ProcessUnit> getStreamProcessUnit();
                    static bool putProcessUnit(ProcessUnit & unit);
                private:
                    typedef org::esb::util::Queue<boost::shared_ptr<ProcessUnit> > ProcessUnitQueue;
                    static ProcessUnitQueue puQueue;
					static org::esb::util::Queue<boost::shared_ptr<ProcessUnit>, 500 > audioQueue;
//                    static std::deque<boost::shared_ptr<ProcessUnit> > audioQueue;
                    static std::map<std::string, int> ip2stream;
                    static bool _isStopSignal;
                    static boost::mutex terminationMutex;
                    static boost::condition termination_wait;
                    static bool _isRunning;

                    std::map<int, boost::shared_ptr<ProcessUnit> > unit_map;
                    //                    std::map<int, int> idx;
                    //                    std::map<int, int> inout;
                    //                    std::map<int, int> stream_type;
                    void readJobs();
                    //                    void processAudioPacket(boost::shared_ptr<Packet>);
                    //                    void processVideoPacket(boost::shared_ptr<Packet>);
                    //                    std::map<int, std::list<boost::shared_ptr<Packet> > > stream_packets;
                    std::deque<boost::shared_ptr<Packet> > packet_queue;
                    //                    std::map<int, int> stream_packet_counter;
                    //                    int min_frame_group_count;
                    //                    int b_frame_offset;
                    bool q_filled;
                    int job_id;
                    org::esb::sql::Connection * _con_tmp;
                    org::esb::sql::Connection * _con_tmp2;
                    static org::esb::sql::PreparedStatement * _stmt;
                    //                  void flushStreamPackets();
                    //                  void buildProcessUnit(int sIdx, bool lastPackets=false);
                    static boost::mutex put_pu_mutex;
                    static boost::mutex get_pu_mutex;
                    static boost::mutex get_stream_pu_mutex;
                    static org::esb::sql::PreparedStatement * _stmt_fr;

                    struct StreamData {
                        int instream;
                        int outstream;
                        int type;
                        int64_t last_start_ts;
                        int b_frame_offset;

                        boost::shared_ptr<Decoder> decoder;
                        boost::shared_ptr<Encoder> encoder;
                        std::list<boost::shared_ptr<Packet> > packets;
                        int packet_count;
                        int min_packet_count;
                        int64_t last_bytes_offset;
                        //                        int64_t last_process_unit_id;
                    };
                    static map<int, StreamData> _stream_map;
                    void buildProcessUnit(PacketListPtr list, bool last_packet = false);
                };
            }
        }
    }
}
#endif



