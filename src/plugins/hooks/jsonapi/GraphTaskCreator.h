/* 
 * File:   GraphTaskCreator.h
 * Author: jhoelscher
 *
 * Created on 10. Dezember 2011, 16:44
 */

#ifndef GRAPHTASKCREATOR_H
#define	GRAPHTASKCREATOR_H
#include "org/esb/libjson/libjson.h"
#include "exports.h"

namespace graph {

  class JSONAPI_EXPORT GraphTaskCreator {
  public:
    static GraphTaskCreator * getInstance();
    bool create(JSONNode & node);
    JSONNode getResult();
  private:
    GraphTaskCreator();
    virtual ~GraphTaskCreator();
    static GraphTaskCreator * _instance;
  private:

  };
}
#endif	/* GRAPHTASKCREATOR_H */

