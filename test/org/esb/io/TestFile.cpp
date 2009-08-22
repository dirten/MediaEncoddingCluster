#include "org/esb/io/File.h"

int main(){
	return 0;
  std::string path=MEC_SOURCE_DIR;
  
  org::esb::io::File f(path);
  org::esb::io::File dir(path);
  assert(f.exists());
  logdebug(f.getPath());
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

  logdebug(dir.getPath());
  logdebug(dir.getParent());
  logdebug(f.getParent());

//  f.getExtension();
  return 0;
}
