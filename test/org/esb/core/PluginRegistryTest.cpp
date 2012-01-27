/* 
 * File:   PluginRegistryTest.cpp
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 14:54
 */

#include <cstdlib>
#include "org/esb/lang/SharedObjectLoader.h"
#include "org/esb/lang/NotFoundException.h"
#include "org/esb/util/Log.h"
#include "org/esb/io/File.h"

using namespace std;
using namespace org::esb::lang;

/*
 * 
 */
int main(int argc, char** argv) {
  typedef void (*fptr)(int);
  fptr func;
  //typedef void (*fptr)();
  //typedef myfptr fptr(int);
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
    {
    SharedObjectLoader loader(f.getParent() + "/libplugin." + ext);
    }
    //func = (fptr) loader.getFunctionHandle("initPlugin");
    //func(1);
  } catch (NotFoundException & ex) {
    LOGERROR("catch error:" << ex.what());
  }catch(std::exception & ex){
    LOGERROR("catch error:" << ex.what());
  }catch(...){
    LOGERROR("unknown catch error:");
  }
  Log::close();
  return 0;
}

