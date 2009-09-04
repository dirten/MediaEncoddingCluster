#include "org/esb/lang/Exception.h"
#include <iostream>
using namespace org::esb::lang;
int main(){
  try{
  throw new Exception(__FILE__,__LINE__,"test trace Exception");
  }catch(Exception * ex){
    std::cout << ex->what()<<std::endl;
  }
}
