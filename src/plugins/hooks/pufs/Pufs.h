/* 
 * File:   Pufs.h
 * Author: HoelscJ
 *
 * Created on 22. September 2011, 10:36
 */

#ifndef PUFS_H
#define	PUFS_H
#include "org/esb/core/HookPlugin.h"

class Pufs : public org::esb::core::HookPlugin {
public:
  Pufs();
  virtual ~Pufs();
  void handleRequest(org::esb::core::Request * req, org::esb::core::Response*res);
private:

};
REGISTER_HOOK("web.api.Service", Pufs, Pufs::handleRequest, 1);
#endif	/* PUFS_H */

