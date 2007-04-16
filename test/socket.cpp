

#include <iostream>
#include "org/esb/socket/socket.h"
#include <fstream>
using namespace std;
using namespace org::esb::socket;
int main(int argc,char**argv){

    Socket *so1=new Socket();
//    cout << "Hostname"<<so1->getHostname()<<endl;
//    so1->setHostname("localhost");
    so1->setPort(10000);
    so1->Connect();
    SocketData * r=so1->read();
    fstream file("/tmp/frame.container/1/frame.dump.ppm",ios::out|ios::binary);
    file.write(r->data, r->data_length);
    file.close();
//    cout<<"receive:"<<r->data_length<<endl;
//    cout << r->data<<endl;
    so1->Close();
}
