#include <vector>
#include "ProcessUnit.h"
#include "JobHandler.h"
#include <boost/thread/mutex.hpp>
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Queue.h"
#include <list>
#include <boost/shared_ptr.hpp>
using namespace std;
using namespace org::esb;
namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        class ClientHandler {
        public:
          ClientHandler();
          ~ClientHandler();
          //	bool getProcessUnit(ProcessUnit & unit);
          ProcessUnit getProcessUnit();
          ProcessUnit getProcessUnit2();
          ProcessUnit getProcessUnit3();
          bool putProcessUnit(ProcessUnit & unit);
          static bool addProcessUnit(boost::shared_ptr<ProcessUnit> unit);
        private:
          logger("hive.clienthandler");
          void fillProcessUnit();
          ProcessUnit * unit;
          JobHandler * _handler;
          static boost::mutex m_mutex;
          static boost::mutex unit_list_mutex;
          sql::Connection * _con;
          sql::Connection * _con2;
          sql::Connection * _con3;
          sql::PreparedStatement * _stmt;
          sql::PreparedStatement * _stmt3;
          sql::PreparedStatement * _stmt_fr;
          sql::PreparedStatement * _stmt_pu;
          sql::PreparedStatement * _stmt_ps;
          sql::PreparedStatement * _stmt_job_log;
          sql::Statement * _stmt_ps2;
          //		sql::PreparedStatement _stmt_test;
          static map<int, boost::shared_ptr<ProcessUnit> > process_unit_list;
          static util::Queue<boost::shared_ptr<ProcessUnit> > puQueue;
        };
      }
    }
  }
}

