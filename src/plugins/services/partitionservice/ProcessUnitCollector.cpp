/* 
 * File:   ProcessUnitCollector.cpp
 * Author: HoelscJ
 * 
 * Created on 27. Oktober 2011, 16:07
 */

#include "ProcessUnitCollector.h"
#include "org/esb/util/UUID.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/File.h"
namespace partitionservice {

  ProcessUnitCollector::ProcessUnitCollector(std::string path) {
    _directory = path;
    org::esb::io::File dir(_directory.c_str());
    if (!dir.exists()) {
      dir.mkdir();
    }
  }

  ProcessUnitCollector::~ProcessUnitCollector() {
  }

  void ProcessUnitCollector::putProcessUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit > unit) {
    std::string name = _directory;
    name += "/";
    std::string uuid = org::esb::util::PUUID();
    name += uuid;

    org::esb::io::File out(name.c_str());
    org::esb::io::FileOutputStream fos(&out);
    org::esb::io::ObjectOutputStream ous(&fos);

    ous.writeObject(unit);
    ous.close();
  }
}
