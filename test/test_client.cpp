#include "org/esb/net/Socket.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <queue>
using namespace org::esb;

typedef boost::shared_ptr<av::Packet> packetPtr;
//std::queue<packetPtr>q;
std::queue<av::Packet*>q;
bool stop=false;
class Receiver:public Runnable{
    public:
	Receiver(InputStream * is){
	    _is=is;
	}
	void run(){
	    io::ObjectInputStream ois(_is);
	    while(true&&!stop){
		av::Packet packet;
		ois.readObject(packet);
//		cout << packet.getData();
//		packetPtr p(new av::Packet(packet));
//		q.push(p);
//		av::Packet* pak=new av::Packet(packet);
//		q.push(packet);
		q.push(new av::Packet(packet));
	    }
	    cout << "Exit Receiver"<<endl;
	}
	
    private:
	InputStream * _is;
};


class Sender:public Runnable{
    public:
	Sender(OutputStream * os){
	    _os=os;
	}
	void run(){
	    io::ObjectOutputStream oos(_os);

	    while(true&&!stop){
		if(!q.empty()){
//		    packetPtr pak=q.front();
		    av::Packet * bla=q.front();
//		    int d=packetPtr.pts;
		    oos.writeObject(*bla);
//		    delete l.front();
		    q.pop();
		    delete bla;
		}else{
		    cout << "sleep one second"<<endl;
		    Thread::sleep(1000);
		}
	    }
	    cout << "Sender warte"<<endl;
	    
	    Thread::sleep(1000);
	    cout << "Aufräumen"<<endl;
	    int c=0;
	    while(!q.empty()){
		    ++c;
		    av::Packet * bla=q.front();
		    q.pop();
		    delete bla;
	    }
	    cout << "Exit Sender "<<c<<endl;
	}
	
    private:
	OutputStream * _os;
};

class Status:public Runnable{
    public:
	Status(){
	}

	void run(){
	    while(true&&!stop){
    		Thread::sleep(1000);
    		cout << "QueueCount="<<q.size()<<endl;
    		cout.flush();
    		
//    		cout << q.back()->getData();
	    }
	}
	
    private:
};
class Kill:public Runnable{
    public:
	Kill(){
	}

	void run(){
    		Thread::sleep(10000);
		stop=true;
    		Thread::sleep(2000);
//    		exit(0);
	}
	
};

int main(){
    
    net::Socket socket("10.122.6.133",20000);
    socket.connect();

    Kill kill;
    lang::Thread kill_thread(&kill);
    kill_thread.start();

    Status stat;
    lang::Thread stat_thread(&stat);
    stat_thread.start();
    
    io::ObjectInputStream ois(socket.getInputStream());
    Receiver recv(socket.getInputStream());
    lang::Thread recv_thread(&recv);
    recv_thread.start();

    io::ObjectOutputStream oos(socket.getOutputStream());

    Sender sndr(socket.getOutputStream());
    lang::Thread sndr_thread(&sndr);
    sndr_thread.start();

    stat_thread.join();
    kill_thread.join();
    recv_thread.join();
    sndr_thread.join();
/*
    int count=0;
    while(true){
	av::Packet packet;
	ois.readObject(packet);
	oos.writeObject(packet);
	++count;
	if(count%10000==0)
	    cout << "Packet readed"<<count << endl;
	if(count%30000==0)
	    break;
    }
    */
}
