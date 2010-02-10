#ifndef ORG_ESB_HIVE_DIRECTORYSCANNER_H
#define ORG_ESB_HIVE_DIRECTORYSCANNER_H
#include "org/esb/signal/MessageListener.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileFilter.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

#include <string>

namespace org {
    namespace esb {
        namespace sql {
            class Connection;
            class PreparedStatement;
            class Statement;
        }
        namespace hive {

            class DirectoryScanner : public org::esb::signal::MessageListener {
              classlogger("org.esb.hive.DirectoryScanner")
            public:
                DirectoryScanner(std::string dir, int interval);
                DirectoryScanner();
                ~DirectoryScanner();
                void onMessage(org::esb::signal::Message & msg);
            private:
                void scan();
                void scan(std::string indir, std::string outdir, int profile, io::FileFilter&);
                void computeFile(org::esb::io::File & file, int profile, std::string);
                bool _halt;
                boost::thread * th;
                int _interval;
                int _level;
                sql::Connection * _con;
                sql::PreparedStatement * _stmt;
                sql::Connection * _con2;
                sql::Statement * _stmt2;
                boost::mutex terminationMutex;
                boost::condition termination_wait;

            };
        }
    }
}
#endif

