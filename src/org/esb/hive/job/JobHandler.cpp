#include "JobHandler.h"
#include "Job.h"
#include "JobWatcher.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/lang/Thread.h"
#include <iostream>
using namespace org::esb::hive::job;
using namespace org::esb::io;

JobHandler * JobHandler::_handler=0;



JobHandler::JobHandler(){
//	JobWatcher *_watcher=new JobWatcher(*this);
//	Thread *runner=new Thread(_watcher);
//	runner->start();
	
}

Job * JobHandler::getJob(){
        list<Job*>::iterator i;
        for(i=_jobList.begin();i!=_jobList.end();++i) {
            Job *tmp=(Job*)*i;
			if(tmp->getCompleteTime()==NULL){
				return tmp;
			}
        }
    return NULL;
}

Job * JobHandler::getJob(int id){
        list<Job*>::iterator i;
        for(i=_jobList.begin();i!=_jobList.end();++i) {
            Job *tmp=(Job*)*i;
			if(tmp->getId()==id){
				return tmp;
			}
        }
        return NULL;
}

bool JobHandler::addJob(Job & job){
        list<Job*>::iterator i;
        bool toAdd=true, result=false;
        for(i=_jobList.begin();i!=_jobList.end();++i) {
            Job *tmp=(Job*)*i;
			if(tmp->getId()==job.getId()){
				toAdd=false;
			}
        }
        if(toAdd){
        	_jobList.push_back(&job);
        	cout << "Job with ID added:"<<job.getId()<<endl;
        	result=true;
        }
        return result;
}

JobHandler * JobHandler::getInstance(){
    if(_handler==NULL){
		JobHandler::_handler=new JobHandler();
    }
    return _handler;
}
