#ifndef ORG_ESB_HIVE_JOB_PROCESSUNITWATCHER_H
#define ORG_ESB_HIVE_JOB_PROCESSUNITWATCHER_H
#include "org/esb/lang/Runnable.h"
#include "org/esb/util/Log.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"


namespace org{
namespace esb{
namespace hive{
namespace job{

	class ProcessUnitWatcher: public org::esb::signal::MessageListener{
		logger("hive.ProcessUnitWatcher")
		public:
			ProcessUnitWatcher();
//			void run();
			void start();
			void stop();
			void onMessage(org::esb::signal::Message & msg);
		private:
			bool _isStopSignal;

	};

}}}}
#endif



