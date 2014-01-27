#ifndef ORG_ESB_IO_FORMATBASESTREAM_H
#define ORG_ESB_IO_FORMATBASESTREAM_H
#include "org/esb/util/Log.h"
#include "exports.h"
#include <map>
#include <list>
namespace org {
  namespace esb {
    namespace av {

      class AV_EXPORT FormatBaseStream {
        classlogger("org.esb.av.FormatBaseStream")
      public:
        static void initialize();
        static std::list<std::string> getLastAvMessage(void * ptr);
      protected:
        FormatBaseStream();

      private:
        static void mhive_log_default_callback(void* ptr, int level, const char* fmt, va_list vl);
        static std::map<std::string, std::list<std::string> > logMap;
        static const int MAX_HISTORY=10;
      };
    }
  }
}
#endif
