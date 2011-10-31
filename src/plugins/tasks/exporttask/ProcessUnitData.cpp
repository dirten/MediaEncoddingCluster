/* 
 * File:   ProcessUnitData.cpp
 * Author: HoelscJ
 * 
 * Created on 31. Oktober 2011, 12:04
 */

#include "ProcessUnitData.h"
#include "org/esb/util/Log.h"
namespace plugin {

  ProcessUnitData::ProcessUnitData() {
    _sequence=0;
  }

  ProcessUnitData::~ProcessUnitData() {
  }

  bool ProcessUnitData::operator==(const ProcessUnitData&a)const {
    return _sequence==a._sequence;
  }

  bool ProcessUnitData::operator<(const ProcessUnitData&a)const {
    return _sequence<a._sequence;
  }

  bool ProcessUnitData::operator>(const ProcessUnitData&a)const {
    return _sequence>a._sequence;
  }

}
