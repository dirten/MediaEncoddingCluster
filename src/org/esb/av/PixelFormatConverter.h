#ifndef ORG_ESB_AV_PIXELCONVERTER_H
#define ORG_ESB_AV_PIXELCONVERTER_H
#include "Format.h"
#include "Frame.h"
#include "AV.h"

namespace org{
  namespace esb{
    namespace av{
      class PixelFormatConverter{
      public:
        PixelFormatConverter(Format in, Format out);
        ~PixelFormatConverter();
        void process(Frame & inbuffer, Frame & outbuffer);
        void open();
        void close();

      private:
        SwsContext * _swsContext;
        Format _in_format;
        Format _out_format;
      };
    }
  }
}
#endif
