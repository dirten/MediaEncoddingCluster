#ifndef ORG_ESB_AV_DECODER
#define ORG_ESB_AV_DECODER
#include "Codec.h"
#include "AV.h"
#ifdef WIN32
#define DEPRICATED(func) __declspec(deprecated) func
#else
#define DEPRICATED(f) __attribute__((deprecated)) f
#endif
namespace org {
    namespace esb {
        namespace av {
            class Frame;

            class Decoder : public Codec {
            public:
                Decoder(CodecID id);
                Decoder(AVCodecContext * c);
                Decoder();
//                DEPRICATED(Frame decode(Packet & packet));
                Frame * decode2(Packet & packet);
                Frame decodeLast();
//                void analyzePacket(Packet & packet);
/*
                template<class Archive>
                void serialize(Archive & ar, const unsigned int version) {
                    ar & boost::serialization::base_object<Codec > (*this);
                }
                 */
//                DEPRICATED(Frame decodeVideo(Packet & packet));
//                DEPRICATED(Frame decodeAudio(Packet & packet));
                Frame * decodeVideo2(Packet & packet);
                Frame * decodeAudio2(Packet & packet);
            private:
                int64_t _last_pts;
            };
        }
    }
}

#endif
