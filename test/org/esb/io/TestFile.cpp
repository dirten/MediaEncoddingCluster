#include "org/esb/io/File.h"

int main(){
  org::esb::io::File f("Makefile");
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

//  f.getExtension();
  return 0;
}
