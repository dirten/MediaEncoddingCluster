#include "org/esb/net/Socket.h"
#include "org/esb/io/File.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include <iostream>
#include <stdexcept>
#include "org/esb/sql/sqlite3x.hpp"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/ResultSet.h"
#include <boost/shared_ptr.hpp>
using namespace std;
using namespace org::esb;
using namespace org::esb::net;
using namespace org::esb::hive::job;
using namespace sqlite3x;


int main(){


    Socket sock("localhost", 20000);
    sock.connect();
    ObjectInputStream ois(sock.getInputStream());
    int size=0;
    while(true){
	char * text="get process_unit";
	sock.getOutputStream()->write(text, strlen(text));
//	cout << "hier"<<endl;
	ProcessUnit unit;
	ois.readObject(unit);
	list< boost::shared_ptr<Packet> >::iterator it; 
	if(unit._input_packets.size()==0)break;
//	boost::shared_ptr<Packet> p=unit._input_packets.front();
//	p->size;
	for(it=unit._input_packets.begin();it!=unit._input_packets.end();it++){
	    boost::shared_ptr<Packet> p=*it;
	    size+=p->size;
	}
	
	cout << "\rDataSize="<<size/1024/1024;
	cout.flush();
    }
}

