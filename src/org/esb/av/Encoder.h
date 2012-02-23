/*----------------------------------------------------------------------
 *  File    : Encoder.cpp
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
#ifndef ORG_ESBG_AV_ENCODER
#define ORG_ESBG_AV_ENCODER


#include "Codec.h"
#include "PacketOutputStream.h"
#include "Sink.h"
#include "PixelFormatConverter.h"
#include "org/esb/lang/Ptr.h"
#include "Decoder.h"
#include "exports.h"


//class PacketTermSink;
namespace org {
    namespace esb {
        namespace av {
            class Packet;

            class AV_EXPORT Encoder : public Codec {
              classlogger("org.esb.av.Encoder")
            public:
                Encoder(CodecID id);
                Encoder(std::string name);
                Encoder(std::map<std::string, std::string>);
                Encoder();
                ~Encoder();
                virtual bool open();
                int encode(Frame & f);
                int encode();
                char * getStatistics();
                void setStatistics(char *);
                void setOutputStream(PacketOutputStream *);
                void setSink(Sink*);
                int64_t getLastTimeStamp();
                int64_t getNextTimeStamp();
                int64_t getSamplesBufferd();
                void setPassLogfile(std::string);
                std::string getPassLogfile();
            private:
                int encodeVideo(Frame & f);
                int encodeVideo(AVFrame *);
                int encodeAudio(Frame & f);
                void writeStatistics(std::string data);
                std::string readStatistics();
                //        AVFifoBuffer *fifo;
                PacketOutputStream * _pos;
                Sink*_sink;
                int64_t _last_dts;
                int64_t _last_duration;
                int _last_idx;
                AVRational _last_time_base;
                AVRational _pixel_aspect_ratio;
                int64_t _byte_counter;
                int _frame_counter;
                int _frames;
                Ptr<PixelFormatConverter> _pix_fmt_converter;

                /**
                 * this decoder will be used to calculate the psnr/ssim
                 * it can be enabled by setting the codecOption psnr=1 ot ssim=1
                 */
                Ptr<Decoder>_refDecoder;
                void processPsnr(Frame *ref, Frame * cmp);
                Frame * _actualFrame;
                std::string _passlogfile;
            };
        }
    }
}

#endif
