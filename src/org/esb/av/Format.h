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
#include "exports.h"
namespace org {
  namespace esb {
    namespace av {

      class AV_EXPORT Format {
      public:

        enum FormatType {
          FORMAT_UNKNOWN,
          FORMAT_VIDEO,
          FORMAT_AUDIO
        };
        Format();
        Format(FormatType);
        Format(CodecID);
        FormatType getType()const;
        void setFormatOption(std::string key, std::string val);
        FormatType _type;
        PixelFormat pixel_format;
        int width;
        int height;
        int channels;
        int samplerate;
        SampleFormat sampleformat;
        protected:
        std::map<std::string, std::string> _options;
        CodecID _codec_id;

      };
    }
  }
}

#endif	/* FORMAT_H */

