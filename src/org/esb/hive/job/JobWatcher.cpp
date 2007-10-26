
#include "JobWatcher.h"
#include "JobHandler.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/sql/Connection.h"

using namespace std;
using namespace org::esb::io;
using namespace org::esb::lang;
using namespace org::esb::hive::job;
JobHandler * _handler=NULL;
//vector<Job*> _jobList;
/*
int jobs(void *NotUsed, int argc, char **argv, char **azColName){
	if(atoi(argv[0])>0&&atoi(argv[0])!=prev_job_id){
		job=new Job();
		job->setId(atoi(argv[0]));
		File *source=new File(argv[1]);
		File *target=new File(argv[2]);
		job->setSourceFile(*source);
		job->setTargetFile(*target);
	}
	JobDetail * detail=new JobDetail();
	detail->setId(atoi(argv[5]));
	detail->setSourceStream(atoi(argv[7]));
	detail->setTargetStream(atoi(argv[8]));
	job->addJobDetails(*detail);
	if(atoi(argv[0])>0&&atoi(argv[0])!=prev_job_id){
		bool isAdded=_handler->addJob(*job);
	}
	prev_job_id=atoi(argv[0]);
  return 0;
}
*/

JobWatcher::JobWatcher(JobHandler & handler){
	_handler=&handler;
	_isStopSignal=false;
	File file("./test.db2");
	_con=new Connection(file);
	_stmt=&_con->createStatement();
}
int JobWatcher::jobs(void *NotUsed, int argc, char **argv, char **azColName){
	if(atoi(argv[0])>0&&atoi(argv[0])!=prev_job_id){
		job=new Job();
		job->setId(atoi(argv[0]));
		File *source=new File(argv[1]);
		File *target=new File(argv[2]);
		job->setSourceFile(*source);
		job->setTargetFile(*target);
	}
	JobDetail * detail=new JobDetail();
	detail->setId(atoi(argv[5]));
	detail->setSourceStream(atoi(argv[7]));
	detail->setTargetStream(atoi(argv[8]));
	job->addJobDetails(*detail);
	if(atoi(argv[0])>0&&atoi(argv[0])!=prev_job_id){
		bool isAdded=_handler->addJob(*job);
	}
	prev_job_id=atoi(argv[0]);
  return 0;


}
void JobWatcher::run(){
	while(!_isStopSignal){
			cout << "JobWatcher run"<<endl;
			_stmt->executeQuery("select * from jobs, job_details where jobs.id=job_details.job_id and complete is null order by jobs.id", (void *)JobWatcher::jobs);
			Thread::sleep(10000);
	}
}


