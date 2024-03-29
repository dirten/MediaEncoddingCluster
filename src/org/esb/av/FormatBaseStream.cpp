#include "FormatBaseStream.h"

#include "AV.h"

#include <iostream>
#include <list>
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"

namespace org {
  namespace esb {
    namespace av {
      boost::mutex FormatBaseStream::log_mutex;
      bool FormatBaseStream::isInitialized=false;
      std::map<std::string, std::list<std::string> > FormatBaseStream::logMap;

      void FormatBaseStream::mhive_log_default_callback(void* ptr, int level, const char* fmt, va_list vl) {
        boost::mutex::scoped_lock log_lock(log_mutex);

        static int print_prefix = 1;
        static int count;
        static char line[4096], prev[4096];
        //char ptrString[10];
        //char ptrLine[4096];
        AVClass* avc = ptr ? *(AVClass**) ptr : NULL;
#undef fprintf
#ifdef __WIN32__
#define snprintf _snprintf
#endif
        if (print_prefix && avc) {
          snprintf(line, sizeof (line), "[%s @ %p]", avc->item_name(ptr), ptr);
          //snprintf(ptrString, sizeof (ptrString), "%p", ptr);
        } else {
          line[0] = 0;
          //ptrString[0] = 0;
          return;
        }

        vsnprintf(line + strlen(line), sizeof (line) - strlen(line), fmt, vl);
        std::string msg = org::esb::util::StringUtil::trim(line, "\n");
        //std::string msgPtr = org::esb::util::StringUtil::trim(line, "\n");
        /*
        if (logMap.count(ptrString)) {
          if (logMap[ptrString].size() > MAX_HISTORY) {
            logMap[ptrString].erase(--logMap[ptrString].end());
          }
        }
        logMap[ptrString].push_front(msgPtr);
        */
        /*filter out unwanted messages by loglevel*/
        try{
        if(level>av_log_get_level())return;
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
          LOGERROR("Unknown LogLevel:" << level << " - " << msg);
          break;
        }
        }catch(Poco::AssertionViolationException & ex){
          std::cout << "error logging"<< ex.displayText() <<std::endl;
        //LOGERROR("error in logging")
      }
      }

      std::list<std::string> FormatBaseStream::getLastAvMessage(void * ptr) {
        char ptrString[10];
        snprintf(ptrString, sizeof (ptrString), "%p", ptr);
        return logMap[ptrString];
      }

      FormatBaseStream::FormatBaseStream() {
        initialize();
      }

      static int lockmgr(void **mtx, enum AVLockOp op) {
        switch (op) {
        case AV_LOCK_CREATE:
          *mtx = new boost::mutex();
          if (!*mtx)
            return 1;
          break;
        case AV_LOCK_OBTAIN:
          static_cast<boost::mutex*> (*mtx)->lock();
          break;
        case AV_LOCK_RELEASE:
          static_cast<boost::mutex*> (*mtx)->unlock();
          break;
        case AV_LOCK_DESTROY:
          delete static_cast<boost::mutex*> (*mtx);
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
          avfilter_register_all();
          avformat_network_init();

          /*setup own logging callback*/
          av_log_set_callback(FormatBaseStream::mhive_log_default_callback);
          av_log_set_level(AV_LOG_INFO);

          /* install my own lock manager
           * this is needed for multithreaded environment
           */
          av_lockmgr_register(lockmgr);

          isInitialized = true;
        }
      }
      }
    }
  }


