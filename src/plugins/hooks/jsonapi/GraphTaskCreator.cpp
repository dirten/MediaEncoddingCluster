/* 
 * File:   GraphTaskCreator.cpp
 * Author: jhoelscher
 * 
 * Created on 10. Dezember 2011, 16:44
 */

#include "GraphTaskCreator.h"
namespace graph {
  GraphTaskCreator * GraphTaskCreator::_instance=0;

  GraphTaskCreator * GraphTaskCreator::getInstance() {
    if(_instance==0)
      _instance=new GraphTaskCreator();
    return _instance;
  }
  
  bool GraphTaskCreator::create(JSONNode& node) {
    bool result=true;
    
    return result;
  }

  JSONNode GraphTaskCreator::getResult() {
    JSONNode result(JSON_NODE);
    
    return result;
  }
  GraphTaskCreator::GraphTaskCreator() {
  }

  GraphTaskCreator::~GraphTaskCreator() {
  }
}
