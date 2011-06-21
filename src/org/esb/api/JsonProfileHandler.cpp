/* 
 * File:   JsonProfileHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 20. Mai 2011, 11:35
 */
#include "org/esb/hive/DatabaseService.h"

#include "JsonProfileHandler.h"
#include "org/esb/util/Log.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
#include <boost/foreach.hpp>



namespace org {
  namespace esb {
    namespace api {

      JsonProfileHandler::JsonProfileHandler() {

      }

      JsonProfileHandler::~JsonProfileHandler() {

      }

      bool JsonProfileHandler::contains(JSONNode& node, std::string name) {
        bool result = false;
        int size = node.size();
        //LOGDEBUG("NodeSize=" << size);
        if (size > 0) {
          for (int a = 0; a < size; a++) {
            JSONNode n = node[a];
            //LOGDEBUG("name=" << n.name());
            if (name == n.name()) {
              result = true;
            }
          }
        }
        return result;
      }

      std::string JsonProfileHandler::checkJsonProfile(JSONNode&root) {
        std::string result;
        /*check the root conatins required data*/
        if (!contains(root, "name")) {
          result = "no profile name given!";
        } else
          if (!contains(root, "format")) {
          result = "no format attribute found!";
        } else
          if (!contains(root, "video")) {
          result = "no video attribute found!";
        } else
          if (!contains(root, "audio")) {
          result = "no audio attribute found!";
        }else
          if (!contains(root["format"], "id")) {
          result = "no id attribute found in attribute \"format\"!";
        }else
          if (!contains(root["video"], "id")) {
          result = "no id attribute found in attribute \"video\"!";
        }else
          if (!contains(root["audio"], "id")) {
          result = "no id attribute found in attribute \"audio\"!";
        }
        return result;
      }

      JSONNode JsonProfileHandler::handle(struct mg_connection *conn, const struct mg_request_info *request_info, db::HiveDb &db, std::string postdata) {


        //JSONNode n(JSON_NODE);
        JSONNode response;

        if (strcmp(request_info->request_method, "DELETE") == 0) {
          char iddata[100];
          memset(&iddata, 0, 100);
          if (request_info->query_string != NULL) {
            mg_get_var(request_info->query_string, strlen(request_info->query_string), "id", iddata, sizeof (iddata));
            LOGDEBUG("DataId" << iddata);
          }
          if (strlen(iddata) > 0) {
            LOGDEBUG("loading preset data for id " << iddata);
            litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (db, db::Preset::Uuid == iddata);
            if (s.count() == 1) {
              db::Preset preset = s.one();
              preset.del();
              JSONNode ok(JSON_NODE);
              ok.set_name("ok");
              ok.push_back(JSONNode("code", "profile_deleted"));
              ok.push_back(JSONNode("description", "profile succesful deleted"));
              response.push_back(ok);
            } else {
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "profile_not_found"));
              error.push_back(JSONNode("description", "profile not found"));
              response.push_back(error);
            }
          }else{
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "no_id"));
              error.push_back(JSONNode("description", "no id given for delete action"));
              response.push_back(error);

          }
         }else
        if (strcmp(request_info->request_method, "GET") == 0) {
          char iddata[100];
          memset(&iddata, 0, 100);
          if (request_info->query_string != NULL) {
            mg_get_var(request_info->query_string, strlen(request_info->query_string), "id", iddata, sizeof (iddata));
            LOGDEBUG("DataId" << iddata);
          }

          if (strlen(iddata) > 0) {
            LOGDEBUG("loading preset data for id " << iddata);
            litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (db, db::Preset::Uuid == iddata);
            if (s.count() > 0) {
              db::Preset preset = s.one();
              JSONNode data = libjson::parse(preset.data);
              data.set_name("data");
              data["id"]=JSONNode("id", iddata);
              //data.push_back(JSONNode("id", iddata));
              data.preparse();
              //LOGDEBUG(data.write_formatted());
              response=JSONNode(JSON_NODE);
              response.push_back(data);
            } else {
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "profile_not_found"));
              error.push_back(JSONNode("description", "profile not found"));
              response.push_back(error);
            }
          } else {
            LOGDEBUG("loading all preset");
            vector<db::Preset> presets = litesql::select<db::Preset > (db).all();
            JSONNode c(JSON_ARRAY);
            c.set_name("data");

            BOOST_FOREACH(db::Preset preset, presets) {
              JSONNode prnode(JSON_NODE);
              prnode.push_back(JSONNode("id", preset.uuid.value()));
              prnode.push_back(JSONNode("name", preset.name.value()));
              c.push_back(prnode);
            }
            response.push_back(c);
          }
        }          /*
           * when the preset will be updated
           *
           */
        else if (strcmp(request_info->request_method, "POST") == 0) {

          /*check if the incomming data is valid json data*/
          JSONNode inode;
          try {
            if (libjson::is_valid(postdata)) {
              LOGDEBUG("Data is valid");
              inode = libjson::parse(postdata);
              std::string msg = checkJsonProfile(inode);
              if (msg.length() > 0) {
                JSONNode error(JSON_NODE);
                error.set_name("error");
                error.push_back(JSONNode("code", "attribute_error"));
                error.push_back(JSONNode("description", msg));
                response.push_back(error);
                return response;

              }
            } else {
              JSONNode error(JSON_NODE);
              
              error.set_name("error");
              error.push_back(JSONNode("code", "parse_error"));
              error.push_back(JSONNode("description", "no valid json format given"));
              response.push_back(error);
              return response;
            }
          } catch (std::exception &ex) {
            LOGDEBUG(ex.what());
            JSONNode error(JSON_NODE);
            error.set_name("error");
            error.push_back(JSONNode("code", "parse_error"));
            error.push_back(JSONNode("description", "no valid json format given"));
            response.push_back(error);
            return response;
          }

          /*getting the get parameter "id" to find out if a profile will be updated or created*/


          char iddata[100];
          memset(&iddata, 0, 100);
          if (request_info->query_string != NULL) {
            mg_get_var(request_info->query_string, strlen(request_info->query_string), "id", iddata, sizeof (iddata));
            LOGDEBUG("DataId found :" << iddata);
          }
          
          /*case when "id" data is given, that means a profile update*/
          if (strlen(iddata) > 0) {
            litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (db, db::Preset::Uuid == iddata);
            if (s.count() == 1) {
              LOGDEBUG("Update profile");
              db::Preset preset = s.one();
              preset.data = postdata;
              preset.name=inode["name"].as_string();
              preset.update();
              response.push_back(JSONNode("id", iddata));
            } else {
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "profile_not_found"));
              error.push_back(JSONNode("description", "profile with the given id not found"));
              response.push_back(error);
            }
          }/*case when no "id" data is given, that means a profile create*/
          else {
            LOGDEBUG("Create new Profile");
            boost::uuids::uuid uuid = boost::uuids::random_generator()();
            std::string uuidstr = boost::lexical_cast<std::string > (uuid);
            response.push_back(JSONNode("id", uuidstr));
            db::Preset preset(db);
            preset.data = postdata;
            preset.uuid = uuidstr;
            preset.name = inode["name"].as_string();
            preset.update();
          }
        }
        return response;
      }
    }
  }
}
