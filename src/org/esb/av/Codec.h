#ifndef ORG_ESB_AV_CODEC_H
#define ORG_ESB_AV_CODEC_H

#include "AV.h"

#include <boost/serialization/serialization.hpp>
#include "Packet.h"
#include "org/esb/util/Log.h"
#include <boost/shared_ptr.hpp>

#include <list>
namespace org {
    namespace esb {
        namespace av {
            class Frame;

            /*class CodecID;
      class AVCodecContext;
      class PixelFormat;
      class SampleFormat;*/

            class Codec {
                logger("hive.av.codec")
            public:
                const static int DECODER = 1;
                const static int ENCODER = 2;
                Codec(const CodecID codecId, int mode = DECODER);
                Codec(AVCodecContext * codec, int mode = DECODER);
                Codec(int mode= DECODER);
                ~Codec();
                /**
                 *
                 * @return The type of this Codec, as a enum Type
                 */
                CodecType getCodecType();

                /**
                 * @return The name of the Codec.
                 */
                char *getCodecName();
                /**
                 * @return the ID of this codec, an enum CodecID
                 */
                CodecID getCodecId();
                int open();
                void setWidth(int w);
                void setHeight(int h);
                void setPixelFormat(PixelFormat pfm);
                void setBitRate(int rate);
                void setTimeBase(AVRational tb);
                void setGopSize(int size);
                void setChannels(int size);
                void setSampleRate(int size);
                void setSampleFormat(SampleFormat size);
                void setFlag(int flag);
                void setCodecId(CodecID);
                int getWidth();
                int getHeight();
                PixelFormat getPixelFormat();
                int getSampleRate();
                int getBitRate();
                int getFlags();
                SampleFormat getSampleFormat();
                int getChannels();
                int getGopSize();
                int getBitsPerCodedSample();
                void setBitsPerCodedSample(int);
                AVRational getTimeBase();
//                void setStartTime(int64_t start);
                std::string toString(void);
                //				int getCodecType ();
                //				string getCodecName ();

                template < class Archive >
                void serialize(Archive & ar, const unsigned int version) {


                    ar & ctx->codec_id;
                    ar & _mode;
//                    ar & _opened;
                    ar & ctx->flags;
                    ar & ctx->pix_fmt;
                    ar & ctx->width;
                    ar & ctx->height;
                    ar & ctx->time_base.num;
                    ar & ctx->time_base.den;
                    ar & ctx->gop_size;
                    ar & ctx->bit_rate;
                    ar & ctx->channels;
                    ar & ctx->sample_rate;
                    ar & ctx->sample_fmt;
//                    ar & ctx->bits_per_coded_sample;

                }

  //              CodecID _codec_id;
                //			  protected:

                AVCodec * _codec;
                bool findCodec(int mode);
                int _mode;
                AVCodecContext * ctx;
            protected:
                bool _opened;
                AVFifoBuffer *fifo;
            private:
                void close();
                void setParams();
                void setContextDefaults();
                bool _codec_resolved;
                bool _pre_allocated;
            };
        }
    }
}
#endif
