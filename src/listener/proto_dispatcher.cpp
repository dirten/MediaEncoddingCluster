#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "proto_dispatcher.h"
#include <list>
using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
using namespace org::esb::util;

//list<ProtoCommand*>  l;
ProtocolServer::~ProtocolServer() {
    cout << "Closing socket"<<endl;
    socket->close();
}

ProtocolServer::ProtocolServer(Socket * socket) {
    this->socket=socket;
    //      l=new list<ProtoCommand*>();
    l.push_back((ProtoCommand*)new ProtoHelp(socket));
}

void ProtocolServer::run() {
    while(!socket->isClosed()) {

        //          char * protoversion="MediaEncodingCluster ProtocolServer-1.0.0";
        unsigned char * buffer=new unsigned char[32000];
        cout << socket << endl;
        int bytes=socket->getInputStream()->read(buffer, 32000);
        //      char * data=recvData->data;
        string s1=(char*)buffer;
        string s2="\n;";
        char* command=(char*)StringUtil::trim(s1,s2);
        cout << "Command : "<<command<<endl;

        list<ProtoCommand*>::iterator i;
        for(i=l.begin();i!=l.end();++i) {
            //      cout << "test" << endl;
            ProtoCommand * tmp=(ProtoCommand *)*i;
            if(tmp->isResponsible(command)) {
                cout << "Command responsible"<<endl;
                tmp->process(command);
            }
        }

        if(strcmp(command, "disconnect")==0) {
            cout << "disconnect Command"<<endl;
            socket->getOutputStream()->write((unsigned char *)"disconnecting",13);
            socket->close();
            break;
        } else {
            string *error=new string("Unknown Command:");
            error->append(command);
            socket->getOutputStream()->write((unsigned char *)error->c_str(),error->length());
            delete error;
        }
        //      bzero(command, strlen(command));
        //      if(command)delete command;
        //      delete recvData;
    }
    cout << "Elvis has left the building"<<endl;
}
