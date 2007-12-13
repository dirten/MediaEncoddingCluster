#include <vector>
#include "ProcessUnit.h"
#include "JobHandler.h"
using namespace std;
namespace org{
namespace esb{
namespace hive{
namespace job{
class ClientHandler{
    public:
	ClientHandler();
	ProcessUnit * getProcessUnit();
	void setPacketList(vector<void*>);
    private:
		ProcessUnit *  unit;
		JobHandler * _handler;
};
}}}}

