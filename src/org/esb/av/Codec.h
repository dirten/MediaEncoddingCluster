/*----------------------------------------------------------------------
 *  File    : ProcessUnit.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 6. November 2009, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 * ----------------------------------------------------------------------
 */
#ifndef ORG_ESB_AV_CODEC_H
#define ORG_ESB_AV_CODEC_H

#include "AV.h"

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>

#include "Format.h"
#include "Packet.h"
#include "org/esb/util/Log.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread/condition.hpp>
#include <list>
#include "exports.h"
namespace org {
  namespace esb {
    namespace av {
      class Frame;

      /*class CodecID;
class AVCodecContext;
class PixelFormat;
class SampleFormat;*/

      class AV_EXPORT Codec {
        classlogger("org.esb.av.Codec")
      public:
        const static int ENCODER = 1;
        const static int DECODER = 2;
        Codec(std::string codec_name, int mode = DECODER);
        Codec(const CodecID codecId, int mode = DECODER);
        Codec(AVStream * stream, int mode = DECODER);
        Codec(int mode = DECODER);
        Codec(std::map<std::string, std::string>,int mode = DECODER);
        virtual ~Codec();
        /**
         *
         * @return The type of this Codec, as a enum Type
         */
        AVMediaType getCodecType();

        /**
         * @return The name of the Codec.
         */
        const char *getCodecName();
        /**
         * @return the ID of this codec, an enum CodecID
         */
        CodecID getCodecId();
        virtual bool open();
        void setWidth(int w);
        void setHeight(int h);
        void setPixelFormat(PixelFormat pfm);
        void setBitRate(int rate);
        void setTimeBase(AVRational tb);
        void setTimeBase(int num, int den);
        void setFrameRate(int num, int den);
        void setFrameRate(AVRational fr);
        void setGopSize(int size);
        void setChannels(int size);
        void setSampleRate(int size);
        void setSampleFormat(AVSampleFormat size);
        void setFlag(int flag);
        void setCodecId(CodecID);
        int getWidth();
        int getHeight();
        PixelFormat getPixelFormat();
        int getSampleRate();
        int getBitRate();
        int getFlags();
        AVSampleFormat getSampleFormat();
        int getChannels();
        int getGopSize();
        int getBitsPerCodedSample();
        void setBitsPerCodedSample(int);
        AVRational getTimeBase();
        AVRational getFrameRate();
        int64_t getFrameBytes();
        int setCodecOption(std::string opt, std::string arg);
        std::string getCodecOption(std::string opt);
        std::map<std::string, std::string> getCodecOptions();
        Format getOutputFormat();
        Format getInputFormat();
        void setOutputFormat(Format);
        void setInputFormat(Format);
        std::list<Format> getSupportedInputFormats();
        //                void setStartTime(int64_t start);
        std::string toString(void);
        void setStreamIndex(int idx);
        //				int getCodecType ();
        //				string getCodecName ();

        template < class Archive >
        void baseserialize(Archive & ar, const unsigned int version) {
          ar & ctx->codec_id;
          ar & _mode;
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
          ar & _bytes_discard;
          ar & ctx->bits_per_coded_sample;
          ar & ctx->extradata_size;
        }

        template<class Archive>
        void save(Archive & ar, const unsigned int version) const {
          //LOGTRACE("serialization save");
          ar & ctx->codec_id;
          ar & _mode;
          ar & ctx->flags;
//          ar & ctx->flags2;
          ar & ctx->pix_fmt;
          ar & ctx->width;
          ar & ctx->height;
          ar & ctx->time_base.num;
          ar & ctx->time_base.den;
          ar & _frame_rate.num;
          ar & _frame_rate.den;
          ar & ctx->gop_size;
          ar & ctx->bit_rate;
          ar & ctx->channels;
          ar & ctx->sample_rate;
          ar & ctx->sample_fmt;
          ar & _bytes_discard;
          ar & ctx->bits_per_coded_sample;
          ar & _options;
          ar & ctx->codec_type;
          ar & _stream_index;
          if (_mode == Codec::DECODER) {
            ar & ctx->extradata_size;
            if (ctx->extradata_size > 0) {
              ar & boost::serialization::make_binary_object(ctx->extradata, ctx->extradata_size);
            }
          }

          //                    saveCodecOption();
        };

        template<class Archive>
        void load(Archive & ar, const unsigned int version) {
          
          //LOGTRACE("serialization load");
          ar & ctx->codec_id;
          ar & _mode;
          ar & ctx->flags;
//          ar & ctx->flags2;
          ar & ctx->pix_fmt;
          ar & ctx->width;
          ar & ctx->height;
          ar & ctx->time_base.num;
          ar & ctx->time_base.den;
          ar & _frame_rate.num;
          ar & _frame_rate.den;
          ar & ctx->gop_size;
          ar & ctx->bit_rate;
          ar & ctx->channels;
          ar & ctx->sample_rate;
          ar & ctx->sample_fmt;
          ar & _bytes_discard;
          ar & ctx->bits_per_coded_sample;
          ar & _options;
          ar & ctx->codec_type;
          ar & _stream_index;

          if (_mode == Codec::DECODER) {
            ar & ctx->extradata_size;
            //LOGDEBUG("Extra data"<<ctx->extradata_size);
            if (ctx->extradata_size > 0) {
              ctx->extradata = static_cast<boost::uint8_t*> (av_malloc(ctx->extradata_size + FF_INPUT_BUFFER_PADDING_SIZE));
              memset(ctx->extradata, 0, ctx->extradata_size + FF_INPUT_BUFFER_PADDING_SIZE);
              ar & boost::serialization::make_binary_object(ctx->extradata, ctx->extradata_size);
            } else {
              ctx->extradata = NULL;
            }
          }
          //findCodec(_mode);
        };
        BOOST_SERIALIZATION_SPLIT_MEMBER();

        AVCodec * _codec;
        AVDictionary * _codec_opts;
        bool findCodec(int mode);
        AVCodec * findCodecByName(std::string name, int mode);

        int _mode;
        AVCodecContext * ctx;
        AVRational _frame_rate;
        int64_t _bytes_discard;
        //                bool saveCodecOption();
        //                bool loadCodecOption();
        void close();
        void reset();
      protected:
        bool _opened;
        AVFifoBuffer *fifo;
        std::string _codec_options;
        Format _input_format;
        Format _output_format;
        static const PixelFormat STD_PIX_FMT=PIX_FMT_YUV420P;
        int _stream_index;
      private:
        void setParams();
        void setContextDefaults();
        bool _codec_resolved;
        bool _pre_allocated;
        static boost::mutex open_close_mutex;
        std::map<std::string, std::string> _options;
        AVDictionary * _dict;
      };
    }
  }
}
#endif
