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
    
    void HiveControl::startup(){
	if(!isRunning){
	    isRunning=true;
	}else{
	    cout << "HiveControl Allready started";
	}
    }

    void HiveControl::shutdown(){
	if(isRunning){
	    isRunning=false;
	}else{
	    cout << "HiveControl Allready stopped";
	}
    }
