//#include "org/esb/hivecontrol/ClientData.h"
#include "org/esb/lang/Runnable.h"

using namespace org::esb::lang;
namespace org{
    namespace esb{
	namespace hive{
	    class ClientHandler: public Runnable{
		public:
		    ClientHandler();
		    void run();
	    };
	}
    }
}


