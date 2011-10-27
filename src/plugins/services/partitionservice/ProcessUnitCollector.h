/* 
 * File:   ProcessUnitCollector.h
 * Author: HoelscJ
 *
 * Created on 27. Oktober 2011, 20:07
 */

#ifndef PROCESSUNITCOLLECTOR_H
#define	PROCESSUNITCOLLECTOR_H
#include <boost/shared_ptr.hpp>
#include "org/esb/hive/job/ProcessUnit.h"
namespace partitionservice {

  class ProcessUnitCollector {
  public:
    ProcessUnitCollector(std::string );
    virtual ~ProcessUnitCollector();
    void putProcessUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit >);
  private:
    std::string _directory;
  };
}
#endif	/* PROCESSUNITCOLLECTOR_H */

