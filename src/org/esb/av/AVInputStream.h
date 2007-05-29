#include "Codec.h"
#include "avformat.h"




namespace org{
    namespace esb{
        namespace av{
            class AVInputStream{
                public:
                    AVInputStream(AVStream * stream);
                    Codec * getCodec();
                private:
                    AVStream * _avStream;
            };
        }
    }
}

