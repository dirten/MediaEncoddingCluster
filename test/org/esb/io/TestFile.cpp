#include "org/esb/io/File.h"

int main(){
  org::esb::io::File f("Makefile");
  org::esb::io::File dir("../lang");
  assert(f.exists());
  logdebug(f.getPath());
  assert(f.getPath().length()>0);
  assert(f.getFileName().length()>0);
  assert(f.getFilePath().length()>0);
  assert(f.canRead());
//canWrite is currently not implemented correctly
  //  assert(f.canWrite()==true);
  assert(f.isFile());
  assert(!f.isDirectory());

  assert(!dir.isFile());
  assert(dir.isDirectory());

  logdebug(dir.getPath());
  logdebug(dir.getParent());
  logdebug(f.getParent());

//  f.getExtension();
  return 0;
}
