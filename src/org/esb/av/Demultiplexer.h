/* 
 * File:   Demultiplexer.h
 * Author: jholscher
 *
 * Created on 29. August 2010, 20:01
 */

#ifndef DEMULTIPLEXER_H
#define	DEMULTIPLEXER_H
#include "Duration.h"
#include "Track.h"
#include "DataSource.h"
#include <list>
#include "org/esb/lang/Ptr.h"
#include "boost/thread/condition.hpp"
#include "boost/thread/mutex.hpp"
#include "exports.h"
namespace org {
  namespace esb {
    namespace av {

      class AV_EXPORT Demultiplexer {
      public:
        Demultiplexer(DataSource& );
        virtual ~Demultiplexer();
        Duration getDuration();
        TimeStamp getMediaTime();
        std::list<Ptr<Track> > getTracks();
        bool isPositionable();
        bool isRandomAccess();
        TimeStamp setPosition(TimeStamp, int);
      private:
        DataSource & _source;
        std::list<Ptr<Track> > _track_list;
        friend class Track;
        void readFrame(Buffer&, int );
        Ptr<AVPacket>_next_packet;
        boost::mutex _read_frame_mutex;
        boost::mutex _next_packet_wait_mutex;
        boost::condition _next_packet_wait_condition;
        bool readFrameFromContext();
        bool _end_of_media;
      };
    }
  }
}

#endif	/* DEMULTIPLEXER_H */

