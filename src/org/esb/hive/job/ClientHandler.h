#include <vector>
#include "ProcessUnit.h"
#include "JobHandler.h"
#include <boost/thread/mutex.hpp>

using namespace std;
namespace org{
namespace esb{
namespace hive{
namespace job{
class ClientHandler{
    public:
	ClientHandler();
	bool getProcessUnit(ProcessUnit & unit);
	bool putProcessUnit(ProcessUnit & unit);
    private:
		ProcessUnit *  unit;
		JobHandler * _handler;
		mutable boost::mutex m_mutex;

};
}}}}

