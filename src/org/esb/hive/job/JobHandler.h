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
	JobHandler();
	list<Job*>_jobList;
	Job * _testJob;
    public:
	static JobHandler * getInstance();
	Job * getJob();
	Properties _props;
};
}}}}
#endif
