/* 
 * File:   DebugPrinter.cpp
 * Author: HoelscJ
 * 
 * Created on 5. Oktober 2011, 14:30
 */

#include "DebugPrinter.h"
#include "org/esb/util/Log.h"
DebugPrinter::DebugPrinter() {
}

void DebugPrinter::print(org::esb::core::Request*req, org::esb::core::Response*res) {
  LOGDEBUG("Hook called"<<typeid(req->getProperty("node").get()).name());
}


DebugPrinter::~DebugPrinter() {
}

