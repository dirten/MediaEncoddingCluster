#ifndef ORG_ESB_HIVE_JOB_PROCESSUNITWATCHER_H
#define ORG_ESB_HIVE_JOB_PROCESSUNITWATCHER_H
#include "org/esb/lang/Runnable.h"
#include "org/esb/util/Log.h"

using namespace org::esb;
namespace org{
namespace esb{
namespace hive{
namespace job{

	class ProcessUnitWatcher: public lang::Runnable{
		logger("hive.ProcessUnitWatcher")
		public:
			ProcessUnitWatcher();
			void run();
		private:
			bool _isStopSignal;

	};

}}}}
#endif



