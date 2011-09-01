/* 
 * File:   ObjectTest.cpp
 * Author: HoelscJ
 *
 * Created on 10. März 2011, 11:38
 */

#include <cstdlib>
#include <iostream>
class TestSharedObject  {
public:
  TestSharedObject(){
    
  }
  void loadedFunction(){
    
  }
};

  #if defined(_WIN32)
      #define DLL_EXPORT __declspec( dllexport )
  #else
    #define DLL_EXPORT
  #endif

#ifdef	__cplusplus
extern "C" {
#endif
  void DLL_EXPORT initPlugin(unsigned int d){
    std::cout<<"init plugin:"<<d<<std::endl;
  }
#ifdef	__cplusplus
}
#endif

/*
 * 
 */
int main(int argc, char** argv) {

  return 0;
}

