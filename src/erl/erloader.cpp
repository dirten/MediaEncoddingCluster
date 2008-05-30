#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/PacketInputStream.h"

#include "epi.hpp"

#include <iostream>

#define BUFSIZE 1000
using namespace org::esb;




using namespace epi::type;
 using namespace epi::error;
  using namespace epi::node;
int main(int argc, char ** argv){
  const std::string LOCALNODE = "c@tron";
  const std::string REMOTENODE = "a@tron";
  const std::string COOKIE = "secretcookie";
  
  AutoNode node(LOCALNODE, COOKIE);  
  MailBox *mailbox = node.createMailBox();

	io::File f("/home/jhoelscher/pfad.avi");
	av::FormatInputStream fis(&f);
	av::PacketInputStream pis(&fis);

    while(true){
      av::Packet p;
      pis.readPacket(p);
      ErlTermPtr<> tuple(new ErlTuple(mailbox->self(), new ErlTuple(new ErlAtom("hello"),new ErlAtom("hello"),new ErlBinary(p.packet->data, p.packet->size))));
      mailbox->send(REMOTENODE, "packet_collector", tuple.get());
    }


  return 0;
}