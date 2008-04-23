#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"

#include <boost/shared_ptr.hpp>
#include "org/esb/hive/job/ProcessUnit.h"

using namespace org::esb::io;
using namespace org::esb::av;
using namespace org::esb::hive::job;



int main(int argc, char ** argv){
	
	File file(argv[1]);
	FormatInputStream fis(&file);
	PacketInputStream pis(&fis);
	
	
	ProcessUnit unit;
	for(int a=0;a<100;a++){
		Packet p;
		pis.readPacket(p);
		if(p.packet->stream_index!=0)continue;
		boost::shared_ptr<Packet>packet(new Packet(p));
		unit._input_packets.push_back(packet);
	}
	
	FileOutputStream fos("new.unit");
	ObjectOutputStream oos(&fos);
	oos.writeObject(unit);
	oos.close();
	
	cout << "Data Serailized"<<endl;

     ProcessUnit unit2;   
     FileInputStream ffis("new.unit");
     ObjectInputStream ois(&ffis);
     ois.readObject(unit2);

   //  unit2.process();
   

}