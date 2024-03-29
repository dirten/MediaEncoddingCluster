/* 
 * File:   StreamInfo.h
 * Author: jhoelscher
 *
 * Created on 15. Juni 2009, 11:31
 */
#include "AV.h"
#include "exports.h"
#ifndef _STREAMINFO_H
#define	_STREAMINFO_H
namespace org {
  namespace esb {
    namespace av {

      class AV_EXPORT StreamInfo {
      public:
        StreamInfo(AVStream*, int idx);
        int getIndex();
        AVMediaType getCodecType();
        CodecID getCodecId();
        AVCodecContext * getCodec();
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
        int64_t getDuration();
		int getFrameBytes();
        //float getQuality();
        long long int getFirstPts();
        long long int getFirstDts();
		int getNBIndexEntries();
      private:
        AVStream * _s;
        int _idx;
      };
    }
  }
}

#endif	/* _STREAMINFO_H */

