#include "ClientHandler.h"
#include "ProcessUnit.h"
#include "JobHandler.h"
#include <vector>

using namespace std;
using namespace org::esb::hive::job;

ClientHandler::ClientHandler(){
	_handler=JobHandler::getInstance();
}

bool ClientHandler::getProcessUnit(ProcessUnit & unit){
	Job * job=_handler->getJob();
	if(job !=NULL){
	    job->getNextProcessUnit(unit);
	    return true;
	}
	return false;
}

bool ClientHandler::putProcessUnit(ProcessUnit & unit){
    return true;
}



