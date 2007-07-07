#include "HiveControl.h"
#include "HiveListener.cpp"
#include "org/esb/lang/Exception.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Properties.h"
//#include "hive.client.listener.cpp"
#include <iostream>
using namespace std;
using namespace org::esb::lang;
using namespace org::esb::util;
using namespace org::esb::hive;

    HiveControl * instance=0;

    HiveControl::HiveControl(){
	status=new Properties();
	status->setProperty("running","false");
    }

    HiveControl::~HiveControl(){
	cout<< "delete HiveControl Instance"<<endl;
	delete status;
	status=0;
	delete instance;
	instance=0;
    }

    HiveControl* HiveControl::getInstance(){
	if(instance==0){
	    instance=new HiveControl();
	}
	return instance;
    }
    
    bool HiveControl::startup(){
	bool result=false;
	if(strcmp(status->getProperty("running"),"false")==0){	    
        
	    listener=new HiveListener();
	    Thread * thread=new Thread(listener);
	    thread->start();
	    status->setProperty("running","true");
	    result=true;
        
	}else{
	    throw new Exception( __FILE__, __LINE__,"HiveControl is allready running");
	}
	return result;
    }

    bool HiveControl::shutdown(){
	bool result=false;
	if(strcmp(status->getProperty("running"),"true")==0){
	    delete listener;
	    status->setProperty("running","false");
	    result=true;
	}else{
	    throw Exception( __FILE__, __LINE__,"HiveControl is not running");
	}
	return result;
    }
    
    Properties * HiveControl::getStatus(){
	return status;
    }
    
