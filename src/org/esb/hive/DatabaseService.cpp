/*----------------------------------------------------------------------
 *  File    : DatabaseService.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose : Global Database Service for the Database Connection
 *  Created : 18 Feb 2009 by Jan Hölscher <jan.hoelscher@esblab.com>
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
#include "DatabaseService.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/sql/my_sql.h"
#include "org/esb/util/Log.h"
#include <iostream>
namespace org {
  namespace esb {
    namespace hive {
      bool DatabaseService::_running = false;
      std::string DatabaseService::_base_path;

      DatabaseService::DatabaseService(std::string base_path) {
        _base_path = base_path;
        _running = false;
      }

      void DatabaseService::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty("databaseservice") == org::esb::hive::START) {
          start(_base_path);
        }
        if (msg.getProperty("databaseservice") == org::esb::hive::STOP) {
          stop();
        }
      }

      void DatabaseService::start(std::string base_path) {
        LOGINFO("starting Database Service");
        if (_running == false) {
          //          std::string base_path = _base_path;
          std::string lang = "--language=";
          lang.append(base_path);
          lang.append("/res");

          std::string datadir = "--datadir=";
          datadir.append(base_path);
          datadir.append("/");
          char * dbdir=const_cast<char*> (datadir.c_str());
          char * langdir= const_cast<char*> (lang.c_str());

          char *server_options[] = {"dbservice", dbdir,langdir, (char*)NULL};
          int num_elements = (sizeof (server_options) / sizeof (char *))-1;

          static char *server_groups[] = {"embedded","server","dbservice_SERVER", (char*)NULL};
          if (mysql_library_init(num_elements, server_options, server_groups) != 0) {
            LOGFATAL("initialising DatabaseService datadir="<<datadir<<" resource="<<lang);
          }
          _running = true;
        }
      }

      void DatabaseService::stop() {
        LOGINFO("stopping Database Service");
        if (_running)
          mysql_library_end();
        _running = false;
      }

      void DatabaseService::thread_init(){
        mysql_thread_init();
      }

      void DatabaseService::thread_end(){
        mysql_thread_end();
      }
    }
  }
}
