#ifndef ORG_ESB_AV_FRAMECONVERTER_H
#define ORG_ESB_AV_FRAMECONVERTER_H

#include "AV.h"
#include "Encoder.h"
#include "Decoder.h"

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
      public:
        FrameConverter(FrameFormat& targetFormat);
        //		FrameConverter(FrameFormat& sourceFormat,FrameFormat& targetFormat);
        FrameConverter(Decoder* dec, Encoder* enc);
        ~FrameConverter();
        DEPRICATED(Frame convert(Frame & input));
        void convert(Frame & input, Frame & out);
      private:
        Frame convertVideo(Frame & input);
        Frame convertAudio(Frame & input);
        void convertVideo(Frame & input, Frame & out);
        void convertAudio(Frame & input, Frame & out);
        void rescaleTimestamp(Frame & input, Frame & out);
        //	    void getSwsContext();
        SwsContext * _swsContext;
        FrameFormat * _inFormat;
        FrameFormat * _outFormat;
        Decoder * _dec;
        Encoder * _enc;

        ReSampleContext * _audioCtx;
      };
    }
  }
}


#endif


