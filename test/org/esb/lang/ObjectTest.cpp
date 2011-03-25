/* 
 * File:   ObjectTest.cpp
 * Author: HoelscJ
 *
 * Created on 10. März 2011, 11:38
 */

#include <cstdlib>
#include "org/esb/lang/Object.h"
using namespace std;
using namespace org::esb::lang;

class TestObject : public Object<TestObject> {
};

/*
 * 
 */
int main(int argc, char** argv) {
  TestObject * obj=new TestObject();
  return 0;
}

