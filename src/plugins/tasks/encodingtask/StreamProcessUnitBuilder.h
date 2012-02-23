/* 
 * File:   ProcessUnitBuilder.h
 * Author: HoelscJ
 *
 * Created on 16. Juli 2010, 17:10
 */

#ifndef STREAMPROCESSUNITBUILDER_H
#define	STREAMPROCESSUNITBUILDER_H
#include "StreamData.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "boost/shared_ptr.hpp"
#include<map>
namespace encodingtask {

  class StreamProcessUnitBuilder {
  public:
    StreamProcessUnitBuilder();
    virtual ~StreamProcessUnitBuilder();
    boost::shared_ptr<org::esb::hive::job::ProcessUnit> build(org::esb::av::PacketListPtr);
  private:
    //map<int, StreamData> & _map_data;
    double frameRateCompensateBase;
  };

}

#endif	/* PROCESSUNITBUILDER_H */

