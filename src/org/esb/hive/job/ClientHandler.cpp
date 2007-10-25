#include "ClientHandler.h"
#include "ProcessUnit.h"
#include "JobHandler.h"
#include <vector>

using namespace std;
using namespace org::esb::hive::job;

ClientHandler::ClientHandler(){
	_handler=JobHandler::getInstance();
}

ProcessUnit & ClientHandler::getProcessUnit(){
	Job * job=_handler->getJob();
	return unit;
}


void ClientHandler::setPacketList(vector<void*>list){


}


