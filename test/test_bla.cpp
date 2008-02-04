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


    sql::Connection my_con("/tmp/hive.db");
    sql::Statement *stmt=&my_con.createStatement("select data_size,data from packets limit 100;");
    sql::ResultSet *rs=&stmt->executeQuery();

    while(rs->read()) {
	cout << rs->getcolname(0) << ": " << rs->getint(0)<<"---";
	cout << rs->getcolname(1) << ": " << rs->getblob(1).length()<<": "<<strlen(rs->getblob(1).c_str()) << endl;
    }
/*    
    sqlite3_connection con("/tmp/hive.db");
    sqlite3_command cmd(con, "select data_size,data from packets limit 100;");
    sqlite3_reader reader = cmd.executereader();
    while(reader.read()) {
	cout << reader.getcolname(0) << ": " << reader.getint(0)<<"---";
	cout << reader.getcolname(1) << ": " << reader.getblob(1).length()<<": "<<strlen(reader.getblob(1).c_str()) << endl;
    }
*/

/*
    File outfile("/tmp/test.file");
    FileOutputStream fos(&outfile);
    ObjectOutputStream oos(&fos);
    
    ProcessUnit unit_out;
    Packet p1;
    unit_out._input_packets.push_back(&p1);
    oos.writeObject(unit_out);
    fos.close();

    File infile("/tmp/test.file");
    FileInputStream fis(&infile);
    ObjectInputStream ois(&fis);
    
    ProcessUnit unit_in;
    ois.readObject(unit_in);

*/

/*
    Socket sock("localhost", 20000);
    sock.connect();
    ObjectInputStream ois(sock.getInputStream());
    char * text="get process_unit";
    sock.getOutputStream()->write(text, strlen(text));
    cout << "hier"<<endl;
    ProcessUnit unit;
    ois.readObject(unit);
*/    
}

