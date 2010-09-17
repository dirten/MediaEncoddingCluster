#ifndef ORG_ESB_HIVE_DIRECTORYSCANNER_H
#define ORG_ESB_HIVE_DIRECTORYSCANNER_H
#include "org/esb/signal/MessageListener.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileFilter.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include "org/esb/hive/FileImporter.h"
#include "org/esb/util/Log.h"
#include <string>

namespace org {
    namespace esb {
       
        namespace hive {

            class DirectoryScanner : public org::esb::signal::MessageListener {
              classlogger("org.esb.hive.DirectoryScanner")
            public:
                DirectoryScanner(std::string dir, int interval);
                DirectoryScanner();
                ~DirectoryScanner();
                void onMessage(org::esb::signal::Message & msg);
                void scan();

            private:
//                void scan();
                void scan(std::string indir, std::string outdir, int profile, io::FileFilter&);
                void computeFile(org::esb::io::File & file, int profile, std::string);
                bool _halt;
                boost::thread * th;
                int _interval;
                int _level;
                boost::mutex terminationMutex;
                boost::condition termination_wait;
                FileImporter _importer;
            };
        }
    }
}
#endif

