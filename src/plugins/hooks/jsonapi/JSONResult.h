/* 
 * File:   JSONResult.h
 * Author: jhoelscher
 *
 * Created on 29. Januar 2012, 17:57
 */

#ifndef JSONRESULT_H
#define	JSONRESULT_H
#include "org/esb/libjson/JSONNode.h"
#include "org/esb/core/http/HTTPServerRequest.h"
#include "org/esb/core/http/HTTPServerResponse.h"
class JSONResult:public JSONNode {
public:
  //JSONResult(std::string uuid);
  JSONResult(org::esb::core::http::HTTPServerRequest&);
  virtual ~JSONResult();
  void setData(JSONNode & data);
  void setRequestId(std::string id);
  void setStatus(std::string status, std::string msg="");
  void setStatus(Poco::Net::HTTPResponse::HTTPStatus, std::string msg, std::string desc="");
  std::string toString();
private:
  bool _statusAllreadySetted;
  std::string _uuid;
  org::esb::core::http::HTTPServerRequest & _req;
};

#endif	/* JSONRESULT_H */

