/* 
 * File:   ProcessUnitBuilder.cpp
 * Author: HoelscJ
 * 
 * Created on 16. Juli 2010, 17:10
 */

#include "ProcessUnitBuilder.h"
#include "StreamData.h"
#include <math.h>
namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        ProcessUnitBuilder::ProcessUnitBuilder(map<int, StreamData> & data) : _map_data(data) {
          map<int, StreamData>::iterator it = _map_data.begin();
          for (; it != data.end(); it++) {
            (*it).second.frameRateCompensateBase = 0.0;
          }
        }

        ProcessUnitBuilder::ProcessUnitBuilder(const ProcessUnitBuilder& orig) : _map_data(orig._map_data) {

        }

        ProcessUnitBuilder::~ProcessUnitBuilder() {
        }

        boost::shared_ptr<ProcessUnit> ProcessUnitBuilder::build(org::esb::av::PacketListPtr list) {
          boost::shared_ptr<ProcessUnit> u(new ProcessUnit());
          PacketPtr p = list.front();
          int idx = p->getStreamIndex();
          u->_source_stream = _map_data[idx].instream;
          u->_target_stream = _map_data[idx].outstream;
          u->_decoder = _map_data[idx].decoder;
          u->_encoder = _map_data[idx].encoder;
          u->_input_packets = std::list<boost::shared_ptr<Packet> >(list.begin(), list.end());
          u->_gop_size = list.size(); //- _map_data[idx].b_frame_offset;
          u->_frameRateCompensateBase = _map_data[idx].frameRateCompensateBase;
          if (u->_decoder->getCodecType() == AVMEDIA_TYPE_VIDEO) {
            AVRational input_framerate = u->_decoder->getFrameRate();
            AVRational output_framerate = u->_encoder->getFrameRate();

            double in = (((double) u->_gop_size) / input_framerate.num * input_framerate.den * output_framerate.num / output_framerate.den);
            in += _map_data[idx].frameRateCompensateBase;
            //+0.001 is against some rounding issues
            double out;
            double delta = modf(in, &out);
            //= floor(in + 0.0001);

            u->_expected_frame_count = static_cast<int> (out);
            _map_data[idx].frameRateCompensateBase = delta;
          }
          if (u->_decoder->getCodecType() == AVMEDIA_TYPE_AUDIO) {
            AVRational input_timebase = u->_decoder->getTimeBase();
            AVRational output_timebase = u->_encoder->getTimeBase();

            int64_t input_samplerate = u->_decoder->getSampleRate();
            int64_t output_samplerate = u->_encoder->getSampleRate();

            int64_t output_framesize=u->_encoder->getFrameBytes();
            /*
             * the decoder did not returns a correct frame size
             * calculation on the base of the packet->duration and packet->timebase
             */
            int64_t input_framesize=av_rescale_q(p->getDuration(),p->getTimeBase(),input_timebase);
            int osize = av_get_bits_per_sample_format(u->_decoder->getSampleFormat()) / 8;
            input_framesize*=osize;
            input_framesize*=u->_decoder->ctx->request_channel_layout;

            int64_t raw_in_samples=input_framesize*list.size();
            int64_t raw_out_samples=av_rescale_q(raw_in_samples,input_timebase,output_timebase);
            raw_out_samples+=_map_data[idx].frameRateCompensateBase;
            int rest=raw_out_samples%output_framesize;
            double out;
            double delta = modf(raw_out_samples/output_framesize, &out);
            u->_expected_frame_count = static_cast<int> (out);
            _map_data[idx].frameRateCompensateBase =rest;
//            LOGDEBUG(rest);
          }

          return u;
        }
      }
    }
  }
}
