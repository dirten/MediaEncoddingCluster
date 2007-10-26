#ifndef JOBWATCHER_H_
#define JOBWATCHER_H_

#include "org/esb/lang/Runnable.h"
#include "org/esb/hive/job/JobHandler.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include <vector.h>

using namespace org::esb::lang;
using namespace org::esb::sql;
using namespace org::esb::hive::job;


namespace org{
	namespace esb{
		namespace hive{
			namespace job{
				class JobWatcher :public Runnable{
					public:
						JobWatcher(JobHandler & handler);
						void run();
					private:
						int jobs(void *NotUsed, int argc, char **argv, char **azColName);
						bool _isStopSignal;
						Connection * _con;
						Statement  *_stmt;
						int prev_job_id;
						Job * job;
						
				};
			}
		}
	}
}

#endif /*JOBWATCHER_H_*/
