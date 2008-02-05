#include "Job.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameInputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include <vector>
#include <iostream>
#include <list>

using namespace std;
using namespace org::esb::hive::job;
using namespace org::esb::sql;
using namespace org::esb::av;
using namespace org::esb::io;

//int Job::_frame_group=0;
//ProcessUnit*Job::_unit=0;

//queue<ProcessUnit*> Job::_unit_queue;



class JobProcess:public Runnable{
    public:
	JobProcess(Job*job){
	    _job=job;
	    _frame_group=0;
	}
	void run(){
	    File file("/tmp/hive.db");
	    Connection con(file);
	    Statement stmt=con.createStatement("select * from packets where frame_group=?");
	    while(true){
		if(_job->_unit_queue.size()<100){
    		    for(int a =0;a<20;a++){
    			stmt.bind(1,_frame_group);
        		ResultSet rs=stmt.executeQuery();
        		ProcessUnit * u=new ProcessUnit();
        		while(rs.next()){
        		
        		}
        		_job->_unit_queue.push(u);
        		_frame_group++;
		    }
		}
		cout << "ProcessCount"<<_job->getId()<<":"<<_job->_unit_queue.size()<<endl;
		Thread::sleep(500);
	    }	    
	}
    private:
	int _frame_group;
	Job*_job;
};

Job::Job(){
    Thread * runner=new Thread(new JobProcess(this));
    runner->start();
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
}

bool Job::getNextProcessUnit(ProcessUnit & unit){
    {
	boost::mutex::scoped_lock scoped_lock(m_mutex);
	bool result=false;
	if(_unit_queue.size()>0){
	    unit = *_unit_queue.front();
	    _unit_queue.pop();
	    result=true;
	}
	return result;
    }
}

