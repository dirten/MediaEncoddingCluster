#include "hivecontrol.h"
#include "org/esb/lang/Exception.h"
#include "org/esb/util/SimpleProperties.cpp"
#include <iostream>

using namespace std;
using namespace org::esb::lang;
using namespace org::esb::util;

    HiveControl * instance=0;

    HiveControl::HiveControl(){
	status=new SimpleProperties();
	status->setProperty("running","false");
    }

    HiveControl::~HiveControl(){
	cout<< "delete HiveControl Instance"<<endl;
	delete status;
	status=0;
    }

    HiveControl* HiveControl::getInstance(){
	if(instance==0){
	    cout << "Creating new instance"<<endl;
	    instance=new HiveControl();
	}
	return instance;
    }
    
    bool HiveControl::startup(){
	bool result=false;
	if(strcmp(status->getProperty("running"),"false")==0){
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
	    status->setProperty("running","false");
	    result=true;
	}else{
	    throw new Exception( __FILE__, __LINE__,"HiveControl is not running");
	}
	return result;
    }
    
    Properties * HiveControl::getStatus(){
	return status;
    }
    
