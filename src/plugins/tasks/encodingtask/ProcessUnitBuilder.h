/* 
 * File:   ProcessUnitBuilder.h
 * Author: HoelscJ
 *
 * Created on 16. Juli 2010, 17:10
 */

#ifndef PROCESSUNITBUILDER_H
#define	PROCESSUNITBUILDER_H
#include "StreamData.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "boost/shared_ptr.hpp"
#include<map>
namespace encodingtask {

  class ProcessUnitBuilder {
  public:
    ProcessUnitBuilder(map<int, StreamData>&);
    ProcessUnitBuilder(const ProcessUnitBuilder&);
    virtual ~ProcessUnitBuilder();
    boost::shared_ptr<org::esb::hive::job::ProcessUnit> build(org::esb::av::PacketListPtr);
  private:
    map<int, StreamData> & _map_data;
  };

}

#endif	/* PROCESSUNITBUILDER_H */

