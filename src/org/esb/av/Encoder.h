#ifndef ORG_ESBG_AV_ENCODER
#define ORG_ESBG_AV_ENCODER

#include "Codec.h"
#include "AV.h"




namespace org {
    namespace esb {
        namespace av {
            class Packet;

            class Encoder : public Codec {
            public:
                Encoder(CodecID id);
                Encoder();
                Packet encode(Frame & f);
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
                Packet encodeVideo(Frame & f);
                Packet encodeAudio(Frame & f);

            };
        }
    }
}

#endif
