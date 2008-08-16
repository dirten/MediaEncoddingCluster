
using namespace org::esb::config;
using namespace std;




bool checkConfigKey(char * key){
	
    
    if(Config::getProperty(key)==NULL||strlen(Config::getProperty(key))==0){
		cout << key << " not set in config file!!!" <<endl;
	}
	return true;
}

bool checkEnvironment(){
	bool result=true;
	cout << "Checking Environment before startup!!!" << endl;

	result=checkConfigKey("protocol.listener.port");
	result=checkConfigKey("data.dir");
	result=checkConfigKey("data.file");
	cout << "Environment seems to be ";
	if(result)
	    cout << "clean."<<endl;
	else
	    cout << "dirty - check config please!!!"<<endl;
	    
	return result;
}
