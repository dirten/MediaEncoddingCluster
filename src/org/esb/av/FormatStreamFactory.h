#ifndef ORG_ESB_HIVE_FORMATSTREAMFACTORY
#define ORG_ESB_HIVE_FORMATSTREAMFACTORY
#include <string>
#include <map>
#include "org/esb/util/Log.h"

namespace org {
  namespace esb {
    namespace av {
      class FormatInputStream;


      class FormatStreamFactory {
        classlogger("org.esb.av.FormatStreamFactory")
      public:
        static org::esb::av::FormatInputStream * getInputStream(std::string & filename, long long int offset=-1);
      private:
        static std::map<std::string, org::esb::av::FormatInputStream*> _inputStreams;
      };

    }
  }
}
#endif

