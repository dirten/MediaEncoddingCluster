#include "JobDetail.h"


using namespace org::esb::hive::job;

JobDetail::JobDetail(){
}

int JobDetail::getId(){return _id;}
int JobDetail::getSourceStream(){return _source;}		
int JobDetail::getTargetStream(){return _target;}
void JobDetail::setId(int id){_id=id;}
void JobDetail::setSourceStream(int source){_source=source;}
void JobDetail::setTargetStream(int target){_target=target;}	
