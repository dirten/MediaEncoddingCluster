

#include <iostream>
#include "socket.h"
#include <fstream>
using namespace std;
int main(int argc,char**argv){

    Socket *so1=new Socket();
    so1->setHostname("localhost");
    so1->setPort(10000);
    
    cout<<so1->getHostname()<<endl;
    cout<<so1->getPort()<<endl;
    so1->Connect();
    SocketData * r=so1->Recv();
    fstream file("/tmp/frame.container/1/frame.dump.ppm",ios::out|ios::binary);
    file.write(r->data, r->data_length);
    file.close();
//    cout<<"receive:"<<r->data_length<<endl;
//    cout << r->data<<endl;
    so1->Close();
}
