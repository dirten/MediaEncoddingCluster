#ifndef ORG_ESB_AV_CODEC_H
#define ORG_ESB_AV_CODEC_H

#include "AV.h"

#include <boost/serialization/binary_object.hpp>
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
        Codec(AVCodecContext * codec);
        Codec();
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
        void close();
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
        int getWidth();
        int getHeight();
        PixelFormat getPixelFormat();
        int getSampleRate();
        int getBitRate();
        int getFlags();
        SampleFormat getSampleFormat();
        int getChannels();
        int getGopSize();
        AVRational getTimeBase();
        void setStartTime(int64_t start);
        std::string toString(void);
        //				int getCodecType ();
        //				string getCodecName ();

        template < class Archive >
        void serialize(Archive & ar, const unsigned int version) {


          ar & _codec_id;
          ar & _mode;
          ar & _flags;
          ar & _pix_fmt;
          ar & _width;
          ar & _height;
          ar & _time_base.num;
          ar & _time_base.den;
          ar & _gop_size;
          ar & _bit_rate;
          ar & _channels;
          ar & _sample_rate;
          ar & _sample_format;

        }

        CodecID _codec_id;
        //			  protected:

        AVCodec * _codec;
        bool findCodec(int mode);
        int _mode;
        AVCodecContext * ctx;
      protected:
        int _width;
        int _flags;
        int _height;
        PixelFormat _pix_fmt;
        int _bit_rate;
        AVRational _time_base;
        int _gop_size;
        int _channels;
        int _sample_rate;
        int64_t _start_time;
        SampleFormat _sample_format;
        bool _opened;

      private:
        void setParams();

      };
    }
  }
}
#endif
