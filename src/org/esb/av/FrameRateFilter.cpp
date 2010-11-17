/* 
 * File:   FrameRateFilter.cpp
 * Author: jholscher
 * 
 * Created on 17. November 2010, 21:22
 */

#include "FrameRateFilter.h"
namespace org {
  namespace esb {
    namespace av {

      FrameRateFilter::FrameRateFilter(Rational src, Rational trg):_src(src), _trg(trg) {

      }


      FrameRateFilter::~FrameRateFilter() {
      }

      void FrameRateFilter::close() {
      }

      std::string FrameRateFilter::getName() {
      }

      void FrameRateFilter::open() {
      }

      void FrameRateFilter::reset() {
      }

      void FrameRateFilter::setParameter(std::map<std::string, std::string>) {
      }

      PlugIn::ProcessStatus FrameRateFilter::process(Frame & in, Frame & out) {
      }

    }
  }
}
