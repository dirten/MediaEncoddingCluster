#include "hivecontrol.h"
#include <iostream>
using namespace std;
    HiveControl * instance=new HiveControl();

    HiveControl::HiveControl(){
    
    
    }

    HiveControl::~HiveControl(){
    
    }

    HiveControl* HiveControl::getInstance(){
	if(instance==0){
	    instance=new HiveControl();
	}
	return instance;
    }
    
    bool HiveControl::startup(){
	bool result=false;
	if(!isRunning){
	    isRunning=true;
	    result=true;
	}else{
	    cout << "HiveControl Allready started";
	}
	return result;
    }

    bool HiveControl::shutdown(){
	bool result=false;
	if(isRunning){
	    isRunning=false;
	    result=true;
	}else{
	    cout << "HiveControl Allready stopped";
	}
	return result;
    }
