/*----------------------------------------------------------------------
 *  File    : Decoder.h
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
#ifndef ORG_ESB_AV_DECODER
#define ORG_ESB_AV_DECODER
#include "Codec.h"
#include "AV.h"
#include "PixelFormatConverter.h"
#include "exports.h"
#include "AVPipe.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/lang/Deprecated.h"
namespace org {
  namespace esb {
    namespace av {
      class Frame;

      class AV_EXPORT Decoder : public Codec , public AVPipe {
        classlogger("org.esb.av.Decoder")
      public:
        Decoder(CodecID id);
        //Decoder(std::string name);
        Decoder(AVStream * c);
        Decoder();
        ~Decoder();
        DEPRECATED(Frame * decode2(Packet & packet));
        DEPRECATED(Frame decodeLast());
        int64_t getLastTimeStamp();
        bool newPacket(Ptr<Packet>p);
         bool newFrame(Ptr<Frame>){return false;}
        static std::string getStaticCodecName(CodecID codec_id);
        static CodecID getStaticCodecId(std::string codec_name);
        void reset();
      private:
        bool decode3(Packet & packet);
        DEPRECATED(Frame * decodeVideo2(Packet & packet));
        bool decodeVideo3(Packet & packet);
        DEPRECATED(Frame * decodeAudio2(Packet & packet));
        bool decodeAudio3(Packet & packet);

        int64_t _last_pts;
        int64_t _next_pts;
        Ptr<PixelFormatConverter> _pix_fmt_converter;
        Ptr<Frame> frame;
        AVRational _input_packet_time_base;
      };
    }
  }
}

#endif
