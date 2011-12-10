/* 
 * File:   GraphVerifier.h
 * Author: HoelscJ
 *
 * Created on 6. Dezember 2011, 13:08
 */

#ifndef GRAPHVERIFIER_H
#define	GRAPHVERIFIER_H
#include "org/esb/db/hivedb.hpp"
#include "org/esb/libjson/libjson.h"
#include "org/esb/lang/Ptr.h"
#include "exports.h"
#include <map>
#include <set>
namespace graph {

  class JSONAPI_EXPORT GraphVerifier {
  public:
    static GraphVerifier * getInstance();
    bool verify(JSONNode & node, boost::shared_ptr<db::HiveDb> db);
    JSONNode getResult();
  private:
    GraphVerifier();
    virtual ~GraphVerifier();
    static GraphVerifier * _instance;
    std::string _status;
    std::string _message;
    bool verifyTasks(JSONNode & node);
    bool verifyLinks(JSONNode & node);
    bool verifyTask(JSONNode & node);
    bool verifyLink(JSONNode & node);
    bool verifyCycleRedundance(JSONNode & node);
    std::map<std::string, JSONNode> nodes;
    std::set<std::string> linked_nodes;
    boost::shared_ptr<db::HiveDb> _db;
  };
}
#endif	/* GRAPHVERIFIER_H */

