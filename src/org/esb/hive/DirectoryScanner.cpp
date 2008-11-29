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
namespace org {
  namespace esb {
    namespace hive {
      using namespace org::esb::io;
      using namespace org::esb::sql;
      using namespace org::esb::lang;

      class MyFileFilter : public FileFilter {
      public:

        bool accept(File file) {
          return (boost::ends_with(file.getPath(), ".ts"));
        }
      };

      DirectoryScanner::DirectoryScanner(std::string dir, int interval) {
        _halt = true;
        _interval = interval;
        _dir = dir;
        th=NULL;
      }

      DirectoryScanner::DirectoryScanner() {
        _halt = true;
        th=NULL;
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
          th = new boost::thread(boost::bind(&DirectoryScanner::scan, this));
          logdebug("Directory Scanner running with interval:" << _interval);
          //    boost::thread t(boost::bind(&DirectoryScanner::scan, this));

        }else
          if (msg.getProperty("directoryscan") == "stop") {
          _halt = true;
          //#if BOOST_VERSION > 103500
          if(th){
            th->interrupt();
            delete th;
            th=NULL;
          }
          //#endif
          logdebug("Directory Scanner stopped:");
        }
      }

      void DirectoryScanner::scan() {
		  while (!_halt) {
		Connection con(org::esb::config::Config::getProperty("db.connection"));
		Statement stmt=con.createStatement("select * from watch_folder");
		ResultSet rs=stmt.executeQuery();
		while(rs.next()){
		  if (File(rs.getString("folder").c_str()).exists()) {
            scan(rs.getString("folder"));
          }else {
//            _halt = true;
          }
		}
          Thread::sleep2(_interval);
        }
      }

      void DirectoryScanner::scan(std::string dir) {
        logdebug("Directory Scanner loop:" << ":" << dir);
        MyFileFilter filter;

		FileList list = File(dir.c_str()).listFiles(filter);
        FileList::iterator it = list.begin();
        for (; it != list.end(); it++) {
          if ((*it)->isDirectory())
            scan((*it)->getPath());
          else
            computeFile(*it->get());
        }
      }

      void DirectoryScanner::computeFile(File & file) {

        Connection con(org::esb::config::Config::getProperty("db.connection"));
        PreparedStatement stmt(con.prepareStatement("select * from files where filename=:name and path=:path"));
        stmt.setString("name", file.getFileName());
        stmt.setString("path", file.getFilePath());
        ResultSet rs = stmt.executeQuery();
        if (!rs.next()) {
          //  found=str.find_first_of("aeiou");
          if (file.isFile()) {
            const char * filename = 0;
            std::string name = file.getPath();
            //		filename=name.data();
            char * argv[] = {"", (char*) name.c_str()};
            int fileid = import(2, argv);

          }
          //    std::string sql("insert into files (filename, path) values (\""+file.getFileName()+"\",\""+file.getFilePath()+"\")");
          //    std::cout << sql <<std::endl;
          //    PreparedStatement pstmt=con.prepareStatement("insert into files (filename, path) values (:name,:path)");
          //    pstmt.setString("name", file.getFileName());
          //    pstmt.setString("path", file.getFilePath());
          //    pstmt.execute();
        }


      }

    }
  }
}

