/* 
 * File:   Format.h
 * Author: jholscher
 *
 * Created on 29. August 2010, 17:40
 */

#ifndef FORMAT_H
#define	FORMAT_H
#include "AV.h"
#include "Rational.h"
namespace org {
  namespace esb {
    namespace av {

      class Format {
      public:
        enum FormatType{
          FORMAT_UNKNOWN,
          FORMAT_VIDEO,
          FORMAT_AUDIO
        } ;
        Format();
        FormatType getType()const ;
      protected:

        friend class VideoFormat;
        PixelFormat _pix_fmt;
        int _width;
        int _height;
        Rational _framerate;
        FormatType _type;

        friend class AudioFormat;
        SampleFormat _sample_format;
        int _sample_rate;
        int _sample_size_in_bits;
        int _channels;
        int _signedd;
        int _frame_size_in_bits;
      };
    }
  }
}

#endif	/* FORMAT_H */

