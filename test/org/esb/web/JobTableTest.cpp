/* 
 * File:   JobTableTest.cpp
 * Author: HoelscJ
 *
 * Created on 13. August 2010, 17:40
 */

#include <cstdlib>
#include "org/esb/web/JobTable.h"
#include "org/esb/web/WebServer.h"
#include "org/esb/lang/CtrlCHitWaiter.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
 org::esb::web::WebServer * w;

  w= new org::esb::web::WebServer();
  w->start();
  org::esb::web::JobTable* table=new org::esb::web::JobTable();

  if(argc>1)
    org::esb::lang::CtrlCHitWaiter::wait();

  w->stop();
  delete w;


  //delete table;
  return 0;
}

