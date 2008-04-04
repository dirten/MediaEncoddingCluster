#include <vector>
#include "ProcessUnit.h"
#include "JobHandler.h"
#include <boost/thread/mutex.hpp>
#include "tntdb/connection.h"
#include "tntdb/statement.h"

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
		tntdb::Connection * _con;
		tntdb::Statement * _stmt;
};
}}}}

