#include "ExportScanner.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/io/File.h"
#include "org/esb/hive/FileExporter.h"
#include "org/esb/config/config.h"
#include "org/esb/lang/Thread.h"

namespace org {
  namespace esb {
    namespace hive {

      void ExportScanner::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty("exportscanner") == "start") {
          _run = true;
          boost::thread t(boost::bind(&ExportScanner::start, this));
        } else
          if (msg.getProperty("exportscanner") == "stop") {
          _run = false;

        }
      }

      void ExportScanner::start() {
        org::esb::sql::Connection con(org::esb::config::Config::getProperty("db.connection"));
        org::esb::sql::PreparedStatement stmt = con.prepareStatement("SELECT files.id, filename, path FROM jobs, files WHERE outputfile=files.id and complete is not null;");
        while (_run) {
          org::esb::sql::ResultSet rs = stmt.executeQuery();
          while (rs.next()) {
            std::string filename;
            if (rs.getString("path").size() == 0) {
              filename = org::esb::config::Config::getProperty("hive.path");
            } else {
              filename = rs.getString("path");
            }
            filename += "/";
            filename += rs.getString("filename");
            org::esb::io::File file(filename.c_str());
            if (!file.exists()) {
              FileExporter::exportFile(rs.getInt("files.id"));
            }
          }
          org::esb::lang::Thread::sleep2(10000);
        }
      }
    }
  }
}
