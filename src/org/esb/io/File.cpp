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
#include "boost/filesystem/exception.hpp"
//#include "org/esb/av/AV.h"

using namespace std;
using namespace org::esb::lang;
using namespace org::esb::io;

namespace fs = boost::filesystem;

File::File(const std::string pathname) : _pathname(pathname) {
  fs::path p=fs::path(_pathname, fs::native);
  _full_path = fs::system_complete(p).normalize();
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

long File::length() {
  return fs::file_size(_full_path);
}

const string File::getPath() {
  return _full_path.string();
}

const string File::getParent() {
    return _full_path.branch_path().string();
}

const string File::getFileName() {
  return _full_path.leaf();
}

const string File::getFilePath() {
  if (isDirectory())
      return _full_path.string();
  else
    return _full_path.branch_path().string();
}

bool File::exists() {
  bool result = false;
  try {
    result = fs::exists(_full_path);
  } catch (fs::filesystem_error & ex) {
    //return false;
  }
  return result;
}

bool File::deleteFile() {
  fs::remove(_full_path);
  return !fs::exists(_full_path);
}

bool File::mkdir() {
  return fs::create_directory(_full_path);
}

bool File::mkdirs() {
  return fs::create_directories(_full_path);
}

bool File::createNewFile() {
  bool result = false;
  FILE* fh = fopen(getPath().c_str(), "a");
  if (fh) {
    result = true;
    fclose(fh);
    //deleteFile();
  }
  return result;
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
  bool result = false;
  if(!exists()){
    FILE* fh = fopen(getPath().c_str(), "a");
    if (fh) {
      result = true;
      fclose(fh);
      deleteFile();
    }
  }
  return result;
}

void File::copyTo(File&trg){
  fs::copy_file(_full_path, trg._full_path);
}

void File::moveTo(File&trg){
  fs::copy_file(_full_path, trg._full_path);
  deleteFile();
}

FileList File::listFiles(FileFilter & filter) {
  fs::directory_iterator end_iter;
  FileList files;
  try {
    for (fs::directory_iterator dir_itr(_full_path); dir_itr != end_iter; ++dir_itr) {
      try {
        if (filter.accept(File(dir_itr->path().string().c_str()))) {
          Ptr< File > f(new File(dir_itr->path().string().c_str()));
          files.push_back(f);
        }
      } catch (const fs::filesystem_error & ex) {
        std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
      }
    }
  } catch (const fs::filesystem_error & ex) {
    std::cout << _full_path.filename() << " " << ex.what() << std::endl;
  }
  return files;
}

FileList File::listFiles() {
  fs::directory_iterator end_iter;
  FileList files;
  try {
    for (fs::directory_iterator dir_itr(_full_path); dir_itr != end_iter; ++dir_itr) {
      try {
        Ptr < File > f(new File(dir_itr->path().string().c_str()));
        files.push_back(f);
      } catch (const fs::filesystem_error & ex) {
        std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
      }
    }
  } catch (const fs::filesystem_error & ex) {
    std::cout << _full_path.filename() << " " << ex.what() << std::endl;
  }

  return files;
}

