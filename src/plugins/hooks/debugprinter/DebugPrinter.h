/* 
 * File:   DebugPrinter.h
 * Author: HoelscJ
 *
 * Created on 5. Oktober 2011, 14:30
 */

#ifndef DEBUGPRINTER_H
#define	DEBUGPRINTER_H
#include "org/esb/core/HookPlugin.h"
class DebugPrinter:public org::esb::core::HookPlugin {
public:
  DebugPrinter();
  void print(org::esb::core::Request*req,org::esb::core::Response*res);
  virtual ~DebugPrinter();
private:

};
REGISTER_HOOK("*", DebugPrinter,DebugPrinter::print, 1);
#endif	/* DEBUGPRINTER_H */

