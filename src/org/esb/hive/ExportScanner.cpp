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

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/io/File.h"
#include "org/esb/hive/FileExporter.h"
#include "org/esb/config/config.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace hive {

      ExportScanner::ExportScanner() {
        _run = false;
      }

      void ExportScanner::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty("exportscanner") == "start") {
          LOGDEBUG("Start Request for the ExportScanner");
          _run = true;
          boost::thread t(boost::bind(&ExportScanner::start, this));
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

      void ExportScanner::start() {
        db::HiveDb db("mysql",org::esb::config::Config::getProperty("db.url"));
        while (_run) {
          {
            try {
              std::vector<db::Job> completed_jobs=litesql::select<db::Job>(db, db::Job::Status=="completed").all();
              std::vector<db::Job>::iterator job_it=completed_jobs.begin();
              for(;job_it!=completed_jobs.end();job_it++){
                db::MediaFile mfile=(*job_it).outputfile().get().one();
                FileExporter::exportFile(mfile);
              }
            } catch (litesql::NotFound ex) {
              LOGDEBUG("now new File to export Found");
              org::esb::lang::Thread::sleep2(1000);
            }
          }
          org::esb::lang::Thread::sleep2(5000);
        }
        boost::mutex::scoped_lock terminationLock(terminationMutex);
        termination_wait.notify_all();

      }
    }
  }
}
