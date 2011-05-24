/* 
 * File:   JsonEncoding.cpp
 * Author: HoelscJ
 * 
 * Created on 18. Mai 2011, 14:46
 */

#include "JsonEncoding.h"
namespace org {
  namespace esb {
    namespace api {

      JsonEncoding::JsonEncoding(std::string json_string):JSONNode(JSON_NODE) {
      }
      JsonEncoding::JsonEncoding(db::Job job, bool simple):JSONNode(JSON_NODE) {
        push_back(JSONNode("id",job.id.value()));
        push_back(JSONNode("created",job.created.value().asString("%d-%m-%y %h:%M:%s")));
        push_back(JSONNode("begintime",job.begintime.value().asString("%d-%m-%y %h:%M:%s")));
        push_back(JSONNode("endtime",job.endtime.value().asString("%d-%m-%y %h:%M:%s")));
        push_back(JSONNode("progress",job.progress.value()));
        push_back(JSONNode("infile",job.infile.value()));
        push_back(JSONNode("outfile",job.outfile.value()));
        push_back(JSONNode("status",job.status.value()));
        if(!simple)
          push_back(JSONNode("profile",job.data.value()));
      }

      JsonEncoding::JsonEncoding(const JsonEncoding& orig) {
      }

      JsonEncoding::~JsonEncoding() {
      }
    }
  }
}
