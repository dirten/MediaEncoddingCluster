#ifndef ORG_ESB_HIVE_FORMATSTREAMFACTORY
#define ORG_ESB_HIVE_FORMATSTREAMFACTORY
#include <string>
#include <map>
namespace org {
  namespace esb {
    namespace av {
      class FormatInputStream;
    }
    namespace hive {

      class FormatStreamFactory {
      public:
        static org::esb::av::FormatInputStream * getInputStream(std::string filename);
      private:
        static std::map<std::string, org::esb::av::FormatInputStream*> _inputStreams;
      };

    }
  }
}
#endif

