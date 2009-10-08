#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"
int main(){
//	return 0;
  std::string path=MEC_SOURCE_DIR;
  
  org::esb::io::File f(path.append("/src/./CMakeLists.txt"));
  org::esb::io::File f2("./CMakeLists.txt");
  org::esb::io::File dir(f.getFilePath());
  assert(f.exists());
  logdebug(f.getPath());
  logdebug(f.getFilePath());
  assert(f.getPath().length()>0);
  assert(f.getFileName().length()>0);
  assert(f.getFilePath().length()>0);
  assert(f.canRead());
//canWrite is currently not implemented correctly
  //  assert(f.canWrite()==true);
//  assert(f.isFile());
  assert(!f.isDirectory());

  assert(!dir.isFile());
  assert(dir.isDirectory());

  logdebug("GetPath"<<dir.getPath());
  logdebug(dir.getFilePath()<<" GetParent"<<dir.getParent());
  logdebug("File GetParent"<<f.getParent());
  logdebug("Relative File . "<<f2.getPath());

//  f.getExtension();
  return 0;
}
