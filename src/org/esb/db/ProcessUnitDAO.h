#ifndef PROCESSUNITDAO_H
#define PROCESSUNITDAO_H
#include "hivedb.hpp"

namespace org{
  namespace esb{
    namespace db{

      using ::db::ProcessUnit;
      class ProcessUnitDAO
      {
        public:
          static ProcessUnit getProcessUnitById(int id);
          static void save(ProcessUnit & pu);
        private:
          ProcessUnitDAO();
      };
    }
  }
}

#endif // PROCESSUNITDAO_H
