/* 
 * File:   VideoFormat.h
 * Author: jholscher
 *
 * Created on 29. August 2010, 16:24
 */

#ifndef VIDEOFORMAT_H
#define	VIDEOFORMAT_H
#include "AV.h"
#include "Rational.h"
#include "Format.h"
namespace org {
  namespace esb {
    namespace av {

      class VideoFormat: public Format {
      public:
        VideoFormat(std::string encoding);
        VideoFormat(PixelFormat, int width, int height, Rational framerate);
        VideoFormat(const Format&);
        bool operator==(VideoFormat t);
        std::string toString();
        PixelFormat getPixelFormat();
        int getWidth();
        int getHeight();
        void setFormatOption(std::string key, std::string val);
      private:
        friend class Track;
        /*this is only for internal usage*/
        VideoFormat(AVCodecContext * ctx);
        std::map<std::string, std::string> _options;

      };
    }
  }
}

#endif	/* VIDEOFORMAT_H */

