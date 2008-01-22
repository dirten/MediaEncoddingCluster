#include <iostream>
#include "org/esb/net/ServerSocket.h"
#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/net/Socket.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/File.h"

using namespace std;
//using namespace org::esb::net;
using namespace org::esb;

class ProtocolServer:public Runnable{
    private:
	net::Socket * socket;
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
	    io::File file("/tmp/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
	    av::FormatInputStream fis(&file);
	    av::PacketInputStream pis(&fis);
	    io::ObjectOutputStream oos(socket->getOutputStream());
	    int count=0;
	    while(true){
		av::Packet packet=pis.readPacket();
		++count;
		if(count%10000==0)
		    cout << "FrameSended:"<<count<<endl;
		if(packet.data==NULL)
		    break;
		oos.writeObject(packet);
	    }
	}
};

int main(int argc,char**argv){

    net::ServerSocket *server=new net::ServerSocket(20000);
    server->bind();
    for(;net::Socket * clientSocket=server->accept();){
	Thread * thread=new Thread(new ProtocolServer(clientSocket));
	thread->start();
    }
}
