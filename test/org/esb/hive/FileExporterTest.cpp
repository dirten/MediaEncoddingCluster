/* 
 * File:   FileExporterTest.cpp
 * Author: HoelscJ
 *
 * Created on 15. November 2010, 13:58
 */

#include <cstdlib>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/hive/FileExporter.h"
#include "org/esb/util/Log.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  if(argc<4)
    std::cout << "usage : FileExporterTest outputfilepath<string> basepath<string> fileid<int> "<<std::endl;
  std::string filepath=argv[1];
  std::string path=argv[2];
  int fileid=atoi(argv[3]);

  org::esb::config::Config::setProperty("hive.base_path",path);
  Log::open("");
  org::esb::hive::DatabaseService::start(path);
  db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();
  db::MediaFile outfile=litesql::select<db::MediaFile>(db,db::MediaFile::Id==fileid).one();
  outfile.path=filepath;
  FileExporter::exportFile(outfile);
  return 0;
}

