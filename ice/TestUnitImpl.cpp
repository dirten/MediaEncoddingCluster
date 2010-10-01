#include "TestUnitImpl.h"
#include <iostream>


void testclassimpl::operate(){
  for(int a=0;a<100000;a++){
    if(a%1000==0)std::cout << "line"<<a<<std::endl;
  }
}

testclassimpl * testfactoryimpl::getTestClass(){

}