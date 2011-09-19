/* 
 * File:   HookNotificationCenterTest.cpp
 * Author: HoelscJ
 *
 * Created on 31. August 2011, 12:42
 */

#include <cstdlib>
#include "org/esb/core/HookNotificationCenter.h"
#include "org/esb/core/HookPlugin.h"
#include <boost/bind.hpp>
#include <iostream>
#include "org/esb/core/Request.h"
#include "org/esb/core/Response.h"

using namespace std;
using namespace org::esb::core;
class TestReceiver:public org::esb::core::HookPlugin{
public:
  TestReceiver(){};
  ~TestReceiver(){};
  void hook(){};
  void hook2(void * data){std::cout << data<<std::endl;}
  
  void hook3(Request * req,Response*res){
  std::cout << req<<std::endl;
  std::cout << res<<std::endl;
  };
};



/*
 * 
 */
int main(int argc, char** argv) {
  org::esb::core::HookNotificationCenter *c=org::esb::core::HookNotificationCenter::getInstance();
  TestReceiver r;
  c->addObserver("test.hook", boost::bind(& TestReceiver::hook3, & r,_1,_2),1);
  
  c->postHook("test.hook",new Request(), new Response());
  
  return 0;
}

