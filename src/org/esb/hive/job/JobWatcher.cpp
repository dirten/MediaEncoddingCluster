
#include "JobWatcher.h"
#include "JobHandler.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
using namespace std;
using namespace org::esb::io;
using namespace org::esb::config;
using namespace org::esb::lang;
using namespace org::esb::hive::job;
JobHandler * _handler=NULL;

JobWatcher::JobWatcher(JobHandler & handler){
	_handler=&handler;
	_isStopSignal=false;
}


void JobWatcher::run(){
	Connection con(Config::getProperty("db.connection"));

//	    Connection con("/tmp/hive.db");
//	    con.executenonquery("PRAGMA read_uncommitted=1");
	    Statement stmt=con.createStatement("select jobs.id, job_details.id as detailid ,inputfile, outputfile, instream, outstream from jobs, job_details where jobs.id=job_details.job_id and complete is null order by jobs.id");
//	    PreparedStatement stmt_detail=con.prepareStatement("select id,instream,outstream from job_details where job_id=?");
	while(!_isStopSignal){
	    logdebug("JobWatcher cycle");
	    ResultSet rs=stmt.executeQuery();

	    while(rs.next()){
//		try{
		    if(_handler->getJob(rs.getInt("detailid"))!=NULL)continue;

//		    stmt_detail.setInt(0,rs.getInt(0));
//		    ResultSet rs_d=stmt_detail.executeQuery();
//		    while(rs_d.next()){
			Job * job=new Job();
			job->setId(rs.getInt("detailid"));
			job->setSourceStream(rs.getInt("instream"));
			job->setTargetStream(rs.getInt("outstream"));
			_handler->addJob(*job);
//		    }
//		}catch(exception&ex){
//		    cout << __FILE__<<":"<<__LINE__<<":"<<ex.what()<<endl;
//		}
	    }
    	    Thread::sleep(60000);
	}
}


