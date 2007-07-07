#include "JobHandler.h"
#include "Job.h"
#include "org/esb/io/FileInputStream.h"

using namespace org::esb::hive::job;
using namespace org::esb::io;

JobHandler * _handler=0;

JobHandler::JobHandler(){
    FileInputStream p("data/test.job");
    _props.load(&p);
    _testJob=new Job(_props);
}

Job * JobHandler::getJob(){
    return _testJob;

}

JobHandler * JobHandler::getInstance(){
    if(_handler==NULL){
	_handler=new JobHandler();
    }
    return _handler;
}
