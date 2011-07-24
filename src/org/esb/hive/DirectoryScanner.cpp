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
#include "org/esb/db/hivedb.hpp"
#include "DirectoryScanner.h"
#include <boost/bind.hpp>
#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"


#include "org/esb/hive/FileImporter.h"
#include <boost/algorithm/string.hpp>
#include "org/esb/util/Log.h"
#include "org/esb/hive/JobUtil.h"

//#include "org/esb/av/AV.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/StringUtil.h"
#include "DatabaseService.h"
namespace org {
  namespace esb {
    namespace hive {
      using namespace org::esb::io;
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
        _halt = true;
        _interval = interval;
        th = NULL;
        //        _con=new Connection(std::string(org::esb::config::Config::getProperty("db.connection")));
        //        _stmt=new PreparedStatement(_con->prepareStatement("select * from files where filename=:name and path=:path"));
      }

      DirectoryScanner::DirectoryScanner() {
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
          LOGDEBUG("Directory Scanner running with interval:" << _interval);
          //    boost::thread t(boost::bind(&DirectoryScanner::scan, this));

        } else
          if (msg.getProperty("directoryscan") == "stop") {
          LOGDEBUG("Directory Scanner stop request received");
          if (!_halt) {
            _halt = true;
            boost::mutex::scoped_lock terminationLock(terminationMutex);
            termination_wait.wait(terminationLock);
          }
          LOGDEBUG("Directory Scanner stopped");
        }
      }

      void DirectoryScanner::scan() {
        do{
          db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();
          vector<db::Watchfolder> dbfolder = litesql::select<db::Watchfolder > (db).all();
          vector<db::Watchfolder>::iterator it = dbfolder.begin();
          for (; it != dbfolder.end(); it++) {
            LOGDEBUG("Scandir:" << (*it).infolder);
            File folder((*it).infolder);
            if (folder.exists()) {
              FileFilter * filter = new MyFileFilter((*it).extensionfilter);
              try {
                scan((*it),(*it).infolder);
              } catch (litesql::NotFound ex) {
                LOGERROR(ex);
              }
              delete filter;
            } else {
              //            _halt = true;
            }
          }
          Thread::sleep2(_interval);
        }while (!_halt);
        LOGDEBUG("halting directoryscanner");
        boost::mutex::scoped_lock terminationLock(terminationMutex);
        termination_wait.notify_all();

      }

      void DirectoryScanner::scan(db::Watchfolder folder,std::string indir/*, std::string outdir, db::Preset preset, FileFilter & filter*/) {
        LOGDEBUG("Scanning Directory:" << indir);
        MyFileFilter filter=MyFileFilter(folder.extensionfilter);
        FileList list = File(indir).listFiles(filter);
        FileList::iterator it = list.begin();
        for (; it != list.end(); it++) {
          if ((*it)->isDirectory())
            scan(folder,(*it)->getPath());
          else
            computeFile(folder,*it->get());
        }
      }

      void DirectoryScanner::computeFile(db::Watchfolder folder,File & file/*, db::Preset preset, std::string outdir*/) {
        //db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();
        try {
          
          litesql::DataSource<db::MediaFile> filelist = litesql::select<db::MediaFile > (folder.getDatabase(), db::MediaFile::Filename == file.getFileName() && db::MediaFile::Path == file.getFilePath());
          if(filelist.count()==0){
          if (file.isFile()) {
            LOGDEBUG("new file found:" << file.getPath());
            //const char * filename = 0;
            db::MediaFile mediafile = _importer.import(file);
            if (mediafile.id > 0 ) {

              int id = org::esb::hive::JobUtil::createJob(mediafile, folder.preset().get().one(), folder.outfolder);
              //db::Job pre = litesql::select<db::Job > (db, db::Job::Id == id).one();
              //jobcreator(fileid,p, outdir);
            }
          }

          }
        } catch (litesql::NotFound ex) {

        }
      }
    }
  }
}

