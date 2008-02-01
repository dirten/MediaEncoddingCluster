#include "org/esb/net/Socket.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
using namespace std;
using namespace org::esb::io;
using namespace org::esb::net;
using namespace org::esb::hive::job;




int main(){
    Socket sock("localhost", 20000);
    sock.connect();
    ObjectInputStream ois(sock.getInputStream());
    char * text="get process_unit";
    sock.getOutputStream()->write(text, strlen(text));
    cout << "hier"<<endl;
    ProcessUnit unit;
    ois.readObject(unit);
}

