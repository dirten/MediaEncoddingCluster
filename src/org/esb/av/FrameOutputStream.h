#ifndef ORG_ESB_AV_FRAMEOUTPUTSTREAM_H
#define ORG_ESB_AV_FRAMEOUTPUTSTREAM_H
#include "org/esb/io/OutputStream.h"
#include "Frame.h"
using namespace org::esb::io;
namespace org{
    namespace esb{
        namespace av{
            class FrameOutputStream: public OutputStream{
                public:
                    FrameOutputStream(OutputStream * out);
                    ~FrameOutputStream();
                    void writeFrame(Frame * frame);
                    void write(char * buffer, int length);
                    void write(vector<unsigned char>&buffer);
                private:
                    OutputStream * outStream;
            };
        }
    }
}

#endif

