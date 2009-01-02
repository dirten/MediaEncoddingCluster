#include "DirectoryScanner.h"

//#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
//#include "org/esb/sql/ResultSet.h"
#include "org/esb/io/FileFilter.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"

#include "org/esb/hive/FileImporter.h"
//#include "import.cpp"
#include <boost/algorithm/string.hpp>
#include "org/esb/util/Log.h"
#include "org/esb/hive/JobUtil.h"
#include "org/esb/util/Decimal.h"
namespace org {
  namespace esb {
    namespace hive {
      using namespace org::esb::io;
      using namespace org::esb::sql;
      using namespace org::esb::lang;

      class MyFileFilter : public FileFilter {
      public:
        MyFileFilter(){
          media_ext[".avi"]="";
          media_ext[".ts"]="";
          media_ext[".VOB"]="";
        }
        bool accept(File file) {
          bool result=false;
		  if(file.isDirectory()||media_ext.find(file.getExtension())!=media_ext.end())
            result=true;
          return result;
        }
      private:
        map<std::string,std::string> media_ext;
      };

      DirectoryScanner::DirectoryScanner(std::string dir, int interval) {
        _halt = true;
        _interval = interval;
        _dir = dir;
        th = NULL;
      }

      DirectoryScanner::DirectoryScanner() {
        _halt = true;
        th = NULL;
      }

      DirectoryScanner::~DirectoryScanner() {
        if (th)
          delete th;
      }

      void DirectoryScanner::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty("directoryscan") == "start") {

          _halt = false;
          if (msg.containsProperty("directory")) {
            _dir = msg.getProperty("directory");
          }

          if (msg.containsProperty("interval")) {
            _interval = atoi(msg.getProperty("interval").c_str())*1000;
          }
          boost::thread(boost::bind(&DirectoryScanner::scan, this));
          logdebug("Directory Scanner running with interval:" << _interval);
          //    boost::thread t(boost::bind(&DirectoryScanner::scan, this));

        } else
          if (msg.getProperty("directoryscan") == "stop") {
          _halt = true;
          logdebug("Directory Scanner stopped:");
        }
      }

      void DirectoryScanner::scan() {
        while (!_halt) {
          Connection con(std::string(org::esb::config::Config::getProperty("db.connection")));
          Statement stmt = con.createStatement("select * from watch_folder");
          ResultSet rs = stmt.executeQuery();
          while (rs.next()) {
            if (File(rs.getString("folder").c_str()).exists()) {
              scan(rs.getString("folder"), rs.getInt("profile"));
            } else {
              //            _halt = true;
            }
          }
          Thread::sleep2(_interval);
        }
      }

      void DirectoryScanner::scan(std::string dir, int profile) {
        logdebug("Directory Scanner loop:" << ":" << dir);
        MyFileFilter filter;

        FileList list = File(dir.c_str()).listFiles(filter);
        FileList::iterator it = list.begin();
        for (; it != list.end(); it++) {
          if ((*it)->isDirectory())
            scan((*it)->getPath(), profile);
          else
            computeFile(*it->get(), profile);
        }
      }

      void DirectoryScanner::computeFile(File & file, int p) {

        Connection con(std::string(org::esb::config::Config::getProperty("db.connection")));
        PreparedStatement stmt(con.prepareStatement("select * from files where filename=:name and path=:path"));
        stmt.setString("name", file.getFileName());
        stmt.setString("path", file.getFilePath());
        ResultSet rs = stmt.executeQuery();
        if (!rs.next()) {
          if (file.isFile()) {
            const char * filename = 0;
            std::string name = file.getPath();
            //		filename=name.data();
            char * argv[] = {"", (char*) name.c_str()};
            int fileid = import(2, argv);
            if (fileid > 0 && p > 0) {
              std::string file = org::esb::util::Decimal(fileid).toString();
              std::string profile = org::esb::util::Decimal(p).toString();
              char * jobarg[] = {"", "", (char*) file.c_str(), (char*) profile.c_str()};
              std::cout << "FileId:" << jobarg[2] << ":" << std::endl;
              std::cout << "ProfileId:" << jobarg[3] << ":" << std::endl;
              jobcreator(3, jobarg);
            }
          }
        }
      }
    }
  }
}

