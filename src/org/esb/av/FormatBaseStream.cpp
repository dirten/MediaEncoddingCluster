#include "FormatBaseStream.h"

#include "AV.h"

#include <iostream>
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Log.h"
extern URLProtocol test_protocol;
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
        if (level > av_log_level)
          return;
#undef fprintf
#ifdef __WIN32__
#define snprintf _snprintf
#endif
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
            LOGDEBUG(msg);
            break;
          case AV_LOG_INFO:
            LOGINFO(msg);
            break;
          case AV_LOG_ERROR:
            LOGERROR(msg);
            break;
          case AV_LOG_WARNING:
            LOGWARN(msg);
            break;
          case AV_LOG_PANIC:
            LOGFATAL(msg);
            break;
          case AV_LOG_VERBOSE:
            LOGTRACE(msg);
            break;
          default:
            LOGERROR("Unknown LogLevel:"<<level<<" - "<<msg);
            break;
        }

//        logerror(msg);
        //    fputs(line, stderr);
//        strcpy(prev, line);
      }

      FormatBaseStream::FormatBaseStream() {
        initialize();
      }
	static int lockmgr(void **mtx, enum AVLockOp op){
	    switch(op){
		case AV_LOCK_CREATE:
		    *mtx=new boost::mutex();
		    if(!*mtx)
			return 1;
		    break;
		case AV_LOCK_OBTAIN:
		    static_cast<boost::mutex*>(*mtx)->lock();
		    break;
		case AV_LOCK_RELEASE:
		    static_cast<boost::mutex*>(*mtx)->unlock();
		    break;
		case AV_LOCK_DESTROY:
		    delete static_cast<boost::mutex*>(*mtx);
		    break;
	    }
	    return 0;
	}
      void FormatBaseStream::initialize() {
        if (!isInitialized) {
          LOGDEBUG("Init ffmpeg Libraries");
          /* register all codecs, demux and protocols */
          avcodec_register_all();
          av_register_all();
          av_log_set_callback(mhive_log_default_callback);
          av_log_set_level(AV_LOG_DEBUG);
          av_register_protocol2(&test_protocol, sizeof(URLProtocol));
          av_lockmgr_register(lockmgr);
          isInitialized = true;
        }
      }
    }
  }
}


