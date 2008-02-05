
#include "JobWatcher.h"
#include "JobHandler.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"

using namespace std;
using namespace org::esb::io;
using namespace org::esb::lang;
using namespace org::esb::hive::job;
JobHandler * _handler=NULL;

JobWatcher::JobWatcher(JobHandler & handler){
	_handler=&handler;
	_isStopSignal=false;
}


void JobWatcher::run(){
	while(!_isStopSignal){
	    cout << "JobWatcher cycle"<<endl;
	    /**
	    * @TODO Path entries must come from the Configuration
	    */
	    Connection con("/tmp/hive.db");
	    Statement stmt=con.createStatement("select id,infile,outfile from jobs where complete is null order by id");
	    Statement stmt_detail=con.createStatement("select id,instream,outstream from job_details where job_id=?");
	    ResultSet rs=stmt.executeQuery();

	    while(rs.next()){
		try{
		    if(_handler->getJob(rs.getint(0))!=NULL)continue;
		    Job * job=new Job();
		    job->setId(rs.getint(0));
		    File *source=new File(rs.getstring(1).c_str());
		    File *target=new File(rs.getstring(2).c_str());
		    job->setSourceFile(*source);
		    job->setTargetFile(*target);

		    stmt_detail.bind(1,rs.getint(0));
		    ResultSet rs_d=stmt_detail.executeQuery();
		    while(rs_d.next()){
			JobDetail * detail=new JobDetail();
			detail->setId(rs_d.getint(0));
			detail->setSourceStream(rs_d.getint(1));
			detail->setTargetStream(rs_d.getint(2));
			job->addJobDetails(*detail);
		    }
		    bool isAdded=_handler->addJob(*job);
		}catch(exception&ex){
		    cout << __FILE__<<":"<<__LINE__<<":"<<ex.what()<<endl;
		}
	    }
    	    Thread::sleep(10000);
	}
}


