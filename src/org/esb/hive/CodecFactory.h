#ifndef ORG_ESB_HIVE_CODECFACTORY_H
#define ORG_ESB_HIVE_CODECFACTORY_H


#include <map>
#include <boost/shared_ptr.hpp>

namespace org {
    namespace esb {
        namespace av {
            class Decoder;
            class Encoder;
            class Codec;
        }
    }
}
namespace org {
    namespace esb {
        namespace hive {

            class CodecFactory {
            public:
                static boost::shared_ptr<org::esb::av::Decoder> getStreamDecoder(int streamid);
                static boost::shared_ptr<org::esb::av::Encoder> getStreamEncoder(int streamid);
                static void clearCodec(int streamid);
                static void free();
            private:
                CodecFactory();
                static std::map<int, boost::shared_ptr<org::esb::av::Decoder> > decoder_map;
                static std::map<int, boost::shared_ptr<org::esb::av::Encoder> > encoder_map;

            };
        }
    }
}
#endif

