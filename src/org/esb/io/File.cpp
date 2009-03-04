#include <iostream>
//#include <assert.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unistd.h>
//#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include "File.h"
#include "org/esb/lang/Exception.h"
//#include <boost/shared_ptr.hpp>
//#include "boost/filesystem/operations.hpp"
//#include "boost/filesystem/convenience.hpp"


using namespace std;
using namespace org::esb::lang;
using namespace org::esb::io;

//namespace fs = boost::filesystem;

File::File(const std::string pathname):_pathname(pathname) {
//  logdebug("FIle:"<<_pathname);

  int desc=open(pathname.c_str(), O_RDONLY) == 0;
  _exist=fstat(desc, &status) == 0;
  if (!_exist) {
    logdebug("status for file not found:"<<pathname);
    logdebug("errormsg:"<<strerror(errno));
    logdebug("PathMax:"<<PATH_MAX);

  }
}

File::~File() {
}

const string File::getExtension() {
  throw Exception(__FILE__, __LINE__, "File::getExtension not implemenhted");
//  return _pathname;
}

void File::changeExtension(const std::string & ext) {
  throw Exception(__FILE__, __LINE__, "File::changeExtension not implemenhted");
}

const string File::getPath() {
  return _pathname;
}

const string File::getFileName() {
  throw Exception(__FILE__, __LINE__, "File::getFileName not implemented");
}

const string File::getFilePath() {
  throw Exception(__FILE__, __LINE__, "File::getFilePath not implemented");
}

bool File::exists() {
  return _exist;
}

bool File::mkdir() {
  throw Exception(__FILE__, __LINE__, "File::mkdir not implemented");
}

bool File::isFile() {
  throw Exception(__FILE__, __LINE__, "File::isFile not implemented");
}

bool File::isDirectory() {
  throw Exception(__FILE__, __LINE__, "File::isDirectory not implemented");
}

bool File::canRead() {
  throw Exception(__FILE__, __LINE__, "File::canRead not implemented");
}

bool File::canWrite() {
  throw Exception(__FILE__, __LINE__, "File::canWrite not implemented");
}

/*
std::list < boost::shared_ptr < File > >File::listFiles (FileFilter & filter)
{
    fs::directory_iterator end_iter;
    std::list < boost::shared_ptr < File > >files;
    for ( fs::directory_iterator dir_itr( _full_path );dir_itr != end_iter;++dir_itr ){
      if(filter.accept(File(dir_itr->path().string().c_str()))){
            boost::shared_ptr < File > f (new File (dir_itr->path().string().c_str()));
            files.push_back (f);
          }
    }
    return files;
}

std::list < boost::shared_ptr < File > >File::listFiles ()
{
    fs::directory_iterator end_iter;
    std::list < boost::shared_ptr < File > >files;
    for ( fs::directory_iterator dir_itr( _full_path );dir_itr != end_iter;++dir_itr ){
            boost::shared_ptr < File > f (new File (dir_itr->path().string().c_str()));
            files.push_back (f);
    }
    return files;
}
 */


