/* 
 * File:   JSONHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 1. September 2011, 15:52
 */

#include "JSONHandler.h"
#include "ServiceRequest.h"
#include "ServiceResponse.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/libjson/JSONNode.h"
//#include "org/esb/api/JsonEncoding.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace api {

      JSONHandler::JSONHandler() {
        base_uri = "/api/v1";
        LOGDEBUG("JSONHandler::JSONHandler()");
        db = new db::HiveDb("sqlite3","database=hive.db");
      }

      JSONHandler::~JSONHandler() {
        LOGDEBUG("JSONHandler::~JSONHandler()")
        delete db;
      }

      void JSONHandler::handleEncoding(Request * req, Response*res) {
        ServiceRequest*sreq = ((ServiceRequest*) req);
        if (sreq->getRequestURI().find(base_uri + "/encoding") == 0) {
          std::vector<db::Job> jobs = litesql::select<db::Job > (*db, db::Job::Status != "deleted").orderBy(db::Job::Id, false).all();
          JSONNode n(JSON_NODE);
          JSONNode c(JSON_ARRAY);
          c.set_name("data");
          std::vector<db::Job>::iterator jobit = jobs.begin();
          for (; jobit != jobs.end(); jobit++) {
            c.push_back(JSONNode("id",(*jobit).uuid.value()));
            //c.push_back(JsonEncoding((*jobit), false));
          }
          n.push_back(c);
          ServiceResponse*sres = ((ServiceResponse*) res);
          sres->setStatus(200);
          sres->getOutputStream()->write(n.write());
        }
      }
    }
  }
}
