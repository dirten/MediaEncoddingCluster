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
        VideoFormat(PixelFormat pxfm, int width, int height, Rational framerate);
        VideoFormat(const Format&);
        bool operator==(VideoFormat t);
        std::string toString();
        PixelFormat getPixelFormat();
        int getWidth();
        int getHeight();
      private:
      };
    }
  }
}

#endif	/* VIDEOFORMAT_H */

