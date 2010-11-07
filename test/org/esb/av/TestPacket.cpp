#include "org/esb/av/Packet.h"
#include <cstdlib>
using namespace org::esb::av;

//checking the internal resource handling from packet
int main(){

//  return (EXIT_SUCCESS);

  Packet p(1000);
  Packet pCopy=p;
  Packet pCopy2(p);
  Packet *pCopyp=&p;

  Packet pac;
  pac.setPtsTimeStamp(TimeStamp(1, Rational(1,25)));
  pac.setDtsTimeStamp(TimeStamp(3, Rational(1,25)));
  pac.setTimeDuration(TimeStamp(5, Rational(3,25)));
  
  assert(pac.getPtsTimeStamp()==TimeStamp(1, Rational(1,25)));
  assert(pac.getDtsTimeStamp()==TimeStamp(3, Rational(1,25)));
  assert(pac.getTimeDuration()==TimeStamp(5, Rational(3,25)));


  Packet pac2=pac;
  assert(pac.getPtsTimeStamp()==pac2.getPtsTimeStamp());
  assert(pac.getDtsTimeStamp()==pac2.getDtsTimeStamp());
  assert(pac.getTimeDuration()==pac2.getTimeDuration());
  
}
