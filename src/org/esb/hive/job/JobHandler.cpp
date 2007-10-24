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
	JobWatcher *_watcher=new JobWatcher(*this);
	Thread *runner=new Thread(_watcher);
	runner->start();
	
}

Job * JobHandler::getJob(){
    return _testJob;
}

void  JobHandler::addJob(Job & job){

}

JobHandler * JobHandler::getInstance(){
    if(_handler==NULL){
		JobHandler::_handler=new JobHandler();
    }
    return _handler;
}
