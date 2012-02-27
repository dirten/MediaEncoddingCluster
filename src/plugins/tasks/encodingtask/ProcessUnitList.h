#ifndef PROCESSUNITLIST_H
#define PROCESSUNITLIST_H
#include <set>
#include "org/esb/hive/job/ProcessUnit.h"
namespace encodingtask{
  struct classcomp {
    bool operator() (boost::shared_ptr<org::esb::hive::job::ProcessUnit> l, boost::shared_ptr<org::esb::hive::job::ProcessUnit> r) const
    {return l->_sequence<r->_sequence;}
  };
  class ProcessUnitList
  {
  public:
    ProcessUnitList();
  private:
    std::set<boost::shared_ptr<org::esb::hive::job::ProcessUnit>, classcomp> _unit_list;

  };
}
#endif // PROCESSUNITLIST_H
