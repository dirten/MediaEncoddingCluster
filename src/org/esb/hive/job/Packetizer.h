/* 
 * File:   Packetizer.h
 * Author: jholscher
 *
 * Created on 27. Oktober 2009, 23:35
 */

#ifndef _PACKETIZER_H
#define	_PACKETIZER_H
#include <boost/shared_ptr.hpp>
#include "org/esb/av/Packet.h"
#include "org/esb/hive/job/ProcessUnit.h"

#include <deque>
#include <map>
namespace org {
    namespace esb {
        namespace hive {
            namespace job {
                typedef boost::shared_ptr<org::esb::av::Packet> PacketPtr;
                typedef std::deque<PacketPtr> PacketListPtr;

                class Packetizer {
                public:
                    struct StreamData {
                        PacketListPtr packets;
                        int min_packet_count;
                        CodecType codec_type;
                        int jut_size;
                        bool jut_size_reached;
                    };
                    Packetizer(std::map<int,StreamData>);
                    ~Packetizer();
                    bool putPacket(PacketPtr);
                    void flushStreams();
                    PacketListPtr getPacketList();
                    PacketListPtr removePacketList();
                    int getPacketListCount();
                private:
                    bool processAudioPacket(PacketPtr);
                    bool processVideoPacket(PacketPtr);
                    bool buildList(int stream_id);

                    std::map<int, StreamData> _streams;
                    std::list<PacketListPtr> _packet_list;
                    std::map<int, PacketListPtr> _jut_queue;
                };
            }
        }
    }
}

#endif	/* _PACKETIZER_H */

