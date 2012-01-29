/* 
 * File:   JSONResult.h
 * Author: jhoelscher
 *
 * Created on 29. Januar 2012, 17:57
 */

#ifndef JSONRESULT_H
#define	JSONRESULT_H
#include "org/esb/libjson/JSONNode.h"
class JSONResult:public JSONNode {
public:
  JSONResult();
  virtual ~JSONResult();
  void setData(JSONNode & data);
  void setRequestId(std::string id);
  void setStatus(std::string status, std::string msg="");
  std::string toString();
private:

};

#endif	/* JSONRESULT_H */

