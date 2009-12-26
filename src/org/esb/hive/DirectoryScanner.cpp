/*----------------------------------------------------------------------
 *  File    : JobScanner.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 6. November 2009, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 * ----------------------------------------------------------------------
 */

#include "DirectoryScanner.h"
#include <boost/bind.hpp>
#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"

#include "org/esb/hive/FileImporter.h"
#include <boost/algorithm/string.hpp>
#include "org/esb/util/Log.h"
#include "org/esb/hive/JobUtil.h"
#include "org/esb/util/Decimal.h"
//#include "org/esb/av/AV.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace hive {
      using namespace org::esb::io;
      using namespace org::esb::sql;
      using namespace org::esb::lang;

      class MyFileFilter : public FileFilter {
      public:

        MyFileFilter(std::string ext) {
          //          logdebug("extensions"<<ext);
          StringTokenizer tokenizer(ext, ",");
          while (tokenizer.hasMoreTokens()) {
            std::string tok = tokenizer.nextToken();
            tok = StringUtil::trim(tok);
            if (tok.length() > 0) {
              std::string e = ".";
              e += tok;
              media_ext[e];
              //logdebug("Extension added:"<<e);
            }
          }
        }

        bool accept(File file) {
          bool result = false;
          if (file.isDirectory() || media_ext.size() == 0 || media_ext.find(file.getExtension()) != media_ext.end())
            result = true;
          return result;
        }
      private:
        map<std::string, std::string> media_ext;
      };

      DirectoryScanner::DirectoryScanner(std::string dir, int interval) {
        _stmt = NULL;
        _con = NULL;
        _halt = true;
        _interval = interval;
        th = NULL;
        //        _con=new Connection(std::string(org::esb::config::Config::getProperty("db.connection")));
        //        _stmt=new PreparedStatement(_con->prepareStatement("select * from files where filename=:name and path=:path"));
      }

      DirectoryScanner::DirectoryScanner() {
        _stmt = NULL;
        _con = NULL;
        _halt = true;
        th = NULL;
        _interval = 300000;

      }

      DirectoryScanner::~DirectoryScanner() {
        if (th)
          delete th;

      }

      void DirectoryScanner::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty("directoryscan") == "start") {
          _halt = false;
          if (msg.containsProperty("interval")) {
            _interval = atoi(msg.getProperty("interval").c_str())*1000;
          }
          boost::thread(boost::bind(&DirectoryScanner::scan, this));
          logdebug("Directory Scanner running with interval:" << _interval);
          //    boost::thread t(boost::bind(&DirectoryScanner::scan, this));

        } else
          if (msg.getProperty("directoryscan") == "stop") {
          logdebug("Directory Scanner stop request received");
          if (!_halt) {
            _halt = true;
            boost::mutex::scoped_lock terminationLock(terminationMutex);
            termination_wait.wait(terminationLock);
          }
          logdebug("Directory Scanner stopped");
        }
      }

      void DirectoryScanner::scan() {
        _con = new Connection(std::string(org::esb::config::Config::getProperty("db.connection")));
        _stmt = new PreparedStatement(_con->prepareStatement("select * from files where filename=:name and path=:path"));
        _con2 = new Connection(std::string(org::esb::config::Config::getProperty("db.connection")));
        _stmt2 = new Statement(_con2->createStatement("select * from watch_folder"));
        while (!_halt) {
          ResultSet rs = _stmt2->executeQuery();
          while (rs.next()) {
            if (File(rs.getString("infolder").c_str()).exists()) {
              FileFilter * filter = new MyFileFilter(rs.getString("extension_filter"));
              scan(rs.getString("infolder"), rs.getString("outfolder"), rs.getInt("profile"), *filter);
              delete filter;
            } else {
              //            _halt = true;
            }
          }
          Thread::sleep2(_interval);
        }
        if (_stmt)
          delete _stmt;
        if (_con)
          delete _con;
        _stmt = NULL;
        _con = NULL;
        delete _stmt2;
        delete _con2;

        boost::mutex::scoped_lock terminationLock(terminationMutex);
        termination_wait.notify_all();

      }

      void DirectoryScanner::scan(std::string indir, std::string outdir, int profile, FileFilter & filter) {
        //        logdebug("Scanning Directory:" << indir);
        //

        FileList list = File(indir.c_str()).listFiles(filter);
        FileList::iterator it = list.begin();
        for (; it != list.end(); it++) {
          if ((*it)->isDirectory())
            scan((*it)->getPath(), outdir, profile, filter);
          else
            computeFile(*it->get(), profile, outdir);
        }
      }

      void DirectoryScanner::computeFile(File & file, int p, std::string outdir) {

        _stmt->setString("name", file.getFileName());
        _stmt->setString("path", file.getFilePath());
        ResultSet rs = _stmt->executeQuery();
        if (!rs.next()) {
          if (file.isFile()) {
            const char * filename = 0;
            std::string name = file.getPath();
            //		filename=name.data();
            char * argv[] = {"", (char*) name.c_str()};
            int fileid = import(2, argv);
            if (false && fileid > 0 && p > 0) {
              std::string file = org::esb::util::Decimal(fileid).toString();
              std::string profile = org::esb::util::Decimal(p).toString();
              char * jobarg[] = {"", "", (char*) file.c_str(), (char*) profile.c_str(), (char*) outdir.c_str()};
              std::cout << "FileId:" << jobarg[2] << ":" << std::endl;
              std::cout << "ProfileId:" << jobarg[3] << ":" << std::endl;
              jobcreator(4, jobarg);
            }
          }
        }
      }
    }
  }
}

