/* 
 * File:   JsonStatisticsHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 7. Juli 2011, 13:55
 */

#include "JsonStatisticsHandler.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/StringTokenizer.h"
namespace org {
  namespace esb {
    namespace api {

      JsonStatisticsHandler::JsonStatisticsHandler() {
      }

      JsonStatisticsHandler::~JsonStatisticsHandler() {
      }

      JSONNode JsonStatisticsHandler::handle(struct mg_connection *conn, const struct mg_request_info *request_info, db::HiveDb& db, std::string postdata) {
        JSONNode result(JSON_NODE);
        char iddata[100];
        memset(&iddata, 0, 100);
        if (request_info->query_string != NULL) {
          mg_get_var(request_info->query_string, strlen(request_info->query_string), "id", iddata, sizeof (iddata));
        }
        if (strlen(iddata) == 0) {
          JSONNode error(JSON_NODE);
          error.set_name("error");
          error.push_back(JSONNode("code", "id_error"));
          error.push_back(JSONNode("description", "no id specified, please provide a encoding id for statistic"));
          result.push_back(error);

        } else {
          litesql::DataSource<db::Job>s = litesql::select<db::Job > (db, db::Job::Uuid == iddata);
          if (s.count() > 0) {
            db::Job job = s.one();
            org::esb::io::File tfile(job.outfile.value());
            org::esb::io::File sfile(tfile.getFileName() + ".stats");
            LOGDEBUG("Reading stats file:" << tfile.getFileName());
            if (sfile.exists()) {
              org::esb::io::FileInputStream fis(&sfile);
              std::string data;
              fis.read(data);
              JSONNode datan(JSON_NODE);
              datan.set_name("data");
              JSONNode rate(JSON_ARRAY);
              rate.set_name("bitrate");
              JSONNode quality(JSON_ARRAY);
              quality.set_name("quality");

              /*splitting the stats data*/
              org::esb::util::StringTokenizer tok(data, ",");
              while (tok.hasMoreTokens()) {
                org::esb::util::StringTokenizer tok2(tok.nextToken(), ":");
                if (tok2.countTokens() == 2) {
                  quality.push_back(JSONNode("", atoi(tok2.nextToken().c_str())));
                  rate.push_back(JSONNode("", atoi(tok2.nextToken().c_str())));
                }
              }
              datan.push_back(rate);
              datan.push_back(quality);
              result.push_back(datan);
            }
          } else {
            JSONNode error(JSON_NODE);
            error.set_name("error");
            error.push_back(JSONNode("code", "encoding_not_found"));
            error.push_back(JSONNode("description", "encoding not found"));
            result.push_back(error);
          }
        }
        return result;
      }
    }
  }
}
