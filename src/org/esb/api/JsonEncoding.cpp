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
        push_back(JSONNode("id",job.uuid.value()));
        if(!simple){
          push_back(JSONNode("created",job.created));
          push_back(JSONNode("begintime",job.begintime));
          push_back(JSONNode("endtime",job.endtime));
          push_back(JSONNode("progress",job.progress.value()));
          push_back(JSONNode("infile",job.infile.value()));
          push_back(JSONNode("outfile",job.outfile.value()));
          push_back(JSONNode("status",job.status.value()));
          std::string profileid;
          if (libjson::is_valid(job.data.value())) {
            JSONNode profile = libjson::parse(job.data.value());
            profile.set_name("profile");
            push_back(profile);
            //profileid=profile["id"].as_string();
          }
          db::Preset preset=job.preset().get().one();
          push_back(JSONNode("profileid",preset.uuid));
        }
      }

      JsonEncoding::JsonEncoding(const JsonEncoding& orig) {
      }

      JsonEncoding::~JsonEncoding() {
      }
    }
  }
}
