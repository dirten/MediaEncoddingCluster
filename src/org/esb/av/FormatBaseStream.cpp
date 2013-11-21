#include "FormatBaseStream.h"

#include "AV.h"

#include <iostream>
#include <list>
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"
//extern URLProtocol test_protocol;
namespace org {
  namespace esb {
    namespace av {

      bool isInitialized = false;
      //int av_log_level = AV_LOG_INFO;

      void mhive_log_default_callback(void* ptr, int level, const char* fmt, va_list vl) {
        return;
        static int print_prefix = 1;
        static int count;
        static char line[4096], prev[4096];
        char ptrString[10];
        char ptrLine[4096];
        AVClass* avc = ptr ? *(AVClass**) ptr : NULL;
        //if (level > av_log_level)
        //  return;
#undef fprintf
#ifdef __WIN32__
#define snprintf _snprintf
#endif
        if (print_prefix && avc) {
          snprintf(line, sizeof (line), "[%s @ %p]", avc->item_name(ptr), ptr);
          snprintf(ptrString, sizeof (ptrString), "%p", ptr);
        } else
          line[0] = 0;
        ptrLine[0] = 0;

        vsnprintf(line + strlen(line), sizeof (line) - strlen(line), fmt, vl);
        vsnprintf(ptrLine, sizeof (ptrLine), fmt, vl);
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
        std::string msg = org::esb::util::StringUtil::trim(line, "\n");
        std::string msgPtr = org::esb::util::StringUtil::trim(ptrLine, "\n");
        //if(logMap.count(ptrString)>MAX_HISTORY){
        //  logMap.erase(--logMap.end());
        //}
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

        //        logerror(msg);
        //    fputs(line, stderr);
        //        strcpy(prev, line);
      }
      std::map<std::string, std::list<std::string> > FormatBaseStream::logMap;

      void FormatBaseStream::mhive_log_default_callback(void* ptr, int level, const char* fmt, va_list vl) {
        static int print_prefix = 1;
        static int count;
        static char line[4096], prev[4096];
        char ptrString[10];
        char ptrLine[4096];
        AVClass* avc = ptr ? *(AVClass**) ptr : NULL;
        //if (level > av_log_level)
        //  return;
#undef fprintf
#ifdef __WIN32__
#define snprintf _snprintf
#endif
        if (print_prefix && avc) {
          snprintf(line, sizeof (line), "[%s @ %p]", avc->item_name(ptr), ptr);
          snprintf(ptrString, sizeof (ptrString), "%p", ptr);
        } else {
          line[0] = 0;
          ptrString[0] = 0;
        }

        vsnprintf(line + strlen(line), sizeof (line) - strlen(line), fmt, vl);
        //vsnprintf(ptrLine, sizeof (ptrLine), fmt, vl);
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
        std::string msg = org::esb::util::StringUtil::trim(line, "\n");
        std::string msgPtr = org::esb::util::StringUtil::trim(line, "\n");
        if (logMap.count(ptrString)) {
          if (logMap[ptrString].size() > MAX_HISTORY) {
            logMap[ptrString].erase(--logMap[ptrString].end());
          }
        }
        logMap[ptrString].push_front(msgPtr);

        /*filter out unwnated logging by loglevel*/
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

        //        logerror(msg);
        //    fputs(line, stderr);
        //        strcpy(prev, line);

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
          avformat_network_init();
          //av_log_set_callback(mhive_log_default_callback);
          av_log_set_callback(FormatBaseStream::mhive_log_default_callback);
          av_log_set_level(AV_LOG_INFO);
          //av_register_protocol2(&test_protocol, sizeof (URLProtocol));
          av_lockmgr_register(lockmgr);
          isInitialized = true;
        }
      }
    }
  }
}


