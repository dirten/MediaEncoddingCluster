#ifndef ORG_ESB_HIVE_HIVELISTENER_H
#define ORG_ESB_HIVE_HIVELISTENER_H

#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include "org/esb/lang/Runnable.h"
#include <boost/thread.hpp>
namespace org{
namespace esb{
namespace hive{
class HiveListener:public org::esb::signal::MessageListener, public org::esb::lang::Runnable {
	public:
		HiveListener();
		~HiveListener();
		void onMessage(org::esb::signal::Message & msg);
		void run();
		void startListener();
	private:
		bool main_nextloop;
		bool is_running;
		boost::thread *t;
};
}}}
#endif

