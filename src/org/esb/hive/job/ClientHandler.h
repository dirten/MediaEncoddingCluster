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
	bool getProcessUnit(ProcessUnit & unit);
	bool putProcessUnit(ProcessUnit & unit);
    private:
		ProcessUnit *  unit;
		JobHandler * _handler;
};
}}}}

