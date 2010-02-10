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
//#include "AV.h"



//class PacketTermSink;
namespace org {
    namespace esb {
        namespace av {
            class Packet;

            class Encoder : public Codec {
              classlogger("org.esb.av.Encoder")
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
                int64_t getLastTimeStamp();
                int64_t getSamplesBufferd();
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
                int64_t _byte_counter;
                int _frame_counter;
                int _frames;
            };
        }
    }
}

#endif
