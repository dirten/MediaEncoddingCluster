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
      class Demultiplexer;
      class Track {
      public:
        virtual ~Track();
        Format getFormat();
        TimeStamp getStartTime();
        bool isEnabled();
        void readFrame(Buffer&);
        void setEnabled(bool);
        int getStreamIndex();
      private:
        friend class Demultiplexer;
        Track(AVStream * str, Demultiplexer *);
        AVStream * _str;
        Demultiplexer * _plexer;
        bool _enabled;
        int64_t _seq;
      };
    }
  }
}

#endif	/* TRACK_H */

