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
          logdebug("Start Request for the ExportScanner");
          _run = true;
          boost::thread t(boost::bind(&ExportScanner::start, this));
          logdebug("ExportScanner started");
        } else
          if (msg.getProperty("exportscanner") == "stop") {
          logdebug("Export Scanner stop request received");
          if (_run) {
            _run = false;
            boost::mutex::scoped_lock terminationLock(terminationMutex);
            termination_wait.wait(terminationLock);
          }
          logdebug("Export Scanner stopped");
        }
      }

      void ExportScanner::start() {
        while (_run) {
          {
            org::esb::sql::Connection con(org::esb::config::Config::getProperty("db.connection"));
            //            org::esb::sql::PreparedStatement stmt = con.prepareStatement("SELECT files.id, filename, path FROM jobs, files WHERE outputfile=files.id and complete is not null;");
            //            org::esb::sql::PreparedStatement stmt = con.prepareStatement("SELECT f.id, filename, path FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id  group by fileid having round(count(complete)/count(*)*100,2)=100.00 order by f.id DESC");
            org::esb::sql::PreparedStatement stmt = con.prepareStatement("SELECT files.id, filename, path from jobs, files where jobs.outputfile= files.id and jobs.complete is not null and status='completed'");
            org::esb::sql::ResultSet rs = stmt.executeQuery();
            while (rs.next()) {
              std::string filename;
              if (rs.getString("path").size() == 0) {
                filename = org::esb::config::Config::getProperty("hive.base_path");
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
          }
          org::esb::lang::Thread::sleep2(5000);
        }
        boost::mutex::scoped_lock terminationLock(terminationMutex);
        termination_wait.notify_all();

      }
    }
  }
}
