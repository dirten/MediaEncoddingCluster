/*----------------------------------------------------------------------
 *  File:   : JsonEncoding.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 18. Mai 2011 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program License under the terms in the LICENSE file
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * ----------------------------------------------------------------------
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
          push_back(JSONNode("fps",job.fps.value()));
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
