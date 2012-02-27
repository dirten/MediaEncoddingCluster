#include "ProcessUnitList.h"
namespace encodingtask{

  ProcessUnitList::ProcessUnitList()
  {
    lastSequence=0;
  }

  void ProcessUnitList::pushUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit> unit)
  {
    _unit_list.insert(unit);
    while(_unit_list.size()&&(*_unit_list.begin())->_sequence==lastSequence){
      std::cerr<<"lastSequence found:"<<lastSequence<<std::endl;
      if(_func)
        _func((*_unit_list.begin()));
      /*foreach(boost::shared_ptr<Packet>  p,(*_unit_list.begin())->_output_packets){
        Task::pushBuffer(Ptr<Packet>(p));
      }*/
      _unit_list.erase(_unit_list.begin());
      lastSequence++;
    }
  }

  void ProcessUnitList::addCallback(boost::function<void (boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)> func)
  {
    _func=func;
  }
}
