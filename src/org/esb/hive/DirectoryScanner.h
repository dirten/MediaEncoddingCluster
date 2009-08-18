#ifndef ORG_ESB_HIVE_DIRECTORYSCANNER_H
#define ORG_ESB_HIVE_DIRECTORYSCANNER_H
#include "org/esb/signal/MessageListener.h"
#include "org/esb/io/File.h"
#include <boost/thread.hpp>

#include <string>

namespace org {
  namespace esb {
    namespace hive {
        const static std::string RESTART="restart";
        const static std::string STOP="stop";
        const static std::string START="start";

      class DirectoryScanner : public org::esb::signal::MessageListener {
      public:
        DirectoryScanner(std::string dir, int interval);
        DirectoryScanner();
        ~DirectoryScanner();
        void onMessage(org::esb::signal::Message & msg);
      private:
        void scan();
        void scan(std::string indir,std::string outdir,int profile,std::string);
        void computeFile(org::esb::io::File & file, int profile, std::string outpath);
        bool _halt;
        boost::thread * th;
        int _interval;
        int _level;
      };
    }
  }
}
#endif

