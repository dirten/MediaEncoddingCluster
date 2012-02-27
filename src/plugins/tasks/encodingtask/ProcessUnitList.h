#ifndef PROCESSUNITLIST_H
#define PROCESSUNITLIST_H
#include <set>
#include "org/esb/hive/job/ProcessUnit.h"
#include "boost/function.hpp"
namespace encodingtask{
  struct classcomp {
    bool operator() (boost::shared_ptr<org::esb::hive::job::ProcessUnit> l, boost::shared_ptr<org::esb::hive::job::ProcessUnit> r) const
    {return l->_sequence<r->_sequence;}
  };
  class ProcessUnitList
  {
  public:
    ProcessUnitList();
    void pushUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit>);
    void addCallback( boost::function<void (boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)> func);

  private:
    boost::function<void (boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)> _func;
    std::set<boost::shared_ptr<org::esb::hive::job::ProcessUnit>, classcomp> _unit_list;
    int lastSequence;
  };
}
#endif // PROCESSUNITLIST_H
