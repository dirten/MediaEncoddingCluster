
#include "JobWatcher.h"
#include "JobHandler.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/sql/Connection.h"

using namespace org::esb::io;
using namespace org::esb::lang;
using namespace org::esb::hive::job;
JobHandler * _handler=NULL;
static int jobs(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	Job * job=new Job();
	File *source=new File(argv[1]);
	File *target=new File(argv[1]);
	job->setSourceFile(*source);
	job->setTargetFile(*target);
	_handler->addJob(*job);
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}


JobWatcher::JobWatcher(JobHandler & handler){
	_handler=&handler;
	_isStopSignal=false;
	File file("./test.db2");
	_con=new Connection(file);
	_stmt=&_con->createStatement();
}

void JobWatcher::run(){
	while(!_isStopSignal){
			cout << "JobWatcher run"<<endl;
			_stmt->executeQuery("select id, infile, outfile, begin, complete from jobs where complete is null", (void *)jobs);

			Thread::sleep(10000);
	}
}


