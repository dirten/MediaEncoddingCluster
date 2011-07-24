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
#include "org/esb/net/TcpSocket.h"
#include "org/esb/net/TcpSocket.h"
#include "org/esb/av/AV.h"

using namespace std;
using namespace org::esb::lang;
using namespace org::esb::io;

namespace fs = boost::filesystem;
string char2hex( char dec )
{
    char dig1 = (dec&0xF0)>>4;
    char dig2 = (dec&0x0F);
    if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
    if (10<= dig1 && dig1<=15) dig1+=97-10; //a,97inascii
    if ( 0<= dig2 && dig2<= 9) dig2+=48;
    if (10<= dig2 && dig2<=15) dig2+=97-10;

    string r;
    r.append( &dig1, 1);
    r.append( &dig2, 1);
    return r;
}
string urlencode(const string &c)
{

    string escaped="";
    int max = c.length();
    for(int i=0; i<max; i++)
    {
        if ( (46 <= c[i] && c[i] <= 58) ||//0-9
             (65 <= c[i] && c[i] <= 90) ||//abc...xyz
             (97 <= c[i] && c[i] <= 122) || //ABC...XYZ
             (c[i]=='~' || c[i]=='!' || c[i]=='*' || c[i]=='(' || c[i]==')' || c[i]=='\'')
        )
        {
            escaped.append( &c[i], 1);
        }
        else
        {
            escaped.append("%");
            escaped.append( char2hex(c[i]) );//converts char 255 to string "ff"
        }
    }
    return escaped;
}

File::File(const std::string pathname) : _pathname(pathname) {
  _isUrl = false;
  if (pathname.find("http://") != string::npos) {
    std::cout << "URL given:" << pathname << std::endl;
    _pathname = pathname.substr(6);
    std::cout << "new PathName:" << _pathname << std::endl;
    _isUrl = true;
  }
  _full_path = fs::system_complete(fs::path(_pathname, fs::native)).normalize();
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
  if (_isUrl) {
    return std::string("http:/").append(urlencode(_pathname));
  }
  return _full_path.string();
}

const string File::getParent() {
  if (_isUrl)
    return std::string("http:/").append(_full_path.branch_path().string());
  else
    return _full_path.branch_path().string();
}

const string File::getFileName() {
  return _full_path.leaf();
}

const string File::getFilePath() {
  if (isDirectory())
    if (_isUrl)
      return std::string("http:/").append(urlencode(_full_path.string()));
    else
      return _full_path.string();
  else
    if (_isUrl)
    return std::string("http:/").append(_full_path.branch_path().string());
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
  if (_isUrl) {
    //result=true;
    result = http_url_exist(_pathname.substr(1));
  }
  return result;
}

bool File::deleteFile() {
  fs::remove(_full_path);
  return !fs::exists(_full_path);
}

bool File::mkdir() {
  if (_isUrl)
    return true;
  return fs::create_directory(_full_path);
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
  if(_isUrl)return true;
  return fs::is_regular(fs::status(_full_path));
}

bool File::canWrite() {
  bool result = false;
  FILE* fh = fopen(getPath().c_str(), "a");
  if (fh) {
    result = true;
    fclose(fh);
    deleteFile();
  }
  return result;
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

bool File::http_url_exist(std::string url) {
  bool result = false;
  try {
    std::string host = getFilePath().substr(7);
    host = host.substr(0, host.find("/"));
    host = host.substr(0, host.find(":"));

    LOGDEBUG("try connect to host:" << host);
    LOGDEBUG("getFilePath().substr(getFilePath().find(\"/\", 7)) von : "<<getFilePath())
    std::string url = getFilePath().substr(getFilePath().find("/", 7));
    LOGDEBUG("RulPath:" << urlencode(url))
    std::string port = "80";
    //LOGDEBUG("Port Index:"<<getFilePath().find(":", 7))
    if (getFilePath().find(":", 7) !=string::npos) {
      port = getFilePath().substr(getFilePath().find(":", 7) + 1);
      port = port.substr(0, port.find("/"));
    }
    LOGDEBUG("Port:" << port);
    org::esb::net::TcpSocket socket(host, atoi(port.c_str()));
    socket.connect();
    ostringstream request_stream;
    request_stream << "GET " << urlencode(url) << "/" << urlencode(getFileName()) << " HTTP/1.0\r\n";
    request_stream << "Host: " << host << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";
    LOGDEBUG("REQUEST:" << request_stream.str());
    std::string out = request_stream.str();
    socket.getOutputStream()->writeRaw(out);
    std::string data;
    socket.getInputStream()->readUntil(data, "");
    std::istringstream is(data);
    std::string header;
    std::string status;
    is >> header;
    is >> status;
    LOGDEBUG(header);
    LOGDEBUG(status);
    result = (status == "200" || status == "302");
  } catch (std::exception & ex) {
    LOGERROR(ex.what());
  }catch(...){
    LOGERROR("Unknown Exception")
  }
  return result;
}

size_t File::http_url_size(std::string url) {
	return 0;
}

