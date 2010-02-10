#ifndef JOBWATCHER_H_
#define JOBWATCHER_H_

#include "org/esb/lang/Runnable.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include "org/esb/hive/job/JobHandler.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include <vector>
#include "org/esb/util/Log.h"

using namespace org::esb::lang;
using namespace org::esb::sql;
using namespace org::esb::hive::job;


namespace org{
	namespace esb{
		namespace hive{
			namespace job{
				class JobWatcher :public Runnable, public org::esb::signal::MessageListener{
					classlogger("hive.JobWatcher")
					public:
						JobWatcher(JobHandler & handler);
						void run();
						void onMessage(org::esb::signal::Message & msg);
					private:
						static int jobs(void *NotUsed, int argc, char **argv, char **azColName);
						bool _isStopSignal;
//						Connection  _con;
//						Statement  *_stmt;
						//int prev_job_id;
						static Job * job;
						
				};
			}
		}
	}
}

#endif /*JOBWATCHER_H_*/
