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
#include "org/esb/lang/Ptr.h"
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
        classlogger("org.esb.av.Decoder")
      public:
        Decoder(CodecID id);
        Decoder(AVStream * c);
        Decoder();
        ~Decoder();
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
        int64_t getLastTimeStamp();
        static std::string getStaticCodecName(CodecID codec_id);
        static CodecID getStaticCodecId(std::string codec_name);

      private:
        int64_t _last_pts;
        int64_t _next_pts;
        Ptr<PixelFormatConverter> _pix_fmt_converter;
      };
    }
  }
}

#endif
