/* 
 * File:   StreamInfo.h
 * Author: jhoelscher
 *
 * Created on 15. Juni 2009, 11:31
 */
#include "AV.h"
#ifndef _STREAMINFO_H
#define	_STREAMINFO_H
namespace org {
  namespace esb {
    namespace av {

      class StreamInfo {
      public:
        StreamInfo(AVStream*, int idx);
        int getIndex();
        int getCodecType();
        int getCodecId();
        int getCodecBitrate();
        AVRational getFrameRate();
//        AVRational getCodecFrameRate();
        int getCodecSampleRate();
        AVRational getTimeBase();
        AVRational getCodecTimeBase();
        int getWidth();
        int getheight();
        int getChannels();
        int getGOPSize();
        int getPixelFormat();
        int getSampleFormat();
        int getDuration();
        int getQuality();
        long long int getFirstPts();
        long long int getFirstDts();
      private:
        AVStream * _s;
        int _idx;
      };
    }
  }
}

#endif	/* _STREAMINFO_H */
