/* 
 * File:   AudioFormat.h
 * Author: jholscher
 *
 * Created on 29. August 2010, 19:29
 */

#ifndef AUDIOFORMAT_H
#define	AUDIOFORMAT_H
#include "AV.h"
#include "Format.h"
namespace org {
  namespace esb {
    namespace av {

      class AudioFormat:public Format {
      public:
        AudioFormat(AVSampleFormat fmt,int samplerate, int sampleSizeInBits, int channels, int signedd, int frameSizeInBits);
        AudioFormat(AVCodecContext*);
        AudioFormat(const Format& orig);
        bool operator==(const AudioFormat&);
        AVSampleFormat getSampleFormat();
        int getSampleRate();
        int getSampleSizeInBits();
        int getChannels();
        int getSigned();
        int getFrameSizeInBits();

      private:

      };
    }
  }
}

#endif	/* AUDIOFORMAT_H */

