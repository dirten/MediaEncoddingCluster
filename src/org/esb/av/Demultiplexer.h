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
namespace org {
  namespace esb {
    namespace av {

      class Demultiplexer {
      public:
        Demultiplexer(DataSource& );
        virtual ~Demultiplexer();
        Duration getDuration();
        TimeStamp getMediaTime();
        std::list<Track> getTracks();
        bool isPositionable();
        bool isRandomAccess();
        TimeStamp setPosition(TimeStamp, int);
      private:
        DataSource & _source;
      };
    }
  }
}

#endif	/* DEMULTIPLEXER_H */

