/* 
 * File:   Buffer.h
 * Author: jholscher
 *
 * Created on 29. August 2010, 17:37
 */

#ifndef BUFFER_H
#define	BUFFER_H
#include "Format.h"
#include "TimeStamp.h"
#include "Duration.h"
#include <stdint.h>
#include "org/esb/lang/Ptr.h"
namespace org {
  namespace esb {
    namespace av {

      class Buffer {
      public:
        Buffer();
        virtual ~Buffer();
        uint8_t * getData();
        Ptr<AVPacket> getAVPacket();
        Duration getDuration();
        int getFlags();
        const Format getFormat();
        int getLength();
        int64_t getSequenceNumber();
        TimeStamp getTimeStamp();
        bool isDiscard();
        bool isEOM();

        void setData(uint8_t*);
        void setAVPacket(Ptr<AVPacket>);
        void setDiscard(bool);
        void setDuration(Duration);
        void setEOM(bool);
        void setFlags(int);
        void setFormat(Format);
        void setLength(int);
        void setSequenceNumber(int64_t);
        void setTimeStamp(TimeStamp);
      private:
        uint8_t * _data;
        Ptr<AVPacket> _packet;
        bool _discard;
        Duration _duration;
        bool _eom;
        int _flags;
        Format _format;
        int _length;
        int64_t _sequenceNumber;
        TimeStamp _timeStamp;
      };
    }
  }
}

#endif	/* BUFFER_H */

