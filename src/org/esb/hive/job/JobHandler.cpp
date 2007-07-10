#include "JobHandler.h"
#include "Job.h"
#include "org/esb/io/FileInputStream.h"
#include <iostream>
using namespace org::esb::hive::job;
using namespace org::esb::io;

JobHandler * JobHandler::_handler=0;

JobHandler::JobHandler(){
    FileInputStream p("data/test.job");
    _props.load(&p);
    _testJob=new Job(_props);

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutexattr_destroy(&attr);

    cout << "Create JobHandler"<<endl;
}

Job * JobHandler::getJob(){
    return _testJob;
}

JobHandler * JobHandler::getInstance(){

    if(_handler==NULL){
		JobHandler::_handler=new JobHandler();
    }
	
    return _handler;

}
