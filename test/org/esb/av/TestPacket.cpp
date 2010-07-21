#include "org/esb/av/Packet.h"

using namespace org::esb::av;

//checking the internal resource handling from packet
int main(){

  return (EXIT_SUCCESS);

  Packet p(1000);
  Packet pCopy=p;
  Packet pCopy2(p);
  Packet *pCopyp=&p;
}
