#include "org/esb/net/Socket.h"
#include "org/esb/io/File.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"

using namespace std;
using namespace org::esb::io;
using namespace org::esb::net;
using namespace org::esb::hive::job;




int main(){

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

