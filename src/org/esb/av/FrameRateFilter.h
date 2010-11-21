/* 
 * File:   FrameRateFilter.h
 * Author: jholscher
 *
 * Created on 17. November 2010, 21:22
 */

#ifndef FRAMERATEFILTER_H
#define	FRAMERATEFILTER_H
#include "PlugIn.h"
#include "Rational.h"
namespace org {
  namespace esb {
    namespace av {

      class FrameRateFilter: public PlugIn {
      public:
        FrameRateFilter(Rational src, Rational trg);
        virtual ~FrameRateFilter();
        void close();
        std::string getName();
        void open();
        void reset();
        void setParameter(std::map<std::string, std::string>);
        void setCompensateBase(double);
        ProcessStatus process(Frame & in, Frame & out);
      private:
        Rational _src;
        Rational _trg;
        double inframes;
        int64_t outframes;
        double coutframes;
        int64_t duplicatedframes;
        double _frameRateCompensateBase;
        int64_t _prev_time;
        double sync_out_pts;
      };
    }
  }
}

#endif	/* FRAMERATEFILTER_H */

