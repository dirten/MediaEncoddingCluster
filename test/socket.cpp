

#include <iostream>
#include "socket.h"

using namespace std;
int main(int argc,char**argv){

    Socket *so1=new Socket();
    so1->setHostname("localhost");
    so1->setPort(10000);
    
    cout<<so1->getHostname()<<endl;
    cout<<so1->getPort()<<endl;
    so1->Connect();
    char * r=so1->Recv();
    cout<<"receive:"<<r<<endl;
    so1->Close();
}
