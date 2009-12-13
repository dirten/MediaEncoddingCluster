#include <iostream>
#include <fstream>
//#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>
//#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include "File.h"
#include "org/esb/lang/Exception.h"
//#include <boost/shared_ptr.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/convenience.hpp"


using namespace std;
using namespace org::esb::lang;
using namespace org::esb::io;

namespace fs = boost::filesystem;

File::File(const std::string pathname) : _pathname(pathname) {
  _full_path = fs::system_complete(fs::path(pathname, fs::native)).normalize();
}

File::~File() {
}

const string File::getExtension() {
  //  throw Exception(__FILE__, __LINE__, "File::getExtension not implemenhted");
  return fs::extension(_full_path);
}

void File::changeExtension(const std::string & ext) {
  _full_path = fs::change_extension(_full_path, "." + ext);
}

const string File::getPath() {
  return _full_path.string();
}

const string File::getParent() {
  return _full_path.branch_path().string();
}

const string File::getFileName() {
  if (isFile())
    return _full_path.leaf();
  return _full_path.leaf();
}

const string File::getFilePath() {
  if (isDirectory())
    return _full_path.string();
  else
    if (isFile())
    return _full_path.branch_path().string();
  return _full_path.branch_path().string();
}

bool File::exists() {
  return fs::exists(_full_path);
}
bool File::deleteFile() {
  
  return fs::remove(_full_path);
}

bool File::mkdir() {
  return fs::create_directory(_full_path);
}

bool File::isFile() {
  return fs::is_regular(_full_path);
}

bool File::isDirectory() {
  return fs::is_directory(fs::status(_full_path));
}

bool File::canRead() {
  return fs::is_regular(fs::status(_full_path));
}

bool File::canWrite() {
  throw Exception(__FILE__, __LINE__, "File::canWrite not implemented");
  return false;
}

std::list < boost::shared_ptr < File > >File::listFiles(FileFilter & filter) {
  fs::directory_iterator end_iter;
  std::list < boost::shared_ptr < File > >files;
  for (fs::directory_iterator dir_itr(_full_path); dir_itr != end_iter; ++dir_itr) {
    if (filter.accept(File(dir_itr->path().string().c_str()))) {
      boost::shared_ptr < File > f(new File(dir_itr->path().string().c_str()));
      files.push_back(f);
    }
  }
  return files;
}

std::list < boost::shared_ptr < File > >File::listFiles() {
  fs::directory_iterator end_iter;
  std::list < boost::shared_ptr < File > >files;
  for (fs::directory_iterator dir_itr(_full_path); dir_itr != end_iter; ++dir_itr) {
    boost::shared_ptr < File > f(new File(dir_itr->path().string().c_str()));
    files.push_back(f);
  }
  return files;
}



