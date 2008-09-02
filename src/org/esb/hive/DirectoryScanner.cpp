#include "DirectoryScanner.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/io/FileFilter.h"

#include <boost/algorithm/string.hpp>
namespace org{
namespace esb{
namespace hive{
using namespace org::esb::io;
using namespace org::esb::sql;
class MyFileFilter:public FileFilter{
  public:
    bool accept(File file){
      return !(boost::starts_with(file.getPath(),"."));
    }
};

DirectoryScanner::DirectoryScanner(std::string dir, int interval){
  _halt=true;
  _interval=interval;
  _dir=dir;
//  _con=org::esb::sql::Connection(org::esb::config::Config::getProperty("db.connection"));
//  _stmt=_con.prepareStatement("select * from files");
}

DirectoryScanner::~DirectoryScanner(){
  delete th;

}

void DirectoryScanner::onMessage(org::esb::signal::Message & msg){
  if(msg.getProperty("directoryscan")=="start"){
    _halt=false;
    th=new boost::thread(boost::bind(&DirectoryScanner::scan, this));
    cout << "Directory Scanner running with interval:"<<_interval<<endl;
  }else
  if(msg.getProperty("directoryscan")=="stop"){
    _halt=true;
#if BOOST_VERSION > 103500
    th->interrupt();
#endif
    cout << "Directory Scanner stopped:"<<endl;
  }
}

void DirectoryScanner::scan(){
  while(!_halt){
    scan(_dir);
    org::esb::lang::Thread::sleep(_interval);
  }
}

void DirectoryScanner::scan(std::string dir){
    cout << "Directory Scanner loop:"<<":"<<dir<<endl;
    MyFileFilter filter;
    std::list<boost::shared_ptr< File > > list=File(dir.c_str()).listFiles();
    std::list<boost::shared_ptr< File > >::iterator it=list.begin();
    for(;it!=list.end();it++){
      if((*it)->isDirectory())
        scan((*it)->getPath());
      else
        computeFile(*it->get());
    }
}

void DirectoryScanner::computeFile(File file){
  Connection con(Connection(org::esb::config::Config::getProperty("db.connection")));
  PreparedStatement stmt(con.prepareStatement("select * from files where filename=:name and path=:path"));
  stmt.setString("name", file.getFileName());
  stmt.setString("path", file.getFilePath());
  ResultSet rs=stmt.executeQuery();
  if(!rs.next()){
    std::string sql("insert into files (filename, path) values (\""+file.getFileName()+"\",\""+file.getFilePath()+"\")");
    std::cout << sql <<std::endl;
    PreparedStatement pstmt=con.prepareStatement("insert into files (filename, path) values (:name,:path)");
/*
    std::string str=file.getFileName();
    int found=str.find_first_of("aeiou");
    while (found!=std::string::npos){
      str[found]='\'';
      found=str.find_first_of("'",found+1);
    }
    cout <<"Replace"<< str<<endl;
    */
    pstmt.setString("name", file.getFileName());
    pstmt.setString("path", file.getFilePath());
    pstmt.execute();
  }
}

}}}

