/* 
 * File:   StreamData.h
 * Author: HoelscJ
 *
 * Created on 16. Juli 2010, 17:11
 */

#ifndef STREAMDATA_H
#define	STREAMDATA_H
//#include "org/esb/db/hivedb.hpp"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Packet.h"
#include "boost/shared_ptr.hpp"

#include <list>

namespace encodingtask {

  enum State {
    STATE_NOP,
    STATE_START_I_FRAME,
    STATE_END_I_FRAME,
    STATE_START_PACKETGROUP,
    STATE_END_PACKETGROUP
  };

  struct StreamData {
    int instream;
    int outstream;
    int type;
    int64_t last_start_dts;
    int64_t last_start_pts;
    int b_frame_offset;
    boost::shared_ptr<org::esb::av::Decoder> decoder;
    boost::shared_ptr<org::esb::av::Encoder> encoder;
    boost::shared_ptr<org::esb::av::Decoder> pass2decoder;
    boost::shared_ptr<org::esb::av::Encoder> pass2encoder;

    org::esb::av::PacketListPtr packets;
    int packet_count;
    unsigned int min_packet_count;
    int64_t last_bytes_offset;
    int process_unit_count;
    double frameRateCompensateBase;
    AVRational stream_time_base;
    int deinterlace;
    int keep_aspect_ratio;
    State state;
    //                        int64_t last_process_unit_id;

  
        int64_t in_start_time;
      int64_t out_start_time;
      int64_t last_timestamp;
      int64_t next_timestamp;
      int packet_duration;
      AVRational packet_timebase;

      int in_stream_index;
      int out_stream_index;
      AVRational in_timebase;
      AVRational out_timebase;
      int stream_type;

  };

}

#endif	/* STREAMDATA_H */

