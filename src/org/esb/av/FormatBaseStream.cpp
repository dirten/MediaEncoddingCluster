#include "FormatBaseStream.h"

#include "AV.h"

#include <iostream>
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace av {

      bool isInitialized = false;
      int av_log_level = AV_LOG_INFO;

      void mhive_log_default_callback(void* ptr, int level, const char* fmt, va_list vl) {
        static int print_prefix = 1;
        static int count;
        static char line[1024], prev[1024];
        AVClass* avc = ptr ? *(AVClass**) ptr : NULL;
/*        if (level > av_log_level)
          return;*/
#undef fprintf
        if (print_prefix && avc) {
          snprintf(line, sizeof (line), "[%s @ %p]", avc->item_name(ptr), ptr);
        } else
          line[0] = 0;

        vsnprintf(line + strlen(line), sizeof (line) - strlen(line), fmt, vl);
/*
        print_prefix = line[strlen(line) - 1] == '\n';
        if (print_prefix && !strcmp(line, prev)) {
          count++;
          return;
        }
        if (count > 0) {
          fprintf(stderr, "    Last message repeated %d times\n", count);
          count = 0;
        }*/
        std::string msg=org::esb::util::StringUtil::trim(line,"\n");
        switch (level) {
          case AV_LOG_DEBUG:
            LOGDEBUG("org.esb.av.FormatBaseStream",msg);
            break;
          case AV_LOG_INFO:
            LOGINFO("org.esb.av.FormatBaseStream",msg);
            break;
          case AV_LOG_ERROR:
            LOGERROR("org.esb.av.FormatBaseStream",msg);
            break;
          case AV_LOG_WARNING:
            LOGWARN("org.esb.av.FormatBaseStream",msg);
            break;
          case AV_LOG_PANIC:
            LOGFATAL("org.esb.av.FormatBaseStream",msg);
            break;
          case AV_LOG_VERBOSE:
            LOGTRACE("org.esb.av.FormatBaseStream",msg);
            break;
          default:
            LOGERROR("org.esb.av.FormatBaseStream","Unknown LogLevel:"<<level<<" - "<<msg);
            break;
        }

//        logerror(msg);
        //    fputs(line, stderr);
//        strcpy(prev, line);
      }

      FormatBaseStream::FormatBaseStream() {
        initialize();
      }

      void FormatBaseStream::initialize() {
        if (!isInitialized) {
          LOGDEBUG("org.esb.av.FormatBaseStream","Init ffmpeg Libraries");
          /* register all codecs, demux and protocols */
          avcodec_register_all();
          av_register_all();
          av_log_set_callback(mhive_log_default_callback);
          av_log_set_level(AV_LOG_DEBUG);

          isInitialized = true;
        }
      }
    }
  }
}


