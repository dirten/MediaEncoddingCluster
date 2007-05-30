#include "Codec.h"
#include "Frame.h"
#include "avformat.h"




namespace org{
    namespace esb{
        namespace av{
            class AVInputStream{
                public:
                    AVInputStream(AVStream * stream);
                    Codec * getCodec();
                    long getDuration();
                    float getTimeBase();
                    long getNumberFrames();
                    Frame * getNextFrame();
                    Frame * getFrame(int frameIndex);
                private:
                    AVStream * _avStream;
            };
        }
    }
}

