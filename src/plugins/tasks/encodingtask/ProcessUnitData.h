/* 
 * File:   ProcessUnitData.h
 * Author: HoelscJ
 *
 * Created on 31. Oktober 2011, 12:04
 */

#ifndef PROCESSUNITDATA_H
#define	PROCESSUNITDATA_H
#include "org/esb/av/TimeStamp.h"
#include "org/esb/av/Encoder.h"
#include <boost/shared_ptr.hpp>
namespace encodingtask {

  class ProcessUnitData {
  public:
    ProcessUnitData();
    virtual ~ProcessUnitData();
    std::string id;
    int stream;
    boost::shared_ptr<org::esb::av::Encoder>encoder;
    int _sequence;
    org::esb::av::TimeStamp startts;
    bool operator==(const ProcessUnitData&a)const;
    bool operator<(const ProcessUnitData&a)const;
    bool operator>(const ProcessUnitData&a)const;

  private:

  };
}
#endif	/* PROCESSUNITDATA_H */

