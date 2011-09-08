#ifndef ORG_ESB_IO_FORMATBASESTREAM_H
#define ORG_ESB_IO_FORMATBASESTREAM_H
#include "org/esb/util/Log.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace av {

      class AV_EXPORT FormatBaseStream {
        classlogger("org.esb.av.FormatBaseStream")
      public:
        FormatBaseStream();
        static void initialize();
      };
    }
  }
}
#endif
