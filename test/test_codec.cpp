#include "org/esb/av/Decoder.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"



using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::hive::job;
int main(){
	Decoder dec((CodecID)13);
	dec.open();
	Decoder dec2;

	ProcessUnit unit;
	ProcessUnit unit2;
	unit._decoder=&dec;


	FileOutputStream fos("test.unit");
	ObjectOutputStream oos(&fos);
	oos.writeObject(unit);
	fos.flush();
	
	
	
	
	FileInputStream fis("test.unit");
	ObjectInputStream ois(&fis);
	ois.readObject(unit2);
	
	
	
}