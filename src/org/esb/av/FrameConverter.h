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

#ifndef ORG_ESB_AV_FRAMECONVERTER_H
#define ORG_ESB_AV_FRAMECONVERTER_H

#include "AV.h"
#include "Encoder.h"
#include "Decoder.h"
#include "org/esb/util/Log.h"
//struct SwsContext;
#ifdef WIN32
#define DEPRICATED(func) __declspec(deprecated) func
#else
#define DEPRICATED(f) __attribute__((deprecated)) f
#endif
namespace org {
    namespace esb {
        namespace av {

            class Frame;
            struct FrameFormat;

            class FrameConverter {
              classlogger("org.esb.av.FrameConverter")
            public:
                FrameConverter(Decoder* dec, Encoder* enc);
                ~FrameConverter();
                void convert(Frame & input, Frame & out);
                void setFrameRateCompensateBase(double);
                void setGopSize(int);
                void setDeinterlace(bool);
                void setKeepAspectRatio(bool);
            private:
                void convertVideo(Frame & input, Frame & out);
                void convertAudio(Frame & input, Frame & out);
                void rescaleTimestamp(Frame & input, Frame & out);
                void compensateAudioResampling(Frame & input, Frame & out);
                void compensateFrameRateConversion(Frame & input, Frame & out);
                void doDeinterlaceFrame(Frame & in_frame);
                SwsContext * _swsContext;
                FrameFormat * _inFormat;
                FrameFormat * _outFormat;
                Decoder * _dec;
                Encoder * _enc;

                ReSampleContext * _audioCtx;
                double _frameRateCompensateBase;
                int _gop_size;
                int _frame_counter;
                bool _deinterlace;
                bool _keep_aspect_ratio;
                int64_t insamples;
                int64_t outsamples;
                int64_t last_insamples;
                int64_t last_outsamples;
                double inframes;
                double outframes;
                int64_t duplicatedframes;
            };
        }
    }
}


#endif


