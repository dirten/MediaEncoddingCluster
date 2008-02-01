#include "Job.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameInputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include <vector>
#include <iostream>
#include <list>

using namespace std;
using namespace org::esb::hive::job;
using namespace org::esb::av;
using namespace org::esb::io;

int Job::_frame_group=0;
ProcessUnit*Job::_unit=0;
int Job::process(void *NotUsed, int argc, char **argv, char **azColName){
    


    return 0;
}

Job::Job(){

}

Job::~Job(){
}

File & Job::getSourceFile(){return *_source;}
File & Job::getTargetFile(){return *_target;}
void Job::setSourceFile(File & source){_source=&source;}
void Job::setTargetFile(File & target){_target=&target;}

int Job::getStartTime(){return _startTime;}
int Job::getCompleteTime(){return _completeTime;}
void Job::setStartTime(int start){_startTime=start;}
void Job::setCompleteTime(int complete){_completeTime=complete;}
void Job::setId(int id){_id=id;}
int Job::getId(){return _id;}


void Job::addJobDetails(JobDetail & detail){
        list<JobDetail*>::iterator i;
        _detailList.push_back(&detail);
        cout << "JobDetail with ID added:"<<detail.getId()<<endl;
}

ProcessUnit * Job::getNextProcessUnit(){
    {
	_unit = new ProcessUnit();
	/**
	* @TODO Path entries must come from the Configuration
	*/
	File file("/tmp/hive.db");
	Connection con(file);
	Statement stmt=con.createStatement();
	string sql="select * from packets where frame_group=";
	sql+=Job::_frame_group;
        cout << sql.c_str()<<endl;

        stmt.executeQuery(sql.c_str(), (void *)process);
        Job::_frame_group++;
        return _unit;
    }
}
