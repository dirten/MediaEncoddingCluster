#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"
int main(){
  Log::open("");
//	return 0;
  std::string path=MEC_SOURCE_DIR;

  
  org::esb::io::File f(path.append("/src/./CMakeLists.txt"));
  org::esb::io::File f2("./CMakeLists.txt");
  org::esb::io::File dir(f.getFilePath());
  org::esb::io::File create("test.create");

  create.createNewFile();
  assert(create.exists());
  create.deleteFile();
  assert(!create.exists());

  assert(f.exists());
  LOGDEBUG(f.getPath());
  LOGDEBUG(f.getFilePath());
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

  LOGDEBUG("GetPath"<<dir.getPath());
  LOGDEBUG(dir.getFilePath()<<" GetParent"<<dir.getParent());
  LOGDEBUG("File GetParent"<<f.getParent());
  LOGDEBUG("Relative File . "<<f2.getPath());

//  f.getExtension();
  Log::close();
  return 0;
}
