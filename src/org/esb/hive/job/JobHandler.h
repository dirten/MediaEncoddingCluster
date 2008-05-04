#ifndef ORG_ESB_HIVE_JOB_JOBHANDLER_H
#define ORG_ESB_HIVE_JOB_JOBHANDLER_H

#include "Job.h"
#include "org/esb/util/Log.h"
#include <list>
#include <boost/thread/mutex.hpp>
namespace org{
namespace esb{
namespace hive{
namespace job{
class JobHandler{
    private:
	logger("hive.jobhandler");
	static JobHandler * _handler;
	JobHandler();
	list<Job*>_jobList;
	Job * _testJob;
	mutable boost::mutex m_mutex;

    public:
	static JobHandler * getInstance();
	Job * getJob();
	list<Job*> getJobList();
	Job * getJob(int id);
	bool addJob(Job & job);
	Properties _props;
};
}}}}
#endif
