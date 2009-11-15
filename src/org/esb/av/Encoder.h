#ifndef ORG_ESBG_AV_ENCODER
#define ORG_ESBG_AV_ENCODER


#include "Codec.h"
#include "PacketOutputStream.h"
#include "Sink.h"
//#include "AV.h"



//class PacketTermSink;
namespace org {
  namespace esb {
    namespace av {
      class Packet;

      class Encoder : public Codec {
      public:
        Encoder(CodecID id);
        Encoder();
        ~Encoder();
        int encode(Frame & f);
        int encode();
        char * getStatistics();
        void setStatistics(char *);
        void setOutputStream(PacketOutputStream *);
        void setSink(Sink*);
        /*
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version){
            ar & codec_id;
            ar & _mode;
            ar & codec_type;
            ar & bit_rate;
            ar & pix_fmt;
            ar & width;
            ar & height;
            ar & has_b_frames;
            ar & extradata_size;
            if(extradata==NULL){
                extradata=new uint8_t[extradata_size];
            }
            ar & boost::serialization::make_binary_object(extradata,extradata_size);
        }*/
      private:
        int encodeVideo(Frame & f);
        int encodeVideo(AVFrame *);
        int encodeAudio(Frame & f);
//        AVFifoBuffer *fifo;
        PacketOutputStream * _pos;
        Sink*_sink;
        int64_t _last_dts;
        int64_t _last_duration;
		int _last_idx;
        AVRational _last_time_base;
      };
    }
  }
}

#endif
