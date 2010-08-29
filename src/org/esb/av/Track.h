/* 
 * File:   Track.h
 * Author: jholscher
 *
 * Created on 29. August 2010, 20:05
 */

#ifndef TRACK_H
#define	TRACK_H
#include "Format.h"
#include "TimeStamp.h"
#include "Buffer.h"
namespace org {
  namespace esb {
    namespace av {

      class Track {
      public:
        virtual ~Track();
        Format getFormat();
        TimeStamp getStartTime();
        bool isEnabled();
        void readFrame(Buffer);
        void setEnabled(bool);
      private:
        Track(AVStream * str);
        AVStream * _str;
      };
    }
  }
}

#endif	/* TRACK_H */

