#include "org/esb/av/Decoder.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/config/config.h"


using namespace org::esb::av;
using namespace org::esb::config;
using namespace org::esb::io;
using namespace org::esb::hive::job;
logger("main")
int main(){
    Config::init("./cluster.cfg");
    const string log="log.properties";
    log_init(cxxtools::Logger::LOG_LEVEL_INFO);

	logdebug("create Decoder");
	Decoder dec((CodecID)13);
	logdebug("open Decoder");
	dec.open();
//	Decoder dec2;

	logdebug("Create Units");
	ProcessUnit unit;
	ProcessUnit unit2;
	logdebug("setting Decoder in Unit");
	unit._decoder=&dec;

	logdebug("serialize Unit");
	FileOutputStream fos("test.unit");
	ObjectOutputStream oos(&fos);
	oos.writeObject(unit);
	fos.flush();
	
	
	
	
	logdebug("deserialize Unit");
	FileInputStream fis("test.unit");
	ObjectInputStream ois(&fis);
	ois.readObject(unit2);
	logdebug("unit Process");
	unit2.process();


//	delete unit2._decoder;
	
	
	logdebug("Config::close()");
	Config::close();
}