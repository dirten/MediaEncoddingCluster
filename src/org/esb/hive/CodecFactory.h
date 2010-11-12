#ifndef ORG_ESB_HIVE_CODECFACTORY_H
#define ORG_ESB_HIVE_CODECFACTORY_H

#include "org/esb/db/hivedb.hpp"

#include <map>
#include <boost/shared_ptr.hpp>
#include "org/esb/util/Log.h"

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
              classlogger("org.esb.hive.CodecFactory")
            public:
                static boost::shared_ptr<org::esb::av::Decoder> getStreamDecoder(int streamid);
                static boost::shared_ptr<org::esb::av::Encoder> getStreamEncoder(int streamid);
                static boost::shared_ptr<org::esb::av::Encoder> getStreamEncoder(std::multimap<std::string, std::string>);
                static void clearCodec(int streamid);
                static void free();
                static void setCodecOptions(boost::shared_ptr<org::esb::av::Encoder>,std::string options);
                static void setCodecOptions(boost::shared_ptr<org::esb::av::Encoder>,std::vector<db::StreamParameter>p);
                static void setCodecOptions(boost::shared_ptr<org::esb::av::Encoder>,std::multimap<std::string, std::string>p);
                static void setCodecOptions(boost::shared_ptr<org::esb::av::Decoder>,std::multimap<std::string, std::string>);
            private:
                CodecFactory();
                static std::map<int, boost::shared_ptr<org::esb::av::Decoder> > decoder_map;
                static std::map<int, boost::shared_ptr<org::esb::av::Encoder> > encoder_map;
//                static void setCodecOptions(boost::shared_ptr<org::esb::av::Encoder>,std::string options);

            };
        }
    }
}
#endif

