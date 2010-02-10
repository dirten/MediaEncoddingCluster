#ifndef ORG_ESB_IO_FORMATBASESTREAM_H
#define ORG_ESB_IO_FORMATBASESTREAM_H
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace av {

      class FormatBaseStream {
        classlogger("org.esb.av.FormatBaseStream")
      public:
        FormatBaseStream();
        static void initialize();
      };
    }
  }
}
#endif
