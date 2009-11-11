#include "FormatBaseStream.h"

#include "AV.h"

#include <iostream>
#include "org/esb/util/Log.h"

namespace org {
  namespace esb {
    namespace av {

      bool isInitialized = false;

      FormatBaseStream::FormatBaseStream() {
        initialize();
      }

      void FormatBaseStream::initialize() {
        if (!isInitialized) {
          logdebug("Init ffmpeg Libraries");
          /* register all codecs, demux and protocols */
          avcodec_register_all();
          av_register_all();
//          av_log_set_level(AV_LOG_DEBUG);

          isInitialized = true;
        }
      }
    }
  }
}


