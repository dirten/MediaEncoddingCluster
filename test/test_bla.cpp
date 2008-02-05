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
using namespace std;
using namespace org::esb;
using namespace org::esb::net;
using namespace org::esb::hive::job;
using namespace sqlite3x;


int main(){


    Socket sock("localhost", 20000);
    sock.connect();
    ObjectInputStream ois(sock.getInputStream());
    while(true){
	char * text="get process_unit";
	sock.getOutputStream()->write(text, strlen(text));
	cout << "hier"<<endl;
	ProcessUnit unit;
	ois.readObject(unit);
    }
    
}

