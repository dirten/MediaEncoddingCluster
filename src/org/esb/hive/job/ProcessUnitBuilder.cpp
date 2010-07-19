/* 
 * File:   ProcessUnitBuilder.cpp
 * Author: HoelscJ
 * 
 * Created on 16. Juli 2010, 17:10
 */

#include "ProcessUnitBuilder.h"
#include "StreamData.h"
namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        ProcessUnitBuilder::ProcessUnitBuilder(map<int, StreamData> & data):_map_data( data ){
          
        }

        ProcessUnitBuilder::ProcessUnitBuilder(const ProcessUnitBuilder& orig):_map_data(orig._map_data) {

        }

        ProcessUnitBuilder::~ProcessUnitBuilder() {
        }

        boost::shared_ptr<ProcessUnit> ProcessUnitBuilder::build(org::esb::av::PacketListPtr list) {
          boost::shared_ptr<ProcessUnit> u(new ProcessUnit());
          PacketPtr p = list.front();
          int idx = p->getStreamIndex();
          u->_decoder = _map_data[idx].decoder;
          u->_encoder = _map_data[idx].encoder;
          u->_input_packets = std::list<boost::shared_ptr<Packet> >(list.begin(), list.end());
          u->_gop_size = list.size() - _map_data[idx].b_frame_offset;
          u->_frameRateCompensateBase = _map_data[idx].frameRateCompensateBase;

          AVRational input_framerate = u->_decoder->getFrameRate();
          AVRational output_framerate = u->_encoder->getFrameRate();

          double in = (((double) u->_gop_size) / input_framerate.num * input_framerate.den * output_framerate.num / output_framerate.den);
          in += _map_data[idx].frameRateCompensateBase;
          //+0.001 is against some rounding issues
          double out = floor(in + 0.0001);
          double delta = in - out;
          u->_expected_frame_count = static_cast<int> (out);
          _map_data[idx].frameRateCompensateBase = delta;

          return u;
        }
      }
    }
  }
}
