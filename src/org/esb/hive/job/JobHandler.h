#ifndef ORG_ESB_HIVE_JOB_JOBHANDLER_H
#define ORG_ESB_HIVE_JOB_JOBHANDLER_H
#include "Job.h"
#include <list>
namespace org{
namespace esb{
namespace hive{
namespace job{
class JobHandler{
    private:
	static JobHandler * _handler;
	JobHandler();
	list<Job*>_jobList;
	Job * _testJob;
    pthread_mutex_t mutex;
    public:
	static JobHandler * getInstance();
	Job * getJob();
	Properties _props;
};
}}}}
#endif
