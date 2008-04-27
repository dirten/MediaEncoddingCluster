#include <vector>
#include "ProcessUnit.h"
#include "JobHandler.h"
#include <boost/thread/mutex.hpp>
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"

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
	bool putProcessUnit(ProcessUnit & unit);
    private:
		ProcessUnit *  unit;
		JobHandler * _handler;
		static boost::mutex m_mutex;
		sql::Connection * _con;
		sql::PreparedStatement * _stmt;
		sql::PreparedStatement * _stmt_fr;
};
}}}}

