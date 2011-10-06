/*----------------------------------------------------------------------
 *  File    : ExportScanner.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose : it watches for completed files and export them to disk
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
#include "ExportScanner.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "org/esb/io/File.h"
#include "FileExporter.h"
#include "org/esb/config/config.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
#include "org/esb/core/PluginContext.h"

namespace org {
  namespace esb {
    namespace plugin {

      ExportScanner::ExportScanner() {
        _run = false;
      }

      void ExportScanner::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty("exportscanner") == "start") {
          LOGDEBUG("Start Request for the ExportScanner");
          _run = true;
          boost::thread t(boost::bind(&ExportScanner::start, this));
          boost::thread(boost::bind(&ExportScanner::restart_failed_exports, this));
          LOGDEBUG("ExportScanner started");
        } else
          if (msg.getProperty("exportscanner") == "stop") {
          LOGDEBUG("Export Scanner stop request received");
          if (_run) {
            _run = false;
            boost::mutex::scoped_lock terminationLock(terminationMutex);
            termination_wait.wait(terminationLock);
          }
          LOGDEBUG("Export Scanner stopped");
        }
      }
        void ExportScanner::startService(){
          LOGDEBUG("Start Request for the ExportScanner");
          _run = true;
          boost::thread t(boost::bind(&ExportScanner::start, this));
          boost::thread(boost::bind(&ExportScanner::restart_failed_exports, this));
          LOGDEBUG("ExportScanner started");          
        }
        void ExportScanner::stopService(){
          LOGDEBUG("Export Scanner stop request received");
          if (_run) {
            _run = false;
            boost::mutex::scoped_lock terminationLock(terminationMutex);
            termination_wait.wait(terminationLock);
          }
          LOGDEBUG("Export Scanner stopped");
          
        }
      org::esb::core::OptionsDescription ExportScanner::getOptionsDescription(){
        return org::esb::core::OptionsDescription();
      }

      void ExportScanner::start() {
        db::HiveDb db=*getContext()->database;//("sqlite3", org::esb::config::Config::get("db.url"));
        while (_run) {
          {
            try {
              std::vector<db::Job> completed_jobs = litesql::select<db::Job > (db, db::Job::Status == "completed").all();
              std::vector<db::Job>::iterator job_it = completed_jobs.begin();
              for (; job_it != completed_jobs.end(); job_it++) {
                db::MediaFile mfile = (*job_it).outputfile().get().one();
                try {
                  db::JobLog log((*job_it).getDatabase());
                  std::string message = "exporting file";
                  message += mfile.path + "/" + mfile.filename;
                  log.message = message;
                  log.update();

                  (*job_it).joblog().link(log);

                  (*job_it).status = "exporting";
                  (*job_it).update();
                  FileExporter::exportFile(mfile);
                } catch (boost::filesystem::filesystem_error & e) {
                  db::JobLog log((*job_it).getDatabase());
                  std::string message = "failed to write the outputfile to ";
                  message += mfile.path + "/" + mfile.filename;
                  message += ": ";
                  message += e.what();
                  log.message = message;
                  log.update();

                  (*job_it).joblog().link(log);

                  (*job_it).status = "failed";
                  (*job_it).update();
                }
              }
            } catch (litesql::NotFound ex) {
              LOGDEBUG("no new File to export Found");
              org::esb::lang::Thread::sleep2(1000);
            }
          }
          org::esb::lang::Thread::sleep2(5000);
        }
        boost::mutex::scoped_lock terminationLock(terminationMutex);
        termination_wait.notify_all();
      }

      void ExportScanner::restart_failed_exports() {
        db::HiveDb db("sqlite3", org::esb::config::Config::get("db.url"));
        while (_run) {
          std::vector<db::Job> completed_jobs = litesql::select<db::Job > (db, db::Job::Status == "exists").all();
          std::vector<db::Job>::iterator job_it = completed_jobs.begin();
          for (; job_it != completed_jobs.end(); job_it++) {
            db::MediaFile mfile = (*job_it).outputfile().get().one();
            org::esb::io::File f(mfile.path + "/" + mfile.filename);
            if (!f.exists()) {
              (*job_it).status = "completed";
              (*job_it).update();
            }
          }
          org::esb::lang::Thread::sleep2(5000);
        }
      }
    }
  }
}
