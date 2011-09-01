/* 
 * File:   ObjectTest.cpp
 * Author: HoelscJ
 *
 * Created on 10. März 2011, 11:38
 */

#include <cstdlib>
#include "org/esb/lang/SharedObjectLoader.h"
#include "org/esb/lang/NotFoundException.h"
#include "org/esb/util/Log.h"
#include "org/esb/io/File.h"
using namespace std;
using namespace org::esb::lang;

class Test{
public:
  Test(){
    std::cout << "hier bin ich"<<std::endl;
  }
}TestInstance;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  org::esb::io::File f(argv[0]);

  std::string path = MEC_SOURCE_DIR;

#ifdef __APPLE__ 
  std::string ext = "dylib";
#elif __LINUX__
  std::string ext = "so";
#elif __WIN32__
  std::string ext = "dll";
#endif

  try {
    SharedObjectLoader loader(f.getParent() + "/libshared_object." + ext);
    boost::function<void (int)> mfunc =  loader.getFunctionHandle<void (int)>("initPlugin");
    boost::function<void (std::string)> mfunc2 =  loader.getFunctionHandle<void (std::string)>("initPlugin");
    mfunc(1);
    //mfunc2("1test");
  } catch (NotFoundException & ex) {
    LOGERROR("catch error:" << ex.what());
  }
  Log::close();
  return 0;
}

