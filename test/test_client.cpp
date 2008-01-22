#include "org/esb/net/Socket.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/av/Packet.h"
#include <iostream>
using namespace org::esb;

int main(){
    
    net::Socket socket("10.122.6.133",20000);
    socket.connect();
    
    io::ObjectInputStream ois(socket.getInputStream());
    int count=0;
    while(true){
	av::Packet packet;
	ois.readObject(packet);
	++count;
	if(count%10000==0)
	    cout << "Packet readed"<<count << endl;
	if(count%300000==0)
	    break;
	    
    }
}