#ifndef ORG_ESB_AV_AVINPUTSTREAM_H
#define ORG_ESB_AV_AVINPUTSTREAM_H
#include "org/esb/io/InputStream.h"
#include "FormatInputStream.h"



#include "Codec.h"
#include "Frame.h"
#include "avformat.h"
#include "Packet.h"



namespace org {
    namespace esb {
        namespace av {
            class AVInputStream
            //:public InputStream 
            {
public:
/**
* Creates a new <code>AVInputStream</code> instance by using the given
* FormatInputStream and the StreamIndex from the FormatInputStream.  
* 
* 
*
* @param   in  FormatInputStream to read from
* @param   index  StreamIndex to read (default is 0)
* @throws  NullPointerException
*          If the <code>in</code> argument is <code>null</code>
*/
		AVInputStream(FormatInputStream * in, int index=0);
                AVInputStream(AVFormatContext * context, int streamIndex);
                ~AVInputStream();
                void selectStreamIndex(int index);
                int getStreamIndex();
                Codec * getCodec();
                long getDuration();
                double getTimeBase();
                long getNumberFrames();
                Packet * readPacket();
                AVPacket getNextPacket();
                Packet * getNextPacket2();
                Frame * getNextFrame();
                Frame * getFrame(int frameIndex);
private:
                FormatInputStream * _fis;
                AVStream * _avStream;
                AVFormatContext * _formatContext;
                AVCodecContext * _codecContext;
                AVCodec * _codec;
                Codec * _intCodec;
                int _streamIndex;
            };
        }
    }
}

#endif

