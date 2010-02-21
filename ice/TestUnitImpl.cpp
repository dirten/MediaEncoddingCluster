#include "TestUnitImpl.h"
#include <iostream>


testclass::operate(){
  for(int a=0;a<100000;a++){
    if(a%1000==0)cout << "line"<<a<<endl;
  }
}

testclass testfactoryimpl::getTestClass(){

}