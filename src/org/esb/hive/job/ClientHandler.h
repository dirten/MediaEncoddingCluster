#include <vector>
#include "ProcessUnit.h"
#include "JobHandler.h"
#include <boost/thread/mutex.hpp>
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/util/Log.h"
#include <list>
#include <boost/shared_ptr.hpp>
using namespace std;
using namespace org::esb;
namespace org{
namespace esb{
namespace hive{
namespace job{
class ClientHandler{
    public:
	ClientHandler();
//	bool getProcessUnit(ProcessUnit & unit);
	ProcessUnit getProcessUnit();
	ProcessUnit getProcessUnit2();
	bool putProcessUnit(ProcessUnit & unit);
	static bool addProcessUnit(boost::shared_ptr<ProcessUnit> unit);
    private:
		logger("hive.clienthandler");
    	void fillProcessUnit(ProcessUnit * pu);
		ProcessUnit *  unit;
		JobHandler * _handler;
		static boost::mutex m_mutex;
		static boost::mutex unit_list_mutex;
		sql::Connection * _con;
		sql::PreparedStatement * _stmt;
		sql::PreparedStatement * _stmt_fr;
		sql::PreparedStatement * _stmt_pu;
		sql::PreparedStatement * _stmt_ps;
//		sql::PreparedStatement _stmt_test;
		static map<int,boost::shared_ptr<ProcessUnit> > process_unit_list;
};
}}}}

