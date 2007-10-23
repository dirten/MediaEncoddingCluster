#include "JobHandler.h"
#include "Job.h"
#include "org/esb/io/FileInputStream.h"
#include <iostream>
using namespace org::esb::hive::job;
using namespace org::esb::io;

JobHandler * JobHandler::_handler=0;

int callback(void *NotUsed, int argc, char **argv, char **azColName){

}

JobHandler::JobHandler(){
	
	
}

Job * JobHandler::getJob(){
	string sql="select * from jobs where complete is null limit 1";


    return _testJob;
}

JobHandler * JobHandler::getInstance(){
    if(_handler==NULL){
		JobHandler::_handler=new JobHandler();
    }
    return _handler;
}
