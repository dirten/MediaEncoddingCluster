/* 
 * File:   FrameRateFilter.cpp
 * Author: jholscher
 * 
 * Created on 17. November 2010, 21:22
 */

#include "FrameRateFilter.h"
#include "TimeStamp.h"
namespace org {
  namespace esb {
    namespace av {

      FrameRateFilter::FrameRateFilter(Rational src, Rational trg) : _src(src), _trg(trg) {
        inframes = 0;
        outframes = 0;
        coutframes = 1;
        duplicatedframes = 0;
        _frameRateCompensateBase = 0.0;
        _prev_time = 0;
        sync_out_pts = 1;
      }

      FrameRateFilter::~FrameRateFilter() {
      }

      void FrameRateFilter::close() {
      }

      std::string FrameRateFilter::getName() {
        return "";
      }

      void FrameRateFilter::open() {
      }

      void FrameRateFilter::reset() {
      }

      void FrameRateFilter::setCompensateBase(double val) {
        _frameRateCompensateBase = val;
      }

      void FrameRateFilter::setParameter(std::map<std::string, std::string> param) {
        if (param.count("frameratecompensate") > 0) {
          sscanf(param["frameratecompensate"].c_str(), "%lf", &_frameRateCompensateBase);
        }
      }

      PlugIn::ProcessStatus FrameRateFilter::process(Frame & in, Frame & out) {
        if (_src != _trg) {
          //LOGDEBUG("Process FrameRateFilter");
          out.setFrameCount(0);

          inframes++;
          double cts = ((((inframes / _src.den) * _src.num) / _trg.num) * _trg.den);
          cts += _frameRateCompensateBase;
          int64_t fr = cts;
          //LOGDEBUG("cts=" << cts << " fr=" << fr << " outframes=" << outframes);
          if (TimeStamp(1, _src) < TimeStamp(1, _trg)) {
            if (fr - outframes > 0) {
              out.setFrameCount(1);
              outframes++;
            }
          } else
            if (TimeStamp(1, _src) > TimeStamp(1, _trg)) {
            out.setFrameCount(1);
            if (fr - outframes > 1.1) {
              int frc = fr - outframes;
              //LOGDEBUG("frc=" << frc);
              out.setFrameCount(frc);
            }
            outframes += out.getFrameCount();
          }
        }
        return PlugIn::BUFFER_PROCESSED_OK;
      }
    }
  }
}
