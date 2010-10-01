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
#include "JobScanner.h"
#include "org/esb/config/config.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/util/Log.h"
#include "org/esb/hive/JobUtil.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "DatabaseService.h"

namespace org {
  namespace esb {
    namespace hive {

      JobScanner::JobScanner() {
        _run = false;
        _running = false;
        //        _con = NULL;
        //        _stmt = NULL;
        _interval = 10000;
      }

      JobScanner::~JobScanner() {
        _run = false;
        _running = false;
      }

      void JobScanner::start() {
        if (!_run && !_running) {
          _run = true;
          //          _con = new org::esb::sql::Connection(org::esb::config::Config::getProperty("db.connection"));
          //          _stmt = _con->prepareStatement2("SELECT * FROM files join watch_folder on files.path=watch_folder.infolder left join jobs on files.id=jobs.inputfile and watch_folder.profile=jobs.profileid WHERE files.parent =0 and jobs.id is null");
          boost::thread(boost::bind(&JobScanner::run, this));
        }
      }

      void JobScanner::stop() {
        if (_run && _running) {
          _run = false;
          boost::mutex::scoped_lock terminationLock(terminationMutex);
          termination_wait.wait(terminationLock);
          _running = false;
          //          if (_stmt)
          //            delete _stmt;
          //          if (_con)
          //            delete _con;
        }
      }

      void JobScanner::onMessage(org::esb::signal::Message& msg) {
        if (msg.getProperty("jobscanner") == "start") {
          LOGDEBUG("Job Scanner start Request received:");
          start();
          LOGDEBUG("Job Scanner running with interval:" << _interval);

        } else
          if (msg.getProperty("jobscanner") == "stop") {
          LOGDEBUG("Job Scanner stop Request received:");
          stop();
          LOGDEBUG("Job Scanner stopped:");
        }
      }

      void JobScanner::run() {
        _running = true;

        db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();

//        org::esb::sql::PreparedStatement stmt = con.prepareStatement("SELECT *,concat(outfolder,\"/\",profile_name,substring( files.path, length( watch_folder.infolder )+1 )) as output_folder FROM files join watch_folder on substring( files.path, 1, length( watch_folder.infolder ) ) =watch_folder.infolder left join jobs on files.id=jobs.inputfile and watch_folder.profile=jobs.profileid, profiles WHERE files.parent =0 and profiles.id=profile and jobs.id is null");
        while (_run) {
          litesql::Records records=db.query("SELECT files.id,watch_folder.profile,concat(outfolder,\"/\",profile_name,substring( files.path, length( watch_folder.infolder )+1 )) as output_folder FROM files join watch_folder on substring( files.path, 1, length( watch_folder.infolder ) ) =watch_folder.infolder left join jobs on files.id=jobs.inputfile and watch_folder.profile=jobs.profileid, profiles WHERE files.parent =0 and profiles.id=profile and jobs.id is null");
//          org::esb::sql::ResultSet rs = stmt.executeQuery();
          for (litesql::Records::iterator i=records.begin();i!=records.end();i++) {

            std::string file = (*i)[0];
            std::string profile = (*i)[1];
            std::string outdir = (*i)[2];
            char * jobarg[] = {
              const_cast<char*>(""), 
              const_cast<char*>(""), 
              const_cast<char*>(file.c_str()), 
              const_cast<char*>(profile.c_str()), 
              const_cast<char*>(outdir.c_str())
            };
//            std::cout << "FileId:" << jobarg[2] << ":" << std::endl;
//            std::cout << "ProfileId:" << jobarg[3] << ":" << std::endl;
            jobcreator(4, jobarg);
          }
          //          logdebug("wait");
          org::esb::lang::Thread::sleep2(_interval);
        }
        boost::mutex::scoped_lock terminationLock(terminationMutex);
        termination_wait.notify_all();

      }
    }
  }
}

