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
list<Job*> JobHandler::getJobList(){
	return _jobList;
}

Job * JobHandler::getJob(){
//	boost::mutex::scoped_lock scoped_lock(m_mutex);
        list<Job*>::iterator i;
        for(i=_jobList.begin();i!=_jobList.end();++i) {
            Job *tmp=(Job*)*i;
	    	if(tmp->getCompleteTime()==0){
				return tmp;
	    	}
        }
        for(i=_jobList.begin();i!=_jobList.end();++i) {
            Job *tmp=(Job*)*i;
	    	if(tmp->getCompleteTime()==0&&tmp->_stream_type==CODEC_TYPE_AUDIO){
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
        	job.activate();
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
