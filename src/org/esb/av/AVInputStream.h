#ifndef ORG_ESB_AV_AVINPUTSTREAM_H
#define ORG_ESB_AV_AVINPUTSTREAM_H

#include "FormatInputStream.h"


#include "Codec.h"
#include "Frame.h"
#include "avformat.h"




namespace org {
    namespace esb {
        namespace av {
            class AVInputStream {
public:
                AVInputStream(FormatInputStream * in, int index=0);
                AVInputStream(AVFormatContext * context, int streamIndex);
                void selectStreamIndex(int index);
                int getStreamIndex();

                Codec * getCodec();
                long getDuration();
                double getTimeBase();
                long getNumberFrames();
                AVPacket * getNextPacket();
                Frame * getNextFrame();
                Frame * getFrame(int frameIndex);
private:
                FormatInputStream * _fis;
                AVStream * _avStream;
                AVFormatContext * _formatContext;
                AVCodecContext * _codecContext;
                AVCodec * _codec;
                int _streamIndex;
            };
        }
    }
}

#endif

