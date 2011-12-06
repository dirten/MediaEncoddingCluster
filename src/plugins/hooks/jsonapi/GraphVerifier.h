/* 
 * File:   GraphVerifier.h
 * Author: HoelscJ
 *
 * Created on 6. Dezember 2011, 13:08
 */

#ifndef GRAPHVERIFIER_H
#define	GRAPHVERIFIER_H
#include "org/esb/libjson/libjson.h"

namespace graph {

  class GraphVerifier {
  public:
    static GraphVerifier * getInstance();
    bool verifyTasks(JSONNode & node);
    JSONNode getResult();
  private:
    GraphVerifier();
    virtual ~GraphVerifier();
    static GraphVerifier * _instance;
    std::string _status;
    std::string _message;
    bool verifyTask(JSONNode & node);
  };
}
#endif	/* GRAPHVERIFIER_H */

