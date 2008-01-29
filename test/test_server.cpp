#include <iostream>
#include "org/esb/net/ServerSocket.h"
#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/net/Socket.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/File.h"
#include <list>
using namespace std;
//using namespace org::esb::net;
using namespace org::esb;

class Receiver:public Runnable{
    public:
	Receiver(InputStream * is){
	    _is=is;
	}
	void run(){
	    io::ObjectInputStream ois(_is);
	    while(true){
		av::Packet packet;
		ois.readObject(packet);
	    }
	}
	
    private:
	InputStream * _is;
};

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
	    io::ObjectInputStream ois(socket->getInputStream());

	    Receiver recv(socket->getInputStream());
	    lang::Thread recv_thread(&recv);
	    recv_thread.start();
	    
	    
	    int count=0;
	    while(true){
		try{
    		    av::Packet packet;
		    if(pis.readPacket(packet)<0){
		        cout << "Fehler beim lesen des Packet"<<endl;
		        break;
		    }
		    ++count;
//		    if(count%10000==0)
		        cout << "FrameSended:"<<count<<endl;
		    if(packet.data==NULL)
		        break;
		    oos.writeObject(packet);
//		    ois.readObject(packet);
		}catch(...){cout << "Fehler"<<endl;break;}
	    }
	}
};

int main(int argc,char**argv){

    net::ServerSocket server(20000);
    server.bind();
    if(net::Socket * clientSocket=server.accept()){
	ProtocolServer server(clientSocket);
	server.run();
	delete clientSocket;
//	Thread * thread=new Thread(new ProtocolServer(clientSocket));
//	thread->start();
//	thread->join();
    }
}
