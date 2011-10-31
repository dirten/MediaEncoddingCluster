/* 
 * File:   TimeStamp.h
 * Author: jholscher
 *
 * Created on 29. August 2010, 16:45
 */

#ifndef TIMESTAMP_H
#define	TIMESTAMP_H
#include "Rational.h"
#include <boost/serialization/serialization.hpp>
#include "exports.h"
namespace org {
  namespace esb {
    namespace av {

      class AV_EXPORT TimeStamp {
      public:
        TimeStamp(int64_t timestamp=1, Rational base=Rational());
        virtual ~TimeStamp();
        TimeStamp rescaleTo(Rational );
        Rational getTimeBase();
        int64_t getTime();
        double toDouble();
        bool operator==(TimeStamp t)const;
        bool operator!=(TimeStamp t)const;
        bool operator>(TimeStamp t)const;
        bool operator<(TimeStamp t)const;
        friend std::ostream& operator<<(std::ostream&out,TimeStamp & t);
        std::string toString();
      private:
        int64_t _timestamp;
        Rational _timebase;
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
          ar & _timestamp;
          ar & _timebase;
        }

      };
    }
  }
}

#endif	/* TIMESTAMP_H */

