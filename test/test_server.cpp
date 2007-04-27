#include <iostream>
#include "org/esb/net/ServerSocket.h"
#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/net/Socket.h"

using namespace std;
using namespace org::esb::net;

class ProtocolServer:public Runnable{
    private:
	Socket * socket;
	const char* trim(string & s, string & drop = *new string(" ")){
	    string r=s.erase(s.find_last_not_of(drop)+1);
	    return r.erase(0,r.find_first_not_of(drop)).c_str();
	}

    public:
	~ProtocolServer(){
	    cout << "Closing socket"<<endl;
	    socket->close();
	}
	ProtocolServer(Socket * socket){
	    this->socket=socket;
	
	}
	void run(){
	    while(true){
    		char * protoversion="MediaEncodingCluster ProtocolServer-1.0.0";
		SocketData *recvData=socket->read();
		char* command=(char*)this->trim(*new string(recvData->data),*new string("\n"));
		
		cout << "Command : "<<command<<endl;
		if(strcmp(command, "show version")==0){
		    cout << "version Command"<<endl;
		    socket->write((unsigned char *)protoversion,strlen(protoversion));
		}
		else if(strcmp(command, "disconnect")==0){
		    cout << "disconnect Command"<<endl;
		    socket->write((unsigned char *)"disconnecting",13);
		    socket->close();
//		    exit(0);
		    break;
		}else{
//		    cout << "error Command"<<endl;
		    string error="Unknown Command:";
		    error+=command;
//		    error+="\n";
		    socket->write((unsigned char *)error.c_str(),strlen(error.c_str()));
		}
//		delete recvData;
//		delete command;
	    }
	}
};

int main(int argc,char**argv){

    ServerSocket *server=new ServerSocket(20000);
    server->bind();
    for(;Socket * clientSocket=server->accept();){
	Thread * thread=new Thread(new ProtocolServer(clientSocket));
	thread->start();
    }
}
