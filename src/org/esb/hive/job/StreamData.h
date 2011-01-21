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

namespace org {
  namespace esb {
    namespace hive {
      namespace job {

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
          
          std::list<boost::shared_ptr<org::esb::av::Packet> > packets;
          int packet_count;
          int min_packet_count;
          int64_t last_bytes_offset;
          int process_unit_count;
          double frameRateCompensateBase;
          AVRational stream_time_base;
          int deinterlace;
          int keep_aspect_ratio;
          //                        int64_t last_process_unit_id;
        };

      }
    }
  }
}

#endif	/* STREAMDATA_H */

